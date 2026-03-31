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
#include <gui/Color.h>
#include <td/ColorID.h>
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

    // Render pipelines for main pass and outline pass
    glx::RenderPipeline _mainRP;
    glx::RenderPipeline _outlineRP;

    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;           // Matrix data
    glx::Buffer _lightingUniformBuffer;   // Lighting data
    glx::Buffer _outlineUniformBuffer;

    // Depth stencil states for different passes
    glx::DepthStencilState _mainDepthStencilState;
    glx::DepthStencilState _outlineDepthStencilState;

    // Wood floor textures for lighting demo
    glx::Texture _textureWoodFloor;
    glx::Texture _textureWoodFloorNormal;
    glx::SamplerState _textureSampler1;
    glx::SamplerState _textureSampler2;

    // Scene state
    gui::Size _viewportSize;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _modelMatrix;
    glm::mat4 _modelViewMatrix;
    glm::mat4 _normalMatrix;
    glm::mat4 _mvpMatrix;
    glm::vec3 _lightPos;
    glm::vec3 _lightColor;

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

    // Uniform buffer structures
    struct MatrixUniformBufferObject {
        glm::mat4 v;           // View matrix
        glm::mat4 mv;          // ModelView matrix
        glm::mat4 n;           // Normal matrix
        glm::mat4 mvp;         // ModelViewProjection matrix
    };

    struct LightingUniformBufferObject {
        glm::vec3 lightPos;
        float _padding1;       // Alignment padding
        glm::vec3 lightColor;
        float _padding2;
    };

    struct OutlineUniformBufferObject {
        glm::mat4 m;           // Model matrix
        glm::mat4 v;           // View matrix
        glm::mat4 p;           // Projection matrix
    };

protected:
    void buildShaders();
    void setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc);
    void setupOutlineVertexDescriptor(glx::RenderPipeline::Descriptor& desc);
    void buildDepthStencilStates();
    void buildMainDepthStencilState();
    void buildOutlineDepthStencilState();
    void buildSampler();
    void loadTextures();
    void uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name);
    void createPlaceholderTexture(glx::Texture& targetTexture, uint32_t color);
    void buildBuffers();
    void updateTransforms();

public:
    Renderer(glx::View* pView);
    ~Renderer();

    void onResize(const gui::Size& sz) override final;

    void updateSpeed(float val);
    void switchRotation();
    void updateLightColor(td::ColorID colorID);
    glx::CommandQueue getCommandQueue() const;
    bool prepareNextFrame();
    void draw(glx::View* pView) override;
    void release() override;
};

// Implementation

void Renderer::buildShaders()
{
    td::String path = gui::getResFileName(":shaders");
    path += "/testStencilGLX.spv";

    glx::Error error;
     glx::Library library = _device.newLibrary(path, error);
     if (!library.isOk())
     {
         mu::DebugConsoleLog::error() << "Failed to load shader library: " << error.localizedFailureReason().c_str();
         assert(false);
     }

    // Build main pass pipeline
    glx::Function mainVertexFn = library.newFunction("mainVS");
    glx::Function mainFragFn = library.newFunction("mainFS");

    glx::RenderPipeline::Descriptor mainDesc = glx::RenderPipeline::Descriptor::alloc();
    mainDesc.init();
    mainDesc.setVertexFunction(mainVertexFn);
    mainDesc.setFragmentFunction(mainFragFn);

    setupVertexDescriptor(mainDesc);
    mainDesc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float_Stencil8);
    mainDesc.setStencilAttachmentPixelFormat(glx::PixelFormat::Depth32Float_Stencil8);

    glx::RenderPipeline::ColorAttachments mainClrAttachments = mainDesc.colorAttachments();
    glx::RenderPipeline::ColorAttachment mainClrAtt = mainClrAttachments[0];
    mainClrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

     _mainRP = _device.newRenderPipelineState(mainDesc, error);
     if (!_mainRP.isOk())
     {
         mu::DebugConsoleLog::error() << "Failed to create main render pipeline";
         assert(false);
     }

    // Build outline pass pipeline
    glx::Function outlineVertexFn = library.newFunction("mainVS_outline");
    glx::Function outlineFragFn = library.newFunction("mainFS_outline");

    glx::RenderPipeline::Descriptor outlineDesc = glx::RenderPipeline::Descriptor::alloc();
    outlineDesc.init();
    outlineDesc.setVertexFunction(outlineVertexFn);
    outlineDesc.setFragmentFunction(outlineFragFn);

    setupOutlineVertexDescriptor(outlineDesc);
    outlineDesc.setDepthAttachmentPixelFormat(glx::PixelFormat::Depth32Float_Stencil8);
    outlineDesc.setStencilAttachmentPixelFormat(glx::PixelFormat::Depth32Float_Stencil8);

    glx::RenderPipeline::ColorAttachments outlineClrAttachments = outlineDesc.colorAttachments();
    glx::RenderPipeline::ColorAttachment outlineClrAtt = outlineClrAttachments[0];
    outlineClrAtt.setPixelFormat(glx::PixelFormat::RGBA8Unorm);

    _outlineRP = _device.newRenderPipelineState(outlineDesc, error);
    if (!_outlineRP.isOk())
    {
        mu::DebugConsoleLog::error() << "Failed to create outline render pipeline";
        assert(false);
    }

    // Clean up resources
    mainVertexFn.release();
    mainFragFn.release();
    mainDesc.release();

    outlineVertexFn.release();
    outlineFragFn.release();
    outlineDesc.release();

    library.release();
}

