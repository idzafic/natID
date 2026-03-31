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
#include <td/Types.h>

namespace gui
{

enum class ThSep : unsigned char {AppDefault=0, DoNotShowThSep, ShowThSep};

class NATGUI_API NumFormat
{
public:
    td::FormatFloat floatFmt = td::FormatFloat::Decimal;
    char nDecForReal = 6;
    NumFormat(int nDec, td::FormatFloat floatingNoFmt = td::FormatFloat::Decimal);
    void operator = (const NumFormat& nf);
};

} //namespace gui
