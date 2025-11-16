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
#include <functional>
#include "RenderCommandEncoder.h"
#include "BlitCommandEncoder.h"
#include "RenderPassDescriptor.h"
#include "Drawable.h"

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



class NATGLX_API CommandBuffer
{
    friend class CommandQueue;
public:
    using HandlerFunction = std::function<void(CommandBuffer*)>;
    enum class ErrorOption : td::BYTE {None = 0, EncoderExecutionStatus = 1};
    enum class Status : td::BYTE {NotEnqueued = 0, Enqueued = 1, Committed = 2, Scheduled = 3, Completed = 4, Error = 5};
    class Descriptor
    {
    private:
        MTL::CommandBufferDescriptor* _pCommandBufferDescriptor;
        Descriptor(MTL::CommandBufferDescriptor* pCommandBufferDescriptor);
    public:
        // constructor with vectors
        Descriptor();
        void operator = (const Descriptor& commandBuffDescriptor);
    public:
        static Descriptor alloc();

        void init();

        bool retainedReferences() const;
        void setRetainedReferences(bool retainedReferences);

        ErrorOption errorOptions() const;
        void setErrorOptions(ErrorOption errorOptions);
        MTL::CommandBufferDescriptor* getNatPtr()
        {
            return _pCommandBufferDescriptor;
        }
        
        const MTL::CommandBufferDescriptor* getNatPtr() const
        {
            return _pCommandBufferDescriptor;
        }
    };
    
private:
    MTL::CommandBuffer* _pCommandBuffer;
    CommandBuffer(MTL::CommandBuffer* pCommandBuffer);
public:
    // constructor with vectors
    CommandBuffer();
    void operator = (const CommandBuffer& commandBuffer);
public:
    void  addScheduledHandler(const HandlerFunction& function);

    void  addCompletedHandler(const HandlerFunction& function);

    Device device() const;

    CommandQueue commandQueue() const;

    bool retainedReferences() const;

    ErrorOption errorOptions() const;

    td::String  label() const;
    void        setLabel(const td::String& label);

    TimeInterval kernelStartTime() const;

    TimeInterval kernelEndTime() const;

//    class LogContainer*  logs() const;

    TimeInterval GPUStartTime() const;

    TimeInterval GPUEndTime() const;

    void enqueue();

    void commit();

//    void  addScheduledHandler(const MTL::CommandBufferHandler block);

    void presentDrawable(const Drawable& drawable);

    void presentDrawableAtTime(const Drawable& drawable, TimeInterval presentationTime);

    void presentDrawableAfterMinimumDuration(const Drawable& drawable, TimeInterval duration);

    void waitUntilScheduled();

//    void  addCompletedHandler(const MTL::CommandBufferHandler block);

    void waitUntilCompleted();
//
    Status status() const;

//    NS::Error*                                 error() const;

    BlitCommandEncoder blitCommandEncoder();

    RenderCommandEncoder renderCommandEncoder(const RenderPassDescriptor& renderPassDescriptor);

//    class ComputeCommandEncoder*               computeCommandEncoder(const class ComputePassDescriptor* computePassDescriptor);

//    BlitCommandEncoder                  blitCommandEncoder(const class BlitPassDescriptor* blitPassDescriptor);

//    class ComputeCommandEncoder*               computeCommandEncoder();

//    class ComputeCommandEncoder*               computeCommandEncoder(MTL::DispatchType dispatchType);

//    void                                       encodeWait(const class Event* event, uint64_t value);

//    void                                       encodeSignalEvent(const class Event* event, uint64_t value);

//    class ParallelRenderCommandEncoder*        parallelRenderCommandEncoder(const class RenderPassDescriptor* renderPassDescriptor);

//    class ResourceStateCommandEncoder*         resourceStateCommandEncoder();

//    class ResourceStateCommandEncoder*         resourceStateCommandEncoder(const class ResourceStatePassDescriptor* resourceStatePassDescriptor);

//    class AccelerationStructureCommandEncoder* accelerationStructureCommandEncoder();

    void pushDebugGroup(const td::String& string);

    void popDebugGroup();
    
};


} //namespace glx

