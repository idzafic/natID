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
#include <gui/Types.h>
#include <gui/Control.h>
#include <td/String.h>

namespace gui
{
class IDrawDelegate
{
public:
    virtual void getMinSize(gui::Size& sz, const gui::Control* pCtrl) const = 0;
    virtual void draw(const gui::Rect& rect, gui::Control* pCtrl) = 0;
};

} //namepace gui

