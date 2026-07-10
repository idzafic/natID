// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ColorID.h
 * @brief Defines the ColorID enumeration and related utility functions for named color identifiers.
 */
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

class Color;
/// @brief Accent level used to indicate status or emphasis.
enum class Accent : unsigned char {Plain = 0, Success, Warning, Error, Critical};
/// @brief Color scheme selection (auto, light, or dark).
enum class ColorScheme : unsigned char {Auto=0, Light, Dark};

/// @brief Enumeration of named color identifiers, including standard, web, and system colors.
enum class ColorID : unsigned char
{
    Maroon=0,
    Rust,               // New: Reddish-brown tone
    DarkRed,
    Brown,
    Terracotta,         // New: Earthy reddish-brown
    Firebrick,
    Crimson,
    Red,
    Vermilion,          // New: Bright red-orange
    Tomato,
    Coral,
    IndianRed,
    Copper,             // New: Metallic reddish-brown
    LightCoral,
    DarkSalmon,
    Salmon,
    LightSalmon,
    OrangeRed,
    DarkOrange,
    Orange,
    Tangerine,           // New: Bright orange
    Apricot,            // New: Soft orange
    Gold,
    DarkGoldenRod,
    GoldenRod,
    PaleGoldenTod,
    DarkKhaki,
    Khaki,
    Olive,
    Yellow,
    YellowGreen,
    DarkOliveGreen,
    OliveDrab,
    LawnGreen,
    Chartreuse,
    GreenYellow,
    DarkGreen,
    Green,
    ForestGreen,
    Lime,
    LimeGreen,
    LightGreen,
    PaleGreen,
    Mint,               // Fresh light green
    Sage,               // New: Muted green
    Moss,               // New: Earthy green
    DarkSeaGreen,
    MediumSpringGreen,
    SpringGreen,
    Pine,               // New: Deep forest green
    SeaGreen,
    MediumAquaMarine,
    MediumSeaGreen,
    LightSeaGreen,
    DarkSlateGray,
    Teal,
    DarkCyan,
    Cyan,
    Aqua,               // Bright cyan-blue
    LightCyan,
    DarkTurquoise,
    Turquoise,
    MediumTurquoise,
    PaleTurquoise,
    AquaMarine,
    PowderBlue,
    CadetBlue,
    SteelBlue,
    CornFlowerBlue,
    DeepSkyBlue,
    DodgerBlue,
    LightBlue,
    SkyBlue,
    LightSkyBlue,
    Periwinkle,         // New: Soft blue-violet
    Cerulean,           // New: Deep sky blue
    MidnightBlue,
    Navy,
    DarkBlue,
    MediumBlue,
    Blue,
    RoyalBlue,
    BlueViolet,
    Indigo,
    DarkSlateBlue,
    SlateBlue,
    MediumSlateBlue,
    MediumPurple,
    DarkMagenta,
    DarkViolet,
    DarkOrchid,
    MediumOrchid,
    Purple,
    Mulberry,           // Rich purplish-red
    Berry,              // New: Deep reddish-purple
    Mauve,              // New: Soft purple
    Eggplant,           // New: Deep purple
    Thistle,
    Plum,
    Violet,
    Magenta,
    Orchid,
    MediumVioletRed,
    PaleVioletRed,
    DeepPink,
    HotPink,
    Flamingo,           // Vibrant pink
    LightPink,
    Pink,
    AntiqueWhite,
    Beige,
    Bisque,
    BlanchedAlmond,
    Wheat,
    CornSilk,
    LemonChiffon,
    LightGoldenRodYellow,
    LightYellow,
    SaddleBrown,
    Sienna,
    Sepia,              // New: Warm brownish tone
    Chocolate,
    Peru,
    Bronze,             // New: Metallic brownish tone
    SandyBrown,
    BurlyWood,
    Tan,
    RosyBrown,
    Moccasin,
    NavajoWhite,
    PeachPuff,
    MistyRose,
    LavenderBlush,
    Linen,
    OldLace,
    PapayaWhip,
    SeaShell,
    MintCream,
    SlateGray,
    LightSlateGray,
    LightSteelBlue,
    Lavender,
    FloralWhite,
    AliceBlue,
    GhostWhite,
    Honeydew,
    Ivory,
    Azure,
    Snow,
    Black,
    ObsidianGray,
    Ash,                // New: Medium gray
    Slate,              // New: Bluish-gray
    DimGray,
    Gray,
    Smoke,              // New: Light gray
    DarkGray,
    Stone,              // New: Dark gray
    Silver,
    LightGray,
    Gainsboro,
    WhiteSmoke,
    White,
    SysSelectedItemBack, //prvi sistem color
    SysSelectedTextBack,
    SysLabel,
    SysHeaderBack,
    SysText, //zanji za color picker
    SysHeaderText,
    SysSelectedText,
    SysBackAlt1,
    SysBackAlt2,
    SysTransparentSelection,
    //SysDisabledCtrlColor,
    SysCtrlText,
    SysCtrlTextDisabled,
    SysCtrlBack,
    SysLabelHighlight,
    SysCritical,
    SysConstructive,
    SysDestructive,
    SysWarning,
    SysError,
    SysHighlightBackground, //e.g. for code editor
    Transparent //Ovaj mora biti zadnji
};

