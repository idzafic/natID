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
    class DepthStencilDescriptor;
} //namespace MTL

namespace glx
{
    class NATGLX_API DepthStencilDescriptor
    {
        MTL::DepthStencilDescriptor* _pDesc = nullptr;
        DepthStencilDescriptor() = default;



        explicit DepthStencilDescriptor(MTL::DepthStencilDescriptor* p) : _pDesc(p) {}
    public:

        ~DepthStencilDescriptor();

        DepthStencilDescriptor(const DepthStencilDescriptor& other);
        DepthStencilDescriptor& operator=(const DepthStencilDescriptor& other);
        DepthStencilDescriptor(DepthStencilDescriptor&& other) noexcept;
        DepthStencilDescriptor& operator=(DepthStencilDescriptor&& other) noexcept;

        static DepthStencilDescriptor alloc();
        void init();

        // Depth testing configuration
        CompareFunction depthCompareFunction() const;
        void setDepthCompareFunction(CompareFunction function);

        bool depthWriteEnabled() const;
        void setDepthWriteEnabled(bool enabled);

        // Stencil testing configuration
        StencilDescriptor frontFaceStencil() const;
        void setFrontFaceStencil(const StencilDescriptor* descriptor);

        StencilDescriptor backFaceStencil() const;
        void setBackFaceStencil(const StencilDescriptor* descriptor);

        // Label functionality
        td::String label() const;
        void setLabel(const td::String& label);

        bool isOk() const;
        void release();

    private:
        MTL::DepthStencilDescriptor* getNatPtr() { return _pDesc; }
        const MTL::DepthStencilDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
    };

} //namespace glx