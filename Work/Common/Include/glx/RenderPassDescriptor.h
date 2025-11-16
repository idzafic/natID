// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include "Buffer.h"
#include <glx/Types.h>


namespace MTL
{
class RenderPassDescriptor;
class RenderPassColorAttachmentDescriptorArray;
class RenderPassColorAttachmentDescriptor;
class RenderPassDepthAttachmentDescriptor;
class RenderPassStencilAttachmentDescriptor;
} //namespace MTL

namespace glx
{
    class Texture;

class NATGLX_API RenderPassDescriptor
{
public:
    class NATGLX_API RenderPassColorAttachmentDescriptor
    {
        MTL::RenderPassColorAttachmentDescriptor* _pColorAttachment = nullptr;
    public:


        RenderPassColorAttachmentDescriptor(MTL::RenderPassColorAttachmentDescriptor* pColorAttachment);

        Texture texture() const;
        void        setTexture(Texture);

        LoadAction  loadAction()  const;
        void        setLoadAction(LoadAction);

        StoreAction storeAction() const;
        void        setStoreAction(StoreAction);

        ClearColor  clearColor()  const;
        void        setClearColor(const ClearColor&);
    };

    class NATGLX_API RenderPassDepthAttachmentDescriptor
    {
        MTL::RenderPassDepthAttachmentDescriptor* _pDepthAttachment = nullptr;
    public:
        RenderPassDepthAttachmentDescriptor(MTL::RenderPassDepthAttachmentDescriptor* pDepthAttachment);

        Texture texture() const;
        void        setTexture(Texture);

        LoadAction  loadAction()  const;
        void        setLoadAction(LoadAction);

        StoreAction storeAction() const;
        void        setStoreAction(StoreAction);

        double  clearDepth()  const;
        void        setClearDepth(double);

        friend class RenderPassDescriptor;
    };

    class NATGLX_API RenderPassStencilAttachmentDescriptor
    {
        MTL::RenderPassStencilAttachmentDescriptor* _pStencilAttachment = nullptr;
    public:
        RenderPassStencilAttachmentDescriptor(MTL::RenderPassStencilAttachmentDescriptor* pStencilAttachment);

        Texture texture() const;
        void        setTexture(Texture);

        LoadAction  loadAction()  const;
        void        setLoadAction(LoadAction);

        StoreAction storeAction() const;
        void        setStoreAction(StoreAction);

        uint32_t  clearStencil()  const;
        void        setClearStencil(uint32_t);

        friend class RenderPassDescriptor;
    };

    class NATGLX_API RenderPassColorAttachmentDescriptorArray
    {
		MTL::RenderPassColorAttachmentDescriptorArray* _pColorAttachments = nullptr;
    public:

		RenderPassColorAttachmentDescriptorArray(MTL::RenderPassColorAttachmentDescriptorArray* pColorAttachments);

        RenderPassColorAttachmentDescriptor operator[](size_t index);
        const RenderPassColorAttachmentDescriptor operator[](size_t index) const;
    };
private:
    MTL::RenderPassDescriptor* _pRenderPassDescriptor = nullptr;
public:
    RenderPassDescriptor();
    RenderPassDescriptor(MTL::RenderPassDescriptor* pRenderPassDescriptor);
    void operator = (const RenderPassDescriptor& renderPassDescriptor);
    
public:
    static RenderPassDescriptor alloc();

    void init();

    static RenderPassDescriptor renderPassDescriptor();
    
    class RenderPassColorAttachmentDescriptorArray colorAttachments() const;

    class RenderPassDepthAttachmentDescriptor depthAttachment() const;
    void setDepthAttachment(const RenderPassDepthAttachmentDescriptor& depthAttachment);

    class RenderPassStencilAttachmentDescriptor stencilAttachment() const;
    void setStencilAttachment(const RenderPassStencilAttachmentDescriptor& stencilAttachment);

    class Buffer visibilityResultBuffer() const;
    void setVisibilityResultBuffer(const Buffer& visibilityResultBuffer);

    size_t renderTargetArrayLength() const;
    void setRenderTargetArrayLength(size_t renderTargetArrayLength);

    size_t imageblockSampleLength() const;
    void setImageblockSampleLength(size_t imageblockSampleLength);

    size_t threadgroupMemoryLength() const;
    void setThreadgroupMemoryLength(size_t threadgroupMemoryLength);

    size_t tileWidth() const;
    void setTileWidth(size_t tileWidth);

    size_t tileHeight() const;
    void setTileHeight(size_t tileHeight);

    size_t defaultRasterSampleCount() const;
    void setDefaultRasterSampleCount(size_t defaultRasterSampleCount);

    size_t renderTargetWidth() const;
    void setRenderTargetWidth(size_t renderTargetWidth);

    size_t renderTargetHeight() const;
    void setRenderTargetHeight(size_t renderTargetHeight);

//    void setSamplePositions(const Point2d& positions, size_t count);//(const MTL::SamplePosition* positions, size_t count);
//
//    size_t getSamplePositions(Point2d& positions, size_t count);

//    class RasterizationRateMap rasterizationRateMap() const;
//    void setRasterizationRateMap(const class RasterizationRateMap& rasterizationRateMap);
//
//    class RenderPassSampleBufferAttachmentDescriptorArray* sampleBufferAttachments() const;
    
    MTL::RenderPassDescriptor* getNatPtr()
    {
        return _pRenderPassDescriptor;
    }
    
    const MTL::RenderPassDescriptor* getNatPtr() const
    {
        return _pRenderPassDescriptor;
    }
    
};


} //namespace glx

