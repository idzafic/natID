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
#include "RenderPipeline.h"
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

class NATGLX_API RenderCommandEncoder
{
    friend class CommandBuffer;
private:
    MTL::RenderCommandEncoder* _pRenderCommandEncoder;
    RenderCommandEncoder(MTL::RenderCommandEncoder* pRenderCommandEncoder);
public:
    // constructor with vectors
    RenderCommandEncoder();
    void operator = (const RenderCommandEncoder& renderCommandEncoder);
public:
    
    void setRenderPipelineState(const RenderPipeline& pipelineState);

    void setVertexBytes(const void* bytes, size_t length, size_t index);

    void setVertexBuffer(const Buffer& buffer, size_t offset, size_t index);

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

    void setViewport(Viewport viewport);

//    void setViewports(const MTL::Viewport* viewports, NS::UInteger count);

    void setFrontFacingWinding(Winding frontFacingWinding);

//    void setVertexAmplificationCount(NS::UInteger count, const MTL::VertexAmplificationViewMapping* viewMappings);

    void setCullMode(CullMode cullMode);

    void setDepthClipMode(DepthClipMode depthClipMode);

    void setDepthBias(float depthBias, float slopeScale, float clamp);

    void setScissorRect(const ScissorRect& rect);

//    void setScissorRects(const MTL::ScissorRect* scissorRects, NS::UInteger count);

    void setTriangleFillMode(TriangleFillMode fillMode);

    void setFragmentBytes(const void* bytes, size_t length, size_t index);

    void setFragmentBuffer(const Buffer& buffer, size_t offset, size_t index);

    void setFragmentBufferOffset(size_t offset, size_t index);

//    void setFragmentBuffers(MTL::Buffer* buffers[], const NS::UInteger offsets[], NS::Range range);

      void setFragmentTexture(const Texture texture, uint32_t index);

//    void setFragmentTextures(MTL::Texture* textures[], NS::Range range);
//
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

    void setBlendColorRed(float red, float green, float blue, float alpha);

    void setDepthStencilState(const DepthStencilState depthStencilState);

    void setStencilReferenceValue(uint32_t referenceValue);

    void setStencilFrontReferenceValue(uint32_t frontReferenceValue, uint32_t backReferenceValue);

    void drawPrimitives(PrimitiveType primitiveType, size_t vertexStart, size_t vertexCount, size_t instanceCount);

    void drawPrimitives(PrimitiveType primitiveType, size_t vertexStart, size_t vertexCount);

    void drawIndexedPrimitives(PrimitiveType primitiveType, size_t indexCount, IndexType indexType, const Buffer& indexBuffer, size_t indexBufferOffset, size_t instanceCount);

    void drawIndexedPrimitives(PrimitiveType primitiveType, size_t indexCount, IndexType indexType, const Buffer& indexBuffer, size_t indexBufferOffset);

    void drawPrimitives(PrimitiveType primitiveType, size_t vertexStart, size_t vertexCount, size_t instanceCount, size_t baseInstance);

    void drawIndexedPrimitives(PrimitiveType primitiveType, size_t indexCount, IndexType indexType, const Buffer& indexBuffer, size_t indexBufferOffset, size_t instanceCount, Integer baseVertex, UInteger baseInstance);

    void drawPrimitives(PrimitiveType primitiveType, const Buffer& indirectBuffer, UInteger indirectBufferOffset);

    void drawIndexedPrimitives(PrimitiveType primitiveType, IndexType indexType, const Buffer& indexBuffer, UInteger indexBufferOffset, const Buffer& indirectBuffer, UInteger indirectBufferOffset);

    void textureBarrier();

//    void updateFence(const class Fence* fence, MTL::RenderStages stages);
//
//    void waitForFence(const class Fence* fence, MTL::RenderStages stages);

    void setTessellationFactorBuffer(const Buffer& buffer, UInteger offset, UInteger instanceStride);

    void setTessellationFactorScale(float scale);

//    void drawPatches(NS::UInteger numberOfPatchControlPoints, NS::UInteger patchStart, NS::UInteger patchCount, const class BatchIndexBuffer, NS::UInteger patchIndexBufferOffset, NS::UInteger instanceCount, NS::UInteger baseInstance);
//
//    void drawPatches(NS::UInteger numberOfPatchControlPoints, const class Buffer* patchIndexBuffer, NS::UInteger pxBufferOffset, const class Buffer* indirectBuffer, NS::UInteger indirectBufferOffset);
//
//    void drawIndexedPatches(NS::UInteger numberOfPatchControlPoints, NS::UInteger patchStart, NS::UInteger patchCount, const class BatchIndexBuffer, NS::UInteger patchIndexBufferOffset, const class Buffer* controlPointIndexBuffer, NS::UInteger cintIndexBufferOffset, NS::UInteger instanceCount, NS::UInteger baseInstance);
//
//    void drawIndexedPatches(NS::UInteger numberOfPatchControlPoints, const class Buffer* patchIndexBuffer, NS::UInteger pxBufferOffset, const class Buffer* controlPointIndexBuffer, NS::UInteger controlPointIndexBufferOffset, const class BndirectBuffer, NS::UInteger indirectBufferOffset);

    size_t tileWidth() const;

    size_t tileHeight() const;

    void setTileBytes(const void* bytes, UInteger length, UInteger index);

    void setTileBuffer(const Buffer& buffer, UInteger offset, UInteger index);

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

    void dispatchThreadsPerTile(const Volume<td::UINT4>& threadsPerTile); //MTL::Size

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
    
    Device device() const;

    td::String label() const;
    void setLabel(const char* label);

    void endEncoding();

    void insertDebugSignpost(const char* string);

    void pushDebugGroup(const char* string);

    void popDebugGroup();
    
    void release();
};


} //namespace glx

