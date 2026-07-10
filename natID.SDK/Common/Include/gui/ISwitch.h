// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISwitch.h
    @brief Interfaces for a two-state switch control and its state observer. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once


namespace gui
{

class ISwitchState;

/// @brief Abstract interface for a binary (on/off) switch control.
class ISwitch
{
public:
    /// @brief Represents the two possible states of the switch.
    enum State : unsigned char
    {
        Off = 0, ///< Switch is in the off (inactive) state.
        On       ///< Switch is in the on (active) state.
    };

    /// @brief Toggles the switch to the opposite state.
    /// @return The new state after toggling.
    //returns new state
    virtual State toggle() = 0;

    /// @brief Sets the switch to the specified state.
    /// @param state The desired state to set.
    virtual void setState(ISwitch::State state) = 0;

    /// @brief Registers an observer that is notified on state changes.
    /// @param pUpdater Pointer to the ISwitchState observer to register.
    virtual void setUpdater(ISwitchState* pUpdater) = 0;
};

/// @brief Abstract interface for objects that observe and react to ISwitch state changes.
class ISwitchState
{
public:
    /// @brief Called when the associated ISwitch changes its state.
    /// @param state The new state of the switch.
    virtual void update(ISwitch::State state) = 0;
};

} //namespace gui
