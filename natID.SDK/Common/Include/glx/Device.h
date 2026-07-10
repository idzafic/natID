
// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Device.h
 * @brief Abstraction of a GPU device used to create rendering resources and command queues.
 */
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
#include <glx/ComputePipelineState.h>


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

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
/// @brief Represents a GPU device capable of creating buffers, textures, pipelines, and command queues.
class NATGLX_API Device
{
    friend class Function;
    friend class Library;
    friend class RenderPipeline;
    friend class ComputePipelineState;
    friend class CommandQueue;
    friend class CommandBuffer;
    friend class View;

public:
    /// @brief Enumerates the physical location of the GPU device.
    enum class Location : td::BYTE { BuiltIn = 0, nSlot = 1, External = 2, Unspecified = 255};
private:
    MTL::Device* _pDevice; ///< Pointer to the underlying Metal device.

public:

    /// @brief Default constructor; creates a device wrapping the system default GPU.
    Device();

    /// @brief Copy constructor.
    /// @param dev The device to copy from.
    Device(const Device& dev);

    /// @brief Constructs a device from a native Metal device pointer.
    /// @param pDevice Pointer to the MTL::Device to wrap.
    Device(MTL::Device* pDevice);

    /// @brief Assignment operator.
    /// @param device The device to assign from.
    void operator = (Device& device);

    /// @brief Destructor; releases the device reference.
    ~Device();

    /// @brief Creates and returns the default system GPU device.
    /// @return A Device wrapping the default Metal device.
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

    /// @brief Returns whether this device is headless (no display attached).
    /// @return True if the device is headless.
    bool isHeadless() const;

    /// @brief Returns the name of this GPU device.
    /// @return C-string name of the device.
    const char* name() const;

    /// @brief Returns the unique registry identifier for this device.
    /// @return The registry ID as a 64-bit unsigned integer.
    uint64_t   registryID() const;

    /// @brief Fills a volume with the maximum threads per threadgroup supported by this device.
    /// @param volume Output volume to populate.
    void maxThreadsPerThreadgroup(Volume<td::UINT4>& volume) const;

    /// @brief Returns whether this device is a low-power GPU.
    /// @return True if the device is low-power.
    bool       lowPower() const;

    /// @brief Returns whether this device is headless.
    /// @return True if the device has no display output.
    bool       headless() const;

    /// @brief Returns whether this device can be removed (e.g. eGPU).
    /// @return True if the device is removable.
    bool       removable() const;

    /// @brief Returns whether this device uses unified memory architecture.
    /// @return True if CPU and GPU share the same memory pool.
    bool       hasUnifiedMemory() const;

    /// @brief Returns the recommended maximum working set size in bytes.
    /// @return The recommended working set size.
    uint64_t   recommendedMaxWorkingSetSize() const;

    /// @brief Returns the physical location category of this device.
    /// @return The Location enum value.
    Location   location() const;

    /// @brief Returns the slot number for external or slotted GPUs.
    /// @return The location number as a byte.
    td::BYTE   locationNumber() const;

    /// @brief Returns the maximum memory transfer rate in bytes per second.
    /// @return The max transfer rate.
    uint64_t   maxTransferRate() const;

    /// @brief Returns whether the device supports a combined 24-bit depth / 8-bit stencil pixel format.
    /// @return True if the format is supported.
    bool       depth24Stencil8PixelFormatSupported() const;

//    MTL::ReadWriteTextureTier       readWriteTextureSupport() const;
//
//    MTL::ArgumentBuffersTier        argumentBuffersSupport() const;

    /// @brief Returns whether raster order groups are supported.
    /// @return True if raster order groups are supported.
    bool  rasterOrderGroupsSupported() const;

    /// @brief Returns whether 32-bit float texture filtering is supported.
    /// @return True if 32-bit float filtering is supported.
    bool  supports32BitFloatFiltering() const;

    /// @brief Returns whether 32-bit MSAA textures are supported.
    /// @return True if 32-bit MSAA is supported.
    bool  supports32BitMSAA() const;

    /// @brief Returns whether anisotropic filtering is supported.
    /// @return True if anisotropic filtering is supported.
    bool  supportsAnisotropicFiltering() const;

    /// @brief Returns whether query texture LOD is supported.
    /// @return True if texture LOD queries are supported.
    bool  supportsQueryTextureLOD() const;

    /// @brief Returns whether BC texture compression is supported.
    /// @return True if BC compression is supported.
    bool  supportsBCTextureCompression() const;

