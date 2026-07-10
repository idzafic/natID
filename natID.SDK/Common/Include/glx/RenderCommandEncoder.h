// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file RenderCommandEncoder.h @brief Records render commands (draw calls, state, resources) into a Metal command buffer. */
#pragma once
#include "natGLX.h"
#include "Buffer.h"
#include "RenderPipeline.h"
#include "RenderPassDescriptor.h"
//#include "CommandEncoder.h"


namespace MTL
{
class RenderCommandEncoder;
} //namespace MTL

namespace glx
{

class CommandBuffer;
class Texture;
class SamplerState;
class DepthStencilState;
class Font;

/// @brief Records rendering commands—pipeline state, resource bindings, and draw calls—into
///        a Metal command buffer for a single render pass.
class NATGLX_API RenderCommandEncoder
{
    friend class CommandBuffer;
private:
    MTL::RenderCommandEncoder* _pRenderCommandEncoder; ///< Pointer to the underlying native Metal render command encoder.
    Size _size;                                        ///< Viewport size associated with this encoder.
    glx::RenderPassDescriptor _currentDescriptor;     ///< The render pass descriptor active for this encoder.

    /// @brief Constructs a RenderCommandEncoder from a native pointer (no pass descriptor).
    /// @param pRenderCommandEncoder Native Metal render command encoder.
    RenderCommandEncoder(MTL::RenderCommandEncoder* pRenderCommandEncoder);

    /// @brief Constructs a RenderCommandEncoder from a native pointer and an explicit render pass descriptor.
    /// @param pRenderCommandEncoder Native Metal render command encoder.
    /// @param renderPassDescriptor The descriptor describing the attachments for this pass.
    RenderCommandEncoder(MTL::RenderCommandEncoder* pRenderCommandEncoder, const RenderPassDescriptor& renderPassDescriptor);
public:
    // constructor with vectors
    /// @brief Default-constructs a null RenderCommandEncoder.
    RenderCommandEncoder();

    /// @brief Copy-assigns a RenderCommandEncoder.
    /// @param renderCommandEncoder The source encoder to copy from.
    void operator = (const RenderCommandEncoder& renderCommandEncoder);
public:

    /// @brief Encodes the draw commands needed to render the given font's buffered text.
    /// @param font The Font object whose accumulated glyph geometry should be drawn.
    void drawFont(glx::Font &font);

    /// @brief Binds a render pipeline state for subsequent draw calls.
    /// @param pipelineState The compiled render pipeline to activate.
    void setRenderPipelineState(const RenderPipeline& pipelineState);

    /// @brief Copies inline bytes into the vertex shader's buffer argument table.
    /// @param bytes Pointer to the data to copy.
    /// @param length Number of bytes to copy.
    /// @param index Buffer argument table index in the vertex shader.
    void setVertexBytes(const void* bytes, size_t length, size_t index);

    /// @brief Binds a GPU buffer to the vertex shader's buffer argument table.
    /// @param buffer The buffer to bind.
    /// @param offset Byte offset within the buffer.
    /// @param index Buffer argument table index in the vertex shader.
    void setVertexBuffer(const Buffer& buffer, size_t offset, size_t index);

    /// @brief Updates the byte offset of a previously bound vertex buffer without rebinding.
    /// @param offset New byte offset within the buffer.
    /// @param index Buffer argument table index in the vertex shader.
    void setVertexBufferOffset(size_t offset, size_t index);

//    void setVertexBuffers(MTL::Buffer* buffers[], const NS::UInteger offsets[], NS::Range range);

//    void setVertexTexture(const class Texture* texture, NS::UInteger index);

//    void setVertexTextures(MTL::Texture* textures[], NS::Range range);

//    void setVertexSamplerState(const class SamplerState* sampler, NS::UInteger index);
//
//    void setVertexSamplerStates(MTL::SamplerState* samplers[], NS::Range range);
//
//    void setVertexSamplerState(const class SamplerState* sampler, float lodMinClamp, float lodMaxClamp, NS::UInteger index);
//
//    void setVertexSamplerStates(MTL::SamplerState* samplers[], const float lodMinClamps[], const float lodMaxClamps[], NS::Range range);
//
//    void setVertexVisibleFunctionTable(const class VisibleFunctionTable* functionTable, NS::UInteger bufferIndex);
//
//    void setVertexVisibleFunctionTables(const class VisibleFunctionTable* functionTables[], NS::Range range);
//
//    void setVertexIntersectionFunctionTable(const class IntersectionFunctionTable* intersectionFunctionTable, NS::UInteger bex);
//
//    void setVertexIntersectionFunctionTables(const class IntersectionFunctionTable* intersectionFunctionTables[], NS::Range range);
//
//    void setVertexAccelerationStructure(const class AccelerationStructure* accelerationStructure, NS::UInteger bufferIndex);

