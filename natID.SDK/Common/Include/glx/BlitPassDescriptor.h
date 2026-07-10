// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BlitPassDescriptor.h
    @brief Provides the BlitPassDescriptor class for configuring a blit render pass. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <glx/GLX.h>

namespace MTL
{
class BlitPassDescriptor;
} //namespace MTL

namespace glx
{

/// @brief Describes the configuration of a blit pass used for GPU copy operations.
class NATGLX_API BlitPassDescriptor
{
private:
    MTL::BlitPassDescriptor* _pBlitPassDescriptor = nullptr; ///< Pointer to the underlying Metal blit pass descriptor.
protected:
    /// @brief Default constructor. Creates an empty BlitPassDescriptor.
    BlitPassDescriptor() {};
    /// @brief Copy constructor is deleted; BlitPassDescriptor is not copyable.
    BlitPassDescriptor(const BlitPassDescriptor&) = delete;
    /// @brief Copy assignment is deleted; BlitPassDescriptor is not copyable.
    BlitPassDescriptor& operator =(const BlitPassDescriptor&) = delete;
public:
    /// @brief Allocates a new BlitPassDescriptor instance.
    /// @return Pointer to the newly allocated BlitPassDescriptor.
    static class BlitPassDescriptor*                     alloc();

    /// @brief Initializes this BlitPassDescriptor.
    /// @return Pointer to this initialized BlitPassDescriptor.
    class BlitPassDescriptor*                            init();

    /// @brief Allocates and initializes a default BlitPassDescriptor.
    /// @return Pointer to the new default BlitPassDescriptor.
    static class BlitPassDescriptor*                     blitPassDescriptor();

    //class BlitPassSampleBufferAttachmentDescriptorArray* sampleBufferAttachments() const;

};


} //namespace glx
