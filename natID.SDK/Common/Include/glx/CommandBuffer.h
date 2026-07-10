// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CommandBuffer.h
    @brief Provides the CommandBuffer class for recording and submitting GPU commands. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <functional>
#include "RenderCommandEncoder.h"
#include "BlitCommandEncoder.h"
#include "ComputeCommandEncoder.h"
#include "RenderPassDescriptor.h"
#include "Drawable.h"

#ifdef MU_LINUX
#ifdef Status
#undef Status
#endif
#endif

namespace MTL
{
class CommandBuffer;
class CommandBufferDescriptor;
} //namespace MTL

namespace glx
{

class CommandQueue;
class Drawable;
class CommandBuffer;



/// @brief Records GPU commands and manages their submission to the command queue.
class NATGLX_API CommandBuffer
{
    friend class CommandQueue;
public:
    /// @brief Function type for scheduled and completed callback handlers.
    using HandlerFunction = std::function<void(CommandBuffer*)>;
    /// @brief Options controlling error behavior during command buffer execution.
    enum class ErrorOption : td::BYTE {
        None = 0,                  ///< No error options specified.
        EncoderExecutionStatus = 1 ///< Report per-encoder execution status on error.
    };
    /// @brief Describes the current lifecycle status of a CommandBuffer.
    enum class Status : td::BYTE {
        NotEnqueued = 0, ///< The command buffer has not been enqueued.
        Enqueued    = 1, ///< The command buffer has been enqueued.
        Committed   = 2, ///< The command buffer has been committed for execution.
        Scheduled   = 3, ///< The command buffer has been scheduled on the GPU.
        Completed   = 4, ///< The command buffer has completed execution.
        Error       = 5  ///< The command buffer encountered an error during execution.
    };
    /// @brief Describes the configuration options for creating a CommandBuffer.
    class Descriptor
    {
    private:
        MTL::CommandBufferDescriptor* _pCommandBufferDescriptor; ///< Pointer to the underlying Metal command buffer descriptor.
        /// @brief Constructs a Descriptor wrapping an existing Metal descriptor.
        /// @param pCommandBufferDescriptor Pointer to the native MTL::CommandBufferDescriptor.
        Descriptor(MTL::CommandBufferDescriptor* pCommandBufferDescriptor);
    public:
        // constructor with vectors
        /// @brief Default constructor. Creates a Descriptor with default settings.
        Descriptor();
        /// @brief Copy-assigns another Descriptor to this one.
        /// @param commandBuffDescriptor The source Descriptor to assign from.
        void operator = (const Descriptor& commandBuffDescriptor);
    public:
        /// @brief Allocates a new Descriptor instance.
        /// @return A newly allocated Descriptor.
        static Descriptor alloc();

        /// @brief Initializes this Descriptor.
        void init();

        /// @brief Checks whether the command buffer retains references to encoded objects.
        /// @return True if retained references are enabled.
        bool retainedReferences() const;
        /// @brief Sets whether the command buffer retains references to encoded objects.
        /// @param retainedReferences True to enable retained references.
        void setRetainedReferences(bool retainedReferences);

        /// @brief Returns the error options configured for the command buffer.
        /// @return The current ErrorOption value.
        ErrorOption errorOptions() const;
        /// @brief Sets the error options for the command buffer.
        /// @param errorOptions The ErrorOption value to apply.
        void setErrorOptions(ErrorOption errorOptions);
        /// @brief Returns a mutable pointer to the underlying Metal descriptor.
        /// @return Pointer to the native MTL::CommandBufferDescriptor.
        MTL::CommandBufferDescriptor* getNatPtr()
        {
            return _pCommandBufferDescriptor;
        }

        /// @brief Returns a const pointer to the underlying Metal descriptor.
        /// @return Const pointer to the native MTL::CommandBufferDescriptor.
        const MTL::CommandBufferDescriptor* getNatPtr() const
        {
            return _pCommandBufferDescriptor;
        }
    };

private:
    MTL::CommandBuffer* _pCommandBuffer; ///< Pointer to the underlying Metal command buffer.
    /// @brief Constructs a CommandBuffer wrapping an existing Metal command buffer.
    /// @param pCommandBuffer Pointer to the native MTL::CommandBuffer.
    CommandBuffer(MTL::CommandBuffer* pCommandBuffer);
public:
    // constructor with vectors
    /// @brief Default constructor. Creates an empty CommandBuffer.
    CommandBuffer();
    /// @brief Copy constructor. Shares the underlying Metal command buffer.
    /// @param commandBuffer The CommandBuffer to copy from.
    CommandBuffer(const CommandBuffer& commandBuffer);
    /// @brief Copy-assigns another CommandBuffer to this one.
    /// @param commandBuffer The CommandBuffer to assign from.
    /// @return Reference to this CommandBuffer.
    CommandBuffer& operator = (const CommandBuffer& commandBuffer);
    /// @brief Destructor. Releases the reference to the Metal command buffer.
    ~CommandBuffer();
public:
    /// @brief Registers a handler to be called when the command buffer is scheduled.
    /// @param function The callback function to invoke on scheduling.
    void  addScheduledHandler(const HandlerFunction& function);

