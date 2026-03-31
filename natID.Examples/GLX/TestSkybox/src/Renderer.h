#define GLM_FORCE_RADIANS 
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 
#pragma once
#define NUM_INSTANCES 45

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
#include <glx/Font.h>
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
#include <ctime>
#include <chrono>
#include <unordered_map>
#include <deque>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <mu/DebugConsoleLog.h>
#include "InstancedObject.h"
#include "Cubemap.h"
#include <gui/Label.h>

struct CubeVertex {
    float position[3];
    float texCoord[2];
};

struct CubeMesh {
    std::vector<CubeVertex> vertices;
    std::vector<uint16_t> indices;
};

inline CubeMesh createCube(float size)
{
    CubeMesh mesh;
    float half = size * 0.5f;
    
    mesh.vertices = {
        {{ half,  half,  half}, {1.0f / 6.0f, 1.0f}},
        {{-half,  half,  half}, {0.0f, 1.0f}},
        {{-half, -half,  half}, {0.0f, 0.0f}},
        {{ half, -half,  half}, {1.0f / 6.0f, 0.0f}},

        {{ half, -half,  half}, {1.0f / 6.0f, 0.0f}},
        {{ half, -half, -half}, {2.0f / 6.0f, 0.0f}},
        {{ half,  half, -half}, {2.0f / 6.0f, 1.0f}},
        {{ half,  half,  half}, {1.0f / 6.0f, 1.0f}},

        {{ half, -half, -half}, {2.0f / 6.0f, 0.0f}},
        {{-half, -half, -half}, {3.0f / 6.0f, 0.0f}},
        {{-half,  half, -half}, {3.0f / 6.0f, 1.0f}},
        {{ half,  half, -half}, {2.0f / 6.0f, 1.0f}},

        {{-half, -half, -half}, {3.0f / 6.0f, 0.0f}},
        {{-half, -half,  half}, {4.0f / 6.0f, 0.0f}},
        {{-half,  half,  half}, {4.0f / 6.0f, 1.0f}},
        {{-half,  half, -half}, {3.0f / 6.0f, 1.0f}},

        {{-half,  half,  half}, {4.0f / 6.0f, 0.0f}},
        {{ half,  half,  half}, {5.0f / 6.0f, 0.0f}},
        {{ half,  half, -half}, {5.0f / 6.0f, 1.0f}},
        {{-half,  half, -half}, {4.0f / 6.0f, 1.0f}},

        {{-half, -half, -half}, {5.0f / 6.0f, 0.0f}},
        {{ half, -half, -half}, {1.0f, 0.0f}},
        {{ half, -half,  half}, {1.0f, 1.0f}},
        {{-half, -half,  half}, {5.0f / 6.0f, 1.0f}},
    };
    
    mesh.indices = {
        0, 1, 2,    2, 3, 0,
        4, 5, 6,    4, 6, 7,
        8, 9, 11,   9, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    
    
    return mesh;
}

inline CubeMesh createSphere(float radius)
{
    CubeMesh mesh;
    const uint32_t segments = 32;
    const uint32_t rings = 16;
    
    for (uint32_t ring = 0; ring <= rings; ++ring)
    {
        float ringRatio = static_cast<float>(ring) / rings;
        float phi = ringRatio * glm::pi<float>();
        
        for (uint32_t seg = 0; seg <= segments; ++seg)
        {
            float segRatio = static_cast<float>(seg) / segments;
            float theta = segRatio * 2.0f * glm::pi<float>();
            
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);
            
            float x = radius * sinPhi * cosTheta;
            float y = radius * cosPhi;
            float z = radius * sinPhi * sinTheta;
            

            CubeVertex vertex;
            vertex.position[0] = x;
            vertex.position[1] = y;
            vertex.position[2] = z;


            theta *= 0.99f;
            sinTheta = std::sin(theta);
            cosTheta = std::cos(theta);
            x = radius * sinPhi * cosTheta;
            z = radius * sinPhi * sinTheta;

            float u = (std::atan2(z, x) / (2.0f * glm::pi<float>()));
			if (u < 0.0f) u += 1.0f; 
			if (u > 1.0f) u = 1.0f - (u - 1.0f); 
            float v = std::acos(glm::clamp(y / radius, -1.0f, 1.0f)) / glm::pi<float>();
            if (v < 0.0f) v += 1.0f;
			if (v > 1.0f) v = 1.0f - (v - 1.0f);
            mu::DebugConsoleLog::debug() << "Vertex (" << x << ", " << y << ", " << z << ") - UV (" << u << ", " << v << ")";
            

            vertex.texCoord[0] = u/1.5f;
            vertex.texCoord[1] = v;
            
            mesh.vertices.push_back(vertex);
        }
    }
    
    for (uint32_t ring = 0; ring < rings; ++ring)
    {
        for (uint32_t seg = 0; seg < segments; ++seg)
        {
            uint32_t a = ring * (segments + 1) + seg;
            uint32_t b = a + segments + 1;
            
            mesh.indices.push_back(a);
            mesh.indices.push_back(b);
            mesh.indices.push_back(a + 1);
            
            mesh.indices.push_back(b);
            mesh.indices.push_back(b + 1);
            mesh.indices.push_back(a + 1);
        }
    }
    
    return mesh;
}

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _uniformBuffer;
    
    glx::DepthStencilState _depthState;
    
    glx::Texture _textureDiceWithNumbers;  
    glx::Texture _textureDiceWithDots;     
    glx::Texture _textureEarth;
    glx::Texture _textureSand;
    CubeMap* _skyboxCubemap = nullptr;
    glx::SamplerState _textureSampler;
    
    glx::Font _fpsFont;
    glx::Font _staticTextFont;
    glx::Font _fancyFont;
    bool _fpsFontInitialized = false;
    bool _staticTextFontInitialized = false;
    bool _fancyFontInitialized = false;
    
    std::chrono::high_resolution_clock::time_point _lastFrameTime;
    float _currentFps = 0.0f;
    float _lastFps = 0.0f;
    
    glx::View* _pView = nullptr;
    
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _cameraProjectionMatrix;
    
    float _accAngle = 0.0f;
    float _dAngle = 0.01f;
    td::BYTE _usedTexture = 0;
    
    // Camera movement
    glm::vec3 _cameraEye = glm::vec3(3.0f, 3.0f, 20.0f);
    glm::vec3 _cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 _cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 _cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float _cameraSpeed = 0.1f;
    
    // Camera rotation
    float _cameraYaw = 0.0f;
    float _cameraPitch = 0.0f;
    float _mouseSensitivity = 0.0045f;
    
    // Mouse tracking
    int _lastMouseX = 0;
    int _lastMouseY = 0;
    bool _firstMouseMove = true;
    
    // Keyboard state tracking
    std::unordered_map<char, bool> _keyStates;
    
    // Helper function to get opposite key for conflict resolution
    char getOppositeKey(char key)
    {
        static const std::unordered_map<char, char> opposites = {
            {'W', 'S'},
            {'S', 'W'},
            {'A', 'D'},
            {'D', 'A'}
        };
        auto it = opposites.find(key);
        return (it != opposites.end()) ? it->second : 0;
    }
    
    struct InstanceData {
        float initialAngle[3];
        float translation[3];
        float rotationDirection[3];
    };

    using InstancedObject = ::InstancedObject<CubeVertex, InstanceData>;
    std::deque<InstancedObject> _objects;

    struct UniformBufferObject {
        float accAngle;
        float pad0, pad1, pad2;
        glm::mat4 cameraWithProjection;
    };

