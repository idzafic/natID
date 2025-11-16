// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <td/String.h>
#include <glx/RenderPipeline.h>

namespace MTL
{
    class VertexDescriptor;
    class VertexAttributeDescriptor;
    class VertexBufferLayoutDescriptor;
    class VertexAttributeDescriptorArray;
    class VertexBufferLayoutDescriptorArray;
}

namespace glx
{
    class NATGLX_API VertexAttribute
    {
        friend class VertexAttributes;
        
    private:
        MTL::VertexAttributeDescriptor* _pVertexAttributeDescriptor;
        VertexAttribute(MTL::VertexAttributeDescriptor* pVertexAttributeDescriptor);
        
    public:
        VertexAttribute();
        VertexAttribute(const VertexAttribute& other);
        VertexAttribute& operator=(const VertexAttribute& other);
        ~VertexAttribute() = default;
        
        VertexFormat format() const;
        void setFormat(VertexFormat format);
        
        uint32_t offset() const;
        void setOffset(uint32_t offset);
        
        uint32_t bufferIndex() const;
        void setBufferIndex(uint32_t index);
    };
    
    class NATGLX_API VertexBufferLayout
    {
        friend class VertexBufferLayouts;
        
    private:
        MTL::VertexBufferLayoutDescriptor* _pVertexBufferLayoutDescriptor;
        VertexBufferLayout(MTL::VertexBufferLayoutDescriptor* pVertexBufferLayoutDescriptor);
        
    public:
        VertexBufferLayout();
        VertexBufferLayout(const VertexBufferLayout& other);
        VertexBufferLayout& operator=(const VertexBufferLayout& other);
        ~VertexBufferLayout() = default;
        
        uint32_t stride() const;
        void setStride(uint32_t stride);
        
        VertexStepFunction stepFunction() const;
        void setStepFunction(VertexStepFunction function);
        
        uint32_t stepRate() const;
        void setStepRate(uint32_t rate);
    };
    
    class NATGLX_API VertexAttributes
    {
        friend class VertexDescriptor;
        
    private:
        MTL::VertexAttributeDescriptorArray* _pArray;
        VertexAttributes(MTL::VertexAttributeDescriptorArray* pArray);
        
    public:
        VertexAttributes();
        VertexAttributes(const VertexAttributes& other);
        VertexAttributes& operator=(const VertexAttributes& other);
        ~VertexAttributes() = default;
        
        VertexAttribute object(size_t index);
        VertexAttribute operator[](size_t index);
    };
    
    class NATGLX_API VertexBufferLayouts
    {
        friend class VertexDescriptor;
        
    private:
        MTL::VertexBufferLayoutDescriptorArray* _pArray;
        VertexBufferLayouts(MTL::VertexBufferLayoutDescriptorArray* pArray);
        
    public:
        VertexBufferLayouts();
        VertexBufferLayouts(const VertexBufferLayouts& other);
        VertexBufferLayouts& operator=(const VertexBufferLayouts& other);
        ~VertexBufferLayouts() = default;
        
        VertexBufferLayout object(size_t index);
        VertexBufferLayout operator[](size_t index);
    };
    
    class NATGLX_API VertexDescriptor
    {
        friend class RenderPipeline::Descriptor;
        
    private:
        MTL::VertexDescriptor* _pVertexDescriptor;
        
        VertexDescriptor(MTL::VertexDescriptor* pVertexDescriptor);
        
    public:
        VertexDescriptor();
        VertexDescriptor(const VertexDescriptor& other);
        VertexDescriptor& operator=(const VertexDescriptor& other);
        ~VertexDescriptor();
        
        static VertexDescriptor alloc();
        
        VertexDescriptor init();
        
        VertexAttributes attributes();
        VertexBufferLayouts layouts();
        
        void reset();
        
        bool isOk() const;
        void release();
        
        MTL::VertexDescriptor* getNatPtr();
        const MTL::VertexDescriptor* getNatPtr() const;
    };
    
} // namespace glx