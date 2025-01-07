// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <mu/muLib.h>

namespace td
{
class Color;
enum class Accent : unsigned char {Plain = 0, Success, Warning, Error, Critical};

enum class ColorID : unsigned char
{
    Maroon=0,
    DarkRed,
    Brown,
    Firebrick,
    Crimson,
    Red,
    Tomato,
    Coral,
    IndianRed,
    LightCoral,
    DarkSalmon,
    Salmon,
    LightSalmon,
    OrangeRed,
    DarkOrange,
    Orange,
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
    DarkSeaGreen,
    MediumSpringGreen,
    SpringGreen,
    SeaGreen,
    MediumAquaMarine,
    MediumSeaGreen,
    LightSeaGreen,
    DarkSlateGray,
    Teal,
    DarkCyan,
    Cyan,
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
    Thistle,
    Plum,
    Violet,
    Magenta,
    Orchid,
    MediumVioletRed,
    PaleVioletRed,
    DeepPink,
    HotPink,
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
    Chocolate,
    Peru,
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
    DimGray,
    Gray,
    DarkGray,
    Silver,
    LightGray,
    Gainsboro,
    WhiteSmoke,
    White,
    SysSelectedItemBack, //prvi sistem color
    SysSelectedTextBack,
    SysLabel,
    SysHeaderText,
    SysHeaderBack,
    SysText, //zanji za color picker
    SysSelectedText,
    SysBackAlternate1,
    SysBackAlternate2,
    SysTransparentSelection,
    SysDisabledCtrlColor,
    SysControlText,
    SysLabelHighlight,
    SysCtrlBack,
    SysCritical,
    SysConstructive,
    SysDestructive,
    SysWarning,
    SysError,
    Transparent //Ovaj mora biti zadnji
};

MAINUTILS_API const char* toString(td::ColorID colorID);

MAINUTILS_API td::ColorID toColorID(const char* pStr, int len = -1);

MAINUTILS_API bool isSystemBackgroundColor(td::ColorID);

inline DataType getType(td::ColorID) { return td::colorID; }

}
