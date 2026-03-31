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
#include <gui/Types.h>
#include <td/ColorID.h>

namespace gui
{

class ILineNumbers
{
public:
    virtual void setTotalLines(td::UINT4 totalNoOfLines, bool forceRedraw = false) = 0;
    virtual td::UINT4 getTotalLines() const = 0;
    virtual void setCurrentLine(td::UINT4 lineNo) = 0;
    virtual td::UINT4 getCurrentLine() const = 0;
    virtual void setScrollPos(CoordType dy) = 0;
    virtual void fontChanged() = 0;
    virtual gui::CoordType getOffsetOnRightSide() const = 0;
    virtual td::ColorID getColor() const = 0;
    virtual td::ColorID getColorForSelectedLine() const = 0;
};

}  //namespace gui
