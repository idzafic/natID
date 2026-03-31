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
#include "Types.h"

namespace gui
{

inline gui::Rect toRect(const gui::Geometry& g)
{
    gui::Rect r(g.point.x, g.point.y, g.point.x + g.size.width, g.point.y + g.size.height);
    return r;
}

} //namespace gui
