// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CommandQueue.h
    @brief Provides the CommandQueue class for creating and managing GPU command buffers. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
//#include <glx/GLX.h>
#include <td/String.h>
#include "Device.h"
#include "CommandBuffer.h"

namespace MTL
{
class CommandQueue;
} //namespace MTL

namespace glx
{

/// @brief Manages an ordered queue for creating and submitting command buffers to the GPU.
class NATGLX_API CommandQueue
{
    friend class Device;
    friend class CommandBuffer;

private:
    MTL::CommandQueue* _pCommandQueue; ///< Pointer to the underlying Metal command queue.
    /// @brief Constructs a CommandQueue wrapping an existing Metal command queue.
    /// @param pCommandQueue Pointer to the native MTL::CommandQueue.
    CommandQueue(MTL::CommandQueue* pCommandQueue);
public:
    // constructor with vectors
    /// @brief Default constructor. Creates an empty CommandQueue.
    CommandQueue();
    /// @brief Copy constructor. Shares the underlying Metal command queue.
    /// @param commandQueue The CommandQueue to copy from.
    CommandQueue(const CommandQueue& commandQueue);
    /// @brief Copy-assigns another CommandQueue to this one.
    /// @param commandQueue The CommandQueue to assign from.
    void operator = (const CommandQueue& commandQueue);

    /// @brief Returns the debug label of this command queue.
    /// @return The label string.
    td::String label() const;
    /// @brief Sets the debug label of this command queue.
    /// @param label The label string to assign.
    void setLabel(const td::String& label);

    /// @brief Returns the device that created this command queue.
    /// @return The associated Device.
    Device  device() const;

    /// @brief Creates and returns a new command buffer from this queue.
    /// @return A new CommandBuffer ready for encoding commands.
    CommandBuffer commandBuffer();

    /// @brief Creates and returns a new command buffer using the given descriptor.
    /// @param descriptor The descriptor configuring the command buffer's options.
    /// @return A new CommandBuffer configured according to the descriptor.
    CommandBuffer commandBuffer(const CommandBuffer::Descriptor& descriptor);

    /// @brief Creates and returns a new command buffer without retaining references to encoded resources.
    /// @return A new CommandBuffer that does not retain resource references.
    CommandBuffer commandBufferWithUnretainedReferences();

    /// @brief Inserts a debug capture boundary in the command stream for GPU frame capture tools.
    void insertDebugCaptureBoundary();

    /// @brief Retains the underlying native resource, incrementing its reference count.
    void retain();
    /// @brief Releases the underlying native resource, decrementing its reference count.
    void release();

    /// @brief Destructor. Releases the native command queue resource.
    ~CommandQueue();
};


} //namespace glx
