// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Color.h
    @brief Utility class for converting between color representations (ColorID, RGBA, hex strings, and names). */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <td/Types.h>
//#include <tuple>

namespace gui
{

/// @brief Provides static utility methods for color conversion and lookup.
class NATGUI_API Color
{
public:
    /// @brief Retrieves RGBA byte components for a given ColorID.
    /// @param colorID The color identifier to convert.
    /// @param R Output red channel (0-255).
    /// @param G Output green channel (0-255).
    /// @param B Output blue channel (0-255).
    /// @param A Output alpha channel (0-255).
    static void getRGBA(td::ColorID, td::BYTE& R, td::BYTE& G, td::BYTE& B, td::BYTE& A);
    /// @brief Retrieves RGBA float components for a given ColorID.
    /// @param colorID The color identifier to convert.
    /// @param R Output red channel (0.0-1.0).
    /// @param G Output green channel (0.0-1.0).
    /// @param B Output blue channel (0.0-1.0).
    /// @param A Output alpha channel (0.0-1.0).
    static void getRGBA(td::ColorID, float& R, float& G, float& B, float& A);
    /// @brief Converts a ColorID to a td::Color value.
    /// @param colorID The color identifier to convert.
    /// @return The corresponding td::Color value.
    static td::Color toTDColor(td::ColorID);
    //static td::Color getColor(td::ColorID colorID);
    /// @brief Converts a td::Color value to the nearest ColorID.
    /// @param color The td::Color to look up.
    /// @return The matching ColorID.
    static td::ColorID toColorID(const td::Color&);
    /// @brief Returns the human-readable name of a color.
    /// @param clrID The color identifier.
    /// @return Null-terminated C string with the color name.
    static const char* getName(td::ColorID clrID);
    /// @brief Converts a ColorID to a 6-character hex string (e.g., "#RRGGBB").
    /// @param clrID The color identifier.
    /// @param hex Output buffer of at least 8 characters.
    static void toHex3(td::ColorID clrID, char hex[8]);
    /// @brief Parses a 6-character hex string and returns the corresponding ColorID.
    /// @param hex Null-terminated hex color string (e.g., "#RRGGBB").
    /// @return The matching ColorID.
    static td::ColorID fromHex3(const char* hex);
//    static void toHex4(td::ColorID clrID, char hex[10]);
};

} //namespace gui
