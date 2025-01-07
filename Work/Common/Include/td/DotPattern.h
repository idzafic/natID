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
enum class DotPattern : unsigned char {Pixel = 0, Plus, X, Circle, FilledCircle, Triangle, FilledTriangle, Square, FilledSqaure, NA};

MAINUTILS_API const char* toString(td::DotPattern dotPattern);
MAINUTILS_API td::DotPattern toDotPattern(const char* pStr);

inline DataType getType(td::DotPattern) { return td::dotPattern; }

}
