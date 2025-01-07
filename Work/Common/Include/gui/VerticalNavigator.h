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
#include "Navigator.h"


namespace gui
{

class NATGUI_API VerticalNavigator : public Navigator
{
protected:

public:
    VerticalNavigator(td::UINT2 nItems, td::UINT2 imageHeight = 64, float widthMultiplier=2.5);
    void setAlignment(td::VAlignment vAlign);
    td::VAlignment getAlignment() const;
};

} //namespace gui