    /// @brief Returns whether pull-model interpolation is supported.
    /// @return True if pull-model interpolation is supported.
    bool  supportsPullModelInterpolation() const;

    /// @brief Returns whether barycentric coordinates are supported.
    /// @return True if barycentric coordinates are supported.
    bool  barycentricCoordsSupported() const;

    /// @brief Returns whether shader barycentric coordinates are supported.
    /// @return True if shader barycentric coordinates are supported.
    bool  supportsShaderBarycentricCoordinates() const;

    /// @brief Returns the total size of memory currently allocated by this device.
    /// @return Currently allocated size in bytes.
    size_t currentAllocatedSize() const;

    /// @brief Creates and returns a new command queue with default capacity.
    /// @return A new CommandQueue.
    CommandQueue newCommandQueue();

    /// @brief Creates and returns a new command queue with the specified maximum command buffer count.
    /// @param maxCommandBufferCount Maximum number of command buffers the queue can hold.
    /// @return A new CommandQueue.
    CommandQueue newCommandQueue(size_t maxCommandBufferCount);

//    MTL::SizeAndAlign               heapTextureSizeAndAlign(const class TextureDescriptor* desc);
//
//    MTL::SizeAndAlign               heapBufferSizeAndAlign(NS::UInteger length, MTL::ResourceOptions options);
//
//    class Heap*                     newHeap(const class HeapDescriptor* descriptor);

    /// @brief Creates a new buffer of the given length with the specified resource options.
    /// @param length Size of the buffer in bytes.
    /// @param options Resource storage and cache options.
    /// @return A new Buffer.
    Buffer newBuffer(size_t length, Resource::Options options);

    /// @brief Creates a new buffer initialized with data from the given pointer.
    /// @param pointer Pointer to the source data.
    /// @param length Size of the data in bytes.
    /// @param options Resource storage and cache options.
    /// @return A new Buffer containing a copy of the data.
    Buffer newBuffer(const void* pointer, size_t length, Resource::Options options);

//    class Buffer*                   newBuffer(const void* pointer, NS::UInteger length, MTL::ResourceOptions options, const void (^deallocator)(void*, NS::UInteger));

    /// @brief Creates a depth/stencil state from the given descriptor.
    /// @param descriptor Pointer to a configured DepthStencilDescriptor.
    /// @return A compiled DepthStencilState.
    DepthStencilState newDepthStencilState(const DepthStencilDescriptor* descriptor);
//
    /// @brief Creates a new texture from the given descriptor.
    /// @param descriptor Pointer to a TextureDescriptor specifying the texture properties.
    /// @return A new Texture.
    Texture newTexture(const TextureDescriptor* descriptor);
//
//    class Texture*                  newTexture(const class TextureDescriptor* descriptor, const IOSurfaceRef iosurface, NS::UInteger plane);
//
//    class Texture*                  newSharedTexture(const class TextureDescriptor* descriptor);
//
//    class Texture*                  newSharedTexture(const class SharedTextureHandle* sharedHandle);
//
//

    /// @brief Creates a sampler state from the given descriptor.
    /// @param descriptor Pointer to a configured SamplerDescriptor.
    /// @return A compiled SamplerState.
    SamplerState newSamplerState(const SamplerDescriptor* descriptor);

    /// @brief Creates the default shader library compiled with the application.
    /// @return The default Library.
    Library newDefaultLibrary();
//
//    class Library*                  newDefaultLibrary(const NS::Bundle* bundle, NS::Error** error);
//
    /// @brief Creates a shader library from a compiled metallib file.
    /// @param filepath Path to the .metallib file.
    /// @param error Output error object populated on failure.
    /// @return The loaded Library.
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
    /// @brief Creates a render pipeline state from a descriptor.
    /// @param descriptor The render pipeline descriptor.
    /// @param error Output error object populated on failure.
    /// @return A compiled RenderPipeline.
    RenderPipeline newRenderPipelineState(const RenderPipeline::Descriptor& descriptor, Error& error);
//
//    class RenderPipelineState*      newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::AutoreleasedRenderPipelineReflection* reflection, NS::Error** error);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, const MTL::NewRenderPipelineStateCompletionHandler completionHandler);
//
//    void                            newRenderPipelineState(const class RenderPipelineDescriptor* descriptor, MTL::PipelineOption options, const MTL::NewRenderPipelineStateWithReflectionCompletionHandler completionHandler);
//
    /// @brief Creates a compute pipeline state for the given kernel function.
    /// @param function The kernel Function to compile into a pipeline.
    /// @param error Output error object populated on failure.
    /// @return A compiled ComputePipelineState.
    ComputePipelineState newComputePipelineState(const Function& function, Error& error);
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

