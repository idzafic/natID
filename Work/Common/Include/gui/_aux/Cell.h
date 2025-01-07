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
#include "CellInfo.h"


namespace gui
{

class Control;

class NATGUI_API Cell : public CellInfo
{
public:
    Control*  pCtrl;
protected:
    
public:
    void clean();
    void operator = (const CellInfo& ci);
    void adjustCtrlVerticalGeometry(Geometry& ctrlGeometry) const;
    void adjustCtrlHorizontalGeometry(Geometry& ctrlGeometry) const;
    void adjustCtrlGeometry(Geometry& ctrlGeometry) const;
};

}  //namespace gui