    /// @brief Registers a handler to be called when the command buffer completes execution.
    /// @param function The callback function to invoke on completion.
    void  addCompletedHandler(const HandlerFunction& function);

    /// @brief Returns the device that created this command buffer.
    /// @return The associated Device.
    Device device() const;

    /// @brief Returns the command queue that owns this command buffer.
    /// @return The associated CommandQueue.
    CommandQueue commandQueue() const;

    /// @brief Checks whether the command buffer retains strong references to encoded resources.
    /// @return True if retained references are enabled.
    bool retainedReferences() const;

    /// @brief Returns the error options configured for this command buffer.
    /// @return The current ErrorOption value.
    ErrorOption errorOptions() const;

    /// @brief Returns the debug label of this command buffer.
    /// @return The label string.
    td::String  label() const;
    /// @brief Sets the debug label of this command buffer.
    /// @param label The label string to assign.
    void        setLabel(const td::String& label);

    /// @brief Returns the CPU-side timestamp when the kernel started executing this command buffer.
    /// @return The kernel start time interval.
    TimeInterval kernelStartTime() const;

    /// @brief Returns the CPU-side timestamp when the kernel finished executing this command buffer.
    /// @return The kernel end time interval.
    TimeInterval kernelEndTime() const;

//    class LogContainer*  logs() const;

    /// @brief Returns the GPU-side timestamp when execution of this command buffer began.
    /// @return The GPU start time interval.
    TimeInterval GPUStartTime() const;

    /// @brief Returns the GPU-side timestamp when execution of this command buffer ended.
    /// @return The GPU end time interval.
    TimeInterval GPUEndTime() const;

    /// @brief Enqueues this command buffer on the command queue for future execution.
    void enqueue();

    /// @brief Commits this command buffer for GPU execution.
    void commit();

//    void  addScheduledHandler(const MTL::CommandBufferHandler block);

    /// @brief Schedules the drawable to be presented after this command buffer completes.
    /// @param drawable The drawable to present.
    void presentDrawable(Drawable drawable);

    //void presentDrawable(const Drawable& drawable);

    /// @brief Schedules the drawable to be presented at the specified time.
    /// @param drawable The drawable to present.
    /// @param presentationTime The absolute time at which to present the drawable.
    void presentDrawableAtTime(const Drawable& drawable, TimeInterval presentationTime);

    /// @brief Schedules the drawable to be presented after a minimum display duration.
    /// @param drawable The drawable to present.
    /// @param duration The minimum duration (in seconds) to hold the previous frame before presenting.
    void presentDrawableAfterMinimumDuration(const Drawable& drawable, TimeInterval duration);

    /// @brief Blocks the calling thread until this command buffer is scheduled.
    void waitUntilScheduled();

//    void  addCompletedHandler(const MTL::CommandBufferHandler block);

    /// @brief Blocks the calling thread until this command buffer finishes execution.
    void waitUntilCompleted();
//
    /// @brief Returns the current execution status of this command buffer.
    /// @return The current Status value.
    Status status() const;

//    NS::Error*                                 error() const;

    /// @brief Creates and returns a blit command encoder for encoding copy operations.
    /// @return A new BlitCommandEncoder.
    BlitCommandEncoder blitCommandEncoder();

    /// @brief Creates and returns a render command encoder for the specified render pass.
    /// @param renderPassDescriptor The descriptor describing the render pass attachments.
    /// @return A new RenderCommandEncoder.
    RenderCommandEncoder renderCommandEncoder(const RenderPassDescriptor& renderPassDescriptor);

    /// @brief Creates and returns a compute command encoder for encoding compute operations.
    /// @return A new ComputeCommandEncoder.
    ComputeCommandEncoder computeCommandEncoder();
//
//    BlitCommandEncoder                  blitCommandEncoder(const class BlitPassDescriptor* blitPassDescriptor);

//    class ComputeCommandEncoder*               computeCommandEncoder();

//    class ComputeCommandEncoder*               computeCommandEncoder(MTL::DispatchType dispatchType);

//    void                                       encodeWait(const class Event* event, uint64_t value);

//    void                                       encodeSignalEvent(const class Event* event, uint64_t value);

//    class ParallelRenderCommandEncoder*        parallelRenderCommandEncoder(const class RenderPassDescriptor* renderPassDescriptor);

//    class ResourceStateCommandEncoder*         resourceStateCommandEncoder();

//    class ResourceStateCommandEncoder*         resourceStateCommandEncoder(const class ResourceStatePassDescriptor* resourceStatePassDescriptor);

//    class AccelerationStructureCommandEncoder* accelerationStructureCommandEncoder();

    /// @brief Pushes a named debug group onto the command buffer's debug group stack.
    /// @param string The name of the debug group.
    void pushDebugGroup(const td::String& string);

    /// @brief Pops the current debug group from the command buffer's debug group stack.
    void popDebugGroup();

    /// @brief Retains the underlying native resource, incrementing its reference count.
    void retain();

    /// @brief Releases the underlying native resource, decrementing its reference count.
    void release();

};


} //namespace glx