protected:
void loadFonts()
{
    mu::DebugConsoleLog::info() << "Loading fonts";
        
    const size_t maxChars = 1000;
        
    // FPS Font - larger for visibility
    td::String fpsFontPath = gui::getResFileName(":font1");
    _fpsFontInitialized = _fpsFont.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, fpsFontPath.c_str(), 39.0, maxChars);
    if (!_fpsFontInitialized)
    {
        mu::DebugConsoleLog::warning() << "Failed to initialize FPS font";
    }
    else
    {
        mu::DebugConsoleLog::info() << "FPS font loaded successfully";
    }
        
    // Static Text Font - smaller for details
    td::String staticFontPath = gui::getResFileName(":font2");
    _staticTextFontInitialized = _staticTextFont.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, staticFontPath.c_str(), 35.0, maxChars);
    if (!_staticTextFontInitialized)
    {
        mu::DebugConsoleLog::warning() << "Failed to initialize static text font";
    }
    else
    {
        mu::DebugConsoleLog::info() << "Static text font loaded successfully";
    }
    
    // Fancy Font - for special text
    td::String fancyFontPath = gui::getResFileName(":fontFancy");
    _fancyFontInitialized = _fancyFont.init(_commandQueue, glx::PixelFormat::RGBA8Unorm, fancyFontPath.c_str(), 35.0, maxChars);
    if (!_fancyFontInitialized)
    {
        mu::DebugConsoleLog::warning() << "Failed to initialize fancy font";
    }
    else
    {
        mu::DebugConsoleLog::info() << "Fancy font loaded successfully";
            
        // Add multiple test strings using both fonts
        float yPos = 10.0f;
        glx::Size textSize;
        gui::Label trMethod;
        
        textSize = _staticTextFont.addText("Press:\nAlt+Shift+LeftMouse to catch the cursor\nEsc to release the cursor", 10.0, yPos, td::Color(255, 0, 0, 255));
        yPos += textSize.height;

        textSize = _fancyFont.addText("Yet another string\nwith a new line\nand another new line !", 10.0, yPos, td::Color(0, 100, 0, 255));
        yPos += textSize.height;

        textSize = _staticTextFont.addText(trMethod.tr("something"), 10.0, yPos, td::Color(0, 0, 139, 255));
        yPos += textSize.height;

        textSize = _fancyFont.addText("Yet another string\nwith a new line\nand another new line...", 10.0, yPos, td::Color(139, 0, 0, 255));
        yPos += textSize.height;

        _staticTextFont.addText(trMethod.tr("something"), 10.0, yPos, td::Color(199, 21, 133, 255));
    }
}
    
