#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#include <glx/IRenderer.h>
#include <glx/Device.h>
#include <glx/CommandQueue.h>
#include <glx/RenderPipeline.h>
#include <glx/Buffer.h>
#include <glx/BlitCommandEncoder.h>
#include <glx/Library.h>
#include <glx/Error.h>
#include <glx/Resource.h>
#include <glx/View.h>
#include <glx/Function.h>
#include <glx/Texture.h>
#include <glx/TextureDescriptor.h>
#include <glx/Region.h>
#include <glx/SamplerDescriptor.h>
#include <glx/SamplerState.h>
#include <glx/DepthStencilDescriptor.h>
#include <glx/DepthStencilState.h>
#include <glx/VertexDescriptor.h>
#include <gui/Texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cmath>
#include <mu/DebugConsoleLog.h>

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    
    glx::RenderPipeline _depthRP;
    glx::RenderPipeline _mainRP;
    
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    
    // Optimized uniform buffers - changes mid-render pass need separate cube/floor buffers
    glx::Buffer _sceneUniformBufferCube;
    glx::Buffer _sceneUniformBufferFloor;
    glx::Buffer _depthUniformBufferCube;
    glx::Buffer _depthUniformBufferFloor;
    glx::Buffer _lightingUniformBuffer;
    
    glx::Texture _shadowMapTexture;
    glx::Texture _mainDepthTexture;
    glx::DepthStencilState _depthState;
    
    glx::Texture _samplerWood;
    glx::Texture _samplerFloor;
    glx::SamplerState _textureSampler;
    glx::SamplerState _shadowMapSampler;
    
    // Transformation and lighting state
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _modelMatrix;
    glm::mat4 _floorModelMatrix;
    glm::mat4 _mvpMatrix;
    glm::mat4 _floorMVPMatrix;
    
    // Model-view matrices for proper normal calculation
    glm::mat4 _modelViewMat;
    glm::mat4 _normalMat;
    
    glm::mat4 _lightProjectionMatrix;
    glm::mat4 _lightViewMatrix;
    glm::mat4 _depthMVP;
    glm::mat4 _floorDepthMVP;
    glm::mat4 _depthBiasMVP;
    glm::mat4 _floorDepthBiasMVP;
    glm::mat4 _biasMatrix;
    
    glm::vec3 _lightDir;
    glm::vec3 _sunColor;
    glm::vec3 _cameraPosition;
    
    float _angleX = 0.0f;
    float _angleY = 0.0f;
    float _dAngle = 0.01f;
    float _dAngleAct = 0.0f;
    bool rotation = false;
    
    bool _cameraRotating = false;
    bool _ccwRotation = true;
    float _cameraOffset = 3.0f;
    float _cameraAngleDelta = 0.01f;
    bool _cameraMoveUp = false;
    bool _cameraMoveDown = false;
    float _cameraVerticalDelta = 0.2f;
    
    struct CubeVertex {
        float position[3];
        float texCoord[2];
        float normal[3];
    };
    
    struct SceneUniforms {
        glm::mat4 v;              // View matrix
        glm::mat4 mv;             // ModelView matrix  
        glm::mat4 n;              // Normal matrix
        glm::mat4 mvp;            // ModelViewProjection matrix
        glm::mat4 DepthBiasMVP;   // Shadow matrix
        glm::vec4 lightPos;       // Light position (vec4 for alignment)
    };
    
    struct DepthPassUniforms {
        glm::mat4 depthMVP;       // Depth MVP matrix
    };
    
    struct LightingUniforms {
        glm::vec4 lightColor;     // Light color (vec4 for alignment)
    };


