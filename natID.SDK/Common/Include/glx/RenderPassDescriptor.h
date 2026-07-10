// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file RenderPassDescriptor.h @brief Describes the color, depth, and stencil attachments for a single render pass. */
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

/// @brief Aggregates attachment descriptors and render-pass parameters required to
///        begin a Metal render pass through a RenderCommandEncoder.
class NATGLX_API RenderPassDescriptor
{
public:
    /// @brief Describes a single color attachment used within a render pass.
    class NATGLX_API RenderPassColorAttachmentDescriptor
    {
        MTL::RenderPassColorAttachmentDescriptor* _pColorAttachment = nullptr; ///< Pointer to the native color attachment descriptor.
    public:

        /// @brief Constructs a descriptor wrapping an existing native pointer.
        /// @param pColorAttachment Pointer to the native MTL color attachment descriptor.
        RenderPassColorAttachmentDescriptor(MTL::RenderPassColorAttachmentDescriptor* pColorAttachment);

        /// @brief Returns the texture used as the color attachment.
        /// @return The color attachment Texture.
        Texture texture() const;

        /// @brief Sets the texture to use as the color attachment.
        /// @param texture The texture to assign.
        void        setTexture(Texture);

        /// @brief Returns the load action applied to the color attachment at the start of the pass.
        /// @return The LoadAction enum value.
        LoadAction  loadAction()  const;

        /// @brief Sets the load action for the color attachment.
        /// @param loadAction The desired load action (clear, load, or don't care).
        void        setLoadAction(LoadAction);

        /// @brief Returns the store action applied to the color attachment at the end of the pass.
        /// @return The StoreAction enum value.
        StoreAction storeAction() const;

        /// @brief Sets the store action for the color attachment.
        /// @param storeAction The desired store action (store, discard, etc.).
        void        setStoreAction(StoreAction);

        /// @brief Returns the clear color applied when the load action is LoadAction::Clear.
        /// @return The ClearColor value.
        ClearColor  clearColor()  const;

        /// @brief Sets the clear color used when the load action is LoadAction::Clear.
        /// @param clearColor The RGBA clear color.
        void        setClearColor(const ClearColor&);
    };

    /// @brief Describes the depth attachment used within a render pass.
    class NATGLX_API RenderPassDepthAttachmentDescriptor
    {
        MTL::RenderPassDepthAttachmentDescriptor* _pDepthAttachment = nullptr; ///< Pointer to the native depth attachment descriptor.
    public:
        /// @brief Constructs a descriptor wrapping an existing native pointer.
        /// @param pDepthAttachment Pointer to the native MTL depth attachment descriptor.
        RenderPassDepthAttachmentDescriptor(MTL::RenderPassDepthAttachmentDescriptor* pDepthAttachment);

        /// @brief Returns the texture used as the depth attachment.
        /// @return The depth attachment Texture.
        Texture texture() const;

        /// @brief Sets the texture to use as the depth attachment.
        /// @param texture The texture to assign.
        void        setTexture(Texture);

        /// @brief Returns the load action applied to the depth attachment at the start of the pass.
        /// @return The LoadAction enum value.
        LoadAction  loadAction()  const;

        /// @brief Sets the load action for the depth attachment.
        /// @param loadAction The desired load action.
        void        setLoadAction(LoadAction);

        /// @brief Returns the store action applied to the depth attachment at the end of the pass.
        /// @return The StoreAction enum value.
        StoreAction storeAction() const;

        /// @brief Sets the store action for the depth attachment.
        /// @param storeAction The desired store action.
        void        setStoreAction(StoreAction);

        /// @brief Returns the depth clear value used when the load action is LoadAction::Clear.
        /// @return The depth clear value in the range [0.0, 1.0].
        double  clearDepth()  const;

        /// @brief Sets the depth clear value used when the load action is LoadAction::Clear.
        /// @param clearDepth The clear depth value in the range [0.0, 1.0].
        void        setClearDepth(double);

        friend class RenderPassDescriptor;
    };

    /// @brief Describes the stencil attachment used within a render pass.
    class NATGLX_API RenderPassStencilAttachmentDescriptor
    {
        MTL::RenderPassStencilAttachmentDescriptor* _pStencilAttachment = nullptr; ///< Pointer to the native stencil attachment descriptor.
    public:
        /// @brief Constructs a descriptor wrapping an existing native pointer.
        /// @param pStencilAttachment Pointer to the native MTL stencil attachment descriptor.
        RenderPassStencilAttachmentDescriptor(MTL::RenderPassStencilAttachmentDescriptor* pStencilAttachment);

