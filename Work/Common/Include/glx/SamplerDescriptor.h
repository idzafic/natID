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
    class SamplerDescriptor;
} //namespace MTL

namespace glx
{
    class NATGLX_API SamplerDescriptor
    {
        MTL::SamplerDescriptor* _pDesc = nullptr;
        SamplerDescriptor() = default;

        SamplerDescriptor(const SamplerDescriptor& other);

        SamplerDescriptor& operator=(const SamplerDescriptor& other);
        SamplerDescriptor(SamplerDescriptor&& other) noexcept;
        SamplerDescriptor& operator=(SamplerDescriptor&& other) noexcept;

        explicit SamplerDescriptor(MTL::SamplerDescriptor* p) : _pDesc(p) {}
    public:

        

        ~SamplerDescriptor();



        static SamplerDescriptor alloc();
        void init();

        // Accessors forwarding to native
        CompareFunction compareFunction() const;
        void setCompareFunction(CompareFunction);

        SamplerBorderColor borderColor() const;
        void setBorderColor(SamplerBorderColor);

        SamplerAddressMode rAddressMode() const;
        void setRAddressMode(SamplerAddressMode);

        SamplerAddressMode sAddressMode() const;
        void setSAddressMode(SamplerAddressMode);

        SamplerAddressMode tAddressMode() const;
        void setTAddressMode(SamplerAddressMode);

        Filter minFilter() const;
        void setMinFilter(Filter);

        Filter magFilter() const;
        void setMagFilter(Filter);

        MipFilter mipFilter() const;
        void setMipFilter(MipFilter);

        bool lodAverage() const;
        void setLodAverage(bool);

        float lodMinClamp() const;
        void setLodMinClamp(float);

        float lodMaxClamp() const;
        void setLodMaxClamp(float);

        uint32_t maxAnisotropy() const;
        void setMaxAnisotropy(uint32_t);

        bool normalizedCoordinates() const;
        void setNormalizedCoordinates(bool);

        bool supportArgumentBuffers() const;
        void setSupportArgumentBuffers(bool);

        // Label functionality using td::String
        td::String label() const;
        void setLabel(const td::String& label);

        bool isOk() const;
        void release();

    private:
        MTL::SamplerDescriptor* getNatPtr() { return _pDesc; }
        const MTL::SamplerDescriptor* getNatPtr() const { return _pDesc; }
        friend class Device;
    };

} //namespace glx