    /// @brief Sets the viewport rectangle for rasterization.
    /// @param viewport The viewport origin, size, and depth range.
    void setViewport(Viewport viewport);

//    void setViewports(const MTL::Viewport* viewports, NS::UInteger count);

    /// @brief Sets the winding order that defines the front face of triangles.
    /// @param frontFacingWinding The winding order (clockwise or counter-clockwise).
    void setFrontFacingWinding(Winding frontFacingWinding);

//    void setVertexAmplificationCount(NS::UInteger count, const MTL::VertexAmplificationViewMapping* viewMappings);

    /// @brief Sets the triangle face culling mode.
    /// @param cullMode Which faces to cull (none, front, or back).
    void setCullMode(CullMode cullMode);

    /// @brief Sets how geometry outside the depth range is handled.
    /// @param depthClipMode Whether to clip or clamp out-of-range depth values.
    void setDepthClipMode(DepthClipMode depthClipMode);

    /// @brief Applies a depth bias to rendered primitives to reduce z-fighting.
    /// @param depthBias Constant bias added to the fragment depth.
    /// @param slopeScale Scale applied to the fragment's slope-based depth estimate.
    /// @param clamp Maximum absolute value the combined bias may reach.
    void setDepthBias(float depthBias, float slopeScale, float clamp);

    /// @brief Restricts rasterization to a rectangular scissor region.
    /// @param rect The scissor rectangle in pixel coordinates.
    void setScissorRect(const ScissorRect& rect);

//    void setScissorRects(const MTL::ScissorRect* scissorRects, NS::UInteger count);

    /// @brief Sets the fill mode for rasterized triangles (solid or wireframe).
    /// @param fillMode The desired triangle fill mode.
    void setTriangleFillMode(TriangleFillMode fillMode);

    /// @brief Copies inline bytes into the fragment shader's buffer argument table.
    /// @param bytes Pointer to the data to copy.
    /// @param length Number of bytes to copy.
    /// @param index Buffer argument table index in the fragment shader.
    void setFragmentBytes(const void* bytes, size_t length, size_t index);

    /// @brief Binds a GPU buffer to the fragment shader's buffer argument table.
    /// @param buffer The buffer to bind.
    /// @param offset Byte offset within the buffer.
    /// @param index Buffer argument table index in the fragment shader.
    void setFragmentBuffer(const Buffer& buffer, size_t offset, size_t index);

    /// @brief Updates the byte offset of a previously bound fragment buffer without rebinding.
    /// @param offset New byte offset within the buffer.
    /// @param index Buffer argument table index in the fragment shader.
    void setFragmentBufferOffset(size_t offset, size_t index);

//    void setFragmentBuffers(MTL::Buffer* buffers[], const NS::UInteger offsets[], NS::Range range);

    /// @brief Binds a texture to the fragment shader's texture argument table.
    /// @param texture The texture to bind.
    /// @param index Texture argument table index in the fragment shader.
    void setFragmentTexture(const Texture texture, uint32_t index);

//    void setFragmentTextures(MTL::Texture* textures[], NS::Range range);
//
    /// @brief Binds a sampler state to the fragment shader's sampler argument table.
    /// @param sampler Pointer to the SamplerState to bind.
    /// @param index Sampler argument table index in the fragment shader.
    void setFragmentSamplerState(const SamplerState* sampler, uint32_t index);
//
//    void setFragmentSamplerStates(MTL::SamplerState* samplers[], NS::Range range);
//
//    void setFragmentSamplerState(const class SamplerState* sampler, float lodMinClamp, float lodMaxClamp, NS::UInteger index);
//
//    void setFragmentSamplerStates(MTL::SamplerState* samplers[], const float lodMinClamps[], const float lodMaxClamps[], NS::Range r
//
//    void setFragmentVisibleFunctionTable(const class VisibleFunctionTable* functionTable, NS::UInteger bufferIndex);
//
//    void setFragmentVisibleFunctionTables(const VisibleFunctionTable* functionTables[], NS::Range range);
//
//    void setFragmentIntersectionFunctionTable(const class IntersectionFunctionTable* intersectionFunctionTable, NS::UInteger bex);
//
//    void setFragmentIntersectionFunctionTables(const class IntersectionFunctionTable* intersectionFunctionTables[], NS::Range r
//
//    void setFragmentAccelerationStructure(const class AccelerationStructure* accelerationStructure, NS::UInteger bufferIndex);

