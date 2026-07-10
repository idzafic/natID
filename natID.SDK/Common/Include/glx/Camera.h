// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Camera.h
    @brief Provides the Camera class representing a viewpoint in 3D space. */

#pragma once
#include "natGLX.h"
#include <glx/GLX.h>

namespace glx
{

/// @brief Represents a camera defining a viewpoint and projection for 3D rendering.
class NATGLX_API Camera
{
public:
    // constructor with vectors
    /// @brief Default constructor. Initializes a camera with default position and orientation.
    Camera();

};


} //namespace glx
