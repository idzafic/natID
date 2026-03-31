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
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    // Textures for lighting demo - wood floor with normal map
    glx::Texture _textureWoodFloor;
    glx::Texture _textureWoodFloorNormal;
    glx::SamplerState _textureSampler1;
    glx::SamplerState _textureSampler2;
    
    // Lighting and transformation state
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _modelViewMatrix;
    glm::mat4 _normalMatrix;
    glm::mat4 _mvpMatrix;
    glm::vec3 _lightPos;
    
    float _angleX = 0.0f;
    float _angleY = 0.0f;
    float _dAngle = 0.01f;
    float _dAngleAct = 0.0f;
    bool _rotation = false;
    
    struct CubeVertex {
        float position[3];
        float texCoord[2];
        float normal[3];
    };
    
    struct UniformBufferObject {
        glm::mat4 v;           // View matrix
        glm::mat4 mv;          // ModelView matrix
        glm::mat4 n;           // Normal matrix
        glm::mat4 mvp;         // ModelViewProjection matrix
        glm::vec4 lightPos;    // Light position (vec4 for proper alignment)
    };

protected:
    void buildShaders()
    {
        td::String path = gui::getResFileName(":shaders");
        path += "/testLightGLX.spv";

        glx::Error error;
        glx::Library library = _device.newLibrary(path, error);
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load shader library: " << err.c_str();
            assert(false);
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

        mu::DebugConsoleLog::ok() << "Lighting pipeline created";
    }

    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
    {
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();

        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();

        // Vertex position (location 0)
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(offsetof(CubeVertex, position));
        posAttr.setBufferIndex(0);

        // Texture coordinates (location 1)
        glx::VertexAttribute texCoordAttr = attributes[1];
        texCoordAttr.setFormat(glx::VertexFormat::Float2);
        texCoordAttr.setOffset(offsetof(CubeVertex, texCoord));
        texCoordAttr.setBufferIndex(0);

        // Vertex normal (location 2)
        glx::VertexAttribute normalAttr = attributes[2];
        normalAttr.setFormat(glx::VertexFormat::Float3);
        normalAttr.setOffset(offsetof(CubeVertex, normal));
        normalAttr.setBufferIndex(0);

        // Vertex buffer layout
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
        mu::DebugConsoleLog::info() << "Loading textures for lighting...";
        
        gui::Texture woodFloor(":woodFloor");
        if (woodFloor.isOk())
        {
            uploadTexture(_textureWoodFloor, woodFloor, "wood_floor");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load wood floor texture, using placeholder";
            createPlaceholderTexture(_textureWoodFloor, 0x8B4513FF); // Brown color
        }
        
        gui::Texture woodFloorNormal(":woodFloorNRM");
        if (woodFloorNormal.isOk())
        {
            uploadTexture(_textureWoodFloorNormal, woodFloorNormal, "wood_floor_normal");
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load wood floor normal texture, using placeholder";
            createPlaceholderTexture(_textureWoodFloorNormal, 0x8080FFFF); // Normal map blue
        }
    }
    
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name)
    {
        const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
        const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
        const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
        const size_t targetBytesPerPixel = 4; // RGBA8Unorm requires 4 bytes per pixel 
 
        mu::DebugConsoleLog::debug() << "Uploading " << name << " texture: " << width << "x" << height << ", " << sourceBytesPerPixel << " source bytes per pixel";
        
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
                
                data[pixelIndex + 0] = (pixelColor >> 0) & 0xFF;  // R
                data[pixelIndex + 1] = (pixelColor >> 8) & 0xFF;  // G
                data[pixelIndex + 2] = (pixelColor >> 16) & 0xFF; // B
                data[pixelIndex + 3] = (pixelColor >> 24) & 0xFF; // A
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
        mu::DebugConsoleLog::info() << "Building lighting buffers...";
        
        // Define cube vertices with normals for lighting
        #define NORMAL_xPLUS 1.0f, 0.0f, 0.0f
        #define NORMAL_xMINUS -1.0f, 0.0f, 0.0f
        #define NORMAL_yPLUS 0.0f, 1.0f, 0.0f
        #define NORMAL_yMINUS 0.0f, -1.0f, 0.0f
        #define NORMAL_zPLUS 0.0f, 0.0f, 1.0f
        #define NORMAL_zMINUS 0.0f, 0.0f, -1.0f

        float a = 1.0f;
        CubeVertex vertices[] = {
            // Front face (z+)
            {{a,   a,  a}, {1.0f, 1.0f}, {NORMAL_zPLUS}},    // 0
            {{-a,  a,  a}, {0.0f, 1.0f}, {NORMAL_zPLUS}},    // 1
            {{-a, -a,  a}, {0.0f, 0.0f}, {NORMAL_zPLUS}},    // 2
            {{a,  -a,  a}, {1.0f, 0.0f}, {NORMAL_zPLUS}},    // 3

            // Right face (x+)
            {{a, -a,  a}, {1.0f, 1.0f}, {NORMAL_xPLUS}},     // 4
            {{a, -a, -a}, {0.0f, 1.0f}, {NORMAL_xPLUS}},     // 5
            {{a,  a, -a}, {0.0f, 0.0f}, {NORMAL_xPLUS}},     // 6
            {{a,  a,  a}, {1.0f, 0.0f}, {NORMAL_xPLUS}},     // 7
            
            // Back face (z-)
            {{a, -a, -a}, {1.0f, 1.0f}, {NORMAL_zMINUS}},    // 8
            {{-a, -a, -a}, {0.0f, 1.0f}, {NORMAL_zMINUS}},   // 9
            {{-a,  a, -a}, {0.0f, 0.0f}, {NORMAL_zMINUS}},   // 10
            {{a,  a, -a}, {1.0f, 0.0f}, {NORMAL_zMINUS}},    // 11
            
            // Left face (x-)
            {{-a, -a, -a}, {1.0f, 1.0f}, {NORMAL_xMINUS}},   // 12
            {{-a, -a,  a}, {0.0f, 1.0f}, {NORMAL_xMINUS}},   // 13
            {{-a,  a,  a}, {0.0f, 0.0f}, {NORMAL_xMINUS}},   // 14
            {{-a,  a, -a}, {1.0f, 0.0f}, {NORMAL_xMINUS}},   // 15

            // Top face (y+)
            {{-a,  a,  a}, {1.0f, 1.0f}, {NORMAL_yPLUS}},    // 16
            {{a,  a,  a}, {0.0f, 1.0f}, {NORMAL_yPLUS}},     // 17
            {{a,  a, -a}, {0.0f, 0.0f}, {NORMAL_yPLUS}},     // 18
            {{-a,  a, -a}, {1.0f, 0.0f}, {NORMAL_yPLUS}},    // 19

            // Bottom face (y-)
            {{-a, -a, -a}, {1.0f, 1.0f}, {NORMAL_yMINUS}},   // 20
            {{a, -a, -a}, {0.0f, 1.0f}, {NORMAL_yMINUS}},    // 21
            {{a, -a,  a}, {0.0f, 0.0f}, {NORMAL_yMINUS}},    // 22
            {{-a, -a,  a}, {1.0f, 0.0f}, {NORMAL_yMINUS}}    // 23
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
        const size_t uniformDataSize = sizeof(UniformBufferObject);
        
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
        
        mu::DebugConsoleLog::ok() << "Lighting buffers created";
        mu::DebugConsoleLog::debug() << "  Vertex buffer: " << vertexDataSize << " bytes (" << sizeof(vertices)/sizeof(CubeVertex) << " vertices)";
        mu::DebugConsoleLog::debug() << "  Index buffer: " << indexDataSize << " bytes (" << sizeof(indices)/sizeof(uint16_t) << " indices)";
        mu::DebugConsoleLog::debug() << "  Uniform buffer: " << uniformDataSize << " bytes";
    }
    
    void updateTransforms()
    {
        const float fov = glm::radians(90.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.1f;
        const float farClip = 100.0f;
        
        _perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        // Camera setup
        glm::vec3 cameraPosition = glm::vec3(2.0f, 0.0f, 2.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        // Model matrix with rotation
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Calculate matrices
        _modelViewMatrix = _viewMatrix * modelMatrix;
        _normalMatrix = glm::transpose(glm::inverse(_modelViewMatrix));
        _mvpMatrix = _perspectiveMatrix * _modelViewMatrix;
        
        // Light position
        _lightPos = glm::vec3(-2.0f, 0.0f, 2.0f);
        
        // Update uniform buffer
        UniformBufferObject ubo;
        ubo.v = _viewMatrix;
        ubo.mv = _modelViewMatrix;
        ubo.n = _normalMatrix;
        ubo.mvp = _mvpMatrix;
        ubo.lightPos = glm::vec4(_lightPos, 1.0f);
        
        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
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
        buildDepthState();
        buildBuffers();
        loadTextures();
        updateTransforms();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        
        mu::DebugConsoleLog::ok() << "Lighting renderer initialized";
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _uniformBuffer.release();
        _depthState.release();
        _textureWoodFloor.release();
        _textureWoodFloorNormal.release();
        _textureSampler1.release();
        _textureSampler2.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        
        updateTransforms(); // Recalculate perspective matrix
        
        mu::DebugConsoleLog::debug() << "Lighting renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        _dAngleAct = _rotation ? _dAngle : 0.0f;
        mu::DebugConsoleLog::debug() << "Animation speed updated to: " << _dAngle;
    }
    
    void switchRotation()
    {
        _rotation = !_rotation;
        _dAngleAct = _rotation ? _dAngle : 0.0f;
        mu::DebugConsoleLog::debug() << "Rotation " << (_rotation ? "enabled" : "disabled");
    }
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
    bool prepareNextFrame()
    {
        _angleX += _dAngleAct;
        updateTransforms();
        return true;
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

        // Set vertex buffer
        cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
        
        // Set uniform buffer (matching shader binding 2)
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 2);

        // Set textures and samplers (matching shader bindings)
        cmdEnc.setFragmentTexture(_textureWoodFloor, 3);       // texWoodFloor binding 3
        cmdEnc.setFragmentTexture(_textureWoodFloorNormal, 4); // texWoodFloorNorm binding 4
        cmdEnc.setFragmentSamplerState(&_textureSampler1, 5);  // woodFloorSampler binding 5
        cmdEnc.setFragmentSamplerState(&_textureSampler2, 6);  // woodFloorNormSampler binding 6

        // Draw the cube
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