void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");

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

        mu::DebugConsoleLog::info() << "Camera and instancing pipeline created";
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
        
        mu::DebugConsoleLog::info() << "Depth state created";
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
            mu::DebugConsoleLog::error() << "Failed to create texture sampler";
            assert(false);
        }

        mu::DebugConsoleLog::info() << "Texture sampler created";
    }
    
    void loadTextures()
    {
        mu::DebugConsoleLog::info() << "Loading textures for instancing";
        
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
        
        // Create cubemap texture
        createCubemapTexture();
    }
    
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name)
    {
        const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
        const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
        const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
        const size_t targetBytesPerPixel = 4;
 
        mu::DebugConsoleLog::info() << "Uploading " << name << " texture: " << width << "x" << height << ", " << sourceBytesPerPixel << " source bytes per pixel";
        
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
            mu::DebugConsoleLog::debug() << "Direct copy: " << sourceBytesPerPixel << " bytes per pixel";
            memcpy(stagingData, sourceData, targetDataSize);
        }
        else if (sourceBytesPerPixel < targetBytesPerPixel)
        {
            mu::DebugConsoleLog::debug() << "Padding from " << sourceBytesPerPixel << " to " << targetBytesPerPixel << " bytes per pixel";
            
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
        
        mu::DebugConsoleLog::info() << name << " texture uploaded and mipmaps generated";
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
        
        mu::DebugConsoleLog::info() << "Placeholder texture created";
    }
    
    void createCubemapTexture()
    {
        const uint32_t size = 256;

        // Create CubeMap instance - it creates its own texture and manages buffers
        _skyboxCubemap = new CubeMap(_device, _commandQueue, size);

        // Load cubemap from sand texture (horizontal strip)
        gui::Texture cubemapStrip(":sand");
        if (cubemapStrip.isOk())
        {
            _skyboxCubemap->uploadFromHorizontalStrip(cubemapStrip);
        }
        else
        {
            mu::DebugConsoleLog::warning() << "Failed to load sand texture for cubemap, using placeholder";
            // Fallback: Use solid colors if texture not available
            const td::Color faceColors[6] = {
                td::Color(255, 0, 0, 255),      // +X Red
                td::Color(0, 255, 0, 255),      // -X Green
                td::Color(0, 0, 255, 255),      // +Y Blue
                td::Color(255, 255, 0, 255),    // -Y Yellow
                td::Color(255, 0, 255, 255),    // +Z Magenta
                td::Color(0, 255, 255, 255)     // -Z Cyan
            };
            _skyboxCubemap->uploadSolidColors(faceColors);
        }
        
        // Initialize the rendering pipeline for the cubemap
        _skyboxCubemap->initPipeline(glx::PixelFormat::RGBA8Unorm, glx::PixelFormat::Depth32Float);
        
        mu::DebugConsoleLog::info() << "CubeMap texture and pipeline initialized";
    }
    
    void buildObjects()
    {
        CubeMesh cubeMesh = createCube(1.0f);
        CubeMesh sphereMesh = createSphere(0.5f);

        _objects.emplace_back(cubeMesh.vertices, cubeMesh.indices, NUM_INSTANCES);
        _objects.emplace_back(sphereMesh.vertices, sphereMesh.indices, NUM_INSTANCES);
        
        // Generate random instance data for cubes and spheres
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        float pi = glm::pi<float>();
        
        for (uint32_t i = 0; i < NUM_INSTANCES; i++)
        {
            _objects[i % 2].addInstance(InstanceData{
                {
                    (std::rand() / static_cast<float>(RAND_MAX)) * 2.0f * pi,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 2.0f * pi,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 2.0f * pi
                },
                {
                    (std::rand() / static_cast<float>(RAND_MAX)) * 10.0f,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 10.0f,
                    (std::rand() / static_cast<float>(RAND_MAX)) * 10.0f,
                },
                {
                    (std::rand() / static_cast<float>(RAND_MAX) < 0.5f) ? -1.0f : 1.0f,
                    (std::rand() / static_cast<float>(RAND_MAX) < 0.5f) ? -1.0f : 1.0f,
                    (std::rand() / static_cast<float>(RAND_MAX) < 0.5f) ? -1.0f : 1.0f
                }
            });
        }
        
        const size_t uniformDataSize = sizeof(UniformBufferObject);
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::info() << "Scene objects created";
        mu::DebugConsoleLog::info() << "  Cubes: " << cubeMesh.vertices.size() << " vertices";
        mu::DebugConsoleLog::info() << "  Spheres: " << sphereMesh.vertices.size() << " vertices";
        mu::DebugConsoleLog::info() << "  Instances per type: " << NUM_INSTANCES;
    }
    
    void updateCamera()
    {
        const float fov = glm::radians(45.0f);
        const float aspectRatio = _viewportSize.width / _viewportSize.height;
        const float nearClip = 0.5f;
        const float farClip = 200.0f;
        
        _perspectiveMatrix = glm::perspective(fov, aspectRatio, nearClip, farClip);
        
        // Use stored camera position and calculate center point ahead of the camera
        glm::vec3 center = _cameraEye + _cameraForward;
        _viewMatrix = glm::lookAt(_cameraEye, center, _cameraUp);
        
        _cameraProjectionMatrix = _perspectiveMatrix * _viewMatrix;
        
        UniformBufferObject ubo;
        ubo.accAngle = _accAngle;
        ubo.cameraWithProjection = _cameraProjectionMatrix;
        
        memcpy(_uniformBuffer.contents(), &ubo, sizeof(UniformBufferObject));
        glx::Buffer::Range range(0, sizeof(UniformBufferObject));
        _uniformBuffer.didModifyRange(range);
    }
    
    void updateCameraDirection()
    {
        // Update camera forward and right vectors based on yaw and pitch angles
        // Clamp pitch to prevent looking too far up/down
        const float maxPitch = glm::pi<float>() * 0.49f;
        if (_cameraPitch > maxPitch)
            _cameraPitch = maxPitch;
        if (_cameraPitch < -maxPitch)
            _cameraPitch = -maxPitch;
        
        // Calculate new forward vector from yaw and pitch
        float cosYaw = std::cos(_cameraYaw);
        float sinYaw = std::sin(_cameraYaw);
        float cosPitch = std::cos(_cameraPitch);
        float sinPitch = std::sin(_cameraPitch);
        
        _cameraForward.x = sinYaw * cosPitch;
        _cameraForward.y = sinPitch;
        _cameraForward.z = -cosYaw * cosPitch;
        _cameraForward = glm::normalize(_cameraForward);
        
        // Recalculate right vector as perpendicular to forward and world up
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        _cameraRight = glm::normalize(glm::cross(_cameraForward, worldUp));
        
        // Recalculate up vector as perpendicular to forward and right
        _cameraUp = glm::normalize(glm::cross(_cameraRight, _cameraForward));
    }
    
    void updateCameraMovement()
    {
        // Update camera position based on held keys
        if (_keyStates['W'])
            _cameraEye += _cameraForward * _cameraSpeed;
        if (_keyStates['S'])
            _cameraEye -= _cameraForward * _cameraSpeed;
        if (_keyStates['A'])
            _cameraEye -= _cameraRight * _cameraSpeed;
        if (_keyStates['D'])
            _cameraEye += _cameraRight * _cameraSpeed;
    }

