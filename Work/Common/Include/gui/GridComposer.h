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
#include "GridLayout.h"

namespace gui
{

class NATGUI_API GridComposer
{
private:
    GridLayout& _grid;
    int _iRow;
    int _iCol;
public:
    GridComposer(GridLayout& grid);
    
    GridComposer& appendRow(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);
    
    GridComposer& appendRow(Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);
    
    GridComposer& startNewRowWithSpace(td::UINT2 horizontalSpace, td::UINT2 verticalSpace);

    GridComposer& appendCol(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);
    
    GridComposer& appendEmptyCols(size_t nEmptyCols);
    
    GridComposer& appendCol(Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);
    
    GridComposer& appendSpace(td::UINT2 horizontalSpace, td::UINT2 verticalSpace);
    
    GridComposer& appendSpacer(td::UINT2 horizontalSpace, td::BYTE nHor, td::UINT2 verticalSpace = 0, td::BYTE nVert = 0);
    
    GridComposer& operator << (Control& ctrl);
    
};

} //namespace gui
