// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LinePattern.h
    @brief Enumeration and utility functions for line dash/dot pattern styles. */
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
/// @brief Defines the dash/dot pattern used when drawing lines.
enum class LinePattern : unsigned char {
    Solid = 0, ///< Continuous solid line.
    Dash,      ///< Dashed line.
    Dot,       ///< Dotted line.
    DashDot,   ///< Alternating dash and dot.
    DashDotDot,///< Alternating dash followed by two dots.
    NA         ///< Not applicable / undefined pattern.
};

/// @brief Returns a C-string name for the given LinePattern value.
/// @param linePattern The line pattern to convert.
/// @return Null-terminated string representation.
MAINUTILS_API const char* toString(td::LinePattern linePattern);

/// @brief Parses a LinePattern from a C-string.
/// @param pStr Null-terminated string naming the pattern.
/// @return The corresponding LinePattern value.
MAINUTILS_API td::LinePattern toLinePattern(const char* pStr);

/// @brief Parses a LinePattern from a td::String.
/// @param str String naming the pattern.
/// @return The corresponding LinePattern value.
MAINUTILS_API td::LinePattern toLinePattern(const td::String& str);

/// @brief Returns the DataType tag for LinePattern.
/// @return td::linePattern
inline DataType getType(td::LinePattern) { return td::linePattern; }

}
