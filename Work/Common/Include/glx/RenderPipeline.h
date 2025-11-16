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
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <cnt/PushBackVector.h>
#include <cnt/SafeFullVector.h>
//#include "Device.h"
#include "Function.h"

namespace MTL
{
class RenderPipelineState;
class RenderPipelineDescriptor;
class RenderPipelineColorAttachmentDescriptorArray;
class RenderPipelineColorAttachmentDescriptor;
} //namespace MTL

namespace glx
{
    class Device;

class NATGLX_API RenderPipeline //RenderPipelineState
{
    friend class Device;
public:
    class ColorAttachments;
    
    class NATGLX_API ColorAttachment
    {
        friend class ColorAttachments;
    private:
        MTL::RenderPipelineColorAttachmentDescriptor* _pRenderPipelineColorAttachmentDescriptor;
        ColorAttachment(MTL::RenderPipelineColorAttachmentDescriptor* pRenderPipelineColorAttachmentDescriptor);
    public:
        ColorAttachment();
        void operator = (const ColorAttachment& colorAttachment);
        
        static ColorAttachment alloc();

        void init();

        PixelFormat pixelFormat() const;
        void setPixelFormat(PixelFormat pixelFormat);

        bool blendingEnabled() const;
        void setBlendingEnabled(bool blendingEnabled);

        BlendFactor    sourceRGBBlendFactor() const;
        void           setSourceRGBBlendFactor(BlendFactor sourceRGBBlendFactor);

        BlendFactor    destinationRGBBlendFactor() const;
        void           setDestinationRGBBlendFactor(BlendFactor destinationRGBBlendFactor);

        BlendOperation rgbBlendOperation() const;
        void           setRgbBlendOperation(BlendOperation rgbBlendOperation);

        BlendFactor    sourceAlphaBlendFactor() const;
        void           setSourceAlphaBlendFactor(BlendFactor sourceAlphaBlendFactor);

        BlendFactor    destinationAlphaBlendFactor() const;
        void           setDestinationAlphaBlendFactor(BlendFactor destinationAlphaBlendFactor);

        BlendOperation alphaBlendOperation() const;
        void           setAlphaBlendOperation(BlendOperation alphaBlendOperation);

        ColorWriteMask writeMask() const;
        void           setWriteMask(ColorWriteMask writeMask);
    };
    
    class Descriptor;
    class NATGLX_API ColorAttachments
    {
        friend class Descriptor;
        
    private:
        MTL::RenderPipelineColorAttachmentDescriptorArray* _pRenderPipelineColorAttachmentDescriptorArray;
        ColorAttachments(MTL::RenderPipelineColorAttachmentDescriptorArray* pRenderPipelineColorAttachmentDescriptorArray);

    public:
        ColorAttachments();
        void operator = (const ColorAttachments& colorAttachments);
        ColorAttachment attachment(size_t pos);
        ColorAttachment operator [] (size_t pos)
        {
            return attachment(pos);
        }
    };
    
    class NATGLX_API Descriptor
    {
        MTL::RenderPipelineDescriptor* _pRenderPipelineDescriptor;
        Descriptor(MTL::RenderPipelineDescriptor* pRenderPipelineDescriptor);
    public:
        // constructor with vectors
        Descriptor();
        void operator = (const Descriptor& descriptor);
        
        static Descriptor alloc();

        void init();

        td::String label() const;
        void setLabel(const char* label);

        Function vertexFunction() const;
        void setVertexFunction(const Function& vertexFunction);

        Function fragmentFunction() const;
        void setFragmentFunction(const Function& fragmentFunction);

        class VertexDescriptor vertexDescriptor() const;
        void setVertexDescriptor(const class VertexDescriptor& vertexDescriptor);

        size_t sampleCount() const;
        void setSampleCount(size_t sampleCount);

        size_t rasterSampleCount() const;
        void setRasterSampleCount(size_t rasterSampleCount);

        bool alphaToCoverageEnabled() const;
        void setAlphaToCoverageEnabled(bool alphaToCoverageEnabled);

        bool alphaToOneEnabled() const;
        void setAlphaToOneEnabled(bool alphaToOneEnabled);

        bool rasterizationEnabled() const;
        void setRasterizationEnabled(bool rasterizationEnabled);

        size_t maxVertexAmplificationCount() const;
        void setMaxVertexAmplificationCount(size_t maxVertexAmplificationCount);

        ColorAttachments colorAttachments() const; //RenderPipelineColorAttachmentDescriptorArray

        PixelFormat depthAttachmentPixelFormat() const;
        void setDepthAttachmentPixelFormat(PixelFormat depthAttachmentPixelFormat);

        PixelFormat stencilAttachmentPixelFormat() const;
        void setStencilAttachmentPixelFormat(PixelFormat stencilAttachmentPixelFormat);
        
