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

    class NATGLX_API DepthStencilState
    {
        MTL::DepthStencilState* _pState = nullptr;



        explicit DepthStencilState(MTL::DepthStencilState* p) : _pState(p) {}

    public:
        ~DepthStencilState();

        DepthStencilState() = default;
        DepthStencilState(const DepthStencilState& other);
        DepthStencilState& operator=(const DepthStencilState& other);
        DepthStencilState(DepthStencilState&& other) noexcept;
        DepthStencilState& operator=(DepthStencilState&& other) noexcept;

        // Metal API properties
        Device device() const;
        td::String label() const;

        bool isOk() const;
        void release();

    private:
        MTL::DepthStencilState* getNatPtr() { return _pState; }
        const MTL::DepthStencilState* getNatPtr() const { return _pState; }
        friend class Device;
        friend class RenderCommandEncoder;
    };

} //namespace glx