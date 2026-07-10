// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ActionItem.h
    @brief Base class representing an interactive action item such as a menu entry or toolbar button. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NatObject.h"

namespace gui
{
/// @brief Base class for a GUI action item (menu item, toolbar button, etc.) that can be enabled, disabled, and checked.
	class NATGUI_API ActionItem : public NatObject
	{
    protected:
        ActionItem(const ActionItem&) = delete;
        ActionItem& operator =(const ActionItem&) = delete;
        /// @brief Default constructor for ActionItem.
        ActionItem();
        //void setID(td::UINT4 id);
        //td::UINT4 getID() const;
    public:
        /// @brief Move constructor for ActionItem.
        /// @param other The ActionItem to move from.
        ActionItem(ActionItem&&);
        /// @brief Move assignment operator for ActionItem.
        /// @param other The ActionItem to move from.
        /// @return Reference to this ActionItem.
        ActionItem& operator =(ActionItem&&);

        /// @brief Enables or disables this action item.
        /// @param bEnable If true, enables the item; otherwise disables it.
        void enable(bool bEnable = true);
        /// @brief Disables this action item.
        void disable();
        /// @brief Returns whether this action item is currently enabled.
        /// @return True if the item is enabled, false otherwise.
        bool isEnabled() const;
        /// @brief Sets the checked state of this action item.
        /// @param bChecked If true, marks the item as checked.
        void setChecked(bool bChecked);
        /// @brief Returns whether this action item is currently checked.
        /// @return True if the item is checked, false otherwise.
        bool isChecked() const;
	};
}
