// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CameraView.h
    @brief Provides the CameraView class for rendering a scene from a camera's perspective. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <glx/GLX.h>

namespace glx
{

/// @brief Represents a view associated with a camera, used to render a 3D scene from a specific viewpoint.
class NATGLX_API CameraView
{
    /// @brief Copy constructor is deleted; CameraView is not copyable.
    CameraView(const CameraView&) = delete;
    /// @brief Copy assignment is deleted; CameraView is not copyable.
    CameraView& operator =(const CameraView&) = delete;
public:
    // constructor with vectors
    /// @brief Default constructor. Initializes a CameraView with default settings.
    CameraView();

};


} //namespace glx
