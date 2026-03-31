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

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexPositionsBuffer;
    glx::Buffer _vertexColorsBuffer;
protected:
    void buildShaders()
    {
        
        td::String path = gui::getResFileName(":shaders");
        path += "/testGLX";
        

        glx::Error error;
        glx::Library library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            assert( false );
        }

        glx::Function vertexFn = library.newFunction("mainVS");
        glx::Function fragFn = library.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction( fragFn );
        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];

		
        clrAtt.setPixelFormat( glx::PixelFormat::BGRA8Unorm);

        _RP = _device.newRenderPipelineState(desc, error );
        if (!_RP.isOk())
            assert( false );

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();
    }
    void buildBuffers()
    {
        const size_t NumVertices = 3;
        const size_t numFlts = 4;

        float positions[] =
        {
            -0.8f,  0.8f, 0.0f, 1.0,
             0.0f, -0.8f, 0.0f, 1.0,
             0.8f,  0.8f, 0.0f, 1.0
        };

        float colors[] =
        {
            1.0, 0.0f, 0.f, 1.0,
            0.0f, 1.0, 0.0f, 1.0,
            0.0f, 0.0f, 1.0, 1.0
        };

        const size_t positionsDataSize = NumVertices * numFlts * sizeof( float );
        const size_t colorDataSize = NumVertices * numFlts * sizeof( float );
        _vertexPositionsBuffer = _device.newBuffer(positionsDataSize, glx::Resource::Options::StorageModeManaged);
        _vertexColorsBuffer = _device.newBuffer(colorDataSize, glx::Resource::Options::StorageModeManaged);

        memcpy(_vertexPositionsBuffer.contents(), positions, positionsDataSize);
        memcpy(_vertexColorsBuffer.contents(), colors, colorDataSize);

        glx::Buffer::Range range1(0, _vertexPositionsBuffer.length());
        _vertexPositionsBuffer.didModifyRange(range1);
        glx::Buffer::Range range2(0, _vertexColorsBuffer.length());
        _vertexColorsBuffer.didModifyRange(range2);
    }
public:
    Renderer(glx::View* pView)
    : _device(pView->device()) //must be retain
    {
        _commandQueue = _device.newCommandQueue();
        buildShaders();
        buildBuffers();
		//pView->setClearColor(td::Color(60, 20, 20, 255));
		pView->setPixelFormat(glx::PixelFormat::BGRA8Unorm);
    }

    ~Renderer()
    {
        _vertexPositionsBuffer.release();
        _vertexColorsBuffer.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }
    
    void onResize(const gui::Size& sz) override final
    {
    }

    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        //Test for Vulkan 1.1
        auto colorAttach = rpd.colorAttachments()[0];
        colorAttach.setClearColor(glx::ClearColor(0.0, 1.0, 0.0, 1.0));  // Green background
        colorAttach.setLoadAction(glx::LoadAction::Clear);  // Ensure clear

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setVertexBuffer(_vertexPositionsBuffer, 0, 0 );
        cmdEnc.setVertexBuffer(_vertexColorsBuffer, 0, 1 );
        cmdEnc.drawPrimitives(glx::PrimitiveType::Triangle, 0, 3);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }
    
    void release() override
    {
        delete this;
    }
};
