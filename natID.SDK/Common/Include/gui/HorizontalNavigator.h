// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HorizontalNavigator.h
    @brief Navigator control with a horizontal orientation for left-to-right item selection. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Navigator.h"


namespace gui
{

/// @brief A navigator control that displays its items horizontally.
class NATGUI_API HorizontalNavigator : public Navigator
{
protected:
    HorizontalNavigator() = delete;
    HorizontalNavigator(const HorizontalNavigator&) = delete;
    HorizontalNavigator& operator =(const HorizontalNavigator&) = delete;
public:
    /// @brief Constructs a HorizontalNavigator with the given number of items.
    /// @param nItems Number of navigation items to display.
    /// @param imageHeight Height of the item images in pixels.
    /// @param widthMultiplier Multiplier applied to image height to determine item width.
    HorizontalNavigator(td::UINT2 nItems, td::UINT2 imageHeight = 64, float widthMultiplier=2.5);

    /// @brief Sets the horizontal alignment of the navigator's content.
    /// @param hAlign Desired horizontal alignment.
    void setAlignment(td::HAlignment hAlign);

    /// @brief Returns the current horizontal alignment of the navigator's content.
    /// @return Current horizontal alignment.
    td::HAlignment getAlignment() const;
};

} //namespace gui