    /// @brief Returns the maximum total threadgroup memory length in bytes.
    /// @return Maximum threadgroup memory length.
    size_t maxThreadgroupMemoryLength() const;

    /// @brief Returns the maximum number of samplers in an argument buffer.
    /// @return Maximum argument buffer sampler count.
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

    /// @brief Returns the peer group ID for multi-GPU configurations.
    /// @return The peer group ID.
    uint64_t peerGroupID() const;

    /// @brief Returns the index of this device within its peer group.
    /// @return The peer index.
    uint32_t peerIndex() const;

    /// @brief Returns the total number of devices in the peer group.
    /// @return The peer count.
    uint32_t peerCount() const;

//    MTL::Size                       sparseTileSize(MTL::TextureType textureType, MTL::PixelFormat pixelFormat, NS::UInteger sampleCount);
//
//    NS::UInteger                    sparseTileSizeInBytes() const;
//
//    void                            convertSparsePixelRegions(const MTL::Region* pixelRegions, MTL::Region* tileRegions, MTL::Size tileSize, MTL::SparseTextureRegionAlignmentMode mode, NS::UInteger numRegions);
//
//    void                            convertSparseTileRegions(const MTL::Region* tileRegions, MTL::Region* pixelRegions, MTL::Size tileSize, NS::UInteger numRegions);

    /// @brief Returns the maximum buffer length in bytes that this device supports.
    /// @return Maximum buffer length.
    size_t maxBufferLength() const;

//    void   counterSets(cnt::SafeFullVector&) const;

//    class CounterSampleBuffer*      newCounterSampleBuffer(const class CounterSampleBufferDescriptor* descriptor, NS::Error** error);

//    void                            sampleTimestamps(MTL::Timestamp* cpuTimestamp, MTL::Timestamp* gpuTimestamp);

//    bool                            supportsCounterSampling(MTL::CounterSamplingPoint samplingPoint);

    /// @brief Returns whether the device supports vertex amplification with the specified count.
    /// @param count The amplification count to query.
    /// @return True if the count is supported.
    bool supportsVertexAmplificationCount(size_t count);

    /// @brief Returns whether the device supports dynamic libraries.
    /// @return True if dynamic libraries are supported.
    bool supportsDynamicLibraries() const;

    /// @brief Returns whether the device supports render dynamic libraries.
    /// @return True if render dynamic libraries are supported.
    bool supportsRenderDynamicLibraries() const;

//    class DynamicLibrary*           newDynamicLibrary(const class Library* library, NS::Error** error);
//
//    class DynamicLibrary*           newDynamicLibrary(const NS::URL* url, NS::Error** error);
//
//    class BinaryArchive*            newBinaryArchive(const class BinaryArchiveDescriptor* descriptor, NS::Error** error);

    /// @brief Returns whether the device supports hardware ray tracing.
    /// @return True if ray tracing is supported.
    bool supportsRaytracing() const;

//    MTL::AccelerationStructureSizes accelerationStructureSizes(const class AccelerationStructureDescriptor* descriptor);
//
//    class AccelerationStructure*    newAccelerationStructure(NS::UInteger size);
//
//    class AccelerationStructure*    newAccelerationStructure(const class AccelerationStructureDescriptor* descriptor);

    /// @brief Returns whether function pointers are supported on this device.
    /// @return True if function pointers are supported.
    bool supportsFunctionPointers() const;

    /// @brief Returns whether function pointers from render shaders are supported.
    /// @return True if render-stage function pointers are supported.
    bool supportsFunctionPointersFromRender() const;

    /// @brief Returns whether ray tracing from render shaders is supported.
    /// @return True if render-stage ray tracing is supported.
    bool supportsRaytracingFromRender() const;

    /// @brief Returns whether primitive motion blur is supported.
    /// @return True if primitive motion blur is supported.
    bool supportsPrimitiveMotionBlur() const;


    /// @brief Increments the retain count of the underlying device object.
    void retain();

    /// @brief Releases the underlying device object.
    void release();
public:// not ideal but necessary for nat:: functions
    /// @brief Returns the mutable native Metal device pointer.
    /// @return Pointer to MTL::Device.
    MTL::Device* getNatPtr();

    /// @brief Returns the const native Metal device pointer.
    /// @return Const pointer to MTL::Device.
    const MTL::Device* getNatPtr() const;
};


} //namespace glx
