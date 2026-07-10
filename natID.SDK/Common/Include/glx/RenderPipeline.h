// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file RenderPipeline.h @brief Compiled Metal render pipeline state and its associated descriptor classes. */
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

/// @brief Wraps a compiled Metal render pipeline state together with its descriptor hierarchy.
///        A RenderPipeline is created by a Device from a filled-out Descriptor and then bound
///        to a RenderCommandEncoder before issuing draw calls.
class NATGLX_API RenderPipeline //RenderPipelineState
{
    friend class Device;
public:
    class ColorAttachments;

    /// @brief Configures the blending and pixel-format settings for a single color attachment in the pipeline.
    class NATGLX_API ColorAttachment
    {
        friend class ColorAttachments;
    private:
        MTL::RenderPipelineColorAttachmentDescriptor* _pRenderPipelineColorAttachmentDescriptor; ///< Pointer to the native color attachment descriptor.

        /// @brief Constructs a ColorAttachment wrapping an existing native pointer.
        /// @param pRenderPipelineColorAttachmentDescriptor Native descriptor pointer.
        ColorAttachment(MTL::RenderPipelineColorAttachmentDescriptor* pRenderPipelineColorAttachmentDescriptor);
    public:
        /// @brief Default-constructs a null ColorAttachment.
        ColorAttachment();

        /// @brief Copy-assigns a ColorAttachment.
        /// @param colorAttachment The source attachment to copy from.
        void operator = (const ColorAttachment& colorAttachment);

        /// @brief Allocates a new uninitialized ColorAttachment.
        /// @return A newly allocated ColorAttachment.
        static ColorAttachment alloc();

        /// @brief Initializes the attachment with default Metal settings.
        void init();

        /// @brief Returns the pixel format of this color attachment.
        /// @return The PixelFormat enum value.
        PixelFormat pixelFormat() const;

        /// @brief Sets the pixel format for this color attachment.
        /// @param pixelFormat The desired pixel format.
        void setPixelFormat(PixelFormat pixelFormat);

        /// @brief Returns whether alpha blending is enabled for this attachment.
        /// @return True if blending is enabled.
        bool blendingEnabled() const;

        /// @brief Enables or disables alpha blending for this attachment.
        /// @param blendingEnabled True to enable blending.
        void setBlendingEnabled(bool blendingEnabled);

        /// @brief Returns the blend factor applied to the source RGB channels.
        /// @return The source RGB BlendFactor.
        BlendFactor    sourceRGBBlendFactor() const;

        /// @brief Sets the blend factor applied to the source RGB channels.
        /// @param sourceRGBBlendFactor The desired source RGB blend factor.
        void           setSourceRGBBlendFactor(BlendFactor sourceRGBBlendFactor);

        /// @brief Returns the blend factor applied to the destination RGB channels.
        /// @return The destination RGB BlendFactor.
        BlendFactor    destinationRGBBlendFactor() const;

        /// @brief Sets the blend factor applied to the destination RGB channels.
        /// @param destinationRGBBlendFactor The desired destination RGB blend factor.
        void           setDestinationRGBBlendFactor(BlendFactor destinationRGBBlendFactor);

        /// @brief Returns the blend operation applied to the RGB channels.
        /// @return The RGB BlendOperation.
        BlendOperation rgbBlendOperation() const;

        /// @brief Sets the blend operation for the RGB channels.
        /// @param rgbBlendOperation The desired RGB blend operation.
        void           setRgbBlendOperation(BlendOperation rgbBlendOperation);

        /// @brief Returns the blend factor applied to the source alpha channel.
        /// @return The source alpha BlendFactor.
        BlendFactor    sourceAlphaBlendFactor() const;

        /// @brief Sets the blend factor applied to the source alpha channel.
        /// @param sourceAlphaBlendFactor The desired source alpha blend factor.
        void           setSourceAlphaBlendFactor(BlendFactor sourceAlphaBlendFactor);

        /// @brief Returns the blend factor applied to the destination alpha channel.
        /// @return The destination alpha BlendFactor.
        BlendFactor    destinationAlphaBlendFactor() const;

        /// @brief Sets the blend factor applied to the destination alpha channel.
        /// @param destinationAlphaBlendFactor The desired destination alpha blend factor.
        void           setDestinationAlphaBlendFactor(BlendFactor destinationAlphaBlendFactor);

