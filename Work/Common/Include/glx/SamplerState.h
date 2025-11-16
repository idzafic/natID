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
#include <td/String.h>

namespace MTL
{
    class SamplerState;
    class Device;
} //namespace MTL

namespace glx
{
    class Device;

    class NATGLX_API SamplerState
    {
        MTL::SamplerState* _pState = nullptr;
        
        SamplerState(const SamplerState& other);


        explicit SamplerState(MTL::SamplerState* p) : _pState(p) {}

    public:

        SamplerState() = default;
        SamplerState& operator=(const SamplerState& other);
        SamplerState(SamplerState&& other) noexcept;
        SamplerState& operator=(SamplerState&& other) noexcept;

        ~SamplerState();

        td::String label() const;
        void setLabel(const td::String& label);
        Device device() const;

        bool isOk() const;
        void release();

    private:
        MTL::SamplerState* getNatPtr() { return _pState; }
        const MTL::SamplerState* getNatPtr() const { return _pState; }
        friend class Device;
        friend class RenderCommandEncoder;
    };

} //namespace glx} //namespace glx