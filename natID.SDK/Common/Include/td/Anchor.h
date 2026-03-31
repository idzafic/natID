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
#include <td/String.h>

namespace td
{
enum class Anchor : unsigned char {Fixed=0, Center, Left, Right, Top, Bottom, TopLeft, TopRight, BottomLeft, BottomRight};

MAINUTILS_API const char* toString(td::Anchor anchor);
MAINUTILS_API td::Anchor toAnchor(const char* pStr);
MAINUTILS_API td::Anchor toAnchor(const td::String& str);

inline DataType getType(td::Anchor) { return td::anchor; }

}
