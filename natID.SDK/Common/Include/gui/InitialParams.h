// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file InitialParams.h
 * @brief Initial application parameters passed at startup.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGUI.h"
#include <td/String.h>
#include <gui/Window.h>

namespace gui
{
/// @brief Holds initial parameters for application startup configuration.
typedef struct _InitialParams
{
    gui::Window::FrameSize initialFrameSize; ///< Initial frame size of the main window.
}InitialParams;

} //namespace gui