        /// @brief Returns the blend operation applied to the alpha channel.
        /// @return The alpha BlendOperation.
        BlendOperation alphaBlendOperation() const;

        /// @brief Sets the blend operation for the alpha channel.
        /// @param alphaBlendOperation The desired alpha blend operation.
        void           setAlphaBlendOperation(BlendOperation alphaBlendOperation);

        /// @brief Returns the color channel write mask for this attachment.
        /// @return The ColorWriteMask bit-field.
        ColorWriteMask writeMask() const;

        /// @brief Sets which color channels are written to this attachment.
        /// @param writeMask The desired ColorWriteMask bit-field.
        void           setWriteMask(ColorWriteMask writeMask);
    };

    class Descriptor;

    /// @brief Provides indexed access to the color attachment descriptors within a pipeline Descriptor.
    class NATGLX_API ColorAttachments
    {
        friend class Descriptor;

    private:
        MTL::RenderPipelineColorAttachmentDescriptorArray* _pRenderPipelineColorAttachmentDescriptorArray; ///< Pointer to the native color attachment array.

        /// @brief Constructs a ColorAttachments wrapper from a native array pointer.
        /// @param pRenderPipelineColorAttachmentDescriptorArray Native array pointer.
        ColorAttachments(MTL::RenderPipelineColorAttachmentDescriptorArray* pRenderPipelineColorAttachmentDescriptorArray);

    public:
        /// @brief Default-constructs a null ColorAttachments.
        ColorAttachments();

        /// @brief Copy-assigns a ColorAttachments.
        /// @param colorAttachments The source to copy from.
        void operator = (const ColorAttachments& colorAttachments);

        /// @brief Returns the color attachment descriptor at the given index.
        /// @param pos Zero-based attachment index.
        /// @return The ColorAttachment at the specified position.
        ColorAttachment attachment(size_t pos);

        /// @brief Subscript operator; returns the color attachment at the given index.
        /// @param pos Zero-based attachment index.
        /// @return The ColorAttachment at the specified position.
        ColorAttachment operator [] (size_t pos)
        {
            return attachment(pos);
        }
    };

    /// @brief Describes the configuration used to compile a RenderPipeline.
    class NATGLX_API Descriptor
    {
        MTL::RenderPipelineDescriptor* _pRenderPipelineDescriptor; ///< Pointer to the native pipeline descriptor.

        /// @brief Constructs a Descriptor wrapping an existing native pointer.
        /// @param pRenderPipelineDescriptor Native descriptor pointer.
        Descriptor(MTL::RenderPipelineDescriptor* pRenderPipelineDescriptor);
    public:
        // constructor with vectors
        /// @brief Default-constructs a null Descriptor.
        Descriptor();

        /// @brief Copy-assigns a Descriptor.
        /// @param descriptor The source descriptor to copy from.
        void operator = (const Descriptor& descriptor);

        /// @brief Allocates a new uninitialized Descriptor.
        /// @return A newly allocated Descriptor.
        static Descriptor alloc();

        /// @brief Initializes the descriptor with Metal default settings.
        void init();

        /// @brief Returns the debug label associated with this descriptor.
        /// @return The label string.
        td::String label() const;

        /// @brief Sets a debug label on this descriptor.
        /// @param label Null-terminated label string.
        void setLabel(const char* label);

        /// @brief Returns the vertex shader function assigned to this pipeline.
        /// @return The vertex Function.
        Function vertexFunction() const;

        /// @brief Assigns a vertex shader function to this pipeline descriptor.
        /// @param vertexFunction The compiled vertex Function to use.
        void setVertexFunction(const Function& vertexFunction);

        /// @brief Returns the fragment shader function assigned to this pipeline.
        /// @return The fragment Function.
        Function fragmentFunction() const;

        /// @brief Assigns a fragment shader function to this pipeline descriptor.
        /// @param fragmentFunction The compiled fragment Function to use.
        void setFragmentFunction(const Function& fragmentFunction);

        /// @brief Returns the vertex descriptor that describes per-vertex attribute layouts.
        /// @return The VertexDescriptor.
        class VertexDescriptor vertexDescriptor() const;

