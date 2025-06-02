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

#include "Button.h"
#include <td/String.h>
#include <gui/ISwitch.h>

namespace gui
{

class NATGUI_API ButtonSwitcher : public Button, public ISwitchState
{
protected:
    gui::ISwitch* _pSwitch;
    td::String _lblOn;
    td::String _lblOff;
    td::String _toolTipOn;
    td::String _toolTipOff;
    
    gui::ISwitch::State _state;
protected:
    void updateVisuals();
    
    ButtonSwitcher() = delete;
public:
    ButtonSwitcher(const td::String& lblOn, const td::String& lblOff, const td::String& toolTipOn, const td::String& toolTipOff, gui::ISwitch* pSwitch, gui::ISwitch::State state = gui::ISwitch::State::On);

    ButtonSwitcher(const td::BYTE lblOn[], const td::BYTE lblOff[], const td::String& toolTipOn, const td::String& toolTipOff, gui::ISwitch* pSwitch, gui::ISwitch::State state = gui::ISwitch::State::On);

    void update(gui::ISwitch::State state) override;
};

} //namespace gui
