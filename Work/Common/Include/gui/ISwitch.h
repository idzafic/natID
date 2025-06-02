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

class ISwitchState;

class ISwitch
{
public:
    enum State : unsigned char {Off = 0, On};
    //returns new state
    virtual State toggle() = 0;
    virtual void setState(ISwitch::State state) = 0;
    virtual void setUpdater(ISwitchState* pUpdater) = 0;
};

class ISwitchState
{
public:
    virtual void update(ISwitch::State state) = 0;
};

} //namespace gui