        /// @brief Sets the vertex descriptor that describes per-vertex attribute layouts.
        /// @param vertexDescriptor The VertexDescriptor to assign.
        void setVertexDescriptor(const class VertexDescriptor& vertexDescriptor);

        /// @brief Returns the number of samples per pixel used for multisampling.
        /// @return The sample count.
        size_t sampleCount() const;

        /// @brief Sets the number of samples per pixel for multisampling.
        /// @param sampleCount The desired sample count.
        void setSampleCount(size_t sampleCount);

        /// @brief Returns the rasterization sample count (may differ from sampleCount for tile shaders).
        /// @return The raster sample count.
        size_t rasterSampleCount() const;

        /// @brief Sets the rasterization sample count.
        /// @param rasterSampleCount The desired raster sample count.
        void setRasterSampleCount(size_t rasterSampleCount);

        /// @brief Returns whether alpha-to-coverage is enabled.
        /// @return True if alpha-to-coverage is enabled.
        bool alphaToCoverageEnabled() const;

        /// @brief Enables or disables alpha-to-coverage.
        /// @param alphaToCoverageEnabled True to enable.
        void setAlphaToCoverageEnabled(bool alphaToCoverageEnabled);

        /// @brief Returns whether alpha-to-one is enabled.
        /// @return True if alpha-to-one is enabled.
        bool alphaToOneEnabled() const;

        /// @brief Enables or disables alpha-to-one.
        /// @param alphaToOneEnabled True to enable.
        void setAlphaToOneEnabled(bool alphaToOneEnabled);

        /// @brief Returns whether rasterization is enabled for this pipeline.
        /// @return True if rasterization is enabled.
        bool rasterizationEnabled() const;

        /// @brief Enables or disables rasterization.
        /// @param rasterizationEnabled True to enable rasterization.
        void setRasterizationEnabled(bool rasterizationEnabled);

        /// @brief Returns the maximum vertex amplification count for multi-viewport rendering.
        /// @return The maximum amplification count.
        size_t maxVertexAmplificationCount() const;

        /// @brief Sets the maximum vertex amplification count for multi-viewport rendering.
        /// @param maxVertexAmplificationCount The desired maximum count.
        void setMaxVertexAmplificationCount(size_t maxVertexAmplificationCount);

        /// @brief Returns the array of color attachment descriptors for this pipeline.
        /// @return The ColorAttachments descriptor array.
        ColorAttachments colorAttachments() const; //RenderPipelineColorAttachmentDescriptorArray

        /// @brief Returns the pixel format of the depth attachment.
        /// @return The depth PixelFormat.
        PixelFormat depthAttachmentPixelFormat() const;

        /// @brief Sets the pixel format of the depth attachment.
        /// @param depthAttachmentPixelFormat The desired depth pixel format.
        void setDepthAttachmentPixelFormat(PixelFormat depthAttachmentPixelFormat);

        /// @brief Returns the pixel format of the stencil attachment.
        /// @return The stencil PixelFormat.
        PixelFormat stencilAttachmentPixelFormat() const;

        /// @brief Sets the pixel format of the stencil attachment.
        /// @param stencilAttachmentPixelFormat The desired stencil pixel format.
        void setStencilAttachmentPixelFormat(PixelFormat stencilAttachmentPixelFormat);

        /// @brief Returns the expected input primitive topology class for this pipeline.
        /// @return The PrimitiveClass enum value.
        PrimitiveClass inputPrimitiveTopology() const; //PrimitiveTopologyClass

        /// @brief Sets the expected input primitive topology class.
        /// @param inputPrimitiveTopology The desired primitive topology class.
        void setInputPrimitiveTopology(PrimitiveClass inputPrimitiveTopology);

        /// @brief Returns the tessellation partition mode.
        /// @return The TessellationPartitionMode.
        TessellationPartitionMode tessellationPartitionMode() const;

        /// @brief Sets the tessellation partition mode.
        /// @param tessellationPartitionMode The desired partition mode.
        void setTessellationPartitionMode(TessellationPartitionMode tessellationPartitionMode);

        /// @brief Returns the maximum tessellation factor allowed by this pipeline.
        /// @return The maximum tessellation factor.
        size_t maxTessellationFactor() const;

        /// @brief Sets the maximum tessellation factor.
        /// @param maxTessellationFactor The desired maximum factor.
        void setMaxTessellationFactor(size_t maxTessellationFactor);

