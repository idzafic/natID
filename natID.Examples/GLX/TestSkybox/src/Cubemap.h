#pragma once

#include <glx/Device.h>
#include <glx/Texture.h>
#include <glx/TextureDescriptor.h>
#include <glx/CommandQueue.h>
#include <glx/CommandBuffer.h>
#include <glx/BlitCommandEncoder.h>
#include <glx/RenderCommandEncoder.h>
#include <glx/Buffer.h>
#include <glx/Region.h>
#include <glx/RenderPipeline.h>
#include <glx/Library.h>
#include <glx/Function.h>
#include <glx/Error.h>
#include <glx/VertexDescriptor.h>
#include <glx/SamplerState.h>
#include <glx/SamplerDescriptor.h>
#include <glx/DepthStencilState.h>
#include <glx/DepthStencilDescriptor.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <td/Color.h>
#include <cstdint>
#include <cstring>
#include <vector>
#include <glm/glm.hpp>
#include <mu/DebugConsoleLog.h>


inline size_t alignTo(size_t value, size_t alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

// Vertex structure for cubemap rendering
struct CubeMapVertex
{
    float position[3];
};


class CubeMap
{
private:
    glx::Device _device;
    glx::CommandQueue _commandQueue;
    glx::Texture _texture;
    glx::Buffer _stagingBuffer;
    uint32_t _size = 0;
    
    // Rendering resources
    glx::RenderPipeline _renderPipeline;
    glx::SamplerState _samplerState;
    glx::DepthStencilState _depthStencilState;
    glx::Buffer _vertexBuffer;
    glx::Buffer _indexBuffer;
    glx::Buffer _uniformBuffer;
    size_t _indexCount = 0;
    bool _pipelineBuilt = false;

    // Ensure staging buffer is large enough for the operation
    void ensureStagingBuffer(size_t requiredSize)
    {
        if (!_stagingBuffer.isOk() || _stagingBuffer.length() < requiredSize)
        {
            if (_stagingBuffer.isOk())
            {
                _stagingBuffer.release();
            }
            _stagingBuffer = _device.newBuffer(requiredSize, glx::Resource::Options::StorageModeManaged);
            mu::DebugConsoleLog::debug() << "CubeMap staging buffer allocated: " << requiredSize << " bytes";
        }
    }

    // Upload staged data to GPU
    void uploadToGPU(uint8_t* data, size_t alignedBytesPerRow, size_t alignedFaceOffset)
    {
        glx::Buffer::Range range(0, _stagingBuffer.length());
        _stagingBuffer.didModifyRange(range);

        glx::CommandBuffer cmBuff = _commandQueue.commandBuffer();
        glx::BlitCommandEncoder blitEnc = cmBuff.blitCommandEncoder();

        // Upload each face
        for (uint32_t face = 0; face < 6; ++face)
        {
            glx::Region faceRegion = glx::Region::Make2D(0, 0, _size, _size);
            size_t bufferOffset = face * alignedFaceOffset;

            blitEnc.copyFromBuffer(
                _stagingBuffer,
                bufferOffset,
                alignedBytesPerRow,
                _texture,
                faceRegion,
                0,      // mipLevel
                face    // slice (cubemap face index)
            );
        }

        // Generate mipmaps
        blitEnc.generateMipmaps(_texture);

        blitEnc.endEncoding();
        cmBuff.commit();
        cmBuff.waitUntilCompleted();

        mu::DebugConsoleLog::info() << "CubeMap uploaded to GPU";
    }
    
    // Create cube geometry for skybox
    void createCubeGeometry()
    {
        // Cube vertices (8 corners)
        std::vector<CubeMapVertex> vertices = {
            {{-100.0f, -100.0f, -100.0f}},  // 0
            {{ 100.0f, -100.0f, -100.0f}},  // 1
            {{ 100.0f,  100.0f, -100.0f}},  // 2
            {{-100.0f,  100.0f, -100.0f}},  // 3
            {{-100.0f, -100.0f,  100.0f}},  // 4
            {{ 100.0f, -100.0f,  100.0f}},  // 5
            {{ 100.0f,  100.0f,  100.0f}},  // 6
            {{-100.0f,  100.0f,  100.0f}}   // 7
        };
        
        // Cube indices (36 indices for 12 triangles, 6 faces)
        std::vector<uint16_t> indices = {
            // Back face
            0, 1, 2, 2, 3, 0,
            // Front face
            4, 6, 5, 6, 4, 7,
            // Left face
            4, 0, 3, 3, 7, 4,
            // Right face
            1, 5, 6, 6, 2, 1,
            // Bottom face
            4, 5, 1, 1, 0, 4,
            // Top face
            3, 2, 6, 6, 7, 3
        };
        
        _indexCount = indices.size();
        
        // Create vertex buffer
        const size_t vertexBufferSize = vertices.size() * sizeof(CubeMapVertex);
        _vertexBuffer = _device.newBuffer(vertexBufferSize, glx::Resource::Options::StorageModeManaged);
        std::memcpy(_vertexBuffer.contents(), vertices.data(), vertexBufferSize);
        glx::Buffer::Range vertexRange(0, vertexBufferSize);
        _vertexBuffer.didModifyRange(vertexRange);
        
        // Create index buffer
        const size_t indexBufferSize = indices.size() * sizeof(uint16_t);
        _indexBuffer = _device.newBuffer(indexBufferSize, glx::Resource::Options::StorageModeManaged);
        std::memcpy(_indexBuffer.contents(), indices.data(), indexBufferSize);
        glx::Buffer::Range indexRange(0, indexBufferSize);
        _indexBuffer.didModifyRange(indexRange);
        
        mu::DebugConsoleLog::info() << "CubeMap geometry created: " << vertices.size() << " vertices, " << indices.size() << " indices";
    }
    

    void buildPipeline(glx::PixelFormat colorFormat, glx::PixelFormat depthFormat)
    {
        if (_pipelineBuilt)
        {
            mu::DebugConsoleLog::warning() << "CubeMap pipeline already built";
            return;
        }
        
        // Load shader library
        td::String shaderPath = gui::getResFileName(":_NATID_cubemap");
        glx::Error error;
        glx::Library library = _device.newLibrary(shaderPath, error);
        
        if (!library.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to load cubemap shader library: " << err.c_str();
            throw std::runtime_error("Failed to load cubemap shader library");
        }
        
   
        glx::Function vertexFn = library.newFunction("mainCubeVS");
        glx::Function fragmentFn = library.newFunction("mainCubeFS");
        

        glx::RenderPipeline::Descriptor pipelineDesc = glx::RenderPipeline::Descriptor::alloc();
        pipelineDesc.init();
        pipelineDesc.setVertexFunction(vertexFn);
        pipelineDesc.setFragmentFunction(fragmentFn);
        
        // Setup vertex descriptor
        glx::VertexDescriptor vertexDesc = glx::VertexDescriptor::alloc();
        vertexDesc.init();
        
        glx::VertexAttributes attributes = vertexDesc.attributes();
        glx::VertexBufferLayouts layouts = vertexDesc.layouts();
        
        // Position attribute
        glx::VertexAttribute posAttr = attributes[0];
        posAttr.setFormat(glx::VertexFormat::Float3);
        posAttr.setOffset(0);
        posAttr.setBufferIndex(0);
        
        // Vertex buffer layout
        glx::VertexBufferLayout layout = layouts[0];
        layout.setStride(sizeof(CubeMapVertex));
        layout.setStepRate(1);
        layout.setStepFunction(glx::VertexStepFunction::PerVertex);
        
        pipelineDesc.setVertexDescriptor(vertexDesc);
        
        // Set render target formats
        pipelineDesc.setDepthAttachmentPixelFormat(depthFormat);
        glx::RenderPipeline::ColorAttachments colorAttachments = pipelineDesc.colorAttachments();
        glx::RenderPipeline::ColorAttachment colorAtt = colorAttachments[0];
        colorAtt.setPixelFormat(colorFormat);
        

        colorAtt.setBlendingEnabled(true);

        /* RGB blending */
        colorAtt.setSourceRGBBlendFactor(glx::BlendFactor::SourceAlpha);
        colorAtt.setDestinationRGBBlendFactor(glx::BlendFactor::OneMinusSourceAlpha);
        colorAtt.setRgbBlendOperation(glx::BlendOperation::Add);

        /* Alpha blending */
        colorAtt.setSourceAlphaBlendFactor(glx::BlendFactor::One);
        colorAtt.setDestinationAlphaBlendFactor(glx::BlendFactor::OneMinusSourceAlpha);
        colorAtt.setAlphaBlendOperation(glx::BlendOperation::Add);
        
  
        _renderPipeline = _device.newRenderPipelineState(pipelineDesc, error);
        
        if (!_renderPipeline.isOk())
        {
            auto err = error.localizedFailureReason();
            mu::DebugConsoleLog::error() << "Failed to create cubemap render pipeline: " << err.c_str();
            vertexDesc.release();
            pipelineDesc.release();
            vertexFn.release();
            fragmentFn.release();
            library.release();
            throw std::runtime_error("Failed to create cubemap render pipeline");
        }
        
     
        vertexDesc.release();
        pipelineDesc.release();
        vertexFn.release();
        fragmentFn.release();
        library.release();
        
        // Create sampler state
        glx::SamplerDescriptor samplerDesc = glx::SamplerDescriptor::alloc();
        samplerDesc.init();
        samplerDesc.setMinFilter(glx::Filter::Linear);
        samplerDesc.setMagFilter(glx::Filter::Linear);
        samplerDesc.setMipFilter(glx::MipFilter::Linear);
        samplerDesc.setSAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setTAddressMode(glx::SamplerAddressMode::ClampToEdge);
        samplerDesc.setRAddressMode(glx::SamplerAddressMode::ClampToEdge);
        
        _samplerState = _device.newSamplerState(&samplerDesc);
        samplerDesc.release();
        
        if (!_samplerState.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create cubemap sampler state";
            throw std::runtime_error("Failed to create cubemap sampler state");
        }
        
        // Create depth stencil state for skybox (render at far plane)
        glx::DepthStencilDescriptor depthDesc = glx::DepthStencilDescriptor::alloc();
        depthDesc.init();
        depthDesc.setDepthCompareFunction(glx::CompareFunction::LessEqual);  // LessEqual for skybox
        depthDesc.setDepthWriteEnabled(false);  // Don't write depth for skybox
        
        _depthStencilState = _device.newDepthStencilState(&depthDesc);
        depthDesc.release();
        
        if (!_depthStencilState.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create cubemap depth stencil state";
            throw std::runtime_error("Failed to create cubemap depth stencil state");
        }
        
        // Create uniform buffer (mat4 only, translation stripped on CPU side)
        const size_t uniformSize = sizeof(glm::mat4);
        _uniformBuffer = _device.newBuffer(uniformSize, glx::Resource::Options::StorageModeManaged);
        
        // Create geometry
        createCubeGeometry();
        
        _pipelineBuilt = true;
        mu::DebugConsoleLog::info() << "CubeMap pipeline built successfully";
    }

public:
    CubeMap(glx::Device device, glx::CommandQueue commandQueue, uint32_t size, glx::PixelFormat format = glx::PixelFormat::RGBA8Unorm, bool mipmapped = true)
        : _device(device), _commandQueue(commandQueue), _size(size)
    {
        // Create cubemap texture
        glx::TextureDescriptor desc = glx::TextureDescriptor::textureCubeDescriptor(format, size, mipmapped);
        desc.setUsage(glx::TextureUsage::ShaderRead);
        desc.setStorageMode(glx::Resource::StorageMode::Private);
        desc.setMipmapLevelCount(6);

        _texture = _device.newTexture(&desc);
        if (!_texture.isOk())
        {
            mu::DebugConsoleLog::error() << "Failed to create CubeMap texture";
            throw std::runtime_error("Failed to create CubeMap texture");
        }

        mu::DebugConsoleLog::info() << "CubeMap created: " << size << "x" << size;
    }

    ~CubeMap()
    {
        if (_stagingBuffer.isOk())
        {
            _stagingBuffer.release();
        }
        if (_texture.isOk())
        {
            _texture.release();
        }
        if (_renderPipeline.isOk())
        {
            _renderPipeline.release();
        }
        if (_samplerState.isOk())
        {
            _samplerState.release();
        }
        if (_depthStencilState.isOk())
        {
            _depthStencilState.release();
        }
        if (_vertexBuffer.isOk())
        {
            _vertexBuffer.release();
        }
        if (_indexBuffer.isOk())
        {
            _indexBuffer.release();
        }
        if (_uniformBuffer.isOk())
        {
            _uniformBuffer.release();
        }
    }

    // Upload solid colors to cubemap (one color per face)
    // faceColors: Array of 6 colors (td::Color RGBA) for +X, -X, +Y, -Y, +Z, -Z
    void uploadSolidColors(const td::Color faceColors[6])
    {
        const size_t bytesPerPixel = 4;  // RGBA8Unorm
        const size_t bytesPerRow = _size * bytesPerPixel;
        const size_t alignedBytesPerRow = alignTo(bytesPerRow, 4);
        const size_t alignedFaceDataSize = alignedBytesPerRow * _size;
        const size_t alignedFaceOffset = alignTo(alignedFaceDataSize, 4);
        const size_t totalDataSize = alignedFaceOffset * 6;

        ensureStagingBuffer(totalDataSize);
        uint8_t* data = static_cast<uint8_t*>(_stagingBuffer.contents());

        // Fill each face with solid color
        for (uint32_t face = 0; face < 6; ++face)
        {
            uint8_t* faceData = data + (face * alignedFaceOffset);
            const td::Color& color = faceColors[face];

            for (uint32_t y = 0; y < _size; ++y)
            {
                for (uint32_t x = 0; x < _size; ++x)
                {
                    size_t pixelIndex = y * alignedBytesPerRow + x * bytesPerPixel;
                    faceData[pixelIndex + 0] = color.R();
                    faceData[pixelIndex + 1] = color.G();
                    faceData[pixelIndex + 2] = color.B();
                    faceData[pixelIndex + 3] = color.A();
                }
            }
        }

        uploadToGPU(data, alignedBytesPerRow, alignedFaceOffset);
    }

    // Upload from a horizontal strip texture containing all 6 faces in one row
    // stripTexture: Texture with all faces laid out horizontally: [+X] [-X] [+Z] [-Z] [Sky] [Ground]
    // Expected dimensions: width = size*6, height = size
    void uploadFromHorizontalStrip(const gui::Texture& stripTexture)
    {
        if (!stripTexture.isOk())
        {
            mu::DebugConsoleLog::error() << "CubeMap strip texture is invalid";
            return;
        }

        // Validate dimensions
        uint32_t expectedWidth = _size * 6;
        if (stripTexture.getWidth() != expectedWidth || stripTexture.getHeight() != _size)
        {
            mu::DebugConsoleLog::error() << "CubeMap strip texture size mismatch: expected " << expectedWidth << "x" << _size 
                                          << ", got " << stripTexture.getWidth() << "x" << stripTexture.getHeight();
            return;
        }

        const size_t bytesPerPixel = 4;  // RGBA8Unorm
        const size_t bytesPerRow = _size * bytesPerPixel;
        const size_t alignedBytesPerRow = alignTo(bytesPerRow, 4);
        const size_t alignedFaceDataSize = alignedBytesPerRow * _size;
        const size_t alignedFaceOffset = alignTo(alignedFaceDataSize, 4);
        const size_t totalDataSize = alignedFaceOffset * 6;

        ensureStagingBuffer(totalDataSize);
        uint8_t* stagingData = static_cast<uint8_t*>(_stagingBuffer.contents());

        const uint8_t* srcData = static_cast<const uint8_t*>(stripTexture.getBytes());
        const size_t srcBytesPerPixel = stripTexture.getBytesPerPixel();
        const size_t srcBytesPerRow = stripTexture.getWidth() * srcBytesPerPixel;

        // Map strip positions to cubemap faces
        // Strip layout: [+X] [-X] [+Z] [-Z] [Sky] [Ground]
        // Cubemap faces: 0=+X, 1=-X, 2=+Y(top), 3=-Y(bottom), 4=+Z, 5=-Z
        const uint32_t stripToFaceMap[6] = {
            1,  // Strip pos 0 -> Face 0 (+X right)
            4,  // Strip pos 1 -> Face 1 (-X left) 4
            0,  // Strip pos 2 -> Face 4 (+Z front) 0
            5,  // Strip pos 3 -> Face 5 (-Z back) 5
            2,  // Strip pos 4 -> Face 2 (+Y top/sky)
            3   // Strip pos 5 -> Face 3 (-Y bottom/ground)
        };

        // Extract each face from the horizontal strip
        for (uint32_t stripPos = 0; stripPos < 6; ++stripPos)
        {
            uint32_t faceIdx = stripToFaceMap[stripPos];
            uint8_t* destFaceData = stagingData + (faceIdx * alignedFaceOffset);
            uint32_t faceOffsetX = stripPos * _size;  // Each face in strip starts at x = stripPos * _size

            // Copy each pixel from the strip to the destination face (with Y-flip)
            for (uint32_t y = 0; y < _size; ++y)
            {
                uint32_t invertedY = _size - 1 - y;  // Flip Y coordinate
                
                for (uint32_t x = 0; x < _size; ++x)
                {
                    const size_t srcPixelIndex = (invertedY * stripTexture.getWidth() + faceOffsetX + x) * srcBytesPerPixel;
                    const size_t destPixelIndex = y * alignedBytesPerRow + x * bytesPerPixel;

                    if (srcBytesPerPixel == 4)
                    {
                        // Direct copy
                        destFaceData[destPixelIndex + 0] = srcData[srcPixelIndex + 0];
                        destFaceData[destPixelIndex + 1] = srcData[srcPixelIndex + 1];
                        destFaceData[destPixelIndex + 2] = srcData[srcPixelIndex + 2];
                        destFaceData[destPixelIndex + 3] = srcData[srcPixelIndex + 3];
                    }
                    else if (srcBytesPerPixel == 3)
                    {
                        destFaceData[destPixelIndex + 0] = srcData[srcPixelIndex + 0];
                        destFaceData[destPixelIndex + 1] = srcData[srcPixelIndex + 1];
                        destFaceData[destPixelIndex + 2] = srcData[srcPixelIndex + 2];
                        destFaceData[destPixelIndex + 3] = 255;
                    }
                    else if (srcBytesPerPixel == 1)
                    {
                        uint8_t gray = srcData[srcPixelIndex];
                        destFaceData[destPixelIndex + 0] = gray;
                        destFaceData[destPixelIndex + 1] = gray;
                        destFaceData[destPixelIndex + 2] = gray;
                        destFaceData[destPixelIndex + 3] = 255;
                    }
                }
            }
        }

        uploadToGPU(stagingData, alignedBytesPerRow, alignedFaceOffset);
        mu::DebugConsoleLog::info() << "CubeMap uploaded from horizontal strip texture";
    }

    // Upload raw RGBA8 data
    // faceData: Array of 6 pointers to face data (each face must be size x size x 4 bytes)
    void uploadRGBA8(const uint8_t* const faceData[6])
    {
        const size_t bytesPerPixel = 4;
        const size_t bytesPerRow = _size * bytesPerPixel;
        const size_t alignedBytesPerRow = alignTo(bytesPerRow, 4);
        const size_t alignedFaceDataSize = alignedBytesPerRow * _size;
        const size_t alignedFaceOffset = alignTo(alignedFaceDataSize, 4);
        const size_t totalDataSize = alignedFaceOffset * 6;

        ensureStagingBuffer(totalDataSize);
        uint8_t* stagingData = static_cast<uint8_t*>(_stagingBuffer.contents());

        // Copy and align each face
        for (uint32_t face = 0; face < 6; ++face)
        {
            uint8_t* destFaceData = stagingData + (face * alignedFaceOffset);
            const uint8_t* srcFaceData = faceData[face];

            if (bytesPerRow == alignedBytesPerRow)
            {
                // Direct copy if no padding needed
                std::memcpy(destFaceData, srcFaceData, bytesPerRow * _size);
            }
            else
            {
                // Copy row by row with padding
                for (uint32_t y = 0; y < _size; ++y)
                {
                    std::memcpy(
                        destFaceData + y * alignedBytesPerRow,
                        srcFaceData + y * bytesPerRow,
                        bytesPerRow
                    );
                }
            }
        }

        uploadToGPU(stagingData, alignedBytesPerRow, alignedFaceOffset);
    }

    // Accessors
    glx::Texture& getTexture() { return _texture; }
    const glx::Texture& getTexture() const { return _texture; }
    uint32_t getSize() const { return _size; }
    bool isOk() const { return _texture.isOk() && _pipelineBuilt; }
    
    // Initialize pipeline for rendering
    void initPipeline(glx::PixelFormat colorFormat, glx::PixelFormat depthFormat)
    {
        if (!_pipelineBuilt)
        {
            buildPipeline(colorFormat, depthFormat);
        }
    }
    
    // Draw the cubemap
    // viewProjectionMatrix: projection * mat4(mat3(view)) - translation must be stripped by the caller
    void draw(glx::RenderCommandEncoder& encoder, const glm::mat4& viewProjectionMatrix)
    {
        if (!_pipelineBuilt)
        {
            mu::DebugConsoleLog::error() << "CubeMap pipeline not built. Call initPipeline() first.";
            return;
        }
        
        if (!_texture.isOk())
        {
            mu::DebugConsoleLog::error() << "CubeMap texture not ready";
            return;
        }
        
        // Update uniform buffer with view-projection matrix (translation already stripped)
        std::memcpy(_uniformBuffer.contents(), &viewProjectionMatrix, sizeof(glm::mat4));
        glx::Buffer::Range range(0, sizeof(glm::mat4));
        _uniformBuffer.didModifyRange(range);
        
        // Set pipeline state
        encoder.setRenderPipelineState(_renderPipeline);
        encoder.setDepthStencilState(_depthStencilState);
        
        // Set vertex buffer at slot 0
        encoder.setVertexBuffer(_vertexBuffer, 0, 0);
        
        // Set uniform buffer at slot 1 (matching shader binding 1)
        encoder.setVertexBuffer(_uniformBuffer, 0, 1);
        
        // Set cubemap texture at binding 3 and sampler at binding 4 (matching fragment shader)
        encoder.setFragmentTexture(_texture, 3);
        encoder.setFragmentSamplerState(&_samplerState, 4);
        
        // Set cull mode (cull front faces for skybox viewed from inside)
        encoder.setCullMode(glx::CullMode::Front);
        
        // Draw the cube
        encoder.drawIndexedPrimitives(
            glx::PrimitiveType::Triangle,
            _indexCount,
            glx::IndexType::UInt16,
            _indexBuffer,
            0
        );
    }
};