#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
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
#include <glx/VertexDescriptor.h>
#include <glx/Texture.h>
#include <glx/TextureDescriptor.h>
#include <glx/DepthStencilDescriptor.h>
#include <glx/DepthStencilState.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <mu/DebugConsoleLog.h>

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _flatRP;
    glx::RenderPipeline _interpolationRP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _colorBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    gui::Size _viewportSize;
    float _angleX = 0.0f;
    float _angleY = 0.0f;
    bool _useFlat = true; // true = flat shading, false = interpolation shading
    
    struct CubeUniforms {
        glm::mat4 mvpMatrix;
    };

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");
        path += "/testAnimation3DFlat";

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load shader library from :shaders: " << err.c_str();
            assert(false);
            throw std::runtime_error("Failed to load shader library from :shaders");
        }

        glx::Function flatVertexFn = library.newFunction("mainVSflat");
        glx::Function flatFragFn = library.newFunction("mainFSflat");
        glx::Function interpVertexFn = library.newFunction("mainVSint");
        glx::Function interpFragFn = library.newFunction("mainFSint");

        glx::RenderPipeline::Descriptor flatDesc = glx::RenderPipeline::Descriptor::alloc();
        flatDesc.init();
        flatDesc.setVertexFunction(flatVertexFn);
        flatDesc.setFragmentFunction(flatFragFn);

        setupVertexDescriptor(flatDesc);
        
        // Enable depth testing
        flatDesc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);

        glx::RenderPipeline::ColorAttachments flatClrAttachments = flatDesc.colorAttachments();
        glx::RenderPipeline::ColorAttachment flatClrAtt = flatClrAttachments[0];
        flatClrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        _flatRP = _device.newRenderPipelineState(flatDesc, error);
        if (!_flatRP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create flat render pipeline";
            assert(false);
        }

        glx::RenderPipeline::Descriptor interpDesc = glx::RenderPipeline::Descriptor::alloc();
        interpDesc.init();
        interpDesc.setVertexFunction(interpVertexFn);
        interpDesc.setFragmentFunction(interpFragFn);

        setupVertexDescriptor(interpDesc);
        
        interpDesc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);

        glx::RenderPipeline::ColorAttachments interpClrAttachments = interpDesc.colorAttachments();
        glx::RenderPipeline::ColorAttachment interpClrAtt = interpClrAttachments[0];
        interpClrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

        _interpolationRP = _device.newRenderPipelineState(interpDesc, error);
        if (!_interpolationRP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create interpolation render pipeline";
            assert(false);
        }
       
       
        flatVertexFn.release();
        flatFragFn.release();
        interpVertexFn.release();
        interpFragFn.release();
        flatDesc.release();
        interpDesc.release();
        library.release();

        mu::DebugConsoleLog::ok() << "3D animated cube pipelines created (flat and interpolation)";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(0);
        posAttr.setBufferIndex(0);

        glx::VertexAttribute colorAttr = attributes[1];
        colorAttr.setFormat(glx::VertexFormat::Float3);
        colorAttr.setOffset(0);
        colorAttr.setBufferIndex(1);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(3 * sizeof(float));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        glx::VertexBufferLayout layout1 = layouts[1];
        layout1.setStride(3 * sizeof(float));
        layout1.setStepRate(1);
        layout1.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    
    void buildDepthState()
    {
        mu::DebugConsoleLog::info() << "Creating depth state...";
        
        glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
        depthDesc.init();
        
        depthDesc.setDepthCompareFunction(glx::CompareFunction::Less);
        depthDesc.setDepthWriteEnabled(true);
        
        _depthState = _device.newDepthStencilState(&depthDesc);
        if (!_depthState.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create depth state";
            assert(false);
        }
        
        
        mu::DebugConsoleLog::ok() << "Depth state created";
        depthDesc.release();
    }
    
    
    void buildBuffers()
    {
        mu::DebugConsoleLog::info() << "Building 3D cube buffers...";
        
        float vertices[] = {
            // Front face
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             // Back face
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
        };

        float vertexColors[] = {
            1.0f, 0.0f, 0.0f,  // Red
            0.0f, 1.0f, 0.0f,  // Green
            0.0f, 0.0f, 1.0f,  // Blue
            1.0f, 1.0f, 0.0f,  // Yellow

            0.0f, 1.0f, 1.0f,  // Cyan
            1.0f, 0.0f, 1.0f,  // Magenta
            1.0f, 1.0f, 1.0f,  // White
            0.5f, 0.5f, 0.5f,  // Gray
        };

        uint16_t indices[] = {
            // Front
            0, 1, 2,
            2, 3, 0,

            // Right
            0, 3, 7,
            7, 4, 0,

            // Bottom
            2, 6, 7,
            7, 3, 2,

            // Left
            1, 5, 6,
            6, 2, 1,

            // Back
            4, 7, 6,
            6, 5, 4,

            // Top
            5, 1, 0,
            0, 4, 5,
        };

        const size_t vertexDataSize = sizeof(vertices);
        const size_t colorDataSize = sizeof(vertexColors);
        const size_t indexDataSize = sizeof(indices);
        const size_t uniformDataSize = sizeof(CubeUniforms);
        
        // Create vertex buffer
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), vertices, vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);
        
        // Create color buffer
        _colorBuffer = _device.newBuffer(colorDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_colorBuffer.contents(), vertexColors, colorDataSize);
        glx::Buffer::Range colorRange(0, _colorBuffer.length());
        _colorBuffer.didModifyRange(colorRange);
        
        // Create index buffer
        _indexBuffer = _device.newBuffer(indexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_indexBuffer.contents(), indices, indexDataSize);
        glx::Buffer::Range indexRange(0, _indexBuffer.length());
        _indexBuffer.didModifyRange(indexRange);
        
        
        // Create uniform buffer
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::ok() << "3D cube buffers created";
        mu::DebugConsoleLog::debug() << "  Vertex buffer: " << vertexDataSize << " bytes (" << sizeof(vertices)/(3*sizeof(float)) << " vertices)";
        mu::DebugConsoleLog::debug() << "  Color buffer: " << colorDataSize << " bytes (" << sizeof(vertexColors)/(3*sizeof(float)) << " colors)";
        mu::DebugConsoleLog::debug() << "  Index buffer: " << indexDataSize << " bytes (" << sizeof(indices)/sizeof(uint16_t) << " indices)";
        mu::DebugConsoleLog::debug() << "  Uniform buffer: " << uniformDataSize << " bytes";
    }
    
    void updateUniforms()
    {
        const float fov = glm::radians(45.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.1f;
        const float farClip = 100.0f;
        
        glm::mat4 perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        glm::vec3 eye(0.0f, 0.0f, 3.0f);
        glm::vec3 center(0.0f, 0.0f, 0.5f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
        
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f));
        
        glm::mat4 mvpMatrix = perspectiveMatrix * viewMatrix * modelMatrix;
        
        CubeUniforms uniforms;
        uniforms.mvpMatrix = mvpMatrix;
        
        memcpy(_uniformBuffer.contents(), &uniforms, sizeof(CubeUniforms));
        glx::Buffer::Range range(0, sizeof(CubeUniforms));
        _uniformBuffer.didModifyRange(range);
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        buildShaders();
        buildDepthState();
        buildBuffers();
        updateUniforms();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        
        mu::DebugConsoleLog::ok() << "3D animated flat cube renderer initialized";
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _colorBuffer.release();
        _indexBuffer.release();
        _uniformBuffer.release();
        _depthState.release();
        _flatRP.release();
        _interpolationRP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        
        mu::DebugConsoleLog::info() << "3D cube renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void switchColorShadingMode()
    {
        _useFlat = !_useFlat;
        mu::DebugConsoleLog::info() << "Switched to " << (_useFlat ? "flat" : "interpolation") << " shading";
    }
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
    bool prepareNextFrame()
    {
        _angleX += 0.01f;
        _angleY += 0.01f;
        updateUniforms();
        return true;
    }

    void draw(glx::View* pView) override
    {
        prepareNextFrame();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        if (_useFlat)
        {
            cmdEnc.setRenderPipelineState(_flatRP);
        }
        else
        {
            cmdEnc.setRenderPipelineState(_interpolationRP);
        }
        
        cmdEnc.setDepthStencilState(_depthState);

        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        cmdEnc.setVertexBuffer(_colorBuffer, 0, 1);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 2);

        // Draw the cube using indices
        const uint32_t indexCount = 36; // 6 faces * 2 triangles * 3 vertices
        cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
