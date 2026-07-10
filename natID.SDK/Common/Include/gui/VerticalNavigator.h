// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VerticalNavigator.h
    @brief Vertical side-navigation control that presents a scrollable list of icon-based items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Navigator.h"


namespace gui
{

/// @brief Side-navigation control that lays out navigator items vertically.
class NATGUI_API VerticalNavigator : public Navigator
{
protected:
    VerticalNavigator(const VerticalNavigator&) = delete;
    VerticalNavigator& operator =(const VerticalNavigator&) = delete;
    VerticalNavigator() = delete;
public:
    /// @brief Constructs a VerticalNavigator with the given capacity and item dimensions.
    /// @param nItems Maximum number of navigation items.
    /// @param imageHeight Pixel height of each item's icon (default 64).
    /// @param widthMultiplier Multiplier applied to imageHeight to derive the control width (default 2.5).
    VerticalNavigator(td::UINT2 nItems, td::UINT2 imageHeight = 64, float widthMultiplier=2.5);
    /// @brief Sets the vertical alignment of items within the navigator.
    /// @param vAlign Desired vertical alignment.
    void setAlignment(td::VAlignment vAlign);
    /// @brief Returns the current vertical alignment of items.
    /// @return Current vertical alignment value.
    td::VAlignment getAlignment() const;
};

} //namespace gui
