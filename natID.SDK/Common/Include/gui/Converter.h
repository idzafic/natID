// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Converter.h
    @brief Provides inline utility functions for converting between GUI geometry types. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Types.h"

namespace gui
{

/// @brief Converts a Geometry (origin + size) value to a Rect (two corner points).
/// @param g  Source geometry describing an origin point and a size.
/// @return A Rect whose top-left is g.point and whose bottom-right is g.point + g.size.
inline gui::Rect toRect(const gui::Geometry& g)
{
    gui::Rect r(g.point.x, g.point.y, g.point.x + g.size.width, g.point.y + g.size.height);
    return r;
}

} //namespace gui