    /// @brief Sets the constant blend color used by blend operations that reference it.
    /// @param red   Red component of the blend color [0, 1].
    /// @param green Green component of the blend color [0, 1].
    /// @param blue  Blue component of the blend color [0, 1].
    /// @param alpha Alpha component of the blend color [0, 1].
    void setBlendColorRed(float red, float green, float blue, float alpha);

    /// @brief Binds a depth-stencil state object for depth and stencil testing.
    /// @param depthStencilState The depth-stencil state to activate.
    void setDepthStencilState(const DepthStencilState depthStencilState);

    /// @brief Sets the same reference value for both front and back stencil tests.
    /// @param referenceValue The stencil reference value.
    void setStencilReferenceValue(uint32_t referenceValue);

    /// @brief Sets independent reference values for front and back stencil tests.
    /// @param frontReferenceValue Reference value for front-facing stencil tests.
    /// @param backReferenceValue  Reference value for back-facing stencil tests.
    void setStencilFrontReferenceValue(uint32_t frontReferenceValue, uint32_t backReferenceValue);

    /// @brief Draws a range of primitives using the currently bound vertex buffers.
    /// @param primitiveType The type of primitive to draw (triangle, line, point, etc.).
    /// @param vertexStart   Index of the first vertex.
    /// @param vertexCount   Number of vertices to draw.
    /// @param instanceCount Number of instances to draw.
    void drawPrimitives(PrimitiveType primitiveType, uint32_t vertexStart, uint32_t vertexCount, uint32_t instanceCount);

    /// @brief Draws a range of non-instanced primitives using the currently bound vertex buffers.
    /// @param primitiveType The type of primitive to draw.
    /// @param vertexStart   Index of the first vertex.
    /// @param vertexCount   Number of vertices to draw.
    void drawPrimitives(PrimitiveType primitiveType, uint32_t vertexStart, uint32_t vertexCount);

    /// @brief Draws indexed primitives with instancing.
    /// @param primitiveType      The type of primitive to draw.
    /// @param indexCount         Number of indices to process.
    /// @param indexType          Data type of the index values (uint16 or uint32).
    /// @param indexBuffer        Buffer containing the index data.
    /// @param indexBufferOffset  Byte offset into the index buffer.
    /// @param instanceCount      Number of instances to draw.
    void drawIndexedPrimitives(PrimitiveType primitiveType, uint32_t indexCount, IndexType indexType, const Buffer& indexBuffer, uint32_t indexBufferOffset, uint32_t instanceCount);

    /// @brief Draws indexed primitives without instancing.
    /// @param primitiveType     The type of primitive to draw.
    /// @param indexCount        Number of indices to process.
    /// @param indexType         Data type of the index values.
    /// @param indexBuffer       Buffer containing the index data.
    /// @param indexBufferOffset Byte offset into the index buffer.
    void drawIndexedPrimitives(PrimitiveType primitiveType, uint32_t indexCount, IndexType indexType, const Buffer& indexBuffer, uint32_t indexBufferOffset);

    /// @brief Draws instanced primitives with a base instance offset.
    /// @param primitiveType  The type of primitive to draw.
    /// @param vertexStart    Index of the first vertex.
    /// @param vertexCount    Number of vertices per instance.
    /// @param instanceCount  Number of instances to draw.
    /// @param baseInstance   Offset added to the instance index in the vertex shader.
    void drawPrimitives(PrimitiveType primitiveType, uint32_t vertexStart, uint32_t vertexCount, uint32_t instanceCount, uint32_t baseInstance);

    /// @brief Draws indexed instanced primitives with base vertex and instance offsets.
    /// @param primitiveType     The type of primitive to draw.
    /// @param indexCount        Number of indices to process.
    /// @param indexType         Data type of the index values.
    /// @param indexBuffer       Buffer containing the index data.
    /// @param indexBufferOffset Byte offset into the index buffer.
    /// @param instanceCount     Number of instances to draw.
    /// @param baseVertex        Offset added to each index value.
    /// @param baseInstance      Offset added to the instance index.
    void drawIndexedPrimitives(PrimitiveType primitiveType, uint32_t indexCount, IndexType indexType, const Buffer& indexBuffer, uint32_t indexBufferOffset, uint32_t instanceCount, Integer baseVertex, UInteger baseInstance);

