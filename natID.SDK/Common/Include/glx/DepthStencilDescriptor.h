// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DepthStencilDescriptor.h
 * @brief Descriptor for configuring depth and stencil testing state.
 */
#pragma once
#include "natGLX.h"
#include <glx/Types.h>
#include <glx/StencilDescriptor.h>
#include <td/String.h>

namespace MTL
{
    class DepthStencilDescriptor;
} //namespace MTL

namespace glx
{
    /// @brief Describes the depth and stencil test configuration used to create a DepthStencilState.
    class NATGLX_API DepthStencilDescriptor
    {
        MTL::DepthStencilDescriptor* _pDesc = nullptr; ///< Pointer to the underlying Metal descriptor.
        DepthStencilDescriptor() = default;



        explicit DepthStencilDescriptor(MTL::DepthStencilDescriptor* p) : _pDesc(p) {}
    public:

        /// @brief Destructor; releases the underlying descriptor resource.
        ~DepthStencilDescriptor();

        /// @brief Copy constructor.
        /// @param other The descriptor to copy from.
        DepthStencilDescriptor(const DepthStencilDescriptor& other);

        /// @brief Copy assignment operator.
        /// @param other The descriptor to copy from.
        /// @return Reference to this descriptor.
        DepthStencilDescriptor& operator=(const DepthStencilDescriptor& other);

        /// @brief Move constructor.
        /// @param other The descriptor to move from.
        DepthStencilDescriptor(DepthStencilDescriptor&& other) noexcept;

        /// @brief Move assignment operator.
        /// @param other The descriptor to move from.
        /// @return Reference to this descriptor.
        DepthStencilDescriptor& operator=(DepthStencilDescriptor&& other) noexcept;

        /// @brief Allocates a new DepthStencilDescriptor.
        /// @return A newly allocated descriptor instance.
        static DepthStencilDescriptor alloc();

        /// @brief Initializes the descriptor with default values.
        void init();

        // Depth testing configuration

        /// @brief Returns the comparison function used for depth testing.
        /// @return The current depth compare function.
        CompareFunction depthCompareFunction() const;

        /// @brief Sets the comparison function used for depth testing.
        /// @param function The compare function to apply.
        void setDepthCompareFunction(CompareFunction function);

        /// @brief Returns whether depth writing is enabled.
        /// @return True if depth writes are enabled.
        bool depthWriteEnabled() const;

        /// @brief Enables or disables writing to the depth buffer.
        /// @param enabled True to enable depth writes.
        void setDepthWriteEnabled(bool enabled);

        // Stencil testing configuration

        /// @brief Returns the stencil descriptor for front-facing primitives.
        /// @return The front-face stencil descriptor.
        StencilDescriptor frontFaceStencil() const;

        /// @brief Sets the stencil descriptor for front-facing primitives.
        /// @param descriptor Pointer to the stencil descriptor to apply.
        void setFrontFaceStencil(const StencilDescriptor* descriptor);

        /// @brief Returns the stencil descriptor for back-facing primitives.
        /// @return The back-face stencil descriptor.
        StencilDescriptor backFaceStencil() const;

        /// @brief Sets the stencil descriptor for back-facing primitives.
        /// @param descriptor Pointer to the stencil descriptor to apply.
        void setBackFaceStencil(const StencilDescriptor* descriptor);

        // Label functionality

        /// @brief Returns the debug label associated with this descriptor.
        /// @return The label string.
        td::String label() const;

        /// @brief Sets a debug label for this descriptor.
        /// @param label The label string to assign.
        void setLabel(const td::String& label);

        /// @brief Returns whether the descriptor holds a valid native pointer.
        /// @return True if the descriptor is valid.
        bool isOk() const;

        /// @brief Releases the underlying native descriptor resource.
        void release();

    private:
        /// @brief Returns the mutable native Metal descriptor pointer.
        /// @return Pointer to MTL::DepthStencilDescriptor.
        MTL::DepthStencilDescriptor* getNatPtr() { return _pDesc; }

        /// @brief Returns the const native Metal descriptor pointer.
        /// @return Const pointer to MTL::DepthStencilDescriptor.
        const MTL::DepthStencilDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
    };

} //namespace glx
