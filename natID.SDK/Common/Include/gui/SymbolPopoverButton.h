// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SymbolPopoverButton.h
    @brief Popover button that presents a grid of selectable symbols. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PopoverButton.h>
#include <gui/SymbolPopoverView.h>

namespace gui
{

/// @brief A button that opens a popover grid populated with Symbol or Image items.
class NATGUI_API SymbolPopoverButton : public PopoverButton
{

protected:
    SymbolPopoverView _symbolPopoverView; ///< The popover canvas that displays the symbol grid.

    SymbolPopoverButton(const SymbolPopoverButton&) = delete;
    SymbolPopoverButton& operator =(const SymbolPopoverButton&) = delete;
    SymbolPopoverButton() = delete;
public:
    /// @brief Constructs a symbol popover button.
    /// @param type Canvas type controlling how items are laid out and styled.
    /// @param nItems Initial capacity for the number of items in the popover grid.
    SymbolPopoverButton(PopoverCanvas::Type type, td::UINT2 nItems = 64);
};

} //namespace gui
