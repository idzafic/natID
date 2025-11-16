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
//#include <glx/GLX.h>
#include <td/String.h>
#include "Resource.h"
#include <cnt/SafeFullVector.h>
#include <glx/Types.h>
#include <glx/Error.h>
#include <glx/Library.h>
#include <glx/RenderPipeline.h>


namespace MTL
{
class Device;
class MetalLayer;
} //namespace MTL

namespace glx
{

class CommandQueue;
class Buffer;
class Function;
class CommandBuffer;
class TextureDescriptor;
class Texture;
class SamplerDescriptor;
class SamplerState;
class DepthStencilDescriptor;
class DepthStencilState;
//class Library;
//class Library::CompileOptions;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class NATGLX_API Device
{
    friend class Function;
    friend class Library;
    friend class RenderPipeline;
    friend class CommandQueue;
    friend class CommandBuffer;
    friend class View;

public:
    enum class Location : td::BYTE { BuiltIn = 0, nSlot = 1, External = 2, Unspecified = 255};
private:
    MTL::Device* _pDevice;

public:

    

    Device()
        : _pDevice(nullptr)
    { 
    }
    Device(const Device& dev)
    {
        if (this != &dev)
            _pDevice = dev._pDevice;
    }

    Device(MTL::Device* pDevice);
    void operator = (const Device& device);
    
    static Device createDefaultDevice();
    //void  newLibrary(const td::String* pSource, const class CompileOptions* pOptions, const NewLibraryCompletionHandlerFunction& completionHandler);

//    void                            newLibrary(const class StitchedLibraryDescriptor* pDescriptor, const MTL::NewLibraryCompletionHandlerFunction& completionHandler);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* pDescriptor, const NewRenderPipelineStateCompletionHandlerFunction& completionHandler);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* pDescriptor, PipelineOption options, const NewRenderPipelineStateWithReflectionCompletionHandlerFunction& completionHandler);
//
//    void                            newRenderPipelineState(const class TileRenderPipelineDescriptor* pDescriptor, PipelineOption options, const NewRenderPipelineStateWithReflectionCompletionHandlerFunction& completionHandler);
//
//    void                            newComputePipelineState(const class Function* pFunction, const NewComputePipelineStateCompletionHandlerFunction& completionHandler);
//
//    void                            newComputePipelineState(const class Function* pFunction, PipelineOption options, const NewComputePipelineStateWithReflectionCompletionHandlerFunction& completionHandler);
//
//    void                            newComputePipelineState(const class ComputePipelineDescriptor* pDescriptor, PipelineOption options, const NewComputePipelineStateWithReflectionCompletionHandlerFunction& completionHandler);

    bool isHeadless() const;

    const char* name() const;

    uint64_t   registryID() const;

    void maxThreadsPerThreadgroup(Volume<td::UINT4>& volume) const;

    bool       lowPower() const;

    bool       headless() const;

    bool       removable() const;

    bool       hasUnifiedMemory() const;

    uint64_t   recommendedMaxWorkingSetSize() const;

    Location   location() const;

    td::BYTE   locationNumber() const;

    uint64_t   maxTransferRate() const;

    bool       depth24Stencil8PixelFormatSupported() const;

//    MTL::ReadWriteTextureTier       readWriteTextureSupport() const;
//
//    MTL::ArgumentBuffersTier        argumentBuffersSupport() const;

    bool  rasterOrderGroupsSupported() const;

    bool  supports32BitFloatFiltering() const;

    bool  supports32BitMSAA() const;
    
    bool  supportsAnisotropicFiltering() const;

    bool  supportsQueryTextureLOD() const;

    bool  supportsBCTextureCompression() const;

    bool  supportsPullModelInterpolation() const;

    bool  barycentricCoordsSupported() const;

    bool  supportsShaderBarycentricCoordinates() const;

    size_t currentAllocatedSize() const;

    CommandQueue newCommandQueue();

    CommandQueue newCommandQueue(size_t maxCommandBufferCount);

//    MTL::SizeAndAlign               heapTextureSizeAndAlign(const class TextureDescriptor* desc);
//
//    MTL::SizeAndAlign               heapBufferSizeAndAlign(NS::UInteger length, MTL::ResourceOptions options);
//
//    class Heap*                     newHeap(const class HeapDescriptor* descriptor);

    Buffer newBuffer(size_t length, Resource::Options options);

    Buffer newBuffer(const void* pointer, size_t length, Resource::Options options);

//    class Buffer*                   newBuffer(const void* pointer, NS::UInteger length, MTL::ResourceOptions options, const void (^deallocator)(void*, NS::UInteger));

    DepthStencilState newDepthStencilState(const DepthStencilDescriptor* descriptor);
//
    Texture newTexture(const TextureDescriptor* descriptor);
//
//    class Texture*                  newTexture(const class TextureDescriptor* descriptor, const IOSurfaceRef iosurface, NS::UInteger plane);
//
//    class Texture*                  newSharedTexture(const class TextureDescriptor* descriptor);
//
//    class Texture*                  newSharedTexture(const class SharedTextureHandle* sharedHandle);
//

//
    SamplerState newSamplerState(const SamplerDescriptor* descriptor);

    Library newDefaultLibrary();
//
//    class Library*                  newDefaultLibrary(const NS::Bundle* bundle, NS::Error** error);
//
    Library newLibrary(const td::String &filepath, Error &error);
//
//    class Library*                  newLibrary(const NS::URL* url, NS::Error** error);
//
//    class Library*                  newLibrary(const dispatch_data_t data, NS::Error** error);
//
    //Library newLibrary(const td::String& source, const Library::CompileOptions* pOptions, Error& error); not supported

//    void                            newLibrary(const NS::String* source, const Library::CompileOptions options, const MTL::NewLibraryCompletionHandler completionHandler);
//
//    class Library*                  newLibrary(const class StitchedLibraryDescriptor* descriptor, NS::Error** error);
//
//    void                            newLibrary(const class StitchedLibraryDescriptor* descriptor, const MTL::NewLibraryCompletionHandler completionHandler);
//
    RenderPipeline newRenderPipelineState(const RenderPipeline::Descriptor& descriptor, Error& error);
//
//    class RenderPipelineState*      newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::AutoreleasedRenderPipelineReflection* reflection, NS::Error** error);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, const MTL::NewRenderPipelineStateCompletionHandler completionHandler);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::NewRenderPipelineStateWithReflectionCompletionHandler completionHandler);
//
//    class ComputePipelineState*     newComputePipelineState(const class Function* computeFunction, NS::Error** error);
//
//    class ComputePipelineState*     newComputePipelineState(const class Function* computeFunction, MTL::PipelineOption options, const MTL::AutoreleasedComputePipelineReflection* reflection, NS::Error** error);
//
//    void                            newComputePipelineState(const class Function* computeFunction, const MTL::NewComputePipelineStateCompletionHandler completionHandler);
//
//    void                            newComputePipelineState(const class Function* computeFunction, MTL::PipelineOption options, const MTL::NewComputePipelineStateWithReflectionCompletionHandler completionHandler);
//
//    class ComputePipelineState*     newComputePipelineState(const class ComputePipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::AutoreleasedComputePipelineReflection* reflection, NS::Error** error);
//
//    void                            newComputePipelineState(const class ComputePipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::NewComputePipelineStateWithReflectionCompletionHandler completionHandler);

//    class Fence*                    newFence();

//    bool supportsFeatureSet(MTL::FeatureSet featureSet);
//
//    bool supportsFamily(MTL::GPUFamily gpuFamily);
//
//    bool supportsTextureSampleCount(NS::UInteger sampleCount);
//
//    size_t minimumLinearTextureAlignmentForPixelFormat(MTL::PixelFormat format);
//
//    size_t minimumTextureBufferAlignmentForPixelFormat(MTL::PixelFormat format);
//
//    class RenderPipelineState*      newRenderPipelineState(const class TileRenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::AutoreleasedRenderPipelineReflection* reflection, NS::Error** error);
//
//    void                            newRenderPipelineState(const class TileRenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::NewRenderPipelineStateWithReflectionCompletionHandler completionHandler);

    size_t maxThreadgroupMemoryLength() const;

    size_t maxArgumentBufferSamplerCount() const;

//    bool                            programmableSamplePositionsSupported() const;

//    void                            getDefaultSamplePositions(MTL::SamplePosition* positions, NS::UInteger count);

//    class ArgumentEncoder*          newArgumentEncoder(const NS::Array* arguments);

//    bool                            supportsRasterizationRateMap(NS::UInteger layerCount);

//    class RasterizationRateMap*     newRasterizationRateMap(const class RasterizationRateMapDescriptor* descriptor);
//
//    class IndirectCommandBuffer*    newIndirectCommandBuffer(const class IndirectCommandBufferDescriptor* descriptor, NS::UInteger maxCount, MTL::ResourceOptions options);
//
//    class Event*                    newEvent();
//
//    class SharedEvent*              newSharedEvent();
//
//    class SharedEvent*              newSharedEvent(const class SharedEventHandle* sharedEventHandle);

    uint64_t peerGroupID() const;

    uint32_t peerIndex() const;

    uint32_t peerCount() const;

//    MTL::Size                       sparseTileSize(MTL::TextureType textureType, MTL::PixelFormat pixelFormat, NS::UInteger sampleCount);
//
//    NS::UInteger                    sparseTileSizeInBytes() const;
//
//    void                            convertSparsePixelRegions(const MTL::Region* pixelRegions, MTL::Region* tileRegions, MTL::Size tileSize, MTL::SparseTextureRegionAlignmentMode mode, NS::UInteger numRegions);
//
//    void                            convertSparseTileRegions(const MTL::Region* tileRegions, MTL::Region* pixelRegions, MTL::Size tileSize, NS::UInteger numRegions);

    size_t maxBufferLength() const;

//    void   counterSets(cnt::SafeFullVector&) const;

//    class CounterSampleBuffer*      newCounterSampleBuffer(const class CounterSampleBufferDescriptor* descriptor, NS::Error** error);

//    void                            sampleTimestamps(MTL::Timestamp* cpuTimestamp, MTL::Timestamp* gpuTimestamp);

//    bool                            supportsCounterSampling(MTL::CounterSamplingPoint samplingPoint);

    bool supportsVertexAmplificationCount(size_t count);

    bool supportsDynamicLibraries() const;

    bool supportsRenderDynamicLibraries() const;

//    class DynamicLibrary*           newDynamicLibrary(const class Library* library, NS::Error** error);
//
//    class DynamicLibrary*           newDynamicLibrary(const NS::URL* url, NS::Error** error);
//
//    class BinaryArchive*            newBinaryArchive(const class BinaryArchiveDescriptor* descriptor, NS::Error** error);

    bool supportsRaytracing() const;

//    MTL::AccelerationStructureSizes accelerationStructureSizes(const class AccelerationStructureDescriptor* descriptor);
//
//    class AccelerationStructure*    newAccelerationStructure(NS::UInteger size);
//
//    class AccelerationStructure*    newAccelerationStructure(const class AccelerationStructureDescriptor* descriptor);

    bool supportsFunctionPointers() const;

    bool supportsFunctionPointersFromRender() const;

    bool supportsRaytracingFromRender() const;

    bool supportsPrimitiveMotionBlur() const;
    
    
    MTL::Device* retain();
    
    void release();
    
    MTL::Device* getNatPtr()
    {
        return _pDevice;
    }
    
    const MTL::Device* getNatPtr() const
    {
        return _pDevice;
    }
};


} //namespace glx
