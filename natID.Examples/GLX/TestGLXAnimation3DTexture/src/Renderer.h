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
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    // Textures and samplers
    glx::Texture _textureNumbers;      // Dice with numbers texture
    glx::Texture _textureDots;         // Dice with dots texture  
    glx::SamplerState _textureSampler;
    
    gui::Size _viewportSize;
    float _angleX = 0.0f;
    float _angleY = 0.0f;
    float _dAngle = 0.001f;
    bool _useDotsTexture = false;
    
    struct CubeVertex {
        float position[3];
        float texCoord[2];
    };
    
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
        path += "/testAnimation3D";

        library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            MU_DEBUG_LOG_ERROR("ERROR: Failed to load shader library from :shaders: %s\n", err.c_str());
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
            MU_DEBUG_LOG_ERROR("Failed to create render pipeline");
            assert(false);
        }
       
        vertexFn.release();
        fragFn.release();
        desc.release();
        library.release();

        MU_DEBUG_LOG_SUCCESS("3D animated cube pipeline created");
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

        glx::VertexBufferLayout layout0 = layouts[0];
        layout0.setStride(sizeof(CubeVertex));
        layout0.setStepRate(1);
        layout0.setStepFunction(glx::VertexStepFunction::PerVertex);

        desc.setVertexDescriptor(vertexDesc);
        vertexDesc.release();
    }
    
    void buildDepthState()
    {
        MU_DEBUG_LOG_INFO("Creating depth state...");
        
        glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
        depthDesc.init();
        
        depthDesc.setDepthCompareFunction(glx::CompareFunction::Less);
        depthDesc.setDepthWriteEnabled(true);
        
        _depthState = _device.newDepthStencilState(&depthDesc);
        if (!_depthState.isOk())
        {
            mu::DebugConsoleLog::error() << "ERROR: Failed to create depth state";
            assert(false);
        }
        
        MU_DEBUG_LOG_SUCCESS("Depth state created");
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
        
        _textureSampler = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler.isOk())
        {
            mu::DebugConsoleLog::error() << "ERROR: Failed to create texture sampler";
            assert(false);
        }

        MU_DEBUG_LOG_SUCCESS("Texture sampler created");
    }
    
    void loadTextures()
    {
        MU_DEBUG_LOG_INFO("Loading dice textures...");
        
        gui::Texture numbersTexture(":numbers");
        if (numbersTexture.isOk())
        {
            uploadTexture(_textureNumbers, numbersTexture, "numbers");
        }
        else
        {
            MU_DEBUG_LOG_WARNING("Failed to load numbers texture, using placeholder");
            createPlaceholderTexture(_textureNumbers, 0xFF0000FF);
        }
        
        gui::Texture dotsTexture(":dots");
        if (dotsTexture.isOk())
        {
            uploadTexture(_textureDots, dotsTexture, "dots");
        }
        else
        {
            MU_DEBUG_LOG_WARNING("WARNING: Failed to load dots texture, using placeholder");
            createPlaceholderTexture(_textureDots, 0x00FF00FF);
        }
    }
    
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name)
    {
        const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
        const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
        const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
        const size_t targetBytesPerPixel = 4; // RGBA8Unorm requires 4 bytes per pixel
 
        MU_DEBUG_LOG_INFO("Uploading %s texture: %dx%d, %zu source bytes per pixel", name, width, height, sourceBytesPerPixel);
        
        glx::TextureDescriptor desc = glx::TextureDescriptor::texture2DDescriptor(
            glx::PixelFormat::RGBA8Unorm,
            width,
            height,
            true // Enable mipmaps
        );
        
        desc.setUsage(glx::TextureUsage::ShaderRead);
        desc.setStorageMode(glx::Resource::StorageMode::Private);
        
        targetTexture = _device.newTexture(&desc);
        if (!targetTexture.isOk())
        {
            MU_DEBUG_LOG_ERROR("Failed to create %s texture", name);
            assert(false);
        }
        
        const size_t sourceDataSize = width * height * sourceBytesPerPixel;
        const size_t targetDataSize = width * height * targetBytesPerPixel;
        
        glx::Buffer stagingBuffer = _device.newBuffer(targetDataSize, glx::Resource::Options::StorageModeManaged);
        uint8_t* stagingData = static_cast<uint8_t*>(stagingBuffer.contents());
        const uint8_t* sourceData = static_cast<const uint8_t*>(sourceTexture.getBytes());
        
        if (sourceBytesPerPixel == targetBytesPerPixel)
        {
            MU_DEBUG_LOG_INFO("  Direct copy: %zu bytes per pixel", sourceBytesPerPixel);
            memcpy(stagingData, sourceData, targetDataSize);
        }
        else if (sourceBytesPerPixel < targetBytesPerPixel)
        {
            MU_DEBUG_LOG_INFO("  Padding from %zu to %zu bytes per pixel", sourceBytesPerPixel, targetBytesPerPixel);
            
            for (uint32_t y = 0; y < height; ++y)
            {
                for (uint32_t x = 0; x < width; ++x)
                {
                    const size_t sourcePixelIndex = (y * width + x) * sourceBytesPerPixel;
                    const size_t targetPixelIndex = (y * width + x) * targetBytesPerPixel;
                    
                    if (sourceBytesPerPixel == 1)
                    {
                        uint8_t gray = sourceData[sourcePixelIndex];
                        stagingData[targetPixelIndex + 0] = gray;   // R
                        stagingData[targetPixelIndex + 1] = gray;   // G
                        stagingData[targetPixelIndex + 2] = gray;   // B
                        stagingData[targetPixelIndex + 3] = 255;    // A
                    }
                    else if (sourceBytesPerPixel == 2)
                    {
                        uint8_t gray = sourceData[sourcePixelIndex + 0];
                        uint8_t alpha = sourceData[sourcePixelIndex + 1];
                        stagingData[targetPixelIndex + 0] = gray;   // R
                        stagingData[targetPixelIndex + 1] = gray;   // G
                        stagingData[targetPixelIndex + 2] = gray;   // B
                        stagingData[targetPixelIndex + 3] = alpha;  // A
                    }
                    else if (sourceBytesPerPixel == 3)
                    {
                        // RGB -> RGBA: copy RGB, set A=255
                        stagingData[targetPixelIndex + 0] = sourceData[sourcePixelIndex + 0]; // R
                        stagingData[targetPixelIndex + 1] = sourceData[sourcePixelIndex + 1]; // G
                        stagingData[targetPixelIndex + 2] = sourceData[sourcePixelIndex + 2]; // B
                        stagingData[targetPixelIndex + 3] = 255;                              // A
                    }
                }
            }
        }
        else
        {
            MU_DEBUG_LOG_WARNING("WARNING: Source has more bytes per pixel (%zu) than target (%zu), truncating\n",
                   sourceBytesPerPixel, targetBytesPerPixel);
            
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
        
        MU_DEBUG_LOG_SUCCESS("SUCCESS: %s texture uploaded and mipmaps generated", name);
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
        
        MU_DEBUG_LOG_SUCCESS("Placeholder texture created");
    }
    
    void buildBuffers()
    {
        mu::DebugConsoleLog::debug() << "Building 3D cube buffers...";
        
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
            0, 1, 2,    2, 3, 0,    // Front face
            4, 5, 6,    4, 6, 7,    // Right face
            8, 9, 11,   9, 10, 11,  // Back face
            12, 13, 14, 12, 14, 15, // Left face
            16, 17, 18, 16, 18, 19, // Top face
            20, 21, 22, 20, 22, 23  // Bottom face
        };

        const size_t vertexDataSize = sizeof(vertices);
        const size_t indexDataSize = sizeof(indices);
        const size_t uniformDataSize = sizeof(CubeUniforms);
        
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
        
        // Create uniform buffer
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        mu::DebugConsoleLog::debug() << "x";
        MU_DEBUG_LOG_SUCCESS("3D cube buffers created");
        MU_DEBUG_LOG("  Vertex buffer: %zu bytes (%zu vertices)", vertexDataSize, sizeof(vertices)/sizeof(CubeVertex));
        MU_DEBUG_LOG("  Index buffer: %zu bytes (%zu indices)", indexDataSize, sizeof(indices)/sizeof(uint16_t));
        MU_DEBUG_LOG("  Uniform buffer: %zu bytes", uniformDataSize);
        mu::DebugConsoleLog::debug() << "y";
    }
    
    void updateUniforms()
    {
        const float fov = glm::radians(45.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.1f;
        const float farClip = 100.0f;
        
        glm::mat4 perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        glm::vec3 eye(3.0f, 3.0f, 3.0f);
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
        buildSampler();
        buildDepthState();
        buildBuffers();
        loadTextures();
        updateUniforms();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        mu::DebugConsoleLog::debug() << "z";
        MU_DEBUG_LOG_SUCCESS("SUCCESS: 3D animated cube renderer initialized");
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _uniformBuffer.release();
        _depthState.release();
        _textureNumbers.release();
        _textureDots.release();
        _textureSampler.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        
        MU_DEBUG_LOG_INFO("3D cube renderer viewport resized to: %.0fx%.0f", sz.width, sz.height);
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
//        MU_DEBUG_LOG("Animation speed updated to: %.4f", _dAngle);
    }
    
    void switchTexture()
    {
        _useDotsTexture = !_useDotsTexture;
        MU_DEBUG_LOG_INFO("Switched to %s texture", _useDotsTexture ? "dots" : "numbers");
    }
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
    bool prepareNextFrame()
    {
        _angleY += _dAngle;
        updateUniforms();
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
        MU_DEBUG_LOG_INFO("Left click at (%.1f, %.1f)", pt.x, pt.y);
    }
    
    void handleRightClick(const gui::Point& pt)
    {
        MU_DEBUG_LOG_INFO("Right click at (%.1f, %.1f)", pt.x, pt.y);
    }

    void draw(glx::View* pView) override
    {
        mu::DebugConsoleLog::debug() << "k";
        prepareNextFrame();
        mu::DebugConsoleLog::debug() << "1";
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();
        if (!rpd.getNatPtr())
        {
            mu::DebugConsoleLog::debug() << "NOK";
            return;
        }
        mu::DebugConsoleLog::debug() << "2";
        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd); //ovdje puca u release modu
        mu::DebugConsoleLog::debug() << "3a";
        cmdEnc.setRenderPipelineState(_RP);
        mu::DebugConsoleLog::debug() << "3b";
        cmdEnc.setDepthStencilState(_depthState);
        mu::DebugConsoleLog::debug() << "4";
        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 1);
        mu::DebugConsoleLog::debug() << "5";
        const glx::Texture& currentTexture = _useDotsTexture ? _textureDots : _textureNumbers;
        cmdEnc.setFragmentTexture(currentTexture, 3);
        cmdEnc.setFragmentSamplerState(&_textureSampler, 4);

        // Draw the cube using indices
        const uint32_t indexCount = 36; // 6 faces * 2 triangles * 3 vertices
        cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
        mu::DebugConsoleLog::debug() << ".:.";
    }

    void release() override
    {
        delete this;
    }
};
