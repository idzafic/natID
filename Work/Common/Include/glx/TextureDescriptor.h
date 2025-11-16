// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include "natGLX.h"
#include <cstdint>
#include <glx/Types.h>
#include <glx/Resource.h>

namespace MTL
{
    class TextureDescriptor;
} //namespace MTL

namespace glx
{
    class NATGLX_API TextureDescriptor
    {
        MTL::TextureDescriptor* _pDesc = nullptr;
        TextureDescriptor() = default;
    public:

        explicit TextureDescriptor(MTL::TextureDescriptor* p) : _pDesc(p) {}


        ~TextureDescriptor();


        TextureDescriptor(const TextureDescriptor& other);


        TextureDescriptor& operator=(const TextureDescriptor& other);
        TextureDescriptor(TextureDescriptor&& other) noexcept;
        TextureDescriptor& operator=(TextureDescriptor&& other) noexcept;


        static TextureDescriptor alloc();
        void init();

        static TextureDescriptor texture2DDescriptor(PixelFormat pixelFormat, uint32_t width, uint32_t height, bool mipmapped);
        static TextureDescriptor textureCubeDescriptor(PixelFormat pixelFormat, uint32_t size, bool mipmapped);
        static TextureDescriptor textureBufferDescriptor(PixelFormat pixelFormat, uint32_t width, Resource::Options resourceOptions, TextureUsage usage);

        // Accessors forwarding to native
        TextureType textureType() const;
        void setTextureType(TextureType);

        PixelFormat pixelFormat() const;
        void setPixelFormat(PixelFormat);

        uint32_t width() const;
        void setWidth(uint32_t);

        uint32_t height() const;
        void setHeight(uint32_t);

        uint32_t depth() const;
        void setDepth(uint32_t);

        uint32_t mipmapLevelCount() const;
        void setMipmapLevelCount(uint32_t);

        uint32_t sampleCount() const;
        void setSampleCount(uint32_t);

        uint32_t arrayLength() const;
        void setArrayLength(uint32_t);

        Resource::Options resourceOptions() const;
        void setResourceOptions(Resource::Options);

        Resource::CPUCacheMode cpuCacheMode() const;
        void setCpuCacheMode(Resource::CPUCacheMode);

        Resource::StorageMode storageMode() const;
        void setStorageMode(Resource::StorageMode);

        Resource::HazardTrackingMode hazardTrackingMode() const;
        void setHazardTrackingMode(Resource::HazardTrackingMode);

        TextureUsage usage() const;
        void setUsage(TextureUsage);

        bool allowGPUOptimizedContents() const;
        void setAllowGPUOptimizedContents(bool);

        TextureSwizzleChannels swizzle() const;
        void setSwizzle(const TextureSwizzleChannels&);

        bool isOk() const;
        void release();

    private:
        MTL::TextureDescriptor* getNatPtr() { return _pDesc; }
        const MTL::TextureDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
		friend class Texture;
    };

} //namespace glx

