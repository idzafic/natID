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
#include <glx/SamplerDescriptor.h>
#include <glx/SamplerState.h>
#include <glx/BlitCommandEncoder.h>
#include <gui/Texture.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <mu/DebugConsoleLog.h>


// Configuration macros
#define ENABLE_ANISOTROPIC_FILTERING 0
#define ANISOTROPIC_LEVEL 16
#define TEXTURE_MIP_LEVELS 2  // Fixed number of mip levels

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::Texture _textureChess1;
    glx::Texture _textureChess2;
    glx::SamplerState _textureSampler;
    
    gui::Size _viewportSize;
    float _textureScale = 1.0f;
    td::UINT1 _usedTexture = 0;
    
    struct TextureVertex {
        float position[2];
        float texCoord[2];
    };
    
    struct TextureUniforms {
        glm::mat4 mvp;
        glm::vec4 params; // x = texture scale
    };

protected:
    void buildShaders()
    {
        td::String path = gui::getResFileName(":shaders");
        path += "/testTextureChessGLX";

        glx::Error error;
        glx::Library library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to load shader library";
            assert(false);
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
        clrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

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
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float2);
        posAttr.setOffset(offsetof(TextureVertex, position));
        posAttr.setBufferIndex(0);

        glx::VertexAttribute texCoordAttr = attributes[1];
        texCoordAttr.setFormat(glx::VertexFormat::Float2);
        texCoordAttr.setOffset(offsetof(TextureVertex, texCoord));
        texCoordAttr.setBufferIndex(0);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(TextureVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    void buildSampler()
    {
        glx::SamplerDescriptor samplerDesc = glx::SamplerDescriptor::alloc();
        samplerDesc.init();
        
        samplerDesc.setMinFilter(glx::Filter::Linear);
        samplerDesc.setMagFilter(glx::Filter::Linear);
        samplerDesc.setMipFilter(glx::MipFilter::Linear);
        
        samplerDesc.setSAddressMode(glx::SamplerAddressMode::Repeat);
        samplerDesc.setTAddressMode(glx::SamplerAddressMode::Repeat);
        samplerDesc.setRAddressMode(glx::SamplerAddressMode::Repeat);
        
        
        #if ENABLE_ANISOTROPIC_FILTERING
        if (_device.supportsAnisotropicFiltering())
        {
            samplerDesc.setMaxAnisotropy(ANISOTROPIC_LEVEL);
        }
        #endif
        
        _textureSampler = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture sampler";
            assert(false);
        }

        samplerDesc.release();
    }
    
    void loadTextures()
    {
        gui::Texture chessTexture1(":chess1");
        if (chessTexture1.isOk())
        {
            uploadTexture(_textureChess1, chessTexture1);
        }
        
        gui::Texture chessTexture2(":chess2");
        if (chessTexture2.isOk())
        {
            uploadTexture(_textureChess2, chessTexture2);
        }
    }
    
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture)
    {
        const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
        const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
        const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
        const size_t targetBytesPerPixel = 4;
        
        glx::TextureDescriptor desc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::RGBA8Unorm,
            width,
            height,
            true  // Enable mipmaps
        );
        
        // Set the fixed number of mip levels
        desc.setMipmapLevelCount(TEXTURE_MIP_LEVELS);
        desc.setUsage(glx::TextureUsage::ShaderRead);
        desc.setStorageMode(glx::Resource::StorageMode::Private);
        
        targetTexture = _device.newTexture(&desc);
        if (!targetTexture.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture";
            assert(false);
        }
        
        const size_t targetDataSize = width * height * targetBytesPerPixel;
        
        glx::Buffer stagingBuffer = _device.newBuffer(targetDataSize, glx::Resource::Options::StorageModeManaged);
        uint8_t* stagingData = static_cast<uint8_t*>(stagingBuffer.contents());
        const uint8_t* sourceData = static_cast<const uint8_t*>(sourceTexture.getBytes());
        
        if (sourceBytesPerPixel == targetBytesPerPixel)
        {
            std::memcpy(stagingData, sourceData, targetDataSize);
        }
        else if (sourceBytesPerPixel == 3 && targetBytesPerPixel == 4)
        {
            for (uint32_t y = 0; y < height; ++y)
            {
                for (uint32_t x = 0; x < width; ++x)
                {
                    const size_t sourcePixelIndex = (y * width + x) * 3;
                    const size_t targetPixelIndex = (y * width + x) * 4;
                    
                    stagingData[targetPixelIndex + 0] = sourceData[sourcePixelIndex + 0];
                    stagingData[targetPixelIndex + 1] = sourceData[sourcePixelIndex + 1];
                    stagingData[targetPixelIndex + 2] = sourceData[sourcePixelIndex + 2];
                    stagingData[targetPixelIndex + 3] = 255;
                }
            }
        }
        else
        {
            mu::DebugConsoleLog::error() << "Unsupported texture format conversion";
            assert(false);
        }
        
        glx::Buffer::Range range(0, stagingBuffer.length());
        stagingBuffer.didModifyRange(range);
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::BlitCommandEncoder blitEnc = cmBuff.blitCommandEncoder();
        
        glx::Region textureRegion = glx::Region::Make2D(0, 0, width, height);
        const size_t bytesPerRow = width * targetBytesPerPixel;
        
        blitEnc.copyFromBuffer(stagingBuffer, 0, bytesPerRow, targetTexture, textureRegion, 0, 0);
        blitEnc.generateMipmaps(targetTexture);
        
        blitEnc.endEncoding();
        cmBuff.commit();
        cmBuff.waitUntilCompleted();
        
        stagingBuffer.release();
    }

    void buildBuffers()
    {
        float centerX = _viewportSize.width * 0.5f;
        float centerY = _viewportSize.height * 0.5f;
        float rectWidth = 600.0f;
        float rectHeight = 400.0f;
        
        TextureVertex vertices[] = {
            {{centerX - rectWidth/2, centerY - rectHeight/2}, {0.0f, 4.0f}},
            {{centerX + rectWidth/2, centerY - rectHeight/2}, {4.0f, 4.0f}},
            {{centerX - rectWidth/2, centerY + rectHeight/2}, {0.0f, 0.0f}},
            
            {{centerX + rectWidth/2, centerY - rectHeight/2}, {4.0f, 4.0f}},
            {{centerX + rectWidth/2, centerY + rectHeight/2}, {4.0f, 0.0f}},
            {{centerX - rectWidth/2, centerY + rectHeight/2}, {0.0f, 0.0f}}
        };
        
        const size_t vertexDataSize = 6 * sizeof(TextureVertex);
        const size_t uniformDataSize = sizeof(TextureUniforms);
        
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        std::memcpy(_vertexBuffer.contents(), vertices, vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);
        
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
    }
    
    void updateMVPMatrix()
    {
        float left = 0.0f;
        float right = _viewportSize.width;
        float top = 0.0f; 
        float bottom = _viewportSize.height;
        float zNear = 0.0f;
        float zFar = 1.0f;
        
        glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
        
        TextureUniforms uniforms = {};
        uniforms.mvp = projectionMatrix;
        uniforms.params.x = _textureScale;
        
        std::memcpy(_uniformBuffer.contents(), &uniforms, sizeof(TextureUniforms));
        glx::Buffer::Range range(0, sizeof(TextureUniforms));
        _uniformBuffer.didModifyRange(range);
    }
    
