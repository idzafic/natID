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

namespace gui
{

class Image;

class IPopoverButton
{
public:
    virtual void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) = 0;
    virtual void sendPopoverMessage() = 0;
    virtual void enablePopover(bool bEnable) = 0;
    virtual void closePopover() = 0;
};

}  //namespace gui
