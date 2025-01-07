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
#include <gui/natGUI.h>

namespace gui
{

class NATGUI_API Cursor
{
public:
    enum class Type : char {None=0, Default, Adder, Move, ResizeUp, ResizeDown, ResizeVertical, ResizeLeft, ResizeRight, ResizeHorizontal, OpenHand, ClosedHand, Finger, Invisible, Custom};
//    static void set(Cursor::Type ct);
//    static void hide();
//    void hideUntilMouseMoves(bool bHide = true);
//    static void unhide();
};



} //namespace gui
