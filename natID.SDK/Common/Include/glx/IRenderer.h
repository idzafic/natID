// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IRenderer.h @brief Abstract renderer interface that a glx::View invokes each frame. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <gui/Types.h>

namespace glx
{

class View;

/// @brief Pure-virtual interface that decouples rendering logic from the view lifecycle.
///        Implementations are responsible for issuing GPU commands each frame and releasing
///        any resources they own when the view is destroyed.
class IRenderer
{
public:
    /// @brief Called every frame to record rendering commands for the given view.
    /// @param pView Pointer to the view requesting a draw; provides drawable and pass descriptor.
    virtual void draw(glx::View* pView) = 0;

    /// @brief Called when the view's drawable area changes size.
    /// @param newSize The new width and height of the view in points.
    virtual void onResize(const gui::Size& newSize) = 0;
    
    /// @brief Releases all GPU and CPU resources owned by this renderer.
    virtual void release() = 0;
};

} //namespace glx
