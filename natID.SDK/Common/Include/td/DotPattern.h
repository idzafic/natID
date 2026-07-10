// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DotPattern.h
    @brief Enumeration and utility functions for dot/marker pattern styles used in charts and graphs. */
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
/// @brief Defines the shape used to draw individual data points or markers.
enum class DotPattern : unsigned char {
    Pixel = 0,      ///< Single pixel dot.
    Plus,           ///< Plus (+) shaped marker.
    X,              ///< X-shaped marker.
    Circle,         ///< Hollow circle marker.
    FilledCircle,   ///< Filled circle marker.
    Triangle,       ///< Hollow triangle marker.
    FilledTriangle, ///< Filled triangle marker.
    Square,         ///< Hollow square marker.
    FilledSqaure,   ///< Filled square marker.
    NA              ///< Not applicable / undefined pattern.
};

/// @brief Returns a C-string name for the given DotPattern value.
/// @param dotPattern The dot pattern to convert.
/// @return Null-terminated string representation.
MAINUTILS_API const char* toString(td::DotPattern dotPattern);

/// @brief Parses a DotPattern from a C-string.
/// @param pStr Null-terminated string naming the pattern.
/// @return The corresponding DotPattern value.
MAINUTILS_API td::DotPattern toDotPattern(const char* pStr);

/// @brief Parses a DotPattern from a td::String.
/// @param str String naming the pattern.
/// @return The corresponding DotPattern value.
MAINUTILS_API td::DotPattern toDotPattern(const td::String& str);

/// @brief Returns the DataType tag for DotPattern.
/// @return td::dotPattern
inline DataType getType(td::DotPattern) { return td::dotPattern; }

}