        PrimitiveClass inputPrimitiveTopology() const; //PrimitiveTopologyClass
        void setInputPrimitiveTopology(PrimitiveClass inputPrimitiveTopology);

        TessellationPartitionMode tessellationPartitionMode() const;
        void setTessellationPartitionMode(TessellationPartitionMode tessellationPartitionMode);

        size_t maxTessellationFactor() const;
        void setMaxTessellationFactor(size_t maxTessellationFactor);

        bool tessellationFactorScaleEnabled() const;
        void setTessellationFactorScaleEnabled(bool tessellationFactorScaleEnabled);

        TessellationFactorFormat tessellationFactorFormat() const;
        void setTessellationFactorFormat(TessellationFactorFormat tessellationFactorFormat);

        TessellationControlPointIndexType tessellationControlPointIndexType() const;
        void setTessellationControlPointIndexType(TessellationControlPointIndexType tessellationControlPointIndexType);

        TessellationFactorStepFunction tessellationFactorStepFunction() const;
        void setTessellationFactorStepFunction(TessellationFactorStepFunction tessellationFactorStepFunction);

        Winding tessellationOutputWindingOrder() const;
        void setTessellationOutputWindingOrder(Winding tessellationOutputWindingOrder);

//        class PipelineBufferDescriptorArray*   vertexBuffers() const;
//
//        class PipelineBufferDescriptorArray*   fragmentBuffers() const;

        bool supportIndirectCommandBuffers() const;
        void setSupportIndirectCommandBuffers(bool supportIndirectCommandBuffers);

//        NS::Array* binaryArchives() const;
//        void       setBinaryArchives(const NS::Array* binaryArchives);
//
//        NS::Array* vertexPreloadedLibraries() const;
//        void       setVertexPreloadedLibraries(const NS::Array* vertexPreloadedLibraries);
//
//        NS::Array* fragmentPreloadedLibraries() const;
//        void       setFragmentPreloadedLibraries(const NS::Array* fragmentPreloadedLibraries);

//        class LinkedFunctions* vertexLinkedFunctions() const;
//        void                   setVertexLinkedFunctions(const class LinkedFunctions* vertexLinkedFunctions);
//
//        class LinkedFunctions* fragmentLinkedFunctions() const;
//        void                   setFragmentLinkedFunctions(const class LinkedFunctions* fragmentLinkedFunctions);

        bool supportAddingVertexBinaryFunctions() const;
        void setSupportAddingVertexBinaryFunctions(bool supportAddingVertexBinaryFunctions);

        bool supportAddingFragmentBinaryFunctions() const;
        void setSupportAddingFragmentBinaryFunctions(bool supportAddingFragmentBinaryFunctions);

        size_t maxVertexCallStackDepth() const;
        void setMaxVertexCallStackDepth(size_t maxVertexCallStackDepth);

        size_t maxFragmentCallStackDepth() const;
        void setMaxFragmentCallStackDepth(size_t maxFragmentCallStackDepth);

        void reset();
        
        MTL::RenderPipelineDescriptor* getNatPtr()
        {
            return _pRenderPipelineDescriptor;
        }
        
        const MTL::RenderPipelineDescriptor* getNatPtr() const
        {
            return _pRenderPipelineDescriptor;
        }
        
        void release();
    };
    
private:
    MTL::RenderPipelineState* _pRenderPipelineState;
    RenderPipeline(MTL::RenderPipelineState* pRenderPipelineState);
public:
    // constructor with vectors
    RenderPipeline();
    void operator = (const RenderPipeline& renderPipeline);
    
    td::String label() const;

    Device device() const;

    size_t maxTotalThreadsPerThreadgroup() const;

    bool threadgroupSizeMatchesTileSize() const;

    size_t imageblockSampleLength() const;

    size_t imageblockMemoryLength(const Volume<td::UINT4>& imageblockDimensions);

    bool supportIndirectCommandBuffers() const;

    Function::Handle functionHandle(const Function& function, RenderStage stage);

//    class VisibleFunctionTable*      newVisibleFunctionTable(const class VisibleFunctionTableDescriptor* descriptor, MTL::RenderStages stage);
//
//    class IntersectionFunctionTable* newIntersectionFunctionTable(const class IntersectionFunctionTableDescriptor* descriptor, MTL::RenderStages stage);

//    class RenderPipelineState*       newRenderPipelineState(const class RenderPipelineFunctionsDescriptor* additionalBinaryFunctions, NS::Error** error);
    
    bool isOk() const;
    
    void release();
    
    MTL::RenderPipelineState* getNatPtr()
    {
        return _pRenderPipelineState;
    }
    
    const MTL::RenderPipelineState* getNatPtr() const
    {
        return _pRenderPipelineState;
    }
};


} //namespace glx

