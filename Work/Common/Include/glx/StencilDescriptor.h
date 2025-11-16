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
#include <td/String.h>

namespace MTL
{
    class StencilDescriptor;
} //namespace MTL

namespace glx
{
    class NATGLX_API StencilDescriptor
    {
        MTL::StencilDescriptor* _pDesc = nullptr;
        StencilDescriptor() = default;

        StencilDescriptor(const StencilDescriptor& other);
        StencilDescriptor& operator=(const StencilDescriptor& other);
        StencilDescriptor(StencilDescriptor&& other) noexcept;
        StencilDescriptor& operator=(StencilDescriptor&& other) noexcept;

        explicit StencilDescriptor(MTL::StencilDescriptor* p) : _pDesc(p) {}
    public:

        ~StencilDescriptor();

        static StencilDescriptor alloc();
        void init();

        // Stencil operation configuration
        StencilOperation stencilFailureOperation() const;
        void setStencilFailureOperation(StencilOperation operation);

        StencilOperation depthFailureOperation() const;
        void setDepthFailureOperation(StencilOperation operation);

        StencilOperation depthStencilPassOperation() const;
        void setDepthStencilPassOperation(StencilOperation operation);

        CompareFunction stencilCompareFunction() const;
        void setStencilCompareFunction(CompareFunction function);

        uint32_t readMask() const;
        void setReadMask(uint32_t mask);

        uint32_t writeMask() const;
        void setWriteMask(uint32_t mask);

        bool isOk() const;
        void release();

    private:
        MTL::StencilDescriptor* getNatPtr() { return _pDesc; }
        const MTL::StencilDescriptor* getNatPtr() const { return _pDesc; }
        friend class DepthStencilDescriptor;
        friend class DepthStencilState;
    };

} //namespace glx