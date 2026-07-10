// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Texture.h
    @brief Declares the glx::Texture class representing a GPU texture resource. */
#pragma once
#include "natGLX.h"
#include <td/String.h>
#include "Device.h"

// Forward declaration for Region
namespace glx
{
    class Region;
}

namespace MTL
{
class Texture;
class Drawable;
} //namespace MTL

namespace glx
{

class BlitCommandEncoder;
class RenderPassDescriptor;
class RenderCommandEncoder;
class ComputeCommandEncoder;


/// @brief Represents a GPU texture resource that can be used for rendering, sampling, or compute operations.
class NATGLX_API Texture
{
    friend class BlitCommandEncoder;
    friend class RenderPassDescriptor;
    friend class RenderCommandEncoder;
    friend class ComputeCommandEncoder;
    
    friend class MTL::Drawable;
    
public:
    /// @brief Describes how a texture may be used by the GPU pipeline.
    enum class Usage : td::BYTE {
        Unknown = 0,       ///< Usage is unknown or unspecified.
        ShaderRead = 1,    ///< The texture can be read by a shader.
        ShaderWrite = 2,   ///< The texture can be written by a shader.
        RenderTarget = 4,  ///< The texture can be used as a render target.
        PixelFormatView = 16 ///< The texture can be reinterpreted with a different pixel format.
    };

    /// @brief Specifies the compression type applied to the texture data.
    enum class CompressionType : td::BYTE {
        Lossless = 0, ///< Lossless compression preserving exact pixel data.
        Lossy = 1     ///< Lossy compression that may reduce quality to save memory.
    };

private:
    MTL::Texture* _pTexture; ///< Pointer to the underlying native Metal texture.
public:
    /// @brief Default constructor; creates an empty (invalid) texture.
    Texture();

    /// @brief Constructs a Texture from a native Metal texture pointer.
    /// @param pTexture Pointer to an existing MTL::Texture.
    Texture(MTL::Texture*);

    /// @brief Copy constructor; retains the underlying native texture.
    /// @param texture The source texture to copy from.
    Texture(const Texture& texture);

    /// @brief Copy-assignment operator; retains the underlying native texture.
    /// @param texture The source texture to copy from.
    void operator = (const Texture& texture);

    /// @brief Destructor; releases the underlying native texture.
    ~Texture();

//    class TextureDescriptor : public NS::Copying<TextureDescriptor>
//    {
//    public:
//        static class TextureDescriptor* alloc();
//
//        class TextureDescriptor*        init();
//
//        static class TextureDescriptor* texture2DDescriptor(MTL::PixelFormat pixelFormat, NS::UInteger width, NS::UInteger height, bool mipmapped);
//
//        static class TextureDescriptor* textureCubeDescriptor(MTL::PixelFormat pixelFormat, NS::UInteger size, bool mipmapped);
//
//        static class TextureDescriptor* textureBufferDescriptor(MTL::PixelFormat pixelFormat, NS::UInteger width, MTL::ResourceOptions resourceOptions, MTL::TextureUsage usage);
//
//        MTL::TextureType                textureType() const;
//        void                            setTextureType(MTL::TextureType textureType);
//
//        void                            setPixelFormat(MTL::PixelFormat pixelFormat);
//
    /// @brief Returns the width of the texture in pixels.
    /// @return Width in pixels.
    uint32_t width() const;
//        void                            setWidth(NS::UInteger width);
//
    /// @brief Returns the height of the texture in pixels.
    /// @return Height in pixels.
    uint32_t height() const;
//        void                            setHeight(NS::UInteger height);