public:
Renderer(glx::View* pView)
    : _device(pView->device())
{
    _pView = pView;
    _commandQueue = _device.newCommandQueue();
    pView->getSize(_viewportSize);

    InstancedObject::initializeBuffers(_device);
    
    // Initialize camera angles based on initial forward vector
    // Initial forward is (0, 0, -1), which corresponds to yaw=0, pitch=0
    _cameraYaw = 0.0f;
    _cameraPitch = 0.0f;
    
    // Initialize frame timing
    _lastFrameTime = std::chrono::high_resolution_clock::now();
        
    buildShaders();
    buildSampler();
    buildDepthState();
    buildObjects();
    loadTextures();
    loadFonts();
    updateCamera();

    // Configure View to manage depth texture
        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float);
        pView->setClearDepth(1.0);
        
        mu::DebugConsoleLog::info() << "Camera and instancing renderer initialized";

}

    ~Renderer()
    {
        InstancedObject::releaseBuffers();
        
        _uniformBuffer.release();
        _depthState.release();
        _textureDiceWithNumbers.release();
        _textureDiceWithDots.release();
        _textureEarth.release();
        _textureSand.release();
        
        if (_skyboxCubemap)
        {
            delete _skyboxCubemap;
            _skyboxCubemap = nullptr;
        }
        
        _textureSampler.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        _viewportSize = sz;
        // Depth texture is now managed by glx::View - no manual recreation needed
        updateCamera();
        
        mu::DebugConsoleLog::debug() << "Camera and instancing renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        mu::DebugConsoleLog::debug() << "Animation speed updated to: " << _dAngle;
    }
    
    void switchTexture()
    {
        _usedTexture = (_usedTexture == 0) ? 1 : 0;
        mu::DebugConsoleLog::debug() << "Switched to texture set " << static_cast<int>(_usedTexture);
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
        
        updateCameraMovement();
        updateCamera();
        return true;
    }
    
    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = std::toupper(key.getChar());
            _keyStates[ch] = true;
            
            // Clear opposite directional key
            char opposite = getOppositeKey(ch);
            if (opposite)
                _keyStates[opposite] = false;
        }
    }
    
    void handleKeyReleased(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = std::toupper(key.getChar());
            _keyStates[ch] = false;
        }
    }
    
    void handleCapturedDeltaMove(const gui::Point& delta)
    {
        // Update yaw and pitch based on mouse movement
        _cameraYaw += delta.x * _mouseSensitivity;
        _cameraPitch += delta.y * _mouseSensitivity;
        
        //mu::dbgLog("Delta = (%.1f, %.1f)", delta.x, delta.y);
        
        // Update camera direction vectors
        updateCameraDirection();
    }
    
