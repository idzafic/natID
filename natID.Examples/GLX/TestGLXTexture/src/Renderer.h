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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Renderer : public glx::IRenderer
{
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::RenderPipeline _RP;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    
    glx::Texture _textureBrick;
    glx::Texture _textureNature;
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
        glm::vec4 params;
    };

    // Shape rendering data
    size_t _squareIndexStart = 0;
    size_t _squareIndexCount = 0;
    size_t _squareVertexStart = 0;
    size_t _pentagonIndexStart = 0;
    size_t _pentagonIndexCount = 0;
    size_t _pentagonVertexStart = 0;
    size_t _hexagonIndexStart = 0;
    size_t _hexagonIndexCount = 0;
    size_t _hexagonVertexStart = 0;
    size_t _circleIndexStart = 0;
    size_t _circleIndexCount = 0;
    size_t _circleVertexStart = 0;
    size_t _totalVertexCount = 0;
    size_t _totalIndexCount = 0;

protected:
    void buildShaders()
    {
        td::String path;
        glx::Error error;
        glx::Library library;

        path = gui::getResFileName(":shaders");
        path += "/testTextureGLX";

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

        mu::DebugConsoleLog::info() << "Texture renderer pipeline created";
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
        posAttr.setOffset(offsetof(TextureVertex, position));
        posAttr.setBufferIndex(0);

        // Texture coordinate attribute
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
        
        _textureSampler = _device.newSamplerState(&samplerDesc);
        if (!_textureSampler.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create texture sampler";
            assert(false);
        }

        mu::DebugConsoleLog::info() << "Texture sampler created";
        samplerDesc.release();
    }
    
    void loadTextures()
    {
        mu::DebugConsoleLog::info() << "Loading textures...";
        
        gui::Texture brickTexture(":brickWall");
        if (brickTexture.isOk())
        {
            uploadTexture(_textureBrick, brickTexture, "brick_wall");
        }
        else
        {
            mu::DebugConsoleLog::info() << "WARNING: Failed to load brick texture, using placeholder";
            createPlaceholderTexture(_textureBrick, 0x8B4513FF);
        }
        
        gui::Texture natureTexture(":nature");
        if (natureTexture.isOk())
        {
            uploadTexture(_textureNature, natureTexture, "nature");
        }
        else
        {
            mu::DebugConsoleLog::info() << "WARNING: Failed to load nature texture, using placeholder";
            createPlaceholderTexture(_textureNature, 0x228B22FF);
        }
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
        
        const size_t targetDataSize = width * height * targetBytesPerPixel;
        
        glx::Buffer stagingBuffer = _device.newBuffer(targetDataSize, glx::Resource::Options::StorageModeManaged);
        uint8_t* stagingData = static_cast<uint8_t*>(stagingBuffer.contents());
        const uint8_t* sourceData = static_cast<const uint8_t*>(sourceTexture.getBytes());
        
        // Convert texture format if needed
        if (sourceBytesPerPixel == targetBytesPerPixel)
        {
            memcpy(stagingData, sourceData, targetDataSize);
        }
        else if (sourceBytesPerPixel == 3 && targetBytesPerPixel == 4)
        {
            // RGB -> RGBA conversion
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
            true
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
        blitEnc.generateMipmaps(targetTexture);
        
        blitEnc.endEncoding();
        cmBuff.commit();
        cmBuff.waitUntilCompleted();
        
        stagingBuffer.release();
        
        mu::DebugConsoleLog::info() << "Placeholder texture created";
    }

    void generatePolygonVertices(float centerX, float centerY, float radius, int numSides, 
                                 float startAngle, std::vector<TextureVertex>& vertices, std::vector<uint16_t>& indices)
    {
        vertices.clear();
        indices.clear();
        
        uint16_t startIdx = 0;
        
        // Center vertex
        vertices.push_back({{centerX, centerY}, {0.5f, 0.5f}});
        
        // Outer vertices
        for (int i = 0; i <= numSides; ++i)
        {
            float angle = startAngle + (i * 2.0f * M_PI) / numSides;
            float x = centerX + radius * cos(angle);
            float y = centerY + radius * sin(angle);
            
            float u = 0.5f + 0.5f * cos(angle);
            float v = 0.5f + 0.5f * sin(angle);
            
            vertices.push_back({{x, y}, {u, v}});
        }
        
        // Convert triangle fan to triangles
        for (int i = 1; i < numSides + 1; ++i) {
            indices.push_back(startIdx);
            indices.push_back(startIdx+i);
            indices.push_back(startIdx + i + 1);
        }
    }
    
    void buildBuffers()
    {
        // Release old buffers if they exist
        if (_vertexBuffer.isOk())
            _vertexBuffer.release();
        if (_indexBuffer.isOk())
            _indexBuffer.release();
        if (_uniformBuffer.isOk())
            _uniformBuffer.release();
        
        mu::DebugConsoleLog::info() << "Building texture renderer buffers...";
        
        std::vector<TextureVertex> allVertices;
        std::vector<uint16_t> allIndices;
        allVertices.reserve(1024);
        allIndices.reserve(2048);
        
        float centerX = _viewportSize.width * 0.5f;
        float centerY = _viewportSize.height * 0.5f;
        
        // Square (upper right quadrant)
        _squareVertexStart = allVertices.size();
        _squareIndexStart = allIndices.size();
        std::vector<TextureVertex> squareVerts;
        std::vector<uint16_t> squareIndices;
        float squareCenterX = centerX + 150.0f;
        float squareCenterY = centerY - 100.0f;
        generatePolygonVertices(squareCenterX, squareCenterY, 90.0f, 4, M_PI/4.0f, squareVerts, squareIndices);
        
        allVertices.insert(allVertices.end(), squareVerts.begin(), squareVerts.end());
        allIndices.insert(allIndices.end(), squareIndices.begin(), squareIndices.end());
        _squareIndexCount = squareIndices.size();
        
        // Pentagon (upper left quadrant)
        _pentagonVertexStart = allVertices.size();
        _pentagonIndexStart = allIndices.size();
        std::vector<TextureVertex> pentagonVerts;
        std::vector<uint16_t> pentagonIndices;
        float pentagonCenterX = centerX - 150.0f;
        float pentagonCenterY = centerY - 100.0f;
        generatePolygonVertices(pentagonCenterX, pentagonCenterY, 90.0f, 5, 0.0f, pentagonVerts, pentagonIndices);
        
        allVertices.insert(allVertices.end(), pentagonVerts.begin(), pentagonVerts.end());
        allIndices.insert(allIndices.end(), pentagonIndices.begin(), pentagonIndices.end());
        _pentagonIndexCount = pentagonIndices.size();
        
        // Hexagon (bottom left quadrant)
        _hexagonVertexStart = allVertices.size();
        _hexagonIndexStart = allIndices.size();
        std::vector<TextureVertex> hexagonVerts;
        std::vector<uint16_t> hexagonIndices;
        float hexagonCenterX = centerX - 150.0f;
        float hexagonCenterY = centerY + 100.0f;
        generatePolygonVertices(hexagonCenterX, hexagonCenterY, 90.0f, 6, 0.0f, hexagonVerts, hexagonIndices);
        
        allVertices.insert(allVertices.end(), hexagonVerts.begin(), hexagonVerts.end());
        allIndices.insert(allIndices.end(), hexagonIndices.begin(), hexagonIndices.end());
        _hexagonIndexCount = hexagonIndices.size();
        
        // Circle (bottom right quadrant)
        _circleVertexStart = allVertices.size();
        _circleIndexStart = allIndices.size();
        std::vector<TextureVertex> circleVerts;
        std::vector<uint16_t> circleIndices;
        float circleCenterX = centerX + 150.0f;
        float circleCenterY = centerY + 100.0f;
        generatePolygonVertices(circleCenterX, circleCenterY, 90.0f, 72, 0.0f, circleVerts, circleIndices);
        
        allVertices.insert(allVertices.end(), circleVerts.begin(), circleVerts.end());
        allIndices.insert(allIndices.end(), circleIndices.begin(), circleIndices.end());
        _circleIndexCount = circleIndices.size();
        
        _totalVertexCount = allVertices.size();
        _totalIndexCount = allIndices.size();
        
        const size_t vertexDataSize = allVertices.size() * sizeof(TextureVertex);
        const size_t indexDataSize = allIndices.size() * sizeof(uint16_t);
        const size_t uniformDataSize = sizeof(TextureUniforms);
        
        _vertexBuffer = _device.newBuffer(vertexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_vertexBuffer.contents(), allVertices.data(), vertexDataSize);
        glx::Buffer::Range vertexRange(0, _vertexBuffer.length());
        _vertexBuffer.didModifyRange(vertexRange);
        
        _indexBuffer = _device.newBuffer(indexDataSize, glx::Resource::Options::StorageModeManaged);
        memcpy(_indexBuffer.contents(), allIndices.data(), indexDataSize);
        glx::Buffer::Range indexRange(0, _indexBuffer.length());
        _indexBuffer.didModifyRange(indexRange);
        
        _uniformBuffer = _device.newBuffer(uniformDataSize, glx::Resource::Options::StorageModeManaged);
        
        mu::DebugConsoleLog::info() << "Texture renderer buffers created";
        mu::DebugConsoleLog::info() << "  Vertex buffer: " << vertexDataSize << " bytes (" << _totalVertexCount << " total vertices)";
        mu::DebugConsoleLog::info() << "  Index buffer: " << indexDataSize << " bytes (" << _totalIndexCount << " total indices)";
        mu::DebugConsoleLog::info() << "  Square: " << _squareIndexCount << " indices at center (" << squareCenterX << ", " << squareCenterY << "), vertex start: " << _squareVertexStart;
        mu::DebugConsoleLog::info() << "  Pentagon: " << _pentagonIndexCount << " indices at center (" << pentagonCenterX << ", " << pentagonCenterY << "), vertex start: " << _pentagonVertexStart;
        mu::DebugConsoleLog::info() << "  Hexagon: " << _hexagonIndexCount << " indices at center (" << hexagonCenterX << ", " << hexagonCenterY << "), vertex start: " << _hexagonVertexStart;
        mu::DebugConsoleLog::info() << "  Circle: " << _circleIndexCount << " indices at center (" << circleCenterX << ", " << circleCenterY << "), vertex start: " << _circleVertexStart;
        mu::DebugConsoleLog::info() << "  Uniform buffer: " << uniformDataSize << " bytes";
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
        
        memcpy(_uniformBuffer.contents(), &uniforms, sizeof(TextureUniforms));
        glx::Buffer::Range range(0, sizeof(TextureUniforms));
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
        loadTextures();
        buildBuffers();

        
        updateMVPMatrix();

        pView->setPixelFormat(glx::PixelFormat::RGBA8Unorm);
        
        mu::DebugConsoleLog::info() << "Texture renderer initialized";
        mu::DebugConsoleLog::info() << "  Viewport: " << _viewportSize.width << "x" << _viewportSize.height;
    }

    ~Renderer()
    {
        _vertexBuffer.release();
        _indexBuffer.release();
        _uniformBuffer.release();
        _textureBrick.release();
        _textureNature.release();
        _textureSampler.release();
        _RP.release();
        _commandQueue.release();
        _device.release();
    }

    void onResize(const gui::Size& sz) override final
    {
        static bool firstResize = true;
        
        _viewportSize = sz;
        
        if (firstResize)
        {
            buildBuffers();
            firstResize = false;
        }
        
        updateMVPMatrix();
        mu::DebugConsoleLog::info() << "Texture renderer viewport resized to: " << sz.width << "x" << sz.height;
    }
    
    void setTextureScale(float scale)
    {
        _textureScale = scale;
        updateMVPMatrix();
        mu::DebugConsoleLog::info() << "Texture scale set to: " << _textureScale;
    }
    
    void increaseTextureScale()
    {
        if (_textureScale >= 5.0f)
            return;
        _textureScale += 0.1f;
        updateMVPMatrix();
        mu::DebugConsoleLog::info() << "Texture scale increased to: " << _textureScale;
    }
    
    void decreaseTextureScale()
    {
        if (_textureScale <= 0.11f)
            return;
        _textureScale -= 0.1f;
        updateMVPMatrix();
        mu::DebugConsoleLog::info() << "Texture scale decreased to: " << _textureScale;
    }
    
    void switchTexture()
    {
        _usedTexture = (_usedTexture == 0) ? 1 : 0;
        mu::DebugConsoleLog::info() << "Switched to " << ((_usedTexture == 0) ? "brick" : "nature") << " texture";
    }
    
    void handleKeyPressed(const gui::Key& key)
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch == 'G' || ch == 'g')
            {
                increaseTextureScale();
            }
            else if (ch == 'H' || ch == 'h')
            {
                decreaseTextureScale();
            }
            else if (ch == 'T' || ch == 't')
            {
                switchTexture();
            }
        }
    }
    
    void handleLeftButtonPressed(const gui::Point& pt)
    {
        increaseTextureScale();
        mu::DebugConsoleLog::info() << "Left click at (" << pt.x << ", " << pt.y << ") - texture scale increased";
    }
    
    void handleRightButtonPressed(const gui::Point& pt)
    {
        decreaseTextureScale();
        mu::DebugConsoleLog::info() << "Right click at (" << pt.x << ", " << pt.y << ") - texture scale decreased";
    }
    
    glx::CommandQueue getCommandQueue() const
    {
        return _commandQueue;
    }
    
