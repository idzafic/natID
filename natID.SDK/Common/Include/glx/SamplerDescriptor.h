// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SamplerDescriptor.h
    @brief Declares the glx::SamplerDescriptor class for configuring texture sampler state. */
#pragma once

#include "natGLX.h"
#include <cstdint>
#include <glx/Types.h>
#include <td/String.h>

namespace MTL
{
    class SamplerDescriptor;
} //namespace MTL

namespace glx
{
    /// @brief Describes the sampling parameters used when reading a texture in a shader.
    class NATGLX_API SamplerDescriptor
    {
        MTL::SamplerDescriptor* _pDesc = nullptr; ///< Pointer to the underlying native Metal sampler descriptor.
        SamplerDescriptor() = default;

        /// @brief Copy constructor.
        /// @param other The source descriptor to copy from.
        SamplerDescriptor(const SamplerDescriptor& other);

        /// @brief Copy-assignment operator.
        /// @param other The source descriptor to copy from.
        /// @return Reference to this descriptor.
        SamplerDescriptor& operator=(const SamplerDescriptor& other);

        /// @brief Move constructor.
        /// @param other The source descriptor to move from.
        SamplerDescriptor(SamplerDescriptor&& other) noexcept;

        /// @brief Move-assignment operator.
        /// @param other The source descriptor to move from.
        /// @return Reference to this descriptor.
        SamplerDescriptor& operator=(SamplerDescriptor&& other) noexcept;

        /// @brief Constructs a descriptor from a native Metal pointer.
        /// @param p Pointer to an existing MTL::SamplerDescriptor.
        explicit SamplerDescriptor(MTL::SamplerDescriptor* p) : _pDesc(p) {}
    public:



        /// @brief Destructor; releases the underlying native descriptor.
        ~SamplerDescriptor();



        /// @brief Allocates a new, uninitialized sampler descriptor.
        /// @return A newly allocated SamplerDescriptor.
        static SamplerDescriptor alloc();

        /// @brief Initialises the sampler descriptor with default values.
        void init();

        // Accessors forwarding to native

        /// @brief Returns the comparison function used for depth and stencil comparisons.
        /// @return The current CompareFunction setting.
        CompareFunction compareFunction() const;

        /// @brief Sets the comparison function for depth and stencil comparisons.
        /// @param func The CompareFunction to apply.
        void setCompareFunction(CompareFunction);

        /// @brief Returns the border colour applied when texture coordinates are out of range.
        /// @return The current SamplerBorderColor setting.
        SamplerBorderColor borderColor() const;

        /// @brief Sets the border colour applied when texture coordinates are out of range.
        /// @param color The SamplerBorderColor to apply.
        void setBorderColor(SamplerBorderColor);

        /// @brief Returns the addressing mode used for the R texture coordinate.
        /// @return The current SamplerAddressMode for R.
        SamplerAddressMode rAddressMode() const;

        /// @brief Sets the addressing mode for the R texture coordinate.
        /// @param mode The SamplerAddressMode to apply.
        void setRAddressMode(SamplerAddressMode);

        /// @brief Returns the addressing mode used for the S texture coordinate.
        /// @return The current SamplerAddressMode for S.
        SamplerAddressMode sAddressMode() const;

        /// @brief Sets the addressing mode for the S texture coordinate.
        /// @param mode The SamplerAddressMode to apply.
        void setSAddressMode(SamplerAddressMode);

        /// @brief Returns the addressing mode used for the T texture coordinate.
        /// @return The current SamplerAddressMode for T.
        SamplerAddressMode tAddressMode() const;

        /// @brief Sets the addressing mode for the T texture coordinate.
        /// @param mode The SamplerAddressMode to apply.
        void setTAddressMode(SamplerAddressMode);

        /// @brief Returns the minification filter applied when sampling a texture.
        /// @return The current minification Filter.
        Filter minFilter() const;

        /// @brief Sets the minification filter applied when sampling a texture.
        /// @param filter The Filter to use for minification.
        void setMinFilter(Filter);

        /// @brief Returns the magnification filter applied when sampling a texture.
        /// @return The current magnification Filter.
        Filter magFilter() const;

        /// @brief Sets the magnification filter applied when sampling a texture.
        /// @param filter The Filter to use for magnification.
        void setMagFilter(Filter);

        /// @brief Returns the mip filter used when sampling between mipmap levels.
        /// @return The current MipFilter setting.
        MipFilter mipFilter() const;

        /// @brief Sets the mip filter used when sampling between mipmap levels.
        /// @param filter The MipFilter to apply.
        void setMipFilter(MipFilter);

        /// @brief Returns whether level-of-detail average filtering is enabled.
        /// @return True if LOD average filtering is enabled, false otherwise.
        bool lodAverage() const;

        /// @brief Enables or disables level-of-detail average filtering.
        /// @param enable True to enable LOD average filtering.
        void setLodAverage(bool);

        /// @brief Returns the minimum clamp value for the level of detail.
        /// @return The minimum LOD clamp value.
        float lodMinClamp() const;

        /// @brief Sets the minimum clamp value for the level of detail.
        /// @param clamp The minimum LOD clamp value.
        void setLodMinClamp(float);

        /// @brief Returns the maximum clamp value for the level of detail.
        /// @return The maximum LOD clamp value.
        float lodMaxClamp() const;

        /// @brief Sets the maximum clamp value for the level of detail.
        /// @param clamp The maximum LOD clamp value.
        void setLodMaxClamp(float);

        /// @brief Returns the maximum anisotropy level used for texture sampling.
        /// @return The maximum anisotropy value.
        uint32_t maxAnisotropy() const;

        /// @brief Sets the maximum anisotropy level used for texture sampling.
        /// @param anisotropy The maximum anisotropy value (typically 1–16).
        void setMaxAnisotropy(uint32_t);

        /// @brief Returns whether texture coordinates are normalised (in [0, 1]).
        /// @return True if normalised coordinates are used.
        bool normalizedCoordinates() const;

        /// @brief Sets whether texture coordinates are normalised.
        /// @param normalised True to use normalised coordinates.
        void setNormalizedCoordinates(bool);

        /// @brief Returns whether the sampler supports argument buffers.
        /// @return True if argument buffer support is enabled.
        bool supportArgumentBuffers() const;

        /// @brief Enables or disables argument buffer support for the sampler.
        /// @param support True to enable argument buffer support.
        void setSupportArgumentBuffers(bool);

        // Label functionality using td::String

        /// @brief Returns the debug label associated with the sampler descriptor.
        /// @return The label string.
        td::String label() const;

        /// @brief Sets a debug label for the sampler descriptor.
        /// @param label The label string to assign.
        void setLabel(const td::String& label);

        /// @brief Checks whether the sampler descriptor holds a valid native pointer.
        /// @return True if the descriptor is valid.
        bool isOk() const;

        /// @brief Releases the underlying native sampler descriptor resource.
        void release();

    private:
        /// @brief Returns a mutable pointer to the underlying native descriptor.
        /// @return Pointer to MTL::SamplerDescriptor.
        MTL::SamplerDescriptor* getNatPtr() { return _pDesc; }

        /// @brief Returns a const pointer to the underlying native descriptor.
        /// @return Const pointer to MTL::SamplerDescriptor.
        const MTL::SamplerDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
    };

} //namespace glx
