// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ResourceStateCommandEncoder.h
    @brief Declares the glx::ResourceStateCommandEncoder wrapper class. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <glx/GLX.h>

namespace MTL
{
class ResourceStateCommandEncoder;
} //namespace MTL

namespace glx
{

/// @brief Wraps a Metal ResourceStateCommandEncoder for managing resource state transitions.
class NATGLX_API ResourceStateCommandEncoder
{
private:
    MTL::ResourceStateCommandEncoder* _pResourceStateCommandEncoder = nullptr; ///< Pointer to the underlying Metal resource state command encoder.
protected:
    /// @brief Default constructor; initialises the encoder without an underlying native object.
    ResourceStateCommandEncoder();
public:


};


} //namespace glx