void Renderer::setupVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
{
    glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
    vertexDesc.init();

    glx::VertexAttributes attributes = vertexDesc.attributes();
    glx::VertexBufferLayouts layouts = vertexDesc.layouts();

    // Position (location 0)
    glx::VertexAttribute posAttr = attributes[0];
    posAttr.setFormat(glx::VertexFormat::Float3);
    posAttr.setOffset(offsetof(CubeVertex, position));
    posAttr.setBufferIndex(0);

    // Texture coordinates (location 1)
    glx::VertexAttribute texCoordAttr = attributes[1];
    texCoordAttr.setFormat(glx::VertexFormat::Float2);
    texCoordAttr.setOffset(offsetof(CubeVertex, texCoord));
    texCoordAttr.setBufferIndex(0);

    // Normal (location 2)
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

void Renderer::setupOutlineVertexDescriptor(glx::RenderPipeline::Descriptor& desc)
{
    glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
    vertexDesc.init();

    glx::VertexAttributes attributes = vertexDesc.attributes();
    glx::VertexBufferLayouts layouts = vertexDesc.layouts();

    // Position (location 0)
    glx::VertexAttribute posAttr = attributes[0];
    posAttr.setFormat(glx::VertexFormat::Float3);
    posAttr.setOffset(offsetof(CubeVertex, position));
    posAttr.setBufferIndex(0);

    // Normal (location 2) - needed for outline expansion
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

void Renderer::buildDepthStencilStates()
{
    buildMainDepthStencilState();
    buildOutlineDepthStencilState();
}

void Renderer::buildMainDepthStencilState()
{
    glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
    depthDesc.init();

    // Main pass: enable depth test, write stencil value of 1 where object is rendered
    depthDesc.setDepthCompareFunction(glx::CompareFunction::Less);
    depthDesc.setDepthWriteEnabled(true);

    glx::StencilDescriptor stencilDesc = glx::StencilDescriptor::alloc();
    stencilDesc.init();
    stencilDesc.setStencilCompareFunction(glx::CompareFunction::Always); // Always pass stencil test
    stencilDesc.setStencilFailureOperation(glx::StencilOperation::Keep);
    stencilDesc.setDepthFailureOperation(glx::StencilOperation::Keep);
    stencilDesc.setDepthStencilPassOperation(glx::StencilOperation::Replace); // Write reference value when both depth and stencil pass
    stencilDesc.setReadMask(0xFF);
    stencilDesc.setWriteMask(0xFF);

    depthDesc.setFrontFaceStencil(&stencilDesc);
    depthDesc.setBackFaceStencil(&stencilDesc);

    _mainDepthStencilState = _device.newDepthStencilState(&depthDesc);
    if (!_mainDepthStencilState.isOk())
    {
        mu::DebugConsoleLog::error() << "Failed to create main depth stencil state";
        assert(false);
    }

    stencilDesc.release();
    depthDesc.release();
}

void Renderer::buildOutlineDepthStencilState()
{
    glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
    depthDesc.init();

    // Outline pass: enable depth test and writing
    depthDesc.setDepthCompareFunction(glx::CompareFunction::Less); 
    depthDesc.setDepthWriteEnabled(true); 

    glx::StencilDescriptor stencilDesc = glx::StencilDescriptor::alloc();
    stencilDesc.init();
    stencilDesc.setStencilCompareFunction(glx::CompareFunction::NotEqual); // Only draw where stencil != reference
    stencilDesc.setStencilFailureOperation(glx::StencilOperation::Keep);
    stencilDesc.setDepthFailureOperation(glx::StencilOperation::Keep);
    stencilDesc.setDepthStencilPassOperation(glx::StencilOperation::Keep);
    stencilDesc.setReadMask(0xFF);
    stencilDesc.setWriteMask(0x00); // Don't write to stencil buffer

    depthDesc.setFrontFaceStencil(&stencilDesc);
    depthDesc.setBackFaceStencil(&stencilDesc);

    _outlineDepthStencilState = _device.newDepthStencilState(&depthDesc);
    if (!_outlineDepthStencilState.isOk())
    {
        mu::DebugConsoleLog::error() << "Failed to create outline depth stencil state";
        assert(false);
    }

    stencilDesc.release();
    depthDesc.release();
}

void Renderer::buildSampler()
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
}

void Renderer::loadTextures()
{
    gui::Texture woodFloor(":wood_floor");
    if (woodFloor.isOk())
    {
        uploadTexture(_textureWoodFloor, woodFloor, "wood_floor");
    }
    else
    {
        mu::DebugConsoleLog::warning() << "Failed to load wood floor texture, using placeholder";
        createPlaceholderTexture(_textureWoodFloor, 0x8B4513FF); // Brown color
    }

    gui::Texture woodFloorNormal(":wood_floor_NRM");
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

void Renderer::uploadTexture(glx::Texture& targetTexture, const gui::Texture& sourceTexture, const char* name)
{
    const uint32_t width = static_cast<uint32_t>(sourceTexture.getWidth());
    const uint32_t height = static_cast<uint32_t>(sourceTexture.getHeight());
    const size_t sourceBytesPerPixel = sourceTexture.getBytesPerPixel();
    const size_t targetBytesPerPixel = 4; // RGBA8Unorm requires 4 bytes per pixel 

    mu::DebugConsoleLog::info() << "Uploading " << name << " texture: " << width << "x" << height << ", " << sourceBytesPerPixel << " source bytes per pixel";

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
        memcpy(stagingData, sourceData, targetDataSize);
    }
    else if (sourceBytesPerPixel < targetBytesPerPixel)
    {
        // Convert source format to RGBA
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
                    // RGB -> RGBA
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
        // Truncate if source has more bytes per pixel than target
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
}

void Renderer::createPlaceholderTexture(glx::Texture& targetTexture, uint32_t color)
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
}

void Renderer::buildBuffers()
{
    // Cube vertex data with normals
#define NORMAL_xPLUS 1.0f, 0.0f, 0.0f
#define NORMAL_xMINUS -1.0f, 0.0f, 0.0f
#define NORMAL_yPLUS 0.0f, 1.0f, 0.0f
#define NORMAL_yMINUS 0.0f, -1.0f, 0.0f
#define NORMAL_zPLUS 0.0f, 0.0f, 1.0f
#define NORMAL_zMINUS 0.0f, 0.0f, -1.0f

    float a = 0.5f;
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
    const size_t matrixUniformDataSize = sizeof(MatrixUniformBufferObject);
    const size_t lightingUniformDataSize = sizeof(LightingUniformBufferObject);
    const size_t outlineUniformDataSize = sizeof(OutlineUniformBufferObject);

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

    // Create uniform buffers
    _uniformBuffer = _device.newBuffer(matrixUniformDataSize, glx::Resource::Options::StorageModeManaged);
    _lightingUniformBuffer = _device.newBuffer(lightingUniformDataSize, glx::Resource::Options::StorageModeManaged);
    _outlineUniformBuffer = _device.newBuffer(outlineUniformDataSize, glx::Resource::Options::StorageModeManaged);
}

void Renderer::updateTransforms()
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
    _modelMatrix = glm::mat4(1.0f);
    _modelMatrix = glm::rotate(_modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f));
    _modelMatrix = glm::rotate(_modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    // Calculate combined matrices
    _modelViewMatrix = _viewMatrix * _modelMatrix;
    _normalMatrix = glm::transpose(glm::inverse(_modelViewMatrix));
    _mvpMatrix = _perspectiveMatrix * _modelViewMatrix;

    // Light position
    _lightPos = glm::vec3(-2.0f, 0.0f, 2.0f);

    // Update matrix uniform buffer
    MatrixUniformBufferObject matrixUbo;
    matrixUbo.v = _viewMatrix;
    matrixUbo.mv = _modelViewMatrix;
    matrixUbo.n = _normalMatrix;
    matrixUbo.mvp = _mvpMatrix;

    memcpy(_uniformBuffer.contents(), &matrixUbo, sizeof(MatrixUniformBufferObject));
    glx::Buffer::Range matrixRange(0, sizeof(MatrixUniformBufferObject));
    _uniformBuffer.didModifyRange(matrixRange);

    // Update lighting uniform buffer
    LightingUniformBufferObject lightingUbo;
    lightingUbo.lightPos = _lightPos;
    lightingUbo.lightColor = _lightColor;

    memcpy(_lightingUniformBuffer.contents(), &lightingUbo, sizeof(LightingUniformBufferObject));
    glx::Buffer::Range lightingRange(0, sizeof(LightingUniformBufferObject));
    _lightingUniformBuffer.didModifyRange(lightingRange);

    // Update outline uniform buffer
    OutlineUniformBufferObject outlineUbo;

    float outlineThickness = 0.03f;
    auto scaledModel = glm::scale(_modelMatrix, glm::vec3(1.0f + outlineThickness));
    outlineUbo.m = scaledModel;
    outlineUbo.v = _viewMatrix;
    outlineUbo.p = _perspectiveMatrix;

    memcpy(_outlineUniformBuffer.contents(), &outlineUbo, sizeof(OutlineUniformBufferObject));
    glx::Buffer::Range outlineRange(0, sizeof(OutlineUniformBufferObject));
    _outlineUniformBuffer.didModifyRange(outlineRange);
}