    /// @brief Returns the pixel format of the texture.
    /// @return The glx::PixelFormat describing the texture's data layout.
    glx::PixelFormat pixelFormat() const;
//
//        NS::UInteger                    depth() const;
//        void                            setDepth(NS::UInteger depth);
//
//        NS::UInteger                    mipmapLevelCount() const;
//        void                            setMipmapLevelCount(NS::UInteger mipmapLevelCount);
//
//        NS::UInteger                    sampleCount() const;
//        void                            setSampleCount(NS::UInteger sampleCount);
//
//        NS::UInteger                    arrayLength() const;
//        void                            setArrayLength(NS::UInteger arrayLength);
//
//        MTL::ResourceOptions            resourceOptions() const;
//        void                            setResourceOptions(MTL::ResourceOptions resourceOptions);
//
//        MTL::CPUCacheMode               cpuCacheMode() const;
//        void                            setCpuCacheMode(MTL::CPUCacheMode cpuCacheMode);
//
//        MTL::StorageMode                storageMode() const;
//        void                            setStorageMode(MTL::StorageMode storageMode);
//
//        MTL::HazardTrackingMode         hazardTrackingMode() const;
//        void                            setHazardTrackingMode(MTL::HazardTrackingMode hazardTrackingMode);
//
//        MTL::TextureUsage               usage() const;
//        void                            setUsage(MTL::TextureUsage usage);
//
//        bool                            allowGPUOptimizedContents() const;
//        void                            setAllowGPUOptimizedContents(bool allowGPUOptimizedContents);
//
//        MTL::TextureSwizzleChannels     swizzle() const;
//        void                            setSwizzle(MTL::TextureSwizzleChannels swizzle);
//    };
//
//    class Texture : public NS::Referencing<Texture, Resource>
//    {
//    public:
//        class Resource*             rootResource() const;
//
//        class Texture*              parentTexture() const;
//
//        NS::UInteger                parentRelativeLevel() const;
//
//        NS::UInteger                parentRelativeSlice() const;
//
//        class Buffer*               buffer() const;
//
//        NS::UInteger                bufferOffset() const;
//
//        NS::UInteger                bufferBytesPerRow() const;
//
//        IOSurfaceRef                iosurface() const;
//
//        NS::UInteger                iosurfacePlane() const;
//
//        MTL::TextureType            textureType() const;
//
//        MTL::PixelFormat            pixelFormat() const;
//
//        NS::UInteger                width() const;
//
//        NS::UInteger                height() const;
//
//        NS::UInteger                depth() const;
//
//        NS::UInteger                mipmapLevelCount() const;
//
//        NS::UInteger                sampleCount() const;
//
//        NS::UInteger                arrayLength() const;
//
//        MTL::TextureUsage           usage() const;
//
//        bool                        shareable() const;
//
//        bool                        framebufferOnly() const;
//
//        NS::UInteger                firstMipmapInTail() const;
//
//        NS::UInteger                tailSizeInBytes() const;
//
//        bool                        isSparse() const;
//
//        bool                        allowGPUOptimizedContents() const;
//
//        void                        getBytes(const void* pixelBytes, NS::UInteger bytesPerRow, NS::UInteger bytesPerImage, MTL::Region region, NS::UInteger level, NS::UInteger slice);
//
//        void                        replaceRegion(MTL::Region region, NS::UInteger level, NS::UInteger slice, const void* pixelBytes, NS::UInteger bytesPerRow, NS::UInteger bytesPerImage);
//
//        void                        getBytes(const void* pixelBytes, NS::UInteger bytesPerRow, MTL::Region region, NS::UInteger level);

          /// @brief Copies pixel data from CPU memory into a specific mipmap level of the texture.
          /// @param region     The rectangular sub-region of the texture to update.
          /// @param level      The mipmap level to update (0 is the base level).
          /// @param pixelBytes Pointer to the source pixel data in CPU memory.
          /// @param bytesPerRow Number of bytes per row in the source pixel data.
          void replaceRegion(glx::Region region, uint32_t level, const void* pixelBytes, uint32_t bytesPerRow);
//
//        class Texture*              newTextureView(MTL::PixelFormat pixelFormat);
//
//        class Texture*              newTextureView(MTL::PixelFormat pixelFormat, MTL::TextureType textureType, NS::Range levelRange, NS::Range sliceRange);
//
//        class SharedTextureHandle*  newSharedTextureHandle();
//
//        class Texture*              remoteStorageTexture() const;
//
//        class Texture*              newRemoteTextureViewForDevice(const class Device* device);
//
//        MTL::TextureSwizzleChannels swizzle() const;
//
//        class Texture*              newTextureView(MTL::PixelFormat pixelFormat, MTL::TextureType textureType, NS::Range levelRange, NS::Range sliceRange, MTL::TextureSwizzleChannels swizzle);
//    };
//

    /// @brief Checks whether the texture holds a valid native pointer.
    /// @return True if the texture is valid.
    bool isOk() const;

    /// @brief Releases the underlying native texture resource.
    void release();

private:
    /// @brief Returns a mutable pointer to the underlying native texture.
    /// @return Pointer to MTL::Texture.
    MTL::Texture* getNatPtr()
    {
        return _pTexture;
    }

    /// @brief Returns a const pointer to the underlying native texture.
    /// @return Const pointer to MTL::Texture.
    const MTL::Texture* getNatPtr() const
    {
        return _pTexture;
    }
};


} //namespace glx
