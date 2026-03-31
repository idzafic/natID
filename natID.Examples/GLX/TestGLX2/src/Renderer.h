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
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <cmath>
#include <mu/DebugConsoleLog.h>

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _uniformBuffer;
    
    gui::Size _viewportSize;
    float _zoomFactor = 1.0f;
    
    struct SimpleVertex {
        float position[2];  // x, y
        float color[3];     // r, g, b
    };
    
    struct SimpleUniforms 
    {
        float zoomFactor;
        float pad0;
        float pad1;
        float pad2;
    };
    

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");
		path += "/TestGLX2";

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load shader library from :shaders: " << err.c_str();
            assert(false);
            throw std::runtime_error("Failed to load shader library from :shaders");
        }

        glx::Function vertexFn = library.newFunction("mainVS");
        glx::Function fragFn = library.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);

        setupVertexDescriptor(desc);

        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::BGRA8Unorm);

        _RP = _device.newRenderPipelineState(desc, error);
        if (!_RP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();

        mu::DebugConsoleLog::ok() << "Simple rectangle pipeline created with vertex input";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Position attribute
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float2);
        posAttr.setOffset(offsetof(SimpleVertex, position));
        posAttr.setBufferIndex(0);

        // Color attribute
        glx::VertexAttribute colorAttr = attributes[1];
        colorAttr.setFormat(glx::VertexFormat::Float3);
        colorAttr.setOffset(offsetof(SimpleVertex, color));
        colorAttr.setBufferIndex(0);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(SimpleVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    void buildBuffers()
    {
        // Create vertex data exactly like TestGL but organized for GLX triangle primitives
        // Each vertex: x, y, r, g, b (5 floats per vertex)
        // Note: (1,1) is bottom-right, triangles must be counter-clockwise
        float vertices[] = 
        {
            // Rectangle as two triangles (6 vertices total) - counter-clockwise winding
            // Triangle 1 (counter-clockwise: top-left -> bottom-left -> top-right):
            -0.5f,   0.5f,  1.0f, 0.0f, 0.0f, // Red color (top-left)
            -0.5f,  -0.5f,  1.0f, 1.0f, 0.0f, // Yellow color (bottom-left)
             0.5f,   0.5f,  0.0f, 1.0f, 0.0f, // Green color (top-right)  
            // Triangle 2 (counter-clockwise: bottom-left -> bottom-right -> top-right):
            -0.5f,  -0.5f,  1.0f, 1.0f, 0.0f, // Yellow color (bottom-left)
             0.5f,  -0.5f,  0.0f, 0.0f, 1.0f, // Blue color (bottom-right)
             0.5f,   0.5f,  0.0f, 1.0f, 0.0f, // Green color (top-right)
            
            // Horizontal axis (2 vertices)
            -1.0f,   0.0f,  1.0f, 1.0f, 1.0f,  // White color (left)
             1.0f,   0.0f,  1.0f, 1.0f, 1.0f,  // White color (right)
            
            // Vertical axis (2 vertices)  
             0.0f,  -1.0f,  1.0f, 1.0f, 1.0f,  // White color (top)
             0.0f,   1.0f,  1.0f, 1.0f, 1.0f,  // White color (bottom)
            
            // Bounding box
            -0.98f, -0.98f, 0.0f, 1.0f, 1.0f,
             0.98f, -0.98f, 0.0f, 1.0f, 1.0f,
             0.98f,  0.98f, 0.0f, 1.0f, 1.0f,
            -0.98f,  0.98f, 0.0f, 1.0f, 1.0f,
            -0.98f, -0.98f, 0.0f, 1.0f, 1.0f
        };
        
        const size_t vertexDataSize = sizeof(vertices);
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), vertices, vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);

        const size_t uniformDataSize = sizeof(SimpleUniforms);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::ok() << "Simple rectangle buffers created";
        mu::DebugConsoleLog::debug() << "  Vertex buffer: " << vertexDataSize << " bytes (" << sizeof(vertices)/(5 * sizeof(float)) << " vertices)";
        mu::DebugConsoleLog::debug() << "  Uniform buffer: " << uniformDataSize << " bytes (zoom factor)";
    }
    
    void updateUniforms()
    {
        SimpleUniforms uniforms = {};
        uniforms.zoomFactor = _zoomFactor;
        
        memcpy(_uniformBuffer.contents(), &uniforms, sizeof(SimpleUniforms));
        glx::Buffer::Range range(0, sizeof(SimpleUniforms));
        _uniformBuffer.didModifyRange(range);
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        buildShaders();
        buildBuffers();
        updateUniforms();

        pView->setPixelFormat(glx::PixelFormat::BGRA8Unorm);
        
        mu::DebugConsoleLog::ok() << "Simple TestGL-like renderer initialized with vertex input";
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
        mu::DebugConsoleLog::debug() << "Simple renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void increaseZoom()
    {
        if (_zoomFactor >= 2.0f)
            return;
        _zoomFactor += 0.1f;
        if (_uniformBuffer.isOk()) {
            updateUniforms();
        }
        mu::DebugConsoleLog::debug() << "Zoom increased to: " << _zoomFactor;
    }
    
    void decreaseZoom()
    {
        if (_zoomFactor <= 0.11f)
            return;
        _zoomFactor -= 0.1f;
        if (_uniformBuffer.isOk()) {
            updateUniforms();
        }
        mu::DebugConsoleLog::debug() << "Zoom decreased to: " << _zoomFactor;
    }
    
    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch == 'G' || ch == 'g')
            {
                increaseZoom();
            }
            else if (ch == 'H' || ch == 'h')
            {
                decreaseZoom();
            }
        }
    }
    
    void handleLeftClick(const gui::Point& pt)
    {
        increaseZoom();
        mu::DebugConsoleLog::debug() << "Left click at (" << pt.x << ", " << pt.y << ") - zoom increased";
    }
    
    void handleRightClick(const gui::Point& pt)
    {
        decreaseZoom();
        mu::DebugConsoleLog::debug() << "Right click at (" << pt.x << ", " << pt.y << ") - zoom decreased";
    }

    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);

        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 1);

        cmdEnc.drawPrimitives(glx::PrimitiveType::Triangle, 0, 6);
        cmdEnc.drawPrimitives(glx::PrimitiveType::Line, 6, 2);
        
        // 3. Draw vertical axis (2 vertices starting from 8) 
        cmdEnc.drawPrimitives(glx::PrimitiveType::Line, 8, 2);
        
        // 4. Draw bounding box using LineStrip (5 vertices starting from 10 to close loop)
        cmdEnc.drawPrimitives(glx::PrimitiveType::LineStrip, 10, 5);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