protected:
    void buildShaders()
    {
        buildDepthPassShaders();
        buildMainPassShaders();
    }
    
    void buildDepthPassShaders()
    {
        td::String path = gui::getResFileName(":shaders");
        path += "/testShadowGLX.spv";

        glx::Error error;
        glx::Library library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load depth pass shader library: " << err.c_str();
            assert(false);
        }

        glx::Function vertexFn = library.newFunction("mainDepthVS");
        glx::Function fragFn = library.newFunction("mainDepthFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);
        
        setupDepthVertexDescriptor(desc);
        desc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);
        
        desc.colorAttachments()[0].setPixelFormat(glx::PixelFormat::Invalid);

        _depthRP = _device.newRenderPipelineState(desc, error);
        if (!_depthRP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create depth pass render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();
        
        mu::DebugConsoleLog::ok() << "Depth pass pipeline created";
    }
    
    void buildMainPassShaders()
    {
        td::String path = gui::getResFileName(":shaders");
        path += "/testShadowGLX.spv";

        glx::Error error;
        glx::Library library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load main pass shader library: " << err.c_str();
            assert(false);
        }

        glx::Function vertexFn = library.newFunction("mainVS");
        glx::Function fragFn = library.newFunction("mainFS");

        glx::RenderPipeline::Descriptor desc = glx::RenderPipeline::Descriptor::alloc();
        desc.init();
        desc.setVertexFunction(vertexFn);
        desc.setFragmentFunction(fragFn);
        
        setupMainVertexDescriptor(desc);
        desc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float);

        glx::RenderPipeline::ColorAttachments clrAttachments = desc.colorAttachments();
        glx::RenderPipeline::ColorAttachment clrAtt = clrAttachments[0];
        clrAtt.setPixelFormat(glx::PixelFormat::BGRA8Unorm);
        
        clrAtt.setWriteMask(glx::ColorWriteMask::All);

        _mainRP = _device.newRenderPipelineState(desc, error);
        if (!_mainRP.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create main pass render pipeline";
            assert(false);
        }

        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();
        
        mu::DebugConsoleLog::ok() << "Main pass pipeline created with blending disabled";
    }

    void setupDepthVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Only vertex position for depth pass (location 0)
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(offsetof(CubeVertex, position));
        posAttr.setBufferIndex(0);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(CubeVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }

    void setupMainVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
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

        glx::VertexAttribute normalAttr = attributes[2];
        normalAttr.setFormat(glx::VertexFormat::Float3);
        normalAttr.setOffset(offsetof(CubeVertex, normal));
        normalAttr.setBufferIndex(0);

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(CubeVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    void buildDepthState()
    {
        mu::DebugConsoleLog::info() << "Creating depth state...";
        
        glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
        depthDesc.init();
        
        // Use LessEqual to prevent self-shadowing artifacts
        depthDesc.setDepthCompareFunction(glx::CompareFunction::LessEqual);
        depthDesc.setDepthWriteEnabled(true);
        
        _depthState = _device.newDepthStencilState(&depthDesc);
        if (!_depthState.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create depth state";
            assert(false);
        }
        
        mu::DebugConsoleLog::ok() << "Depth state created with LessEqual comparison";
        depthDesc.release();
    }
    
    void createShadowMapTexture(uint32_t width, uint32_t height)
    {
        mu::DebugConsoleLog::info() << "Creating shadow map texture " << width << "x" << height << "...";
        
        glx::TextureDescriptor shadowMapDesc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::Depth32Float,
            width, 
            height,
            false
        );
        
        // Shadow map needs both RenderTarget (for depth writing) and ShaderRead (for sampling in fragment shader)
        shadowMapDesc.setUsage(static_cast<glx::TextureUsage>(
            static_cast<uint32_t>(glx::TextureUsage::RenderTarget) | 
            static_cast<uint32_t>(glx::TextureUsage::ShaderRead)
        ));
        shadowMapDesc.setStorageMode(glx::Resource::StorageMode::Private);
        
        _shadowMapTexture = _device.newTexture(&shadowMapDesc);
        if (!_shadowMapTexture.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create shadow map texture";
            assert(false);
        }
        
        mu::DebugConsoleLog::ok() << "Shadow map texture created with Depth32Float precision";
    }
    
    void createMainDepthTexture(uint32_t width, uint32_t height)
    {
        mu::DebugConsoleLog::info() << "Creating main depth texture " << width << "x" << height << "...";
        
        glx::TextureDescriptor depthDesc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::Depth32Float,
            width, 
            height,
            false
        );
        
        depthDesc.setUsage(glx::TextureUsage::RenderTarget);
        depthDesc.setStorageMode(glx::Resource::StorageMode::Private);
        
        _mainDepthTexture = _device.newTexture(&depthDesc);
        if (!_mainDepthTexture.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create main depth texture";
            assert(false);
        }
        
        mu::DebugConsoleLog::ok() << "Main depth texture created";
    }
    
    void buildSamplers()
    {
        glx::SamplerDescriptor samplerDesc = glx::SamplerDescriptor::alloc();
        samplerDesc.init();
        
        samplerDesc.setMinFilter(glx::Filter::Linear);
        samplerDesc.setMagFilter(glx::Filter::Linear);
        samplerDesc.setMipFilter(glx::MipFilter::Linear);
        
        samplerDesc.setSAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setTAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setRAddressMode(glx::SamplerAddressMode::ClampToEdge);
        
        _textureSampler = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture sampler";
            assert(false);
        }
        
        // Shadow map sampler with comparison function
        glx::SamplerDescriptor shadowSamplerDesc = glx::SamplerDescriptor::alloc();
        shadowSamplerDesc.init();
        
        shadowSamplerDesc.setMinFilter(glx::Filter::Linear);
        shadowSamplerDesc.setMagFilter(glx::Filter::Linear);
        
        shadowSamplerDesc.setSAddressMode(glx::SamplerAddressMode::ClampToEdge);
        shadowSamplerDesc.setTAddressMode(glx::SamplerAddressMode::ClampToEdge);
        shadowSamplerDesc.setRAddressMode(glx::SamplerAddressMode::ClampToEdge);
        
        // Use LessEqual to match depth state comparison function
        shadowSamplerDesc.setCompareFunction(glx::CompareFunction::LessEqual);
        
        _shadowMapSampler = _device.newSamplerState(&shadowSamplerDesc);
        if (!_shadowMapSampler.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create shadow map sampler";
            assert(false);
        }

        mu::DebugConsoleLog::ok() << "Shadow sampler created with LessEqual comparison";
    }
    
    void loadTextures()
    {
        mu::DebugConsoleLog::info() << "Loading textures for shadow mapping...";
        
        gui::Texture samplerWood(":wood_floor");
        gui::Texture samplerFloor(":wood_floor_NRM");
        
        if (samplerWood.isOk())
        {
            uploadTexture(_samplerWood, samplerWood, "samplerWood");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load wood texture, using placeholder";
            createPlaceholderTexture(_samplerWood, 0x8B4513FF);
        }
        
        if (samplerFloor.isOk())
        {
            uploadTexture(_samplerFloor, samplerFloor, "samplerFloor");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load floor texture, using placeholder";
            createPlaceholderTexture(_samplerFloor, 0x654321FF);
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
            
            // Ensure alpha channel is fully opaque
            if (targetBytesPerPixel == 4) {
                for (uint32_t i = 3; i < targetDataSize; i += 4) {
                    stagingData[i] = 255;
                }
                mu::DebugConsoleLog::debug() << "  Forced alpha channel to 255 for opaque rendering";
            }
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
                        stagingData[targetPixelIndex + 0] = gray;
                        stagingData[targetPixelIndex + 1] = gray;
                        stagingData[targetPixelIndex + 2] = gray;
                        stagingData[targetPixelIndex + 3] = 255;
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
                    
                    for (size_t i = 0; i < targetBytesPerPixel - 1; ++i)
                    {
                        stagingData[targetPixelIndex + i] = sourceData[sourcePixelIndex + i];
                    }
                    stagingData[targetPixelIndex + 3] = 255;
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
        mu::DebugConsoleLog::info() << "Building shadow mapping buffers...";
        
        // Define cube vertices with normals for lighting - EXACT match to OpenGL version
        #define NORMAL_xPLUS 1.0f, 0.0f, 0.0f
        #define NORMAL_xMINUS -1.0f, 0.0f, 0.0f
        #define NORMAL_yPLUS 0.0f, 1.0f, 0.0f
        #define NORMAL_yMINUS 0.0f, -1.0f, 0.0f
        #define NORMAL_zPLUS 0.0f, 0.0f, 1.0f
        #define NORMAL_zMINUS 0.0f, 0.0f, -1.0f

        CubeVertex vertices[] = {
            // Front face (number 1) - exact match to OpenGL
            {{0.5f,   0.5f,  0.5f}, {1.0f, 1.0f}, {NORMAL_zPLUS}},    // Vertex 0 (top-right)
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}, {NORMAL_zPLUS}},    // Vertex 1 (top-left)
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {NORMAL_zPLUS}},    // Vertex 2 (bottom-left)
            {{0.5f,  -0.5f,  0.5f}, {1.0f, 0.0f}, {NORMAL_zPLUS}},    // Vertex 3 (bottom-right)

            // Right face (number 2) - exact match to OpenGL
            {{0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}, {NORMAL_xPLUS}},     // Vertex 8 (bottom-left)
            {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {NORMAL_xPLUS}},     // Vertex 9 (bottom-right)
            {{0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}, {NORMAL_xPLUS}},     // Vertex 10 (top-left)
            {{0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {NORMAL_xPLUS}},     // Vertex 11 (top-right)
            
            // Back face (number 6) - exact match to OpenGL
            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {NORMAL_zMINUS}},    // Vertex 0 (top-right)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {NORMAL_zMINUS}},   // Vertex 1 (top-left)
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}, {NORMAL_zMINUS}},   // Vertex 2 (bottom-left)
            {{0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {NORMAL_zMINUS}},    // Vertex 3 (bottom-right)
            
            // Left face (number 5) - exact match to OpenGL
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {NORMAL_xMINUS}},   // Vertex 12 (bottom-left)
            {{-0.5f,  -0.5f, 0.5f}, {0.0f, 1.0f}, {NORMAL_xMINUS}},   // Vertex 13 (bottom-right)
            {{-0.5f,   0.5f, 0.5f}, {0.0f, 0.0f}, {NORMAL_xMINUS}},   // Vertex 14 (top-left)
            {{-0.5f,   0.5f,-0.5f}, {1.0f, 0.0f}, {NORMAL_xMINUS}},   // Vertex 15 (top-right)

            // Top face (number 3) - exact match to OpenGL
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}, {NORMAL_yPLUS}},    // Vertex 16 (bottom-left)
            {{0.5f,   0.5f,  0.5f}, {0.0f, 1.0f}, {NORMAL_yPLUS}},    // Vertex 17 (bottom-right)
            {{0.5f,   0.5f, -0.5f}, {0.0f, 0.0f}, {NORMAL_yPLUS}},    // Vertex 18 (top-left)
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {NORMAL_yPLUS}},    // Vertex 19 (top-right)

            // Bottom face (number 4) - exact match to OpenGL
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {NORMAL_yMINUS}},   // Vertex 20 (bottom-left)
            {{0.5f,  -0.5f, -0.5f}, {0.0f, 1.0f}, {NORMAL_yMINUS}},   // Vertex 21 (bottom-right)
            {{0.5f,  -0.5f,  0.5f}, {0.0f, 0.0f}, {NORMAL_yMINUS}},   // Vertex 22 (top-left)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {NORMAL_yMINUS}}    // Vertex 23 (top-right)
        };
        
        // Indices exactly match OpenGL version
        uint16_t indices[] = {
            0, 1, 2, // Front face
            2, 3, 0, // Front face

            4, 5, 6, // Right face
            4, 6, 7, // Right face

            8, 9, 11, // Back face
            9, 10, 11, // Back face

            12, 13, 14, // Left face
            12, 14, 15, // Left face

            16, 17, 18, // Top face
            16, 18, 19, // Top face

            20, 21, 22, // Bottom face
            20, 22, 23  // Bottom face
        };

        const size_t vertexDataSize = sizeof(vertices);
        const size_t indexDataSize = sizeof(indices);
        
        // Create vertex buffer
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), vertices, vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);
        
        // Create index buffer
        _indexBuffer = _device.newBuffer(indexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_indexBuffer.contents(), indices, indexDataSize);
        glx::Buffer::Range indexRange(0, _indexBuffer.length());
        _indexBuffer.didModifyRange(indexRange);
        
        // Create optimized uniform buffers based on shader analysis:
        // Each UBO that changes mid-render pass needs separate cube/floor buffers
        const size_t sceneUniformSize = sizeof(SceneUniforms);
        const size_t depthUniformSize = sizeof(DepthPassUniforms);
        const size_t lightingUniformSize = sizeof(LightingUniforms);
        
        // SceneUBO changes mid-render (cube vs floor) - create separate buffers
        _sceneUniformBufferCube = _device.newBuffer(sceneUniformSize, glx::Resource::Options::StorageModeManaged);
        _sceneUniformBufferFloor = _device.newBuffer(sceneUniformSize, glx::Resource::Options::StorageModeManaged);
        
        // DepthPassUBO changes mid-render (cube vs floor) - create separate buffers
        _depthUniformBufferCube = _device.newBuffer(depthUniformSize, glx::Resource::Options::StorageModeManaged);
        _depthUniformBufferFloor = _device.newBuffer(depthUniformSize, glx::Resource::Options::StorageModeManaged);
        
        // LightingUBO doesn't change mid-render - single buffer is sufficient
        _lightingUniformBuffer = _device.newBuffer(lightingUniformSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::ok() << "Optimized uniform buffers created";
        mu::DebugConsoleLog::debug() << "  Vertex buffer: " << vertexDataSize << " bytes (" << sizeof(vertices)/sizeof(CubeVertex) << " vertices)";
        mu::DebugConsoleLog::debug() << "  Index buffer: " << indexDataSize << " bytes (" << sizeof(indices)/sizeof(uint16_t) << " indices)";
        mu::DebugConsoleLog::debug() << "  Scene uniform buffers (cube + floor): " << sceneUniformSize << " bytes each";
        mu::DebugConsoleLog::debug() << "  Depth uniform buffers (cube + floor): " << depthUniformSize << " bytes each";
        mu::DebugConsoleLog::debug() << "  Lighting uniform buffer: " << lightingUniformSize << " bytes";
    }
    
    void populateAllUniformBuffers()
    {
        LightingUniforms lightingUniforms;
        lightingUniforms.lightColor = glm::vec4(_sunColor, 1.0f);
        
        memcpy(_lightingUniformBuffer.contents(), &lightingUniforms, sizeof(LightingUniforms));
        glx::Buffer::Range lightingRange(0, sizeof(LightingUniforms));
        _lightingUniformBuffer.didModifyRange(lightingRange);
        
        SceneUniforms cubeSceneUniforms;
        cubeSceneUniforms.v = _viewMatrix;
        cubeSceneUniforms.mv = _modelViewMat;
        cubeSceneUniforms.n = _normalMat;
        cubeSceneUniforms.mvp = _mvpMatrix;
        cubeSceneUniforms.DepthBiasMVP = _depthBiasMVP;
        cubeSceneUniforms.lightPos = glm::vec4(_lightDir, 1.0f);
        
        memcpy(_sceneUniformBufferCube.contents(), &cubeSceneUniforms, sizeof(SceneUniforms));
        glx::Buffer::Range cubeSceneRange(0, sizeof(SceneUniforms));
        _sceneUniformBufferCube.didModifyRange(cubeSceneRange);
        
        glm::mat4 floorModelViewMatrix = _viewMatrix * _floorModelMatrix;
        glm::mat4 floorNormalMatrix = glm::transpose(glm::inverse(_floorModelMatrix));
        
        SceneUniforms floorSceneUniforms;
        floorSceneUniforms.v = _viewMatrix;
        floorSceneUniforms.mv = floorModelViewMatrix;
        floorSceneUniforms.n = floorNormalMatrix;
        floorSceneUniforms.mvp = _floorMVPMatrix;
        floorSceneUniforms.DepthBiasMVP = _floorDepthBiasMVP;
        floorSceneUniforms.lightPos = glm::vec4(_lightDir, 1.0f);
        
        memcpy(_sceneUniformBufferFloor.contents(), &floorSceneUniforms, sizeof(SceneUniforms));
        glx::Buffer::Range floorSceneRange(0, sizeof(SceneUniforms));
        _sceneUniformBufferFloor.didModifyRange(floorSceneRange);
        
        DepthPassUniforms cubeDepthUniforms;
        cubeDepthUniforms.depthMVP = _depthMVP;
        
        memcpy(_depthUniformBufferCube.contents(), &cubeDepthUniforms, sizeof(DepthPassUniforms));
        glx::Buffer::Range cubeDepthRange(0, sizeof(DepthPassUniforms));
        _depthUniformBufferCube.didModifyRange(cubeDepthRange);
        
        DepthPassUniforms floorDepthUniforms;
        floorDepthUniforms.depthMVP = _floorDepthMVP;
        
        memcpy(_depthUniformBufferFloor.contents(), &floorDepthUniforms, sizeof(DepthPassUniforms));
        glx::Buffer::Range floorDepthRange(0, sizeof(DepthPassUniforms));
        _depthUniformBufferFloor.didModifyRange(floorDepthRange);
    }
    
    void updateTransforms()
    {
        const float fov = glm::radians(90.0f);
        const float aspectRatio = 16.0f / 9.0f;
        const float nearClip = 0.1f;
        const float farClip = 50.0f;
        
        _perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        _viewMatrix = glm::lookAt(_cameraPosition, cameraTarget, cameraUp);
        
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::rotate(_modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f));
        _modelMatrix = glm::rotate(_modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f));
        
        _floorModelMatrix = glm::mat4(1.0f);
        _floorModelMatrix = glm::translate(_floorModelMatrix, glm::vec3(0.0f, -1.5f, 0.0f));
        _floorModelMatrix = glm::scale(_floorModelMatrix, glm::vec3(20.0f, 0.1f, 20.0f));
        
        _modelViewMat = _viewMatrix * _modelMatrix;
        _normalMat = glm::transpose(glm::inverse(_modelViewMat));
        _mvpMatrix = _perspectiveMatrix * _viewMatrix * _modelMatrix;
        _floorMVPMatrix = _perspectiveMatrix * _viewMatrix * _floorModelMatrix;
        
        // Tighter shadow mapping projection for better precision
        _lightProjectionMatrix = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 1.0f, 10.0f);
        
        _lightViewMatrix = glm::lookAt(_lightDir, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        _depthMVP = _lightProjectionMatrix * _lightViewMatrix * _modelMatrix;
        _floorDepthMVP = _lightProjectionMatrix * _lightViewMatrix * _floorModelMatrix;
        
        _depthBiasMVP = _biasMatrix * _depthMVP;
        _floorDepthBiasMVP = _biasMatrix * _floorDepthMVP;
    }
    
    void handleCameraMovement()
    {
        glm::mat4 cameraMatrix = glm::mat4(1.0f);
        
        if (_cameraMoveUp)
        {
            _cameraMoveUp = false;
            cameraMatrix = glm::translate(cameraMatrix, glm::vec3(0.0f, _cameraVerticalDelta, 0.0f));
        }
        
        if (_cameraMoveDown)
        {
            _cameraMoveDown = false;
            cameraMatrix = glm::translate(cameraMatrix, glm::vec3(0.0f, -_cameraVerticalDelta, 0.0f));
        }
        
        if (_cameraRotating)
        {
            float rotationDirection = _ccwRotation ? _cameraAngleDelta : -_cameraAngleDelta;
            cameraMatrix = glm::rotate(cameraMatrix, rotationDirection, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        
        _cameraPosition = glm::vec3(cameraMatrix * glm::vec4(_cameraPosition, 1.0f));
        
        // Update view matrix after camera position changes
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        _viewMatrix = glm::lookAt(_cameraPosition, cameraTarget, cameraUp);
    }

public:
    Renderer(glx::View* pView)
        : _device(pView->device())
    {
        _commandQueue = _device.newCommandQueue();
        


        pView->getSize(_viewportSize);
        
        // Bias matrix for Vulkan/Metal coordinate system with Y-flip and depth bias
        _biasMatrix = glm::mat4(
            0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,
            0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.005f, 1.0f
        );
        
        _lightDir = glm::vec3(2.0f, 5.0f, 0.0f);
        _sunColor = glm::vec3(1.0f, 1.0f, 1.0f);
        _cameraPosition = glm::vec3(0.0f, _cameraOffset, 4.0f);

        buildShaders();
        buildSamplers();
        buildDepthState();
        createShadowMapTexture(static_cast<uint32_t>(_viewportSize.width), static_cast<uint32_t>(_viewportSize.height));
        createMainDepthTexture(static_cast<uint32_t>(_viewportSize.width), static_cast<uint32_t>(_viewportSize.height));
        buildBuffers();
        loadTextures();
        updateTransforms();

        pView->setPixelFormat(glx::PixelFormat::BGRA8Unorm);
        
        
        mu::DebugConsoleLog::ok() << "Shadow mapping renderer initialized";
        mu::DebugConsoleLog::debug() << "  Light position: (" << _lightDir.x << ", " << _lightDir.y << ", " << _lightDir.z << ") looking toward origin";
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _sceneUniformBufferCube.release();
        _sceneUniformBufferFloor.release();
        _depthUniformBufferCube.release();
        _depthUniformBufferFloor.release();
        _lightingUniformBuffer.release();
        _shadowMapTexture.release();
        _mainDepthTexture.release();
        _depthState.release();
        _samplerWood.release();
        _samplerFloor.release();
        _textureSampler.release();
        _shadowMapSampler.release();
        _depthRP.release();
        _mainRP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        
        //IDz: Ovdje imamo memory leak (vjerovatno treba još neki release uraditi)
        if (_viewportSize == sz)
            return;

        _viewportSize = sz;
        
        _shadowMapTexture.release();
        _mainDepthTexture.release();
        createShadowMapTexture(static_cast<uint32_t>(sz.width), static_cast<uint32_t>(sz.height));
        createMainDepthTexture(static_cast<uint32_t>(sz.width), static_cast<uint32_t>(sz.height));
        updateTransforms();
        
        mu::DebugConsoleLog::debug() << "Shadow mapping renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        _dAngleAct = rotation ? _dAngle : 0.0f;
        mu::DebugConsoleLog::debug() << "Animation speed updated to: " << _dAngle;
    }
    
    void switchRotation()
    {
        rotation = !rotation;
        _dAngleAct = rotation ? _dAngle : 0.0f;
        mu::DebugConsoleLog::debug() << "Rotation " << (rotation ? "enabled" : "disabled");
    }
    
    void setCameraRotating(bool rotating) { _cameraRotating = rotating; }
    void setCCWRotation(bool ccw) { _ccwRotation = ccw; }
    void setCameraMoveUp(bool moveUp) { _cameraMoveUp = moveUp; }
    void setCameraMoveDown(bool moveDown) { _cameraMoveDown = moveDown; }
    
    bool getCameraRotating() const { return _cameraRotating; }
    
    void moveLightLeft() { 
        _lightDir.x += 0.1f;
        mu::DebugConsoleLog::debug() << "Light position: (" << _lightDir.x << ", " << _lightDir.y << ", " << _lightDir.z << ")";
    }
    void moveLightRight() { 
        _lightDir.x -= 0.1f;
        mu::DebugConsoleLog::debug() << "Light position: (" << _lightDir.x << ", " << _lightDir.y << ", " << _lightDir.z << ")";
    }
    void moveLightUp() { 
        _lightDir.y += 0.1f;
        mu::DebugConsoleLog::debug() << "Light position: (" << _lightDir.x << ", " << _lightDir.y << ", " << _lightDir.z << ")";
    }
    void moveLightDown() { 
        _lightDir.y -= 0.1f;
        mu::DebugConsoleLog::debug() << "Light position: (" << _lightDir.x << ", " << _lightDir.y << ", " << _lightDir.z << ")";
    }
    
    void setSunColor(const glm::vec3& color) { _sunColor = color; }
    
    bool prepareNextFrame()
    {
        _angleX += _dAngleAct;
        handleCameraMovement();
        updateTransforms();
        return true;
    }

    void draw(glx::View* pView) override
    {
        prepareNextFrame();
        
        populateAllUniformBuffers();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        
        // Depth/Shadow Pass
        {
            glx::RenderPassDescriptor shadowRpd = glx::RenderPassDescriptor::alloc();
            shadowRpd.init();
            
            auto shadowDepthAttachment = shadowRpd.depthAttachment();
            shadowDepthAttachment.setTexture(_shadowMapTexture);
            shadowDepthAttachment.setLoadAction(glx::LoadAction::Clear);
            shadowDepthAttachment.setStoreAction(glx::StoreAction::Store);
            shadowDepthAttachment.setClearDepth(1.0);
            
            glx::RenderCommandEncoder shadowEncoder = cmBuff.renderCommandEncoder(shadowRpd);
            
            shadowEncoder.setRenderPipelineState(_depthRP);
            shadowEncoder.setDepthStencilState(_depthState);

            shadowEncoder.setVertexBuffer(_vertexBuffer, 0, 0);
            
            // Only render cube to shadow map
            shadowEncoder.setVertexBuffer(_depthUniformBufferCube, 0, 8);
            const uint32_t indexCount = 36;
            shadowEncoder.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);
            
            shadowEncoder.endEncoding();
        }
        
        // Main Rendering Pass
        {
            glx::RenderPassDescriptor mainRpd = pView->currentRenderPassDescriptor();
            
            auto mainDepthAttachment = mainRpd.depthAttachment();
            mainDepthAttachment.setTexture(_mainDepthTexture);
            mainDepthAttachment.setLoadAction(glx::LoadAction::Clear);
            mainDepthAttachment.setStoreAction(glx::StoreAction::Store);
            mainDepthAttachment.setClearDepth(1.0);

            glx::RenderCommandEncoder mainEncoder = cmBuff.renderCommandEncoder(mainRpd);

            mainEncoder.setRenderPipelineState(_mainRP);
            mainEncoder.setDepthStencilState(_depthState);
            mainEncoder.setVertexBuffer(_vertexBuffer, 0, 0);
            
            mainEncoder.setFragmentTexture(_shadowMapTexture, 5);
            mainEncoder.setFragmentSamplerState(&_shadowMapSampler, 6);
            mainEncoder.setFragmentBuffer(_lightingUniformBuffer, 0, 7);
            
            const uint32_t indexCount = 36; // 6 faces * 2 triangles * 3 vertices
            
            // Render cube
            mainEncoder.setVertexBuffer(_sceneUniformBufferCube, 0, 2);
            mainEncoder.setFragmentTexture(_samplerWood, 3);
            mainEncoder.setFragmentSamplerState(&_textureSampler, 4);
            mainEncoder.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);
            
            // Render floor
            mainEncoder.setVertexBuffer(_sceneUniformBufferFloor, 0, 2);
            mainEncoder.setFragmentTexture(_samplerFloor, 3);
            mainEncoder.setFragmentSamplerState(&_textureSampler, 4);
            mainEncoder.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);

            mainEncoder.endEncoding();
        }
        
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