/// @brief Converts a ColorID value to its string representation.
/// @param colorID The color identifier to convert.
/// @return A null-terminated string with the color name.
MAINUTILS_API const char* toString(td::ColorID colorID);

/// @brief Converts a string to the corresponding ColorID value.
/// @param pStr Pointer to the string to parse.
/// @param len Length of the string, or -1 to detect automatically.
/// @param defaultColor Default color returned when the string is not recognized.
/// @return The matching ColorID, or defaultColor if not found.
MAINUTILS_API td::ColorID toColorID(const char* pStr, int len = -1, td::ColorID defaultColor = td::ColorID::SysText);

/// @brief Converts a ColorScheme value to its string representation.
/// @param defaultScheme The color scheme to convert.
/// @return A null-terminated string with the scheme name.
MAINUTILS_API const char* toString(td::ColorScheme defaultScheme);

/// @brief Converts a string to the corresponding ColorScheme value.
/// @param pStr Pointer to the string to parse.
/// @param len Length of the string, or -1 to detect automatically.
/// @param defaultScheme Default scheme returned when the string is not recognized.
/// @return The matching ColorScheme, or defaultScheme if not found.
MAINUTILS_API td::ColorScheme toColorScheme(const char* pStr, int len = -1, td::ColorScheme defaultScheme = td::ColorScheme::Auto);

/// @brief Converts a td::String to the corresponding ColorID value.
/// @param str The string to parse.
/// @param defaultColor Default color returned when the string is not recognized.
/// @return The matching ColorID, or defaultColor if not found.
inline td::ColorID toColorID(const td::String& str, td::ColorID defaultColor = td::ColorID::SysText)
{
    return toColorID(str.c_str(), (int) str.length(), defaultColor);
}

/// @brief Converts a StringExt pointer to the corresponding ColorID value.
/// @param pStrExt Pointer to the extended string, may be null.
/// @param defaultColor Default color returned when the pointer is null or string is not recognized.
/// @return The matching ColorID, or defaultColor if not found.
inline td::ColorID toColorID(const td::StringExt* pStrExt, td::ColorID defaultColor = td::ColorID::SysText)
{
    if (!pStrExt)
        return td::ColorID::SysText;

    return toColorID(pStrExt->c_str(), (int) pStrExt->length(), defaultColor);
}

/// @brief Checks whether the given ColorID represents a system background color.
/// @param colorID The color identifier to check.
/// @return True if it is a system background color, false otherwise.
MAINUTILS_API bool isSystemBackgroundColor(td::ColorID);

/// @brief Returns the DataType tag associated with ColorID values.
/// @return td::colorID data type.
inline DataType getType(td::ColorID) { return td::colorID; }

}
