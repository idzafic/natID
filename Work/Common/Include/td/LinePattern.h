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
enum class LinePattern : unsigned char {Solid=0, Dash, Dot, DashDot, DashEq, NA};

MAINUTILS_API const char* toString(td::LinePattern linePattern);
MAINUTILS_API td::LinePattern toLinePattern(const char* pStr);

inline DataType getType(td::LinePattern) { return td::linePattern; }

}
