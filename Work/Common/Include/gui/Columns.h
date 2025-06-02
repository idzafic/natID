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
#include <gui/_aux/Format.h>
#include <cnt/SafeFullVector.h>
#include <td/DateTime.h>

namespace gui
{

class Column
{
public:
    const cnt::SafeFullVector<td::String>* pEnumStrings = nullptr;
    td::String header;
    td::String toolTip;
    td::INT2 width = 100;
    union
    {
        td::Date::Format dateFormat;
        char nDecForReal;
    };
    union
    {
        td::Time::Format timeFormat;
        td::FormatFloat floatingNoFmt;
    };
    td::BYTE posInDS = 0;
    td::HAlignment hAlign = td::HAlignment::Left;
    ThSep showThSep = ThSep::AppDefault;
    Column()
    {
        nDecForReal = 2;
        floatingNoFmt = td::FormatFloat::Decimal;
    }
};

class Header
{
public:
    td::String header;
    td::String toolTip;
    td::INT2 width;
    td::BYTE posInDS;
    td::HAlignment hAlign;
    Header(td::BYTE positionInDS, const char* colHeader, const char* colToolTip = nullptr, td::INT2 colWidth = 100, td::HAlignment hAlignCol = td::HAlignment::Left)
    : header(colHeader)
    , toolTip(colToolTip)
    , width(colWidth)
    , posInDS(positionInDS)
    , hAlign(hAlignCol)
    {}
    Header(td::BYTE positionInDS, const td::String& colHeader, const td::String& colToolTip = "", td::INT2 colWidth = 100, td::HAlignment hAlignCol = td::HAlignment::Left)
    : header(colHeader)
    , toolTip(colToolTip)
    , width(colWidth)
    , posInDS(positionInDS)
    , hAlign(hAlignCol)
    {}
};

class NATGUI_API Columns
{
    cnt::SafeFullVector<gui::Column>& _columns;
    int _iCol;
public:
    Columns(cnt::SafeFullVector<gui::Column>& columns);
    
    Columns& operator << (gui::ThSep showThSep);
    
    Columns& operator << (const NumFormat& numericFormat);
    
    Columns& operator << (td::Date::Format dateFormat);
    
    Columns& operator << (td::Time::Format timeFormat);
    
    Columns& operator << (const Header& header);
};

} //namespace gui