public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        //_viewportSize = gui::Size(800, 600);
        pView->getSize(_viewportSize);

        buildShaders();
        buildSampler();
        loadTextures();
        buildBuffers();
        
        updateMVPMatrix();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        
        mu::DebugConsoleLog::info() << "Texture Chess GLX Demo Initialized";
        #if ENABLE_ANISOTROPIC_FILTERING
        mu::DebugConsoleLog::info() << "Anisotropic Filtering: " << ANISOTROPIC_LEVEL << "x";
        #else
        mu::DebugConsoleLog::info() << "Anisotropic Filtering: Disabled";
        #endif
        mu::DebugConsoleLog::info() << "Mip Levels: " << TEXTURE_MIP_LEVELS;
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _uniformBuffer.release();
        _textureChess1.release();
        _textureChess2.release();
        _textureSampler.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;

        _vertexBuffer.release();
        _uniformBuffer.release();
        buildBuffers();

        updateMVPMatrix();
    }
    
    void setTextureScale(float scale)
    {
        _textureScale = scale;
        updateMVPMatrix();
    }
    
    void switchTexture()
    {
        _usedTexture = (_usedTexture == 0) ? 1 : 0;
    }
    
    void handleLeftButtonPressed(const gui::Point& pt)
    {
        switchTexture();
    }
    
private:
    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        {
            glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);
            
            cmdEnc.setFrontFacingWinding(glx::Winding::Clockwise);
            cmdEnc.setCullMode(glx::CullMode::Back);
            cmdEnc.setRenderPipelineState(_RP);
            
            cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
            cmdEnc.setVertexBuffer(_uniformBuffer, 0, 4);

            const glx::Texture& currentTexture = (_usedTexture == 0) ? _textureChess1 : _textureChess2;
            cmdEnc.setFragmentTexture(currentTexture, 2);
            cmdEnc.setFragmentSamplerState(&_textureSampler, 3);
            
            cmdEnc.drawPrimitives(glx::PrimitiveType::Triangle, 0, 6);

            cmdEnc.endEncoding();
        }

        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};

