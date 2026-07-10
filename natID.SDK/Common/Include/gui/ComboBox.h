// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ComboBox.h
    @brief Declares the ComboBox control for selecting a value from a list of string items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "BaseComboBox.h"
#include <cnt/SafeFullVector.h>
#include <cnt/PushBackVector.h>
#include <td/String.h>
#include <vector>

namespace gui
{
	/// @brief A combo-box control that presents a drop-down list of plain string items.
	class NATGUI_API ComboBox : public BaseComboBox
	{
    private:
        std::function<void()> _onChangedSelection; ///< Callback invoked when the selected item changes.

        ComboBox(const ComboBox&) = delete;
        ComboBox& operator =(const ComboBox&) = delete;
    public:
        /// @brief Default constructor; creates an empty combo box.
        ComboBox();

        /// @brief Constructs a combo box with an initial tooltip.
        /// @param toolTip  Tooltip text displayed when the user hovers over the control.
        ComboBox(const td::String& toolTip);

        /// @brief Returns the GUI object type identifier for this control.
        /// @return ObjType::ComboBox.
        gui::ObjType getObjType() const override { return ObjType::ComboBox;}

        /// @brief Appends a string item to the drop-down list.
        /// @param item  The text of the item to add.
        void addItem(const td::String& item);

        /// @brief Appends a C-string item to the drop-down list.
        /// @param item  The text of the item to add.
        void addItem(const char* item);

        /// @brief Appends multiple items from a raw array.
        /// @param pItems  Pointer to the first element of a string array.
        /// @param nItems  Number of items in the array.
        void addItems(const td::String* pItems, size_t nItems);

        /// @brief Appends all items from a SafeFullVector of strings.
        /// @param strings  Source vector of strings.
        void addItems(const cnt::SafeFullVector<td::String>& strings);

        /// @brief Appends all items from a PushBackVector of strings.
        /// @param strings  Source vector of strings.
        void addItems(const cnt::PushBackVector<td::String>& strings);

        /// @brief Appends all items from a std::vector of strings.
        /// @param strings  Source vector of strings.
        void addItems(const std::vector<td::String>& strings);

        /// @brief Removes the item at the specified position.
        /// @param pos  Zero-based index of the item to remove.
        void removeItem(int pos);

        /// @brief Sets the selected item by value.
        /// @param val          Variant value identifying the item to select.
        /// @param sendMessage  If true, fires the selection-changed notification.
        /// @return true if the value was set successfully, false otherwise.
        bool setValue(const td::Variant& val, bool sendMessage=true) override;

        /// @brief Retrieves the currently selected value.
        /// @param val        Receives the selected item value.
        /// @param checkType  If true, performs a type check before assignment.
        /// @return true if a value was retrieved, false if nothing is selected.
        bool getValue(td::Variant& val, bool checkType = false) const override;

        /// @brief Clears the current selection and removes all items.
        void clean();

        /// @brief Resets the combo box to the zero/empty state (same as clean()).
        void toZero(); //same as clean

        /// @brief Returns the currently registered selection-changed callback.
        /// @return Reference to the callback function.
        const std::function<void()>& getChangedSelectionHandler() const;

        /// @brief Registers a callback to be invoked when the selection changes.
        /// @param fnToCall  Callback function to register.
        void onChangedSelection(const std::function<void()>& fnToCall);
	};
}
