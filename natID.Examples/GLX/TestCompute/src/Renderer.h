#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/CommandBuffer.h>
#include <glx/Library.h>
#include <glx/Function.h>
#include <glx/Font.h>
#include <td/Point.h>
#include <td/Size.h>
#include <td/String.h>
#include <gui/InputDevice.h>
#include <vector>
#include <cstring>
#include <mu/DebugConsoleLog.h>

#define BUFFER_SIZE  (32 * 8) // 256 floats

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    
    gui::Size _viewportSize;

    // Font rendering
    glx::Font _font;
    bool _fontInitialized = false;

    // Compute shader resources
    glx::Buffer _computeInputBuffer;
    glx::Buffer _computeOutputBuffer;
    glx::Buffer _computeParamsBuffer;
    glx::Library _computeLibrary;
    glx::Function _computeFunction;
    glx::ComputePipelineState _computePipeline;
    bool _computeInitialized = false;

    // Compute shader parameters
    struct ComputeParams {
        float multiplier = 2.0f;
        float padding1 = 0.0f;
        float padding2 = 0.0f;
        float padding3 = 0.0f;
    };

    
    ComputeParams _computeParams;  // Store the compute parameters

protected:
    void setupFont()
    {
        mu::DebugConsoleLog::info() << "Setting up font";

        td::String fontPath = gui::getResFileName(":font1");
        const size_t maxChars = 256;
        
        _fontInitialized = _font.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, fontPath.c_str(), 32.0, maxChars);
        if (!_fontInitialized)
        {
            mu::DebugConsoleLog::error() << "Failed to initialize font";
            return;
        }

        mu::DebugConsoleLog::ok() << "Font initialized";
        _font.addText("Compute Shader Demo", 50.0, 50.0, td::Color(255, 255, 255, 255));
    }

    void setupComputeShader()
    {
        mu::DebugConsoleLog::info() << "Setting up compute shader demo";

        // Load compute shader library
        td::String shaderPath = gui::getResFileName(":compute_shader");

        glx::Error error;
        _computeLibrary = _device.newLibrary(shaderPath, error);
        
        if (!_computeLibrary.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to load compute shader library";
            return;
        }

        // Get compute kernel function
        _computeFunction = _computeLibrary.newFunction("mainComp");

        _computePipeline = _device.newComputePipelineState(_computeFunction, error);
        if (!_computePipeline.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create compute pipeline state";
            return;
        }

        // Create input buffer with test data
        std::vector<float> inputData(BUFFER_SIZE);
        for (size_t i = 0; i < BUFFER_SIZE; ++i)
        {
            inputData[i] = static_cast<float>(i);  // Simple sequence: 0, 1, 2, ..., 255
        }

        const size_t bufferSize = BUFFER_SIZE * sizeof(float);
        _computeInputBuffer = _device.newBuffer(inputData.data(), bufferSize, glx::Resource::Options::StorageModeManaged);
        if (!_computeInputBuffer.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create compute input buffer";
            return;
        }

        // Create output buffer (zero-initialized)
        std::vector<float> outputData(BUFFER_SIZE, 0.0f);
        _computeOutputBuffer = _device.newBuffer(outputData.data(), bufferSize, glx::Resource::Options::StorageModeManaged);
        if (!_computeOutputBuffer.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create compute output buffer";
            return;
        }

        // Create parameter buffer
        const size_t paramsBufferSize = sizeof(ComputeParams);
        _computeParamsBuffer = _device.newBuffer(paramsBufferSize, glx::Resource::Options::StorageModeManaged);
        if (!_computeParamsBuffer.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create compute params buffer";
            return;
        }

        // Initialize params


        memcpy(_computeParamsBuffer.contents(), &_computeParams, sizeof(ComputeParams));
        glx::Buffer::Range range(0, sizeof(ComputeParams));
        _computeParamsBuffer.didModifyRange(range);


        mu::DebugConsoleLog::ok() << "Compute shader demo initialized";
        mu::DebugConsoleLog::debug() << "  Input buffer: " << BUFFER_SIZE << " floats (" << bufferSize << " bytes)";
        mu::DebugConsoleLog::debug() << "  Output buffer: " << BUFFER_SIZE << " floats (" << bufferSize << " bytes)";
        mu::DebugConsoleLog::debug() << "  Multiplier: 2.0";

        _computeInitialized = true;
    }

    void runComputeShader()
    {
        if (!_computeInitialized)
        {
            return;
        }

        // Create command buffer for compute work
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();

        // Create compute command encoder
        glx::ComputeCommandEncoder computeEncoder = cmBuff.computeCommandEncoder();
        
        // Set compute pipeline state
        computeEncoder.setComputePipelineState(_computePipeline);

        // Set input and output buffers (bindings 0 and 1)
        computeEncoder.setBuffer(_computeInputBuffer, 0, 0);
        computeEncoder.setBuffer(_computeOutputBuffer, 0, 1);

        // Set parameter buffer (binding 2)
        computeEncoder.setBuffer(_computeParamsBuffer, 0, 2);

        // Dispatch compute work
        // Threadgroup size: 32 threads per group (32 x 1 x 1) - must match shader's local_size
        // Grid size: 8 groups (256 elements / 32 threads per group)
        glx::Volume<td::UINT4> threadgroupsPerGrid(BUFFER_SIZE / 32, 1, 1);
        glx::Volume<td::UINT4> threadsPerThreadgroup(32, 1, 1);
        computeEncoder.dispatchThreadgroups(threadgroupsPerGrid, threadsPerThreadgroup);
        computeEncoder.endEncoding();

        // Commit compute work
        cmBuff.commit();

        // Wait for GPU to complete before reading results
        cmBuff.waitUntilCompleted();

        mu::DebugConsoleLog::debug() << "  Compute kernel dispatched";

        // Read back and verify results
        readAndVerifyResults();
    }

    void readAndVerifyResults()
    {
        // Mark buffer as modified so CPU can read it
        glx::Buffer::Range range(0, _computeOutputBuffer.length());
        _computeOutputBuffer.didModifyRange(range);

        float* inputData = static_cast<float*>(_computeInputBuffer.contents());
        float* outputData = static_cast<float*>(_computeOutputBuffer.contents());

        if (inputData && outputData)
        {
            char buffer[256];
            
            _font.reset();

            snprintf(buffer, sizeof(buffer), "Results (input -> output):");
            mu::DebugConsoleLog::debug() << "  " << buffer;
            _font.addText(buffer, 50.0, 82.0, td::Color(255, 255, 255, 255));
            
            snprintf(buffer, sizeof(buffer), "[0]:   %.1f -> %.1f (expected: %.1f)", 
                   inputData[0], outputData[0], inputData[0] * _computeParams.multiplier);
            mu::DebugConsoleLog::debug() << "    " << buffer;
            _font.addText(buffer, 50.0, 114.0, td::Color(255, 255, 255, 255));
            
            snprintf(buffer, sizeof(buffer), "[%zu]: %.1f -> %.1f (expected: %.1f)", 
                BUFFER_SIZE/2, inputData[BUFFER_SIZE/2], outputData[BUFFER_SIZE/2], inputData[BUFFER_SIZE/2] * _computeParams.multiplier);
            mu::DebugConsoleLog::debug() << "    " << buffer;
            _font.addText(buffer, 50.0, 146.0, td::Color(255, 255, 255, 255));
            
            snprintf(buffer, sizeof(buffer), "[%zu]: %.1f -> %.1f (expected: %.1f)", 
                BUFFER_SIZE-1, inputData[BUFFER_SIZE-1], outputData[BUFFER_SIZE-1], inputData[BUFFER_SIZE-1] * _computeParams.multiplier);
            mu::DebugConsoleLog::debug() << "    " << buffer;
            _font.addText(buffer, 50.0, 178.0, td::Color(255, 255, 255, 255));
        }
    }
    
public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        _viewportSize = gui::Size(800, 600);

        setupFont();
        setupComputeShader();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        
        mu::DebugConsoleLog::ok() << "Compute shader renderer initialized";
        mu::DebugConsoleLog::debug() << "  Viewport: " << _viewportSize.width << "x" << _viewportSize.height;
    }

    ~Renderer()
    {

        _computeInputBuffer.release();
        _computeOutputBuffer.release();
        _computeParamsBuffer.release();
        _computeFunction.release();
        _computePipeline.release();
        _computeLibrary.release();
        
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
    }
    
private:
    void draw(glx::View* pView) override
    {        
        if (_computeInitialized)
            runComputeShader();
        

        _font.draw(pView, true);

        // Present screen
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};

