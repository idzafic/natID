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
#include <gui/_aux/Cell.h>
#include <fstream>

namespace gui
{

class NATGUI_API GridCell : public Cell
{
public:
    td::UINT2 minHorOrig;
    td::UINT2 minVerOrig;
    td::BYTE nResHorOrig;
    td::BYTE nResVerOrig;
    td::BYTE rowSpan;
    td::BYTE colSpan;
protected:
    
public:
    void clean();
    void serialize(std::ofstream& f) const;
};

}  //namespace gui
