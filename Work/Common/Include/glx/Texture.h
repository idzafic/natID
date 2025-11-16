// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
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
} //namespace MTL

namespace glx
{

	class BlitCommandEncoder;
	class RenderPassDescriptor;
    class RenderCommandEncoder;

class NATGLX_API Texture
{
public:
    enum class Usage : td::BYTE { Unknown = 0, ShaderRead = 1, ShaderWrite = 2, RenderTarget = 4, PixelFormatView = 16};
    enum class CompressionType : td::BYTE { Lossless = 0, Lossy = 1};
    
private:
    MTL::Texture* _pTexture;

	friend class BlitCommandEncoder;
	friend class RenderPassDescriptor;
    friend class RenderCommandEncoder;
public:
    Texture();
    Texture(MTL::Texture*);
    void operator = (const Texture& texture);
    
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
//        MTL::PixelFormat                pixelFormat() const;
//        void                            setPixelFormat(MTL::PixelFormat pixelFormat);
//
    uint32_t width() const;
//        void                            setWidth(NS::UInteger width);
//
    uint32_t height() const;
//        void                            setHeight(NS::UInteger height);
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

    bool isOk() const;
    void release();

    private:

    MTL::Texture* getNatPtr()
    {
        return _pTexture;
    }
    
    const MTL::Texture* getNatPtr() const
    {
        return _pTexture;
    }
};


} //namespace glx