private:
    void draw(glx::View* pView) override
    {
        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::RenderPassDescriptor rpd = pView->currentRenderPassDescriptor();

        auto setupRenderState = [&](glx::RenderCommandEncoder& cmdEnc) {
            cmdEnc.setFrontFacingWinding(glx::Winding::Clockwise);
            cmdEnc.setCullMode(glx::CullMode::Back);
            cmdEnc.setRenderPipelineState(_RP);
            cmdEnc.setVertexBuffer(_vertexBuffer, 0, 0);
            cmdEnc.setVertexBuffer(_uniformBuffer, 0, 3);

            const glx::Texture& currentTexture = (_usedTexture == 0) ? _textureBrick : _textureNature;
            cmdEnc.setFragmentTexture(currentTexture, 1);
            cmdEnc.setFragmentSamplerState(&_textureSampler, 2);
        };

        {
            glx::RenderCommandEncoder cmdEnc = cmBuff.renderCommandEncoder(rpd);
            setupRenderState(cmdEnc);

            // Draw square
            if (_squareIndexCount > 0) {
                cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle,
                    _squareIndexCount,
                    glx::IndexType::UInt16, _indexBuffer,
                    _squareIndexStart * sizeof(uint16_t),
                    1, _squareVertexStart, 0);
            }

            // Draw pentagon
            if (_pentagonIndexCount > 0) {
                cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle,
                    _pentagonIndexCount,
                    glx::IndexType::UInt16, _indexBuffer,
                    _pentagonIndexStart * sizeof(uint16_t),
                    1, _pentagonVertexStart, 0);
            }

            // Draw hexagon
            if (_hexagonIndexCount > 0) {
                cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle,
                    _hexagonIndexCount,
                    glx::IndexType::UInt16, _indexBuffer,
                    _hexagonIndexStart * sizeof(uint16_t),
                    1, _hexagonVertexStart, 0);
            }

            // Draw circle
            if (_circleIndexCount > 0) {
                cmdEnc.drawIndexedPrimitives(glx::PrimitiveType::Triangle,
                    _circleIndexCount,
                    glx::IndexType::UInt16, _indexBuffer,
                    _circleIndexStart * sizeof(uint16_t),
                    1, _circleVertexStart, 0);
            }

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

