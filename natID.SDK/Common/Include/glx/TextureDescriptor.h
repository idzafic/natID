// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TextureDescriptor.h
    @brief Declares the glx::TextureDescriptor class for specifying texture creation parameters. */
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
    /// @brief Specifies the properties used to create a new texture object on the GPU.
    class NATGLX_API TextureDescriptor
    {
        MTL::TextureDescriptor* _pDesc = nullptr; ///< Pointer to the underlying native Metal texture descriptor.
        TextureDescriptor() = default;
    public:

        /// @brief Constructs a TextureDescriptor from a native Metal pointer.
        /// @param p Pointer to an existing MTL::TextureDescriptor.
        explicit TextureDescriptor(MTL::TextureDescriptor* p) : _pDesc(p) {}

        /// @brief Destructor; releases the underlying native texture descriptor.
        ~TextureDescriptor();

        /// @brief Copy constructor.
        /// @param other The source descriptor to copy from.
        TextureDescriptor(const TextureDescriptor& other);

        /// @brief Copy-assignment operator.
        /// @param other The source descriptor to copy from.
        /// @return Reference to this descriptor.
        TextureDescriptor& operator=(const TextureDescriptor& other);

        /// @brief Move constructor.
        /// @param other The source descriptor to move from.
        TextureDescriptor(TextureDescriptor&& other) noexcept;

        /// @brief Move-assignment operator.
        /// @param other The source descriptor to move from.
        /// @return Reference to this descriptor.
        TextureDescriptor& operator=(TextureDescriptor&& other) noexcept;

        /// @brief Allocates a new, uninitialized texture descriptor.
        /// @return A newly allocated TextureDescriptor.
        static TextureDescriptor alloc();

        /// @brief Initialises the texture descriptor with default values.
        void init();

        /// @brief Creates a descriptor for a 2D texture.
        /// @param pixelFormat The pixel format of the texture.
        /// @param width       Width of the texture in pixels.
        /// @param height      Height of the texture in pixels.
        /// @param mipmapped   True to enable automatic mipmap generation.
        /// @return A TextureDescriptor configured for a 2D texture.
        static TextureDescriptor texture2DDescriptor(PixelFormat pixelFormat, uint32_t width, uint32_t height, bool mipmapped);

        /// @brief Creates a descriptor for a cube map texture.
        /// @param pixelFormat The pixel format of the texture.
        /// @param size        Width and height of each cube face in pixels.
        /// @param mipmapped   True to enable automatic mipmap generation.
        /// @return A TextureDescriptor configured for a cube texture.
        static TextureDescriptor textureCubeDescriptor(PixelFormat pixelFormat, uint32_t size, bool mipmapped);

        /// @brief Creates a descriptor for a buffer-backed texture.
        /// @param pixelFormat    The pixel format of the texture.
        /// @param width          Width of the texture in pixels.
        /// @param resourceOptions Resource storage and caching options.
        /// @param usage          Permitted usage modes for the texture.
        /// @return A TextureDescriptor configured for a buffer-backed texture.
        static TextureDescriptor textureBufferDescriptor(PixelFormat pixelFormat, uint32_t width, Resource::Options resourceOptions, TextureUsage usage);

        // Accessors forwarding to native

        /// @brief Returns the texture type (e.g., 2D, cube, array).
        /// @return The TextureType setting.
        TextureType textureType() const;

        /// @brief Sets the texture type.
        /// @param type The TextureType to apply.
        void setTextureType(TextureType);

        /// @brief Returns the pixel format of the texture.
        /// @return The PixelFormat setting.
        PixelFormat pixelFormat() const;

        /// @brief Sets the pixel format of the texture.
        /// @param format The PixelFormat to apply.
        void setPixelFormat(PixelFormat);

        /// @brief Returns the width of the texture in pixels.
        /// @return Width in pixels.
        uint32_t width() const;

        /// @brief Sets the width of the texture in pixels.
        /// @param width Width in pixels.
        void setWidth(uint32_t);

        /// @brief Returns the height of the texture in pixels.
        /// @return Height in pixels.
        uint32_t height() const;

        /// @brief Sets the height of the texture in pixels.
        /// @param height Height in pixels.
        void setHeight(uint32_t);

        /// @brief Returns the depth of a 3D texture in pixels.
        /// @return Depth in pixels (1 for 2D textures).
        uint32_t depth() const;

        /// @brief Sets the depth of a 3D texture in pixels.
        /// @param depth Depth in pixels.
        void setDepth(uint32_t);

        /// @brief Returns the number of mipmap levels.
        /// @return The mipmap level count.
        uint32_t mipmapLevelCount() const;

        /// @brief Sets the number of mipmap levels.
        /// @param count The mipmap level count.
        void setMipmapLevelCount(uint32_t);

        /// @brief Returns the number of samples per pixel for multisampled textures.
        /// @return The sample count.
        uint32_t sampleCount() const;

        /// @brief Sets the number of samples per pixel for multisampled textures.
        /// @param count The sample count.
        void setSampleCount(uint32_t);

        /// @brief Returns the number of array slices in a texture array.
        /// @return The array length.
        uint32_t arrayLength() const;

        /// @brief Sets the number of array slices in a texture array.
        /// @param length The array length.
        void setArrayLength(uint32_t);

        /// @brief Returns the resource storage and caching options.
        /// @return The Resource::Options setting.
        Resource::Options resourceOptions() const;

        /// @brief Sets the resource storage and caching options.
        /// @param options The Resource::Options to apply.
        void setResourceOptions(Resource::Options);

        /// @brief Returns the CPU cache mode for the texture resource.
        /// @return The Resource::CPUCacheMode setting.
        Resource::CPUCacheMode cpuCacheMode() const;

        /// @brief Sets the CPU cache mode for the texture resource.
        /// @param mode The Resource::CPUCacheMode to apply.
        void setCpuCacheMode(Resource::CPUCacheMode);

        /// @brief Returns the storage mode for the texture resource.
        /// @return The Resource::StorageMode setting.
        Resource::StorageMode storageMode() const;

        /// @brief Sets the storage mode for the texture resource.
        /// @param mode The Resource::StorageMode to apply.
        void setStorageMode(Resource::StorageMode);

        /// @brief Returns the hazard tracking mode for the texture resource.
        /// @return The Resource::HazardTrackingMode setting.
        Resource::HazardTrackingMode hazardTrackingMode() const;

        /// @brief Sets the hazard tracking mode for the texture resource.
        /// @param mode The Resource::HazardTrackingMode to apply.
        void setHazardTrackingMode(Resource::HazardTrackingMode);

        /// @brief Returns the permitted usage modes for the texture.
        /// @return The TextureUsage flags.
        TextureUsage usage() const;

        /// @brief Sets the permitted usage modes for the texture.
        /// @param usage The TextureUsage flags to apply.
        void setUsage(TextureUsage);

        /// @brief Returns whether GPU-optimised content storage is allowed.
        /// @return True if GPU-optimised contents are allowed.
        bool allowGPUOptimizedContents() const;

        /// @brief Enables or disables GPU-optimised content storage.
        /// @param allow True to allow GPU-optimised contents.
        void setAllowGPUOptimizedContents(bool);

        /// @brief Returns the texture swizzle channel mapping.
        /// @return The TextureSwizzleChannels configuration.
        TextureSwizzleChannels swizzle() const;

        /// @brief Sets the texture swizzle channel mapping.
        /// @param swizzle The TextureSwizzleChannels configuration to apply.
        void setSwizzle(const TextureSwizzleChannels&);

        /// @brief Checks whether the texture descriptor holds a valid native pointer.
        /// @return True if the descriptor is valid.
        bool isOk() const;

        /// @brief Releases the underlying native texture descriptor resource.
        void release();

    private:
        /// @brief Returns a mutable pointer to the underlying native descriptor.
        /// @return Pointer to MTL::TextureDescriptor.
        MTL::TextureDescriptor* getNatPtr() { return _pDesc; }

        /// @brief Returns a const pointer to the underlying native descriptor.
        /// @return Const pointer to MTL::TextureDescriptor.
        const MTL::TextureDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
		friend class Texture;
    };

} //namespace glx
