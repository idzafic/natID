// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IPopoverUpdater.h
    @brief Interface for objects that update the content of a popover canvas. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

namespace gui
{

class PopoverCanvas;

/// @brief Abstract interface for objects that supply updated content to a PopoverCanvas.
class IPopoverUpdater
{
public:
    /// @brief Called when the popover canvas needs to refresh its content.
    /// @param pPopoverView Pointer to the popover canvas to update.
    virtual void updatePopoverView(PopoverCanvas* pPopoverView) = 0;
};

}  //namespace gui