        /// @brief Returns whether tessellation factor scaling is enabled.
        /// @return True if factor scaling is enabled.
        bool tessellationFactorScaleEnabled() const;

        /// @brief Enables or disables tessellation factor scaling.
        /// @param tessellationFactorScaleEnabled True to enable.
        void setTessellationFactorScaleEnabled(bool tessellationFactorScaleEnabled);

        /// @brief Returns the format of the tessellation factor buffer entries.
        /// @return The TessellationFactorFormat.
        TessellationFactorFormat tessellationFactorFormat() const;

        /// @brief Sets the format of tessellation factor buffer entries.
        /// @param tessellationFactorFormat The desired format.
        void setTessellationFactorFormat(TessellationFactorFormat tessellationFactorFormat);

        /// @brief Returns the index type for tessellation control-point indices.
        /// @return The TessellationControlPointIndexType.
        TessellationControlPointIndexType tessellationControlPointIndexType() const;

        /// @brief Sets the index type for tessellation control-point indices.
        /// @param tessellationControlPointIndexType The desired index type.
        void setTessellationControlPointIndexType(TessellationControlPointIndexType tessellationControlPointIndexType);

        /// @brief Returns the step function used to advance through the tessellation factor buffer.
        /// @return The TessellationFactorStepFunction.
        TessellationFactorStepFunction tessellationFactorStepFunction() const;

        /// @brief Sets the step function used to advance through the tessellation factor buffer.
        /// @param tessellationFactorStepFunction The desired step function.
        void setTessellationFactorStepFunction(TessellationFactorStepFunction tessellationFactorStepFunction);

        /// @brief Returns the winding order of the triangles output by the tessellator.
        /// @return The output winding Winding enum value.
        Winding tessellationOutputWindingOrder() const;

        /// @brief Sets the winding order of the triangles output by the tessellator.
        /// @param tessellationOutputWindingOrder The desired winding order.
        void setTessellationOutputWindingOrder(Winding tessellationOutputWindingOrder);

//        class PipelineBufferDescriptorArray*   vertexBuffers() const;
//
//        class PipelineBufferDescriptorArray*   fragmentBuffers() const;

        /// @brief Returns whether this pipeline supports indirect command buffers.
        /// @return True if indirect command buffers are supported.
        bool supportIndirectCommandBuffers() const;

        /// @brief Enables or disables support for indirect command buffers.
        /// @param supportIndirectCommandBuffers True to enable support.
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

        /// @brief Returns whether adding binary vertex functions to this pipeline is supported.
        /// @return True if supported.
        bool supportAddingVertexBinaryFunctions() const;

        /// @brief Enables or disables adding binary vertex functions.
        /// @param supportAddingVertexBinaryFunctions True to enable.
        void setSupportAddingVertexBinaryFunctions(bool supportAddingVertexBinaryFunctions);

        /// @brief Returns whether adding binary fragment functions to this pipeline is supported.
        /// @return True if supported.
        bool supportAddingFragmentBinaryFunctions() const;

        /// @brief Enables or disables adding binary fragment functions.
        /// @param supportAddingFragmentBinaryFunctions True to enable.
        void setSupportAddingFragmentBinaryFunctions(bool supportAddingFragmentBinaryFunctions);

        /// @brief Returns the maximum call stack depth allowed in the vertex shader.
        /// @return Maximum vertex call stack depth.
        size_t maxVertexCallStackDepth() const;

        /// @brief Sets the maximum call stack depth for the vertex shader.
        /// @param maxVertexCallStackDepth The desired maximum depth.
        void setMaxVertexCallStackDepth(size_t maxVertexCallStackDepth);

        /// @brief Returns the maximum call stack depth allowed in the fragment shader.
        /// @return Maximum fragment call stack depth.
        size_t maxFragmentCallStackDepth() const;

        /// @brief Sets the maximum call stack depth for the fragment shader.
        /// @param maxFragmentCallStackDepth The desired maximum depth.
        void setMaxFragmentCallStackDepth(size_t maxFragmentCallStackDepth);

        /// @brief Resets all descriptor properties to their Metal default values.
        void reset();

