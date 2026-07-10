// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IPopoverButton.h
    @brief Interface for buttons that host a popover selection panel. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <gui/Types.h>

namespace gui
{

class Image;

/// @brief Abstract interface for a button control that presents a popover selection panel.
class IPopoverButton
{
public:
    /// @brief Programmatically sets the current selection within the popover.
    /// @param pos Zero-based index of the item to select.
    /// @param closePopover If true, dismisses the popover after setting the selection.
    virtual void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) = 0;

    /// @brief Sends a notification message for the current popover selection change.
    virtual void sendPopoverMessage() = 0;

    /// @brief Enables or disables the popover interaction.
    /// @param bEnable True to enable the popover, false to disable it.
    virtual void enablePopover(bool bEnable) = 0;

    /// @brief Closes the popover if it is currently open.
    virtual void closePopover() = 0;
};

}  //namespace gui
