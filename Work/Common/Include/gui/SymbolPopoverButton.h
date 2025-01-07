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
#include <gui/PopoverButton.h>
#include <gui/SymbolPopoverView.h>

namespace gui
{

class NATGUI_API SymbolPopoverButton : public PopoverButton
{
public:
protected:
    SymbolPopoverView _symbolPopoverView;
public:
    SymbolPopoverButton(PopoverView::Type type, td::UINT2 nItems = 64);
//    ~SymbolPopoverButton();
};

} //namespace gui
