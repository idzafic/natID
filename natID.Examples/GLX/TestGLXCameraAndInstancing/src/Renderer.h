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
#include <glx/DepthStencilDescriptor.h>
#include <glx/DepthStencilState.h>
#include <glx/BlitCommandEncoder.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <td/Point.h>
#include <td/Size.h>
#include <gui/InputDevice.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstring>
#include <mu/DebugConsoleLog.h>

#define NUM_INSTANCES 45

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _instanceBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    glx::Texture _textureDiceWithNumbers;  
    glx::Texture _textureDiceWithDots;     
    glx::Texture _textureEarth;
    glx::Texture _textureSand;
    glx::SamplerState _textureSampler1;
    glx::SamplerState _textureSampler2;
    
    glx::View* _pView = nullptr;
    
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _cameraProjectionMatrix;
    
    float _accAngle = 0.0f;
    float _dAngle = 0.01f;
    td::BYTE _usedTexture = 0;
    
    struct CubeVertex {
        float position[3];
        float texCoord[2];
    };
    
    struct InstanceData {
        float initialAngle[3];
        float translation[3];  
        float rotationDirection[3];
    };
    
    struct UniformBufferObject {
        float accAngle;
        float pad0, pad1, pad2;
        glm::mat4 cameraWithProjection;
    };

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");
        path += "/testCameraAndInstancing.spv";

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
        desc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);

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

        mu::DebugConsoleLog::ok() << "Camera and instancing pipeline created";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(offsetof(CubeVertex, position));
        posAttr.setBufferIndex(0);

        glx::VertexAttribute texCoordAttr = attributes[1];
        texCoordAttr.setFormat(glx::VertexFormat::Float2);
        texCoordAttr.setOffset(offsetof(CubeVertex, texCoord));
        texCoordAttr.setBufferIndex(0);

        glx::VertexAttribute initialAngleAttr = attributes[2];
        initialAngleAttr.setFormat(glx::VertexFormat::Float3);
        initialAngleAttr.setOffset(offsetof(InstanceData, initialAngle));
        initialAngleAttr.setBufferIndex(1);

        glx::VertexAttribute translationAttr = attributes[3];
        translationAttr.setFormat(glx::VertexFormat::Float3);
        translationAttr.setOffset(offsetof(InstanceData, translation));
        translationAttr.setBufferIndex(1);

        glx::VertexAttribute rotationDirectionAttr = attributes[4];
        rotationDirectionAttr.setFormat(glx::VertexFormat::Float3);
        rotationDirectionAttr.setOffset(offsetof(InstanceData, rotationDirection));
        rotationDirectionAttr.setBufferIndex(1);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(CubeVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        glx::VertexBufferLayout layout2 = layouts[1];
        layout2.setStride(sizeof(InstanceData));
        layout2.setStepRate(1);
        layout2.setStepFunction(glx::VertexStepFunction::PerInstance);

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
    
    void buildSampler()
    {
        glx::SamplerDescriptor samplerDesc = glx::SamplerDescriptor::alloc();
        samplerDesc.init();
        
        samplerDesc.setMinFilter(glx::Filter::Linear);
        samplerDesc.setMagFilter(glx::Filter::Linear);
        samplerDesc.setMipFilter(glx::MipFilter::Linear);
        
        samplerDesc.setSAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setTAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setRAddressMode(glx::SamplerAddressMode::ClampToEdge);
        
        _textureSampler1 = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler1.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture sampler 1";
            assert(false);
        }

        _textureSampler2 = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler2.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture sampler 2";
            assert(false);
        }

        
        mu::DebugConsoleLog::ok() << "Texture samplers created";
    }
    
    void loadTextures()
    {
        mu::DebugConsoleLog::info() << "Loading textures for instancing...";
        
        gui::Texture diceNumbers(":numbers");
        if (diceNumbers.isOk())
        {
            uploadTexture(_textureDiceWithNumbers, diceNumbers, "dice_numbers");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load dice numbers texture, using placeholder";
            createPlaceholderTexture(_textureDiceWithNumbers, 0xFF0000FF);
        }
        
        gui::Texture diceDots(":dots");
        if (diceDots.isOk())
        {
            uploadTexture(_textureDiceWithDots, diceDots, "dice_dots");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load dice dots texture, using placeholder";
            createPlaceholderTexture(_textureDiceWithDots, 0x00FF00FF);
        }

        gui::Texture earth(":earth");
        if (earth.isOk())
        {
            uploadTexture(_textureEarth, earth, "earth");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load earth texture, using placeholder";
            createPlaceholderTexture(_textureEarth, 0x0000FFFF);
        }

        gui::Texture sand(":sand");
        if (sand.isOk())
        {
            uploadTexture(_textureSand, sand, "sand");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load sand texture, using placeholder";
            createPlaceholderTexture(_textureSand, 0xFFFF00FF);
        }
    }
    
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name)
    {
        const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
        const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
        const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
        const size_t targetBytesPerPixel = 4;
 
        mu::DebugConsoleLog::debug() << "Uploading " << name << " texture: " << width << "x" << height << ", " << sourceBytesPerPixel << " source bytes per pixel";
        
        glx::TextureDescriptor desc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::RGBA8Unorm,
            width,
            height,
            true
        );
        
        desc.setUsage(glx::TextureUsage::ShaderRead);
        desc.setStorageMode(glx::Resource::StorageMode::Private);
        
        targetTexture = _device.newTexture(&desc);
        if (!targetTexture.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create " << name << " texture";
            assert(false);
        }
        
        const size_t sourceDataSize = width * height * sourceBytesPerPixel;
        const size_t targetDataSize = width * height * targetBytesPerPixel;
        
        glx::Buffer stagingBuffer = _device.newBuffer(targetDataSize, glx::Resource::Options::StorageModeManaged);
        uint8_t* stagingData = static_cast<uint8_t*>(stagingBuffer.contents());
        const uint8_t* sourceData = static_cast<const uint8_t*>(sourceTexture.getBytes());
        
        if (sourceBytesPerPixel == targetBytesPerPixel)
        {
            mu::DebugConsoleLog::debug() << "  Direct copy: " << sourceBytesPerPixel << " bytes per pixel";
            memcpy(stagingData, sourceData, targetDataSize);
        }
        else if (sourceBytesPerPixel < targetBytesPerPixel)
        {
            mu::DebugConsoleLog::debug() << "  Padding from " << sourceBytesPerPixel << " to " << targetBytesPerPixel << " bytes per pixel";
            
            for (uint32_t y = 0; y < height; ++y)
            {
                for (uint32_t x = 0; x < width; ++x)
                {
                    const size_t sourcePixelIndex = (y * width + x) * sourceBytesPerPixel;
                    const size_t targetPixelIndex = (y * width + x) * targetBytesPerPixel;
                    
                    if (sourceBytesPerPixel == 1)
                    {
                        uint8_t gray = sourceData[sourcePixelIndex];
                        stagingData[targetPixelIndex + 0] = gray;
                        stagingData[targetPixelIndex + 1] = gray;
                        stagingData[targetPixelIndex + 2] = gray;
                        stagingData[targetPixelIndex + 3] = 255;
                    }
                    else if (sourceBytesPerPixel == 2)
                    {
                        uint8_t gray = sourceData[sourcePixelIndex + 0];
                        uint8_t alpha = sourceData[sourcePixelIndex + 1];
                        stagingData[targetPixelIndex + 0] = gray;
                        stagingData[targetPixelIndex + 1] = gray;
                        stagingData[targetPixelIndex + 2] = gray;
                        stagingData[targetPixelIndex + 3] = alpha;
                    }
                    else if (sourceBytesPerPixel == 3)
                    {
                        stagingData[targetPixelIndex + 0] = sourceData[sourcePixelIndex + 0];
                        stagingData[targetPixelIndex + 1] = sourceData[sourcePixelIndex + 1];
                        stagingData[targetPixelIndex + 2] = sourceData[sourcePixelIndex + 2];
                        stagingData[targetPixelIndex + 3] = 255;
                    }
                }
            }
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Source has more bytes per pixel (" << sourceBytesPerPixel << ") than target (" << targetBytesPerPixel << "), truncating";
            
            for (uint32_t y = 0; y < height; ++y)
            {
                for (uint32_t x = 0; x < width; ++x)
                {
                    const size_t sourcePixelIndex = (y * width + x) * sourceBytesPerPixel;
                    const size_t targetPixelIndex = (y * width + x) * targetBytesPerPixel;
                    
                    for (size_t i = 0; i < targetBytesPerPixel; ++i)
                    {
                        stagingData[targetPixelIndex + i] = sourceData[sourcePixelIndex + i];
                    }
                }
            }
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
        
        mu::DebugConsoleLog::ok() << name << " texture uploaded and mipmaps generated";
    }
    
    void createPlaceholderTexture(glx::Texture& targetTexture, uint32_t color)
    {
        const uint32_t width = 64;
        const uint32_t height = 64;
        const size_t bytesPerPixel = 4;
        
        glx::TextureDescriptor desc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::RGBA8Unorm,
            width,
            height,
            false
        );
        
        desc.setUsage(glx::TextureUsage::ShaderRead);
        desc.setStorageMode(glx::Resource::StorageMode::Private);
        
        targetTexture = _device.newTexture(&desc);
        
        const size_t textureDataSize = width * height * bytesPerPixel;
        glx::Buffer stagingBuffer = _device.newBuffer(textureDataSize, glx::Resource::Options::StorageModeManaged);
        
        uint8_t* data = static_cast<uint8_t*>(stagingBuffer.contents());
        for (uint32_t y = 0; y < height; ++y)
        {
            for (uint32_t x = 0; x < width; ++x)
            {
                size_t pixelIndex = (y * width + x) * bytesPerPixel;
                bool isEven = ((x / 8) + (y / 8)) % 2 == 0;
                uint32_t pixelColor = isEven ? color : 0xFFFFFFFF;
                
                data[pixelIndex + 0] = (pixelColor >> 0) & 0xFF;
                data[pixelIndex + 1] = (pixelColor >> 8) & 0xFF;
                data[pixelIndex + 2] = (pixelColor >> 16) & 0xFF;
                data[pixelIndex + 3] = (pixelColor >> 24) & 0xFF;
            }
        }
        
        glx::Buffer::Range range(0, stagingBuffer.length());
        stagingBuffer.didModifyRange(range);
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::BlitCommandEncoder blitEnc = cmBuff.blitCommandEncoder();
        
        glx::Region textureRegion = glx::Region::Make2D(0, 0, width, height);
        const size_t bytesPerRow = width * bytesPerPixel;
        
        blitEnc.copyFromBuffer(stagingBuffer, 0, bytesPerRow, targetTexture, textureRegion, 0, 0);
        
        blitEnc.endEncoding();
        cmBuff.commit();
        cmBuff.waitUntilCompleted();
        
        stagingBuffer.release();
        
        mu::DebugConsoleLog::ok() << "Placeholder texture created";
    }
    
    void buildBuffers()
    {
        mu::DebugConsoleLog::info() << "Building camera and instancing buffers...";
        
        CubeVertex vertices[] = {
            {{ 0.5f,  0.5f,  0.5f}, {1.0f/6.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f/6.0f, 0.0f}},

            {{ 0.5f, -0.5f,  0.5f}, {1.0f/6.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {2.0f/6.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {2.0f/6.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {1.0f/6.0f, 1.0f}},
            
            {{ 0.5f, -0.5f, -0.5f}, {2.0f/6.0f, 0.0f}},
            {{-0.5f, -0.5f, -0.5f}, {3.0f/6.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, {3.0f/6.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {2.0f/6.0f, 1.0f}},
            
            {{-0.5f, -0.5f, -0.5f}, {3.0f/6.0f, 0.0f}},
            {{-0.5f, -0.5f,  0.5f}, {4.0f/6.0f, 0.0f}},
            {{-0.5f,  0.5f,  0.5f}, {4.0f/6.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {3.0f/6.0f, 1.0f}},

            {{-0.5f,  0.5f,  0.5f}, {4.0f/6.0f, 0.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {5.0f/6.0f, 0.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {5.0f/6.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {4.0f/6.0f, 1.0f}},

            {{-0.5f, -0.5f, -0.5f}, {5.0f/6.0f, 0.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
            {{-0.5f, -0.5f,  0.5f}, {5.0f/6.0f, 1.0f}},
        };
        
        uint16_t indices[] = {
            0, 1, 2,    2, 3, 0,
            4, 5, 6,    4, 6, 7,
            8, 9, 11,   9, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
        };

        InstanceData instanceData[NUM_INSTANCES] = {
            {{0.0f, 0.0f, 0.0f}, {-4.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, 0.0f}, {-3.0f, 0.0f, 0.0f}, {0.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, 0.0f}, {-2.0f, 0.0f, 0.0f}, {0.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/2.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, 0.0f}, {2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, 0.0f}, {3.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, 0.0f, 0.0f}, {4.0f, 0.0f, 0.0f}, {1.0f, 1.0f, -1.0f}},
            
            {{0.0f, 0.0f, 0.0f}, {-4.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, 1.0f}},
            {{glm::pi<float>()/4.0f, glm::pi<float>()/2.0f, 0.0f}, {-3.0f, -1.0f, -1.0f}, {0.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, glm::pi<float>()/4.0f, 0.0f}, {-2.0f, -1.0f, -1.0f}, {0.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, glm::pi<float>()/3.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, glm::pi<float>()/2.0f, 0.0f}, {0.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, glm::pi<float>()/3.0f, 0.0f}, {1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, glm::pi<float>()/4.0f, 0.0f}, {2.0f, -1.0f, -1.0f}, {1.0f, 0.0f, -1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, 0.0f}, {3.0f, 0.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/2.0f, 0.0f, 0.0f}, {4.0f, -1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}},
            
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {-4.0f, 1.0f, -2.0f}, {-1.0f, 0.0f, 1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {-3.0f, 1.0f, -2.0f}, {0.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {-2.0f, 1.0f, -2.0f}, {0.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {-1.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/2.0f, glm::pi<float>()/2.0f}, {0.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {1.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {2.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {3.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {4.0f, 1.0f, -2.0f}, {1.0f, 1.0f, -1.0f}},
            
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {-4.0f, -3.0f, -2.0f}, {-1.0f, 0.0f, 1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {-3.0f, -3.0f, -2.0f}, {0.0f, 1.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {-2.0f, -3.0f, -2.0f}, {0.0f, 0.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {-1.0f, -3.0f, -2.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.0f, glm::pi<float>()/2.0f, glm::pi<float>()/2.0f}, {0.0f, -3.0f, -2.0f}, {1.0f, 0.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {1.0f, -3.0f, -2.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {2.0f, -3.0f, -2.0f}, {0.0f, -1.0f, 0.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {3.0f, -3.0f, -2.0f}, {0.0f, 0.0f, -1.0f}},
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {4.0f, -3.0f, -2.0f}, {0.0f, 0.0f, 1.0f}},
            
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {-4.0f, -5.0f, -2.0f}, {1.0f, 0.0f, -1.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {-3.0f, -5.0f, -2.0f}, {0.0f, -1.0f, 1.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {-2.0f, -5.0f, -2.0f}, {0.0f, 0.0f, -1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {-1.0f, -5.0f, -2.0f}, {-1.0f, 0.0f, 0.0f}},
            {{0.0f, glm::pi<float>()/2.0f, glm::pi<float>()/2.0f}, {0.0f, -5.0f, -2.0f}, {1.0f, 0.0f, 1.0f}},
            {{0.0f, glm::pi<float>()/3.0f, glm::pi<float>()/2.0f}, {1.0f, -5.0f, -2.0f}, {0.0f, -1.0f, 0.0f}},
            {{0.0f, glm::pi<float>()/4.0f, glm::pi<float>()/2.0f}, {2.0f, -5.0f, -2.0f}, {0.0f, 1.0f, 0.0f}},
            {{glm::pi<float>()/4.0f, 0.0f, glm::pi<float>()/2.0f}, {3.0f, -5.0f, -2.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.0f, 0.0f, glm::pi<float>()/2.0f}, {4.0f, -5.0f, -2.0f}, {0.0f, 0.0f, -1.0f}}
        };

        const size_t vertexDataSize = sizeof(vertices);
        const size_t indexDataSize = sizeof(indices);
        const size_t instanceDataSize = sizeof(instanceData);
        const size_t uniformDataSize = sizeof(UniformBufferObject);
        
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), vertices, vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);
        
        _indexBuffer = _device.newBuffer(indexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_indexBuffer.contents(), indices, indexDataSize);
        glx::Buffer::Range indexRange(0, _indexBuffer.length());
        _indexBuffer.didModifyRange(indexRange);
        
        _instanceBuffer = _device.newBuffer(instanceDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_instanceBuffer.contents(), instanceData, instanceDataSize);
        glx::Buffer::Range instanceRange(0, _instanceBuffer.length());
        _instanceBuffer.didModifyRange(instanceRange);
        
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::ok() << "Camera and instancing buffers created";
        mu::DebugConsoleLog::debug() << "  Vertex buffer: " << vertexDataSize << " bytes (" << sizeof(vertices)/sizeof(CubeVertex) << " vertices)";
        mu::DebugConsoleLog::debug() << "  Index buffer: " << indexDataSize << " bytes (" << sizeof(indices)/sizeof(uint16_t) << " indices)";
        mu::DebugConsoleLog::debug() << "  Instance buffer: " << instanceDataSize << " bytes (" << NUM_INSTANCES << " instances)";
        mu::DebugConsoleLog::debug() << "  Uniform buffer: " << uniformDataSize << " bytes";
    }
    
    void updateCamera()
    {
        const float fov = glm::radians(45.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.1f;
        const float farClip = 100.0f;
        
        _perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        glm::vec3 eye(3.0f, 3.0f, 10.0f);
        glm::vec3 center(0.0f, 0.0f, 0.5f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
        
        _cameraProjectionMatrix = _perspectiveMatrix * viewMatrix;
        
        UniformBufferObject ubo;
        ubo.accAngle = _accAngle;
        ubo.cameraWithProjection = _cameraProjectionMatrix;
        
        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
        _uniformBuffer.didModifyRange(range);


        

    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _pView = pView;
        _commandQueue = _device.newCommandQueue();
        pView->getSize(_viewportSize);

        buildShaders();
        buildSampler();
        buildDepthState();
        buildBuffers();
        loadTextures();
        updateCamera();

        // Configure View to manage depth texture
        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        
        mu::DebugConsoleLog::ok() << "Camera and instancing renderer initialized";

    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _instanceBuffer.release();
        _uniformBuffer.release();
        _depthState.release();
        _textureDiceWithNumbers.release();
        _textureDiceWithDots.release();
        _textureEarth.release();
        _textureSand.release();
        _textureSampler1.release();
        _textureSampler2.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        // Depth texture is now managed by glx::View - no manual recreation needed
        updateCamera();
        
        mu::DebugConsoleLog::info() << "Camera and instancing renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        mu::DebugConsoleLog::debug() << "Animation speed updated to: " << _dAngle;
    }
    
    void switchTexture()
    {
        _usedTexture = (_usedTexture == 0) ? 1 : 0;
        mu::DebugConsoleLog::info() << "Switched to texture set " << _usedTexture;
    }
    

    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
    bool prepareNextFrame()
    {
        _accAngle += _dAngle;
        if (_accAngle >= 2.0f * glm::pi<float>())
        {
            _accAngle -= 2.0f * glm::pi<float>();
        }
        updateCamera();
        return true;
    }
    
    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
        }
    }
    
    void handleLeftClick(const gui::Point& pt)
    {
        mu::DebugConsoleLog::debug() << "Left click at (" << pt.x << ", " << pt.y << ")";
    }
    
    void handleRightClick(const gui::Point& pt)
    {
        mu::DebugConsoleLog::debug() << "Right click at (" << pt.x << ", " << pt.y << ")";
    }

    void draw(glx::View* pView) override
    {
        prepareNextFrame();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        // currentRenderPassDescriptor now includes depth attachment when setDepthStencilPixelFormat was called
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setDepthStencilState(_depthState);

        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        cmdEnc.setVertexBuffer(_instanceBuffer, 0, 1);
        
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 6);

        if (_usedTexture == 0) {
            cmdEnc.setFragmentTexture(_textureDiceWithNumbers, 2);
            cmdEnc.setFragmentTexture(_textureDiceWithDots, 3);
        } else {
            cmdEnc.setFragmentTexture(_textureEarth, 2);
            cmdEnc.setFragmentTexture(_textureSand, 3);
        }
        
        cmdEnc.setFragmentSamplerState(&_textureSampler1, 4);
        cmdEnc.setFragmentSamplerState(&_textureSampler2, 5);

        const uint32_t indexCount = 36;
        cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0, NUM_INSTANCES);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
