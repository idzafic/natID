// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Anchor.h
    @brief Defines the Anchor enumeration and related utility functions for positional anchoring. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <mu/muLib.h>
#include <td/String.h>

namespace td
{
/// @brief Specifies the anchor position used for aligning or attaching UI elements or graphical objects.
enum class Anchor : unsigned char {
    Fixed=0,     ///< Element is fixed in place, no anchoring adjustment.
    Center,      ///< Anchored to the center.
    Left,        ///< Anchored to the left edge.
    Right,       ///< Anchored to the right edge.
    Top,         ///< Anchored to the top edge.
    Bottom,      ///< Anchored to the bottom edge.
    TopLeft,     ///< Anchored to the top-left corner.
    TopRight,    ///< Anchored to the top-right corner.
    BottomLeft,  ///< Anchored to the bottom-left corner.
    BottomRight  ///< Anchored to the bottom-right corner.
};

/// @brief Converts an Anchor value to its string representation.
/// @param anchor The anchor value to convert.
/// @return A C-string describing the anchor position.
MAINUTILS_API const char* toString(td::Anchor anchor);

/// @brief Converts a C-string to the corresponding Anchor enum value.
/// @param pStr A null-terminated string representing an anchor position.
/// @return The Anchor enum value matching the string, or a default if not found.
MAINUTILS_API td::Anchor toAnchor(const char* pStr);

/// @brief Converts a td::String to the corresponding Anchor enum value.
/// @param str A td::String representing an anchor position.
/// @return The Anchor enum value matching the string, or a default if not found.
MAINUTILS_API td::Anchor toAnchor(const td::String& str);

/// @brief Returns the DataType identifier for Anchor values.
/// @return The td::anchor DataType constant.
inline DataType getType(td::Anchor) { return td::anchor; }

}