Renderer::Renderer(glx::View* pView)
    : _device(pView->device())
{
    _commandQueue = _device.newCommandQueue();
    //_viewportSize = gui::Size(800, 600);
    pView->getSize(_viewportSize);

    _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    buildShaders();
    buildSampler();
    buildDepthStencilStates();
    buildBuffers();
    loadTextures();
    updateTransforms();

    pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
    pView->setDepthStencilPixelFormat(glx::PixelFormat::Depth32Float_Stencil8);
    pView->setClearDepth(1.0);
    pView->setClearStencil(0);
}

Renderer::~Renderer()
{
    _vertexBuffer.release();
    _indexBuffer.release();
    _uniformBuffer.release();
    _lightingUniformBuffer.release();
    _outlineUniformBuffer.release();
    _mainDepthStencilState.release();
    _outlineDepthStencilState.release();
    _textureWoodFloor.release();
    _textureWoodFloorNormal.release();
    _textureSampler1.release();
    _textureSampler2.release();
    _mainRP.release();
    _outlineRP.release();
    _commandQueue.release();
    _device.release();
}

void Renderer::onResize(const gui::Size& sz)
{
    _viewportSize = sz;
    updateTransforms(); // Recalculate perspective matrix
}

void Renderer::updateSpeed(float val)
{
    _dAngle = val;
    _dAngleAct = _rotation ? _dAngle : 0.0f;
}

