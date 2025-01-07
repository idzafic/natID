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
#include <gui/natGUI.h>
#include <gui/Types.h>
#include <tuple>

namespace gui
{

class NATGUI_API CellInfo
{
public:
    td::UINT2 minHor = 0;
    td::UINT2 minVer = 0;
    td::BYTE nResHor = 0;
    td::BYTE nResVer = 0;
    td::BYTE hAlign : 2;
    td::BYTE vAlign : 2;
    td::BYTE _rowSpan : 1; //set if cell is spanned over several rows
    td::BYTE _colSpan : 1; //set if cell is spanned over several cols
    //td::BYTE spanRedistributed: 1; //set if initial redistribution was done
    td::BYTE freezed : 1;
    td::BYTE keepAspectRatio : 1;
//    td::BYTE handledInitialAppearance : 1;
    //td::BYTE autoResize: 1; //

    //td::BYTE fixedHeightByOtherRows : 1; //mozda za ImageView
public:
    void clean();
    void operator = (const CellInfo& ci);
    bool isVisible() const;
    void setSpanInfo(td::BYTE rowSpan=0, td::BYTE colSpan=0, td::BYTE redistributed=0);
    void setAlignment(td::HAlignment hAlign, td::VAlignment vAlign);
    
    td::HAlignment getHAlignment() const;
    td::VAlignment getVAlignment() const;
    std::tuple<td::HAlignment, td::VAlignment> getAlignment() const;
    bool isRowSpan() const;
    bool isColSpan() const;
    bool isResizableInAllDirections() const;
};

} // namespace gui