//    void handleMouseMoved(const gui::Point& pt)
//    {
//        if (_firstMouseMove)
//        {
//            _lastMouseX = pt.x;
//            _lastMouseY = pt.y;
//            _firstMouseMove = false;
//            return;
//        }
//        
//        int deltaX = pt.x - _lastMouseX;
//        int deltaY = pt.y - _lastMouseY;
//        
//        _lastMouseX = pt.x;
//        _lastMouseY = pt.y;
//        
//        // Update yaw and pitch based on mouse movement
//        _cameraYaw += deltaX * _mouseSensitivity;
//        _cameraPitch += deltaY * _mouseSensitivity;
//        
//        // Update camera direction vectors
//        updateCameraDirection();
//    }
    
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
        // Calculate FPS using chrono
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - _lastFrameTime;
        float deltaSeconds = deltaTime.count();
        if (deltaSeconds > 0.0f)
        {
            float instantFps = 1.0f / deltaSeconds;
            
            // Smooth FPS by interpolating between last and current (0.1 = 10% of new value per frame)
            const float smoothingFactor = 0.02f;
            _currentFps = _lastFps + (instantFps - _lastFps) * smoothingFactor;
            _lastFps = _currentFps;
        }
        _lastFrameTime = currentTime;
        
        prepareNextFrame();
        
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

        // Draw skybox first (will be rendered at far plane due to depth settings)
        if (_skyboxCubemap && _skyboxCubemap->isOk())
        {
            // Strip translation from view matrix so skybox is always centered on camera
            glm::mat4 skyboxVP = _perspectiveMatrix * glm::mat4(glm::mat3(_viewMatrix));
            _skyboxCubemap->draw(cmdEnc, skyboxVP);
        }

        // Draw scene objects
        cmdEnc.setRenderPipelineState(_RP);
        cmdEnc.setDepthStencilState(_depthState);
        cmdEnc.setCullMode(glx::CullMode::None);
        cmdEnc.setVertexBuffer(_uniformBuffer, 0, 6);

        if (_usedTexture != 0) {
            cmdEnc.setFragmentTexture(_textureDiceWithNumbers, 3);
            cmdEnc.setFragmentTexture(_textureDiceWithDots, 2);
        } else {
            cmdEnc.setFragmentTexture(_textureEarth, 3);
            cmdEnc.setFragmentTexture(_textureSand, 2);
        }
        
        cmdEnc.setFragmentSamplerState(&_textureSampler, 4);

        for (auto& obj : _objects)
        {
            obj.draw(cmdEnc, 0, 1);
        }
        
        // Draw fonts directly in the same render pass
        if (_fpsFontInitialized && _fpsFont.isOk())
        {
            _fpsFont.reset();
            
            // Format FPS text - centered at top
            std::ostringstream fpsStream;
            fpsStream << std::fixed << std::setprecision(1);
            fpsStream << (const char*)u8"FPS: " << int(_currentFps);
            std::string fpsText = fpsStream.str();

            _fpsFont.addText(fpsText.c_str(), _viewportSize.width * 0.5f, 20.0, td::Color(139, 0, 0, 255), glx::Font::HorizontalAlignment::Center);
            
            // Add frame size info
            std::ostringstream frameSizeStream;
            frameSizeStream << "Visible frame size:\n(" << (int)_viewportSize.width << " x " << (int)_viewportSize.height << ")";
            std::string frameSizeText = frameSizeStream.str();
            _fpsFont.addText(frameSizeText.c_str(), _viewportSize.width - 20.0, 20.0, td::Color(255, 250, 205, 255), glx::Font::HorizontalAlignment::Right);

            
            cmdEnc.drawFont(_fpsFont);
        }
        
        if (_staticTextFontInitialized && _staticTextFont.isOk())
        {
            cmdEnc.drawFont(_staticTextFont);
        }
        
        if (_fancyFontInitialized && _fancyFont.isOk())
        {
            cmdEnc.drawFont(_fancyFont);
        }

        cmdEnc.endEncoding();
        cmBuff.presentDrawable(pView->currentDrawable());
        cmBuff.commit();
    }

    void release() override
    {
        delete this;
    }
};
