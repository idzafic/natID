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
#include "Device.h"
#include "CommandBuffer.h"

namespace MTL
{
class CommandQueue;
} //namespace MTL

namespace glx
{

class NATGLX_API CommandQueue
{
    friend class Device;
    friend class CommandBuffer;
    
private:
    MTL::CommandQueue* _pCommandQueue;
    CommandQueue(MTL::CommandQueue* pCommandQueue);
public:
    // constructor with vectors
    CommandQueue();
    void operator = (const CommandQueue& commandQueue);
    
    td::String label() const;
    void setLabel(const td::String& label);

    Device  device() const;

    CommandBuffer commandBuffer();

    CommandBuffer commandBuffer(const CommandBuffer::Descriptor& descriptor);

    CommandBuffer commandBufferWithUnretainedReferences();

    void insertDebugCaptureBoundary();
    
    void release();
};


} //namespace glx

