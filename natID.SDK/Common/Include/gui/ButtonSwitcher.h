// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ButtonSwitcher.h
    @brief A toggle button that switches between two states with distinct labels and tooltips. */
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

/// @brief A button control that toggles between two states (on/off), showing different labels and tooltips for each state.
class NATGUI_API ButtonSwitcher : public Button, public ISwitchState
{
protected:
    gui::ISwitch* _pSwitch; ///< Pointer to the associated switch interface
    td::String _lblOn; ///< Label text displayed when the button is in the On state
    td::String _lblOff; ///< Label text displayed when the button is in the Off state
    td::String _toolTipOn; ///< Tooltip text shown when the button is in the On state
    td::String _toolTipOff; ///< Tooltip text shown when the button is in the Off state

    gui::ISwitch::State _state; ///< Current state of the switcher (On or Off)
protected:
    /// @brief Updates the visual appearance of the button to reflect the current state.
    void updateVisuals();

    ButtonSwitcher() = delete;
    ButtonSwitcher(const ButtonSwitcher&) = delete;
    ButtonSwitcher& operator =(const ButtonSwitcher&) = delete;
public:
    /// @brief Constructs a ButtonSwitcher with string labels and tooltips.
    /// @param lblOn Label to display when the switch is in the On state.
    /// @param lblOff Label to display when the switch is in the Off state.
    /// @param toolTipOn Tooltip text shown when the switch is On.
    /// @param toolTipOff Tooltip text shown when the switch is Off.
    /// @param pSwitch Pointer to the ISwitch interface that controls the state.
    /// @param state Initial state of the button (defaults to On).
    ButtonSwitcher(const td::String& lblOn, const td::String& lblOff, const td::String& toolTipOn, const td::String& toolTipOff, gui::ISwitch* pSwitch, gui::ISwitch::State state = gui::ISwitch::State::On);

    /// @brief Constructs a ButtonSwitcher with byte-array labels and string tooltips.
    /// @param lblOn Byte array containing the label for the On state.
    /// @param lblOff Byte array containing the label for the Off state.
    /// @param toolTipOn Tooltip text shown when the switch is On.
    /// @param toolTipOff Tooltip text shown when the switch is Off.
    /// @param pSwitch Pointer to the ISwitch interface that controls the state.
    /// @param state Initial state of the button (defaults to On).
    ButtonSwitcher(const td::BYTE lblOn[], const td::BYTE lblOff[], const td::String& toolTipOn, const td::String& toolTipOff, gui::ISwitch* pSwitch, gui::ISwitch::State state = gui::ISwitch::State::On);

    /// @brief Updates the button's visual state to match the given switch state.
    /// @param state The new state to apply to the button.
    void update(gui::ISwitch::State state) override;
};

} //namespace gui
