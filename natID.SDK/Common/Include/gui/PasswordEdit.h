// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PasswordEdit.h @brief Provides a single-line text edit control that masks entered characters as a password field. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "LineEdit.h"

namespace gui
{
/// @brief A line-edit control that conceals its text content for password entry.
	class NATGUI_API PasswordEdit : public LineEdit
	{
        PasswordEdit(const PasswordEdit&) = delete;
        PasswordEdit& operator =(const PasswordEdit&) = delete;
    public:
        /// @brief Constructs a PasswordEdit control.
        /// @param sendMsg Whether the control should send change messages.
        PasswordEdit(Messages sendMsg = Messages::DoNotSend);

        /// @brief Constructs a PasswordEdit control with a tooltip.
        /// @param toolTip Tooltip text shown on hover.
        /// @param sendMsg Whether the control should send change messages.
        PasswordEdit(const td::String& toolTip, Messages sendMsg = Messages::DoNotSend);

        /// @brief Returns the GUI object type identifier for PasswordEdit.
        /// @return ObjType::PasswordEdit.
        gui::ObjType getObjType() const override { return ObjType::PasswordEdit;}
//        void toggleVisibility();
	};
}