    /// @brief Issues an indirect draw command for non-indexed primitives.
    /// @param primitiveType          The type of primitive to draw.
    /// @param indirectBuffer         Buffer containing the draw arguments.
    /// @param indirectBufferOffset   Byte offset into the indirect buffer.
    void drawPrimitives(PrimitiveType primitiveType, const Buffer& indirectBuffer, UInteger indirectBufferOffset);

    /// @brief Issues an indirect draw command for indexed primitives.
    /// @param primitiveType          The type of primitive to draw.
    /// @param indexType              Data type of the index values.
    /// @param indexBuffer            Buffer containing the index data.
    /// @param indexBufferOffset      Byte offset into the index buffer.
    /// @param indirectBuffer         Buffer containing the draw arguments.
    /// @param indirectBufferOffset   Byte offset into the indirect buffer.
    void drawIndexedPrimitives(PrimitiveType primitiveType, IndexType indexType, const Buffer& indexBuffer, UInteger indexBufferOffset, const Buffer& indirectBuffer, UInteger indirectBufferOffset);

    /// @brief Inserts a memory barrier between texture reads and writes in the same pass.
    void textureBarrier();

//    void updateFence(const class Fence* fence, MTL::RenderStages stages);
//
//    void waitForFence(const class Fence* fence, MTL::RenderStages stages);

    /// @brief Binds a buffer containing per-patch tessellation factors.
    /// @param buffer          Buffer holding tessellation factor data.
    /// @param offset          Byte offset into the buffer.
    /// @param instanceStride  Stride in bytes between per-instance factor blocks.
    void setTessellationFactorBuffer(const Buffer& buffer, UInteger offset, UInteger instanceStride);

    /// @brief Sets the global scale applied to all tessellation factors.
    /// @param scale The scale factor.
    void setTessellationFactorScale(float scale);

//    void drawPatches(NS::UInteger numberOfPatchControlPoints, NS::UInteger patchStart, NS::UInteger patchCount, const class BatchIndexBuffer, NS::UInteger patchIndexBufferOffset, NS::UInteger instanceCount, NS::UInteger baseInstance);
//
//    void drawPatches(NS::UInteger numberOfPatchControlPoints, const class Buffer* patchIndexBuffer, NS::UInteger pxBufferOffset, const class Buffer* indirectBuffer, NS::UInteger indirectBufferOffset);
//
//    void drawIndexedPatches(NS::UInteger numberOfPatchControlPoints, NS::UInteger patchStart, NS::UInteger patchCount, const class BatchIndexBuffer, NS::UInteger patchIndexBufferOffset, const class Buffer* controlPointIndexBuffer, NS::UInteger cintIndexBufferOffset, NS::UInteger instanceCount, NS::UInteger baseInstance);
//
//    void drawIndexedPatches(NS::UInteger numberOfPatchControlPoints, const class Buffer* patchIndexBuffer, NS::UInteger pxBufferOffset, const class Buffer* controlPointIndexBuffer, NS::UInteger controlPointIndexBufferOffset, const class BndirectBuffer, NS::UInteger indirectBufferOffset);

    /// @brief Returns the width of each tile in pixels for tile shading passes.
    /// @return Tile width in pixels.
    size_t tileWidth() const;

    /// @brief Returns the height of each tile in pixels for tile shading passes.
    /// @return Tile height in pixels.
    size_t tileHeight() const;

    /// @brief Copies inline bytes into the tile shader's buffer argument table.
    /// @param bytes  Pointer to the data to copy.
    /// @param length Number of bytes to copy.
    /// @param index  Buffer argument table index in the tile shader.
    void setTileBytes(const void* bytes, UInteger length, UInteger index);

    /// @brief Binds a GPU buffer to the tile shader's buffer argument table.
    /// @param buffer Buffer to bind.
    /// @param offset Byte offset within the buffer.
    /// @param index  Buffer argument table index in the tile shader.
    void setTileBuffer(const Buffer& buffer, UInteger offset, UInteger index);

