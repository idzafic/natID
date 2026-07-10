// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StencilDescriptor.h
    @brief Declares the glx::StencilDescriptor class for configuring stencil test operations. */
#pragma once
#include "natGLX.h"
#include <glx/Types.h>
#include <td/String.h>

namespace MTL
{
    class StencilDescriptor;
} //namespace MTL

namespace glx
{
    /// @brief Describes the stencil test configuration applied during a depth-stencil pass.
    class NATGLX_API StencilDescriptor
    {
        MTL::StencilDescriptor* _pDesc = nullptr; ///< Pointer to the underlying native Metal stencil descriptor.
        StencilDescriptor() = default;

        /// @brief Copy constructor.
        /// @param other The source descriptor to copy from.
        StencilDescriptor(const StencilDescriptor& other);

        /// @brief Copy-assignment operator.
        /// @param other The source descriptor to copy from.
        /// @return Reference to this descriptor.
        StencilDescriptor& operator=(const StencilDescriptor& other);

        /// @brief Move constructor.
        /// @param other The source descriptor to move from.
        StencilDescriptor(StencilDescriptor&& other) noexcept;

        /// @brief Move-assignment operator.
        /// @param other The source descriptor to move from.
        /// @return Reference to this descriptor.
        StencilDescriptor& operator=(StencilDescriptor&& other) noexcept;

        /// @brief Constructs a descriptor from a native Metal pointer.
        /// @param p Pointer to an existing MTL::StencilDescriptor.
        explicit StencilDescriptor(MTL::StencilDescriptor* p) : _pDesc(p) {}
    public:

        /// @brief Destructor; releases the underlying native stencil descriptor.
        ~StencilDescriptor();

        /// @brief Allocates a new, uninitialized stencil descriptor.
        /// @return A newly allocated StencilDescriptor.
        static StencilDescriptor alloc();

        /// @brief Initialises the stencil descriptor with default values.
        void init();

        // Stencil operation configuration

        /// @brief Returns the operation performed when the stencil test fails.
        /// @return The StencilOperation applied on stencil failure.
        StencilOperation stencilFailureOperation() const;

        /// @brief Sets the operation performed when the stencil test fails.
        /// @param operation The StencilOperation to apply on stencil failure.
        void setStencilFailureOperation(StencilOperation operation);

        /// @brief Returns the operation performed when the depth test fails.
        /// @return The StencilOperation applied on depth failure.
        StencilOperation depthFailureOperation() const;

        /// @brief Sets the operation performed when the depth test fails.
        /// @param operation The StencilOperation to apply on depth failure.
        void setDepthFailureOperation(StencilOperation operation);

        /// @brief Returns the operation performed when both the depth and stencil tests pass.
        /// @return The StencilOperation applied on depth-stencil pass.
        StencilOperation depthStencilPassOperation() const;

        /// @brief Sets the operation performed when both the depth and stencil tests pass.
        /// @param operation The StencilOperation to apply on depth-stencil pass.
        void setDepthStencilPassOperation(StencilOperation operation);

        /// @brief Returns the comparison function used by the stencil test.
        /// @return The CompareFunction used for stencil comparison.
        CompareFunction stencilCompareFunction() const;

        /// @brief Sets the comparison function used by the stencil test.
        /// @param function The CompareFunction to apply.
        void setStencilCompareFunction(CompareFunction function);

        /// @brief Returns the bitmask applied when reading the stencil buffer.
        /// @return The read mask value.
        uint32_t readMask() const;

        /// @brief Sets the bitmask applied when reading the stencil buffer.
        /// @param mask The read mask value.
        void setReadMask(uint32_t mask);

        /// @brief Returns the bitmask applied when writing to the stencil buffer.
        /// @return The write mask value.
        uint32_t writeMask() const;

        /// @brief Sets the bitmask applied when writing to the stencil buffer.
        /// @param mask The write mask value.
        void setWriteMask(uint32_t mask);

        /// @brief Checks whether the stencil descriptor holds a valid native pointer.
        /// @return True if the descriptor is valid.
        bool isOk() const;

        /// @brief Releases the underlying native stencil descriptor resource.
        void release();

    private:
        /// @brief Returns a mutable pointer to the underlying native descriptor.
        /// @return Pointer to MTL::StencilDescriptor.
        MTL::StencilDescriptor* getNatPtr() { return _pDesc; }

        /// @brief Returns a const pointer to the underlying native descriptor.
        /// @return Const pointer to MTL::StencilDescriptor.
        const MTL::StencilDescriptor* getNatPtr() const { return _pDesc; }
        friend class DepthStencilDescriptor;
        friend class DepthStencilState;
    };

} //namespace glx
