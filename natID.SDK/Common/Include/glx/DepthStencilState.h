// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DepthStencilState.h
 * @brief Immutable compiled depth and stencil test state object.
 */
#pragma once
#include "natGLX.h"
#include <glx/Types.h>
#include <glx/StencilDescriptor.h>
#include <td/String.h>

namespace MTL
{
    class DepthStencilState;
} //namespace MTL

namespace glx
{
    class Device;

    /// @brief Represents a compiled, immutable depth and stencil state that can be applied during rendering.
    class NATGLX_API DepthStencilState
    {
        MTL::DepthStencilState* _pState = nullptr; ///< Pointer to the underlying Metal depth/stencil state.



        explicit DepthStencilState(MTL::DepthStencilState* p) : _pState(p) {}

    public:
        /// @brief Destructor; releases the underlying state resource.
        ~DepthStencilState();

        /// @brief Default constructor; creates an empty, invalid state.
        DepthStencilState() = default;

        /// @brief Copy constructor.
        /// @param other The state to copy from.
        DepthStencilState(const DepthStencilState& other);

        /// @brief Copy assignment operator.
        /// @param other The state to copy from.
        /// @return Reference to this state.
        DepthStencilState& operator=(const DepthStencilState& other);

        /// @brief Move constructor.
        /// @param other The state to move from.
        DepthStencilState(DepthStencilState&& other) noexcept;

        /// @brief Move assignment operator.
        /// @param other The state to move from.
        /// @return Reference to this state.
        DepthStencilState& operator=(DepthStencilState&& other) noexcept;

        // Metal API properties

        /// @brief Returns the device that created this depth/stencil state.
        /// @return The owning Device.
        Device device() const;

        /// @brief Returns the debug label associated with this state.
        /// @return The label string.
        td::String label() const;

        /// @brief Returns whether the state holds a valid native pointer.
        /// @return True if the state is valid.
        bool isOk() const;

        /// @brief Releases the underlying native state resource.
        void release();

    private:
        /// @brief Returns the mutable native Metal state pointer.
        /// @return Pointer to MTL::DepthStencilState.
        MTL::DepthStencilState* getNatPtr() { return _pState; }

        /// @brief Returns the const native Metal state pointer.
        /// @return Const pointer to MTL::DepthStencilState.
        const MTL::DepthStencilState* getNatPtr() const { return _pState; }
        friend class Device;
        friend class RenderCommandEncoder;
    };

} //namespace glx