        /// @brief Returns a mutable pointer to the underlying native pipeline descriptor.
        /// @return Raw MTL::RenderPipelineDescriptor pointer.
        MTL::RenderPipelineDescriptor* getNatPtr()
        {
            return _pRenderPipelineDescriptor;
        }

        /// @brief Returns a const pointer to the underlying native pipeline descriptor.
        /// @return Const raw MTL::RenderPipelineDescriptor pointer.
        const MTL::RenderPipelineDescriptor* getNatPtr() const
        {
            return _pRenderPipelineDescriptor;
        }

        /// @brief Releases the underlying native pipeline descriptor.
        void release();
    };

private:
    MTL::RenderPipelineState* _pRenderPipelineState; ///< Pointer to the compiled native Metal pipeline state.

    /// @brief Constructs a RenderPipeline from a compiled native pipeline state.
    /// @param pRenderPipelineState Pointer to the compiled MTL::RenderPipelineState.
    RenderPipeline(MTL::RenderPipelineState* pRenderPipelineState);
public:
    // constructor with vectors
    /// @brief Default-constructs a null RenderPipeline.
    RenderPipeline();

    /// @brief Copy-constructs a RenderPipeline, retaining the native resource.
    /// @param renderPipeline The source pipeline to copy from.
    RenderPipeline(const RenderPipeline& renderPipeline);

    /// @brief Copy-assigns a RenderPipeline.
    /// @param renderPipeline The source pipeline to copy from.
    void operator = (const RenderPipeline& renderPipeline);

    /// @brief Destructor; releases the underlying native pipeline state.
    ~RenderPipeline();

    /// @brief Returns the debug label associated with this pipeline.
    /// @return The label string.
    td::String label() const;

    /// @brief Returns the device on which this pipeline was compiled.
    /// @return The owning Device.
    Device device() const;

    /// @brief Returns the maximum number of threads per threadgroup for tile shaders.
    /// @return Maximum total threads per threadgroup.
    size_t maxTotalThreadsPerThreadgroup() const;

    /// @brief Returns whether the threadgroup size must match the tile size.
    /// @return True if threadgroup size must match tile size.
    bool threadgroupSizeMatchesTileSize() const;

    /// @brief Returns the per-sample byte length of imageblock data.
    /// @return Imageblock sample length in bytes.
    size_t imageblockSampleLength() const;

    /// @brief Returns the total imageblock memory length for the given dimensions.
    /// @param imageblockDimensions Width, height, and depth of the imageblock region.
    /// @return Total imageblock memory in bytes.
    size_t imageblockMemoryLength(const Volume<td::UINT4>& imageblockDimensions);

    /// @brief Returns whether this pipeline supports indirect command buffers.
    /// @return True if indirect command buffers are supported.
    bool supportIndirectCommandBuffers() const;

    /// @brief Returns a handle to a function within this pipeline for use in visible function tables.
    /// @param function The Function whose handle is requested.
    /// @param stage    The render stage (vertex or fragment) where the function resides.
    /// @return A Function::Handle referencing the function within the pipeline.
    Function::Handle functionHandle(const Function& function, RenderStage stage);

//    class VisibleFunctionTable*      newVisibleFunctionTable(const class VisibleFunctionTableDescriptor* descriptor, MTL::RenderStages stage);
//
//    class IntersectionFunctionTable* newIntersectionFunctionTable(const class IntersectionFunctionTableDescriptor* descriptor, MTL::RenderStages stage);

//    class RenderPipelineState*       newRenderPipelineState(const class RenderPipelineFunctionsDescriptor* additionalBinaryFunctions, NS::Error** error);

    /// @brief Returns whether this pipeline wraps a valid compiled native state.
    /// @return True if the pipeline is ready to use.
    bool isOk() const;

    /// @brief Releases the underlying native pipeline state.
    void release();

    /// @brief Returns a mutable pointer to the underlying native pipeline state.
    /// @return Raw MTL::RenderPipelineState pointer.
    MTL::RenderPipelineState* getNatPtr()
    {
        return _pRenderPipelineState;
    }

    /// @brief Returns a const pointer to the underlying native pipeline state.
    /// @return Const raw MTL::RenderPipelineState pointer.
    const MTL::RenderPipelineState* getNatPtr() const
    {
        return _pRenderPipelineState;
    }
};


} //namespace glx