        /// @brief Returns the texture used as the stencil attachment.
        /// @return The stencil attachment Texture.
        Texture texture() const;

        /// @brief Sets the texture to use as the stencil attachment.
        /// @param texture The texture to assign.
        void        setTexture(Texture);

        /// @brief Returns the load action applied to the stencil attachment at the start of the pass.
        /// @return The LoadAction enum value.
        LoadAction  loadAction()  const;

        /// @brief Sets the load action for the stencil attachment.
        /// @param loadAction The desired load action.
        void        setLoadAction(LoadAction);

        /// @brief Returns the store action applied to the stencil attachment at the end of the pass.
        /// @return The StoreAction enum value.
        StoreAction storeAction() const;

        /// @brief Sets the store action for the stencil attachment.
        /// @param storeAction The desired store action.
        void        setStoreAction(StoreAction);

        /// @brief Returns the stencil clear value used when the load action is LoadAction::Clear.
        /// @return The 8-bit stencil clear value.
        uint32_t  clearStencil()  const;

        /// @brief Sets the stencil clear value used when the load action is LoadAction::Clear.
        /// @param clearStencil The 8-bit stencil clear value.
        void        setClearStencil(uint32_t);

        friend class RenderPassDescriptor;
    };

    /// @brief Provides indexed access to the color attachment descriptors of a render pass.
    class NATGLX_API RenderPassColorAttachmentDescriptorArray
    {
		MTL::RenderPassColorAttachmentDescriptorArray* _pColorAttachments = nullptr; ///< Pointer to the native color attachment array.
    public:

        /// @brief Constructs a descriptor array wrapping an existing native pointer.
        /// @param pColorAttachments Pointer to the native MTL color attachment array.
		RenderPassColorAttachmentDescriptorArray(MTL::RenderPassColorAttachmentDescriptorArray* pColorAttachments);

        /// @brief Returns the color attachment descriptor at the given index.
        /// @param index Zero-based attachment index.
        /// @return A mutable color attachment descriptor.
        RenderPassColorAttachmentDescriptor operator[](size_t index);

        /// @brief Returns the color attachment descriptor at the given index (const overload).
        /// @param index Zero-based attachment index.
        /// @return An immutable color attachment descriptor.
        const RenderPassColorAttachmentDescriptor operator[](size_t index) const;
    };
private:
    MTL::RenderPassDescriptor* _pRenderPassDescriptor = nullptr; ///< Pointer to the underlying native Metal render pass descriptor.
public:
    /// @brief Default-constructs a null RenderPassDescriptor.
    RenderPassDescriptor();

    /// @brief Constructs a RenderPassDescriptor wrapping an existing native pointer.
    /// @param pRenderPassDescriptor Pointer to an MTL::RenderPassDescriptor.
    RenderPassDescriptor(MTL::RenderPassDescriptor* pRenderPassDescriptor);

    /// @brief Copy-constructs a RenderPassDescriptor, retaining the native resource.
    /// @param renderPassDescriptor The source descriptor to copy from.
    RenderPassDescriptor(const RenderPassDescriptor& renderPassDescriptor);

    /// @brief Copy-assigns a RenderPassDescriptor, retaining the native resource.
    /// @param renderPassDescriptor The source descriptor to copy from.
    /// @return Reference to this descriptor.
    RenderPassDescriptor& operator = (const RenderPassDescriptor& renderPassDescriptor);

    /// @brief Destructor; releases the underlying native Metal render pass descriptor.
    ~RenderPassDescriptor();

public:
    /// @brief Allocates a new uninitialized RenderPassDescriptor.
    /// @return A newly allocated RenderPassDescriptor.
    static RenderPassDescriptor alloc();

    /// @brief Initializes the render pass descriptor with default attachment settings.
    void init();

    /// @brief Allocates and initializes a ready-to-use RenderPassDescriptor.
    /// @return A fully initialized RenderPassDescriptor.
    static RenderPassDescriptor renderPassDescriptor();

    /// @brief Provides access to the color attachment descriptor array.
    /// @return The color attachment descriptor array for this render pass.
    class RenderPassColorAttachmentDescriptorArray colorAttachments() const;

    /// @brief Returns the depth attachment descriptor for this render pass.
    /// @return The depth attachment descriptor.
    class RenderPassDepthAttachmentDescriptor depthAttachment() const;

    /// @brief Replaces the depth attachment descriptor.
    /// @param depthAttachment The new depth attachment descriptor.
    void setDepthAttachment(const RenderPassDepthAttachmentDescriptor& depthAttachment);