    /// @brief Updates the byte offset of a previously bound tile buffer without rebinding.
    /// @param offset New byte offset within the buffer.
    /// @param index  Buffer argument table index in the tile shader.
    void setTileBufferOffset(UInteger offset, UInteger index);

//    void setTileBuffers(MTL::Buffer* buffers, const NS::UInteger* offsets, NS::Range range);

//    void setTileTexture(const class Texture* texture, NS::UInteger index);
//
//    void setTileTextures(MTL::Texture* textures[], NS::Range range);
//
//    void setTileSamplerState(const class SamplerState* sampler, NS::UInteger index);
//
//    void setTileSamplerStates(MTL::SamplerState* samplers[], NS::Range range);
//
//    void setTileSamplerState(const class SamplerState* sampler, float lodMinClamp, float lodMaxClamp, NS::UInteger index);
//
//    void setTileSamplerStates(MTL::SamplerState* samplers[], const float lodMinClamps[], const float lodMaxClamps[], NS::Range r
//
//    void setTileVisibleFunctionTable(const class VisibleFunctionTable* functionTable, NS::UInteger bufferIndex);
//
//    void setTileVisibleFunctionTables(const class VisibleFunctionTable* functionTables[], NS::Range range);
//
//    void setTileIntersectionFunctionTable(const class IntersectionFunctionTable* intersectionFunctionTable, NS::UInteger bex);
//
//    void setTileIntersectionFunctionTables(const class IntersectionFunctionTable* intersectionFunctionTables[], NS::Range range);
//
//    void setTileAccelerationStructure(const class AccelerationStructure* accelerationStructure, NS::UInteger bufferIndex);

    /// @brief Dispatches tile shader threads with the specified threadgroup size.
    /// @param threadsPerTile Width, height, and depth of the threadgroup dispatched per tile.
    void dispatchThreadsPerTile(const Volume<td::UINT4>& threadsPerTile); //MTL::Size

    /// @brief Sets the threadgroup memory allocation for a tile shader argument.
    /// @param length Number of bytes to allocate.
    /// @param offset Byte offset within the threadgroup memory region.
    /// @param index  Threadgroup memory argument index in the tile shader.
    void setThreadgroupMemoryLength(UInteger length, UInteger offset, UInteger index);

//    void useResource(const class Resource* resource, MTL::ResourceUsage usage);

//    void useResources(MTL::Resource* resources[], NS::UInteger count, MTL::ResourceUsage usage);
//
//    void useResource(const class Resource* resource, MTL::ResourceUsage usage, MTL::RenderStages stages);
//
//    void useResources(MTL::Resource* resources, NS::UInteger count, MTL::ResourceUsage usage, MTL::RenderStages stages);
//
//    void useHeap(const class Heap* heap);
//
//    void useHeaps(MTL::Heap* heaps[], NS::UInteger count);
//
//    void useHeap(const class Heap* heap, MTL::RenderStages stages);
//
//    void useHeaps(MTL::Heap* heaps[], NS::UInteger count, MTL::RenderStages stages);
//
//    void executeCommandsInBuffer(const class IndirectCommandBuffer* indirectCommandBuffer, NS::Range executionRange);
//
//    void executeCommandsInBuffer(const class IndirectCommandBuffer* indirectCommandbuffer, const class Buffer* iangeBuffer, NS::UInteger indirectBufferOffset);
//
//    void memoryBarrier(MTL::BarrierScope scope, MTL::RenderStages after, MTL::RenderStages before);
//
//    void memoryBarrier(MTL::Resource* resources[], NS::UInteger count, MTL::RenderStages after, MTL::RenderStages before);
//
//    void sampleCountersInBuffer(const class CounterSampleBuffer* sampleBuffer, NS::UInteger sampleIndex, bool barrier);

    /// @brief Returns the device that created the command buffer this encoder belongs to.
    /// @return The owning Device.
    Device device() const;

    /// @brief Returns the debug label associated with this encoder.
    /// @return The label string.
    td::String label() const;

    /// @brief Sets a debug label on this encoder for GPU profiling tools.
    /// @param label Null-terminated label string.
    void setLabel(const char* label);

    /// @brief Provides read-only access to the render pass descriptor active for this encoder.
    /// @return Const reference to the current RenderPassDescriptor.
	const RenderPassDescriptor &getRenderPassDescriptor() const;

    /// @brief Finalizes encoding and marks the encoder as complete; no more commands may be added.
    void endEncoding();

    /// @brief Inserts a named marker at the current position in the GPU command stream.
    /// @param string Null-terminated marker name visible in GPU frame debuggers.
    void insertDebugSignpost(const char* string);

    /// @brief Begins a named debug group, nesting subsequent commands under the given label.
    /// @param string Null-terminated group name.
    void pushDebugGroup(const char* string);

    /// @brief Ends the most recently opened debug group.
    void popDebugGroup();

    /// @brief Releases the underlying native Metal render command encoder.
    void release();

    /// @brief Destructor; releases the underlying native resource.
    ~RenderCommandEncoder();

};


} //namespace glx
