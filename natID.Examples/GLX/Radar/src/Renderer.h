#pragma once
#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/RenderPipeline.h>
#include <glx/Buffer.h>
#include <glx/Library.h>
#include <glx/Error.h>
#include <glx/Resource.h>
#include <glx/View.h>
#include <glx/Function.h>
#include <td/Point.h>
#include <td/Size.h>
#include <cmath>
#include <mu/DebugConsoleLog.h>

// Radar Renderer - Metal API version of the OpenGL Radar2 pixel shader demo
class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;  // Fullscreen quad vertices
    glx::Buffer _uniformBuffer; // Uniforms for radar shader
    
    // Radar-specific state
    gui::Point _leftClickPos;      // Normalized coordinates (0.0-1.0)
    gui::Point _rightClickPos;     // Normalized coordinates (0.0-1.0)
    gui::Size _viewportSize;
    int _animationStep;
    int _radarDirection;  // 1 for normal, -1 for reversed
    
    // Uniform data structure (matches GLSL shader uniform buffer layout)
    struct RadarUniforms {
        float leftClick[2];      // vec2 leftClick
        float rightClick[2];     // vec2 rightClick  
        float viewportSize[2];   // vec2 viewportSize
        float animStep;          // float animStep
        float padding[1];        // Padding for 16-byte alignment (required for std140 layout)
    };
    
    static_assert(sizeof(RadarUniforms) % 16 == 0, "RadarUniforms must be 16-byte aligned for uniform buffer");
    static_assert(sizeof(RadarUniforms) == 32, "RadarUniforms size mismatch - should be 32 bytes");

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;
        
        // Try to load S1 shaders (your GLSL shaders)
        path = gui::getResFileName(":shaders");
		path += "/testRadar.spv";

        library = _device.newLibrary(path, error);
        if (!library.isOk()) 
        {        

			assert(false);
			throw std::runtime_error("Failed to load shader library from S1.spv");
        }

            


 
        glx::Function vertexFn = library.newFunction("mainVS");  // S1.vert has mainVS()
        glx::Function fragFn = library.newFunction("mainFS");    // S1.frag has mainFS()
        

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);
        
        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::BGRA8Unorm);

        _RP = _device.newRenderPipelineState(desc, error);
        if (!_RP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create render pipeline";
            mu::DebugConsoleLog::error() << "This usually means shader functions were not found or incompatible";
            mu::DebugConsoleLog::error() << "Probable cause: Missing vertex descriptor for position attribute";
            mu::DebugConsoleLog::error() << "The S1.vert shader expects 'layout (location = 0) in vec2 position;'";
            mu::DebugConsoleLog::error() << "But no vertex descriptor was configured to describe the vertex data layout";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();
        
        mu::DebugConsoleLog::ok() << "Radar pipeline created using GLSL-derived shaders with storage buffer";
    }
    
    void buildBuffers()
    {
        // Note: Using vec4 to match the GLSL layout(std430) alignment requirements
        struct PositionData {
            float positions[6][4]; // 6 vertices, each as vec4 (x, y, z, w)
        };
        
        PositionData posData = {};
        // Fixed triangulation - standard quad approach
        // Triangle 1: 0-1-2 (top-left, bottom-left, bottom-right)
        posData.positions[0][0] = -1.0f; posData.positions[0][1] =  1.0f; posData.positions[0][2] = 0.0f; posData.positions[0][3] = 1.0f; // Top-left
        posData.positions[1][0] = -1.0f; posData.positions[1][1] = -1.0f; posData.positions[1][2] = 0.0f; posData.positions[1][3] = 1.0f; // Bottom-left
        posData.positions[2][0] =  1.0f; posData.positions[2][1] = -1.0f; posData.positions[2][2] = 0.0f; posData.positions[2][3] = 1.0f; // Bottom-right
        // Triangle 2: 3-4-5 (top-left, bottom-right, top-right)
        posData.positions[3][0] = -1.0f; posData.positions[3][1] =  1.0f; posData.positions[3][2] = 0.0f; posData.positions[3][3] = 1.0f; // Top-left
        posData.positions[4][0] =  1.0f; posData.positions[4][1] = -1.0f; posData.positions[4][2] = 0.0f; posData.positions[4][3] = 1.0f; // Bottom-right
        posData.positions[5][0] =  1.0f; posData.positions[5][1] =  1.0f; posData.positions[5][2] = 0.0f; posData.positions[5][3] = 1.0f; // Top-right

        const size_t positionDataSize = sizeof(PositionData);
        _vertexBuffer = _device.newBuffer(positionDataSize, glx::Resource::Options::StorageModeManaged);
        
        // Copy position data to storage buffer
        memcpy(_vertexBuffer.contents(), &posData, positionDataSize);
        glx::Buffer::Range range(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(range);

        // Create uniform buffer for radar parameters
        const size_t uniformDataSize = sizeof(RadarUniforms);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::ok() << "Radar buffers created";
        mu::DebugConsoleLog::debug() << "  Position storage buffer: " << positionDataSize << " bytes (fullscreen quad - 6 vec4 positions)";
        mu::DebugConsoleLog::debug() << "  Uniform buffer: " << uniformDataSize << " bytes (radar parameters)";
    }
    
    void updateUniforms()
    {
        RadarUniforms uniforms = {};
        
        // Convert normalized coordinates to screen coordinates for shader
        float leftClickScreenX = _leftClickPos.x * _viewportSize.width;
        float leftClickScreenY = _leftClickPos.y * _viewportSize.height;
        float rightClickScreenX = _rightClickPos.x * _viewportSize.width;
        float rightClickScreenY = _rightClickPos.y * _viewportSize.height;
        
        uniforms.leftClick[0] = leftClickScreenX;
        uniforms.leftClick[1] = leftClickScreenY;
        uniforms.rightClick[0] = rightClickScreenX;
        uniforms.rightClick[1] = rightClickScreenY;
        
        // Set viewport size
        uniforms.viewportSize[0] = static_cast<float>(_viewportSize.width);
        uniforms.viewportSize[1] = static_cast<float>(_viewportSize.height);
        
        // Set animation step
        uniforms.animStep = static_cast<float>(_animationStep);
        
        // Padding is automatically zero-initialized
        
        // Update uniform buffer with proper range
        memcpy(_uniformBuffer.contents(), &uniforms, sizeof(RadarUniforms));
        glx::Buffer::Range range(0, sizeof(RadarUniforms));
        _uniformBuffer.didModifyRange(range);
    }

public:
Renderer(glx::View* pView)
: _device(pView->device()) // Must retain device
, _animationStep(0)
, _radarDirection(1)  // Start with normal direction
{
    _commandQueue = _device.newCommandQueue();
        
    // Initialize radar state
    pView->getSize(_viewportSize);
    // Store as normalized coordinates (0.0-1.0)
    _leftClickPos = gui::Point(0.25, 0.5);   // 1/4 from left, centered vertically
    _rightClickPos = gui::Point(0.75, 0.5);  // 3/4 from left, centered vertically
        
    buildShaders();
    buildBuffers();
        
    pView->setPixelFormat(glx::PixelFormat::BGRA8Unorm);
}

    ~Renderer()
    {
        _vertexBuffer.release();
        _uniformBuffer.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }
    



    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        mu::DebugConsoleLog::debug() << "Radar viewport resized to: " << sz.width << "x" << sz.height;
    }

    // Public methods to update radar state (called from View)
    void setLeftClick(const gui::Point& pt)
    {
        // Convert screen coordinates to normalized coordinates
        _leftClickPos.x = pt.x / _viewportSize.width;
        _leftClickPos.y = pt.y / _viewportSize.height;
        updateUniforms();
    }
    
    void setRightClick(const gui::Point& pt)
    {
        // Convert screen coordinates to normalized coordinates
        _rightClickPos.x = pt.x / _viewportSize.width;
        _rightClickPos.y = pt.y / _viewportSize.height;
        updateUniforms();
    }
    
    void toggleRadarDirection()
    {
        _radarDirection *= -1;
        mu::DebugConsoleLog::ok() << "*** RADAR DIRECTION TOGGLED *** New direction: " << (_radarDirection > 0 ? "NORMAL (forward)" : "REVERSED (backward)");
    }
    
    bool isRadarReversed() const { return _radarDirection < 0; }
    
    const gui::Size& getViewportSize() const { return _viewportSize; }

    void draw(glx::View* pView) override
    {
        // Increment animation for each frame
		_animationStep += _radarDirection;
        
        // Update uniform buffer with current radar state
        updateUniforms();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();
        
        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);
        
        cmdEnc.setRenderPipelineState(_RP);
        
        // Bind position storage buffer to vertex shader (set = 0, binding = 0)
        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 3);
        

        // Bind uniform buffer to fragment shader (set = 0, binding = 1)  
        cmdEnc.setFragmentBuffer(_uniformBuffer, 0, 1);
        
        // Draw fullscreen quad as 2 triangles (6 vertices) - positions from storage buffer
        cmdEnc.drawPrimitives(glx::PrimitiveType::Triangle, 0, 6);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }
    
    void release() override
    {
        delete this;
    }
};
