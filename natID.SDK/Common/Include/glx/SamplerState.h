// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SamplerState.h
    @brief Declares the glx::SamplerState class representing a compiled texture sampler state object. */
#pragma once

#include "natGLX.h"
#include <cstdint>
#include <glx/Types.h>
#include <td/String.h>

namespace MTL
{
    class SamplerState;
    class Device;
} //namespace MTL

namespace glx
{
    class Device;

    /// @brief Represents an immutable, compiled sampler state object used when sampling textures in shaders.
    class NATGLX_API SamplerState
    {
        MTL::SamplerState* _pState = nullptr; ///< Pointer to the underlying native Metal sampler state.

        /// @brief Private copy constructor to prevent unintended copying.
        /// @param other The source sampler state.
        SamplerState(const SamplerState& other);

        /// @brief Constructs a SamplerState from a native Metal pointer.
        /// @param p Pointer to an existing MTL::SamplerState.
        explicit SamplerState(MTL::SamplerState* p) : _pState(p) {}

    public:

        /// @brief Default constructor; creates an empty (invalid) sampler state.
        SamplerState() = default;

        /// @brief Copy-assignment operator.
        /// @param other The source sampler state to copy from.
        /// @return Reference to this sampler state.
        SamplerState& operator=(const SamplerState& other);

        /// @brief Move constructor.
        /// @param other The source sampler state to move from.
        SamplerState(SamplerState&& other) noexcept;

        /// @brief Move-assignment operator.
        /// @param other The source sampler state to move from.
        /// @return Reference to this sampler state.
        SamplerState& operator=(SamplerState&& other) noexcept;

        /// @brief Destructor; releases the underlying native sampler state.
        ~SamplerState();

        /// @brief Returns the debug label associated with this sampler state.
        /// @return The label string.
        td::String label() const;

        /// @brief Sets a debug label for this sampler state.
        /// @param label The label string to assign.
        void setLabel(const td::String& label);

        /// @brief Returns the device on which this sampler state was created.
        /// @return The associated glx::Device.
        Device device() const;

        /// @brief Checks whether this sampler state holds a valid native pointer.
        /// @return True if the sampler state is valid.
        bool isOk() const;

        /// @brief Releases the underlying native sampler state resource.
        void release();

    private:
        /// @brief Returns a mutable pointer to the underlying native sampler state.
        /// @return Pointer to MTL::SamplerState.
        MTL::SamplerState* getNatPtr() { return _pState; }

        /// @brief Returns a const pointer to the underlying native sampler state.
        /// @return Const pointer to MTL::SamplerState.
        const MTL::SamplerState* getNatPtr() const { return _pState; }
        friend class Device;
        friend class RenderCommandEncoder;
        friend class ComputeCommandEncoder;
    };

} //namespace glx} //namespace glx