void Renderer::switchRotation()
{
    _rotation = !_rotation;
    _dAngleAct = _rotation ? _dAngle : 0.0f;
}

void Renderer::updateLightColor(td::ColorID colorID)
{
    float r, g, b, a;
    gui::Color::getRGBA(colorID, r, g, b, a);
    _lightColor = glm::vec3(r, g, b);
}

glx::CommandQueue Renderer::getCommandQueue() const
{
    return _commandQueue;
}

bool Renderer::prepareNextFrame()
{
    _angleX += _dAngleAct;
    updateTransforms();
    return true;
}

void Renderer::draw(glx::View* pView)
{
    prepareNextFrame();

    glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
    glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();
    

    glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);

    // PASS 1: Draw main object (writes 1 to stencil buffer where object is rendered)
    cmdEnc.setRenderPipelineState(_mainRP);
    cmdEnc.setDepthStencilState(_mainDepthStencilState);
    cmdEnc.setStencilReferenceValue(1);  // Write value 1 to stencil

    cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
    cmdEnc.setVertexBuffer(_uniformBuffer, 0, 1);
    cmdEnc.setVertexBuffer(_lightingUniformBuffer, 0, 2);
    cmdEnc.setFragmentBuffer(_lightingUniformBuffer, 0, 2);

    // Set textures and samplers
    cmdEnc.setFragmentTexture(_textureWoodFloor, 3);
    cmdEnc.setFragmentTexture(_textureWoodFloorNormal, 5);
    cmdEnc.setFragmentSamplerState(&_textureSampler1, 4);
    cmdEnc.setFragmentSamplerState(&_textureSampler2, 6);

    const uint32_t indexCount = 36; // 6 faces * 2 triangles * 3 vertices
    cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);

    // PASS 2: Draw scaled outline (only where stencil != 1, i.e., where main object wasn't drawn)

    cmdEnc.setRenderPipelineState(_outlineRP);
    cmdEnc.setDepthStencilState(_outlineDepthStencilState);
    cmdEnc.setStencilReferenceValue(1);  // Compare against stencil value 1 (written by main pass)

    cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
    cmdEnc.setVertexBuffer(_outlineUniformBuffer, 0, 7);

    cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle, indexCount, glx::IndexType::UInt16, _indexBuffer, 0);

    cmdEnc.endEncoding();
    cmBuff.presentDrawable(pView->currentDrawable());
    cmBuff.commit();
}

void Renderer::release()
{
    delete this;
}