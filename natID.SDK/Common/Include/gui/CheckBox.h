// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CheckBox.h
    @brief A checkbox control that can be toggled on or off, with optional click callback. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DataCtrl.h"

namespace gui
{
	/// @brief A checkbox control that displays a label and supports checked/unchecked state toggling.
	class NATGUI_API CheckBox : public DataCtrl
	{
    private:
        std::function<void()> _onClick; ///< Callback function invoked when the checkbox is clicked
    protected:
        /// @brief Measures the preferred size of the checkbox for layout.
        /// @param cell Layout cell info to fill.
        void measure(CellInfo&) override;
        /// @brief Re-measures the checkbox size after layout changes.
        /// @param cell Layout cell info to update.
        void reMeasure(CellInfo&) override;

        CheckBox(const CheckBox&) = delete;
        CheckBox& operator =(const CheckBox&) = delete;
    public:
        /// @brief Constructs a checkbox with a C-string label.
        /// @param lbl The label text to display next to the checkbox.
        CheckBox(const char* lbl);
        /// @brief Constructs a checkbox with a String label.
        /// @param lbl The label text to display next to the checkbox.
        CheckBox(const td::String& lbl);
        /// @brief Destructor.
        ~CheckBox();
        /// @brief Returns the object type identifier for this control.
        /// @return ObjType::CheckBox
        gui::ObjType getObjType() const override { return ObjType::CheckBox;}
        /// @brief Sets the display title of the checkbox.
        /// @param title The new title string.
        void setTitle(const td::String& title)  override;
        /// @brief Sets the value of the checkbox from a Variant.
        /// @param val The variant value (boolean-compatible expected).
        /// @param sendMessage Whether to trigger change notifications.
        /// @return True if the value was applied successfully.
        bool setValue(const td::Variant& val, bool sendMessage=true) override;
        /// @brief Retrieves the current value of the checkbox as a Variant.
        /// @param val Output variant to receive the value.
        /// @param checkType Whether to enforce type compatibility.
        /// @return True if the value was retrieved successfully.
        bool getValue(td::Variant& val, bool checkType = false) const override;
        /// @brief Returns the current checked state as a boolean.
        /// @return True if checked, false otherwise.
        bool getValue() const;
        /// @brief Sets the checked state from a boolean value.
        /// @param bVal True to check, false to uncheck.
        void setValue(bool bVal);
        /// @brief Returns whether the checkbox is currently checked.
        /// @return True if checked, false otherwise.
        bool isChecked() const;
        /// @brief Sets the checked state of the checkbox.
        /// @param bChecked True to check the checkbox, false to uncheck.
        /// @param sendMessage Whether to trigger change notifications.
        void setChecked(bool bChecked, bool sendMessage = true);

        /// @brief Returns the currently registered click callback.
        /// @return Reference to the click handler function.
        const std::function<void()>& getClickHandler() const;
        /// @brief Registers a callback function to be called on checkbox click.
        /// @param fnToCall The function to invoke when clicked.
        void onClick(const std::function<void()>& fnToCall);
	};
}