    /// @brief Returns the stencil attachment descriptor for this render pass.
    /// @return The stencil attachment descriptor.
    class RenderPassStencilAttachmentDescriptor stencilAttachment() const;

    /// @brief Replaces the stencil attachment descriptor.
    /// @param stencilAttachment The new stencil attachment descriptor.
    void setStencilAttachment(const RenderPassStencilAttachmentDescriptor& stencilAttachment);

    /// @brief Returns the buffer used for occlusion query visibility results.
    /// @return The visibility result buffer.
    class Buffer visibilityResultBuffer() const;

    /// @brief Sets the buffer used to store occlusion query visibility results.
    /// @param visibilityResultBuffer The buffer to assign.
    void setVisibilityResultBuffer(const Buffer& visibilityResultBuffer);

    /// @brief Returns the number of active layers in an array render target.
    /// @return The render target array length.
    size_t renderTargetArrayLength() const;

    /// @brief Sets the number of active layers in an array render target.
    /// @param renderTargetArrayLength The desired layer count.
    void setRenderTargetArrayLength(size_t renderTargetArrayLength);

    /// @brief Returns the per-sample byte length of imageblock data.
    /// @return Imageblock sample length in bytes.
    size_t imageblockSampleLength() const;

    /// @brief Sets the per-sample byte length of imageblock data.
    /// @param imageblockSampleLength Imageblock sample length in bytes.
    void setImageblockSampleLength(size_t imageblockSampleLength);

    /// @brief Returns the byte length of threadgroup memory used in tile shading.
    /// @return Threadgroup memory length in bytes.
    size_t threadgroupMemoryLength() const;

    /// @brief Sets the byte length of threadgroup memory used in tile shading.
    /// @param threadgroupMemoryLength Threadgroup memory length in bytes.
    void setThreadgroupMemoryLength(size_t threadgroupMemoryLength);

    /// @brief Returns the tile width used for tile shading passes.
    /// @return Tile width in pixels.
    size_t tileWidth() const;

    /// @brief Sets the tile width for tile shading passes.
    /// @param tileWidth Tile width in pixels.
    void setTileWidth(size_t tileWidth);

    /// @brief Returns the tile height used for tile shading passes.
    /// @return Tile height in pixels.
    size_t tileHeight() const;

    /// @brief Sets the tile height for tile shading passes.
    /// @param tileHeight Tile height in pixels.
    void setTileHeight(size_t tileHeight);

    /// @brief Returns the default number of rasterization samples per pixel.
    /// @return Default raster sample count.
    size_t defaultRasterSampleCount() const;

    /// @brief Sets the default number of rasterization samples per pixel.
    /// @param defaultRasterSampleCount The desired sample count.
    void setDefaultRasterSampleCount(size_t defaultRasterSampleCount);

    /// @brief Returns the explicit render target width in pixels (for tile-based passes).
    /// @return Render target width in pixels.
    size_t renderTargetWidth() const;

    /// @brief Sets the explicit render target width in pixels.
    /// @param renderTargetWidth Width in pixels.
    void setRenderTargetWidth(size_t renderTargetWidth);

    /// @brief Returns the explicit render target height in pixels.
    /// @return Render target height in pixels.
    size_t renderTargetHeight() const;

    /// @brief Sets the explicit render target height in pixels.
    /// @param renderTargetHeight Height in pixels.
    void setRenderTargetHeight(size_t renderTargetHeight);

//    void setSamplePositions(const Point2d& positions, size_t count);//(const MTL::SamplePosition* positions, size_t count);
//
//    size_t getSamplePositions(Point2d& positions, size_t count);

//    class RasterizationRateMap rasterizationRateMap() const;
//    void setRasterizationRateMap(const class RasterizationRateMap& rasterizationRateMap);
//
//    class RenderPassSampleBufferAttachmentDescriptorArray* sampleBufferAttachments() const;

    /// @brief Increments the retain count of the underlying native descriptor.
    void retain();

    /// @brief Decrements the retain count, releasing the native descriptor if it reaches zero.
    void release();

    /// @brief Returns a mutable pointer to the underlying native Metal render pass descriptor.
    /// @return Raw MTL::RenderPassDescriptor pointer.
    MTL::RenderPassDescriptor* getNatPtr()
    {
        return _pRenderPassDescriptor;
    }

    /// @brief Returns a const pointer to the underlying native Metal render pass descriptor.
    /// @return Const raw MTL::RenderPassDescriptor pointer.
    const MTL::RenderPassDescriptor* getNatPtr() const
    {
        return _pRenderPassDescriptor;
    }

};


} //namespace glx
