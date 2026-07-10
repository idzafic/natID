// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DBComboBox.h
    @brief Declares the DBComboBox control, a combo box that associates each display item with a typed key value. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "BaseComboBox.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	/// @brief A combo box that maps each visible string item to an underlying typed key (database-style).
	///
	/// Unlike ComboBox, DBComboBox stores a td::Variant key alongside each display label,
	/// so getValue/setValue operate on the keys rather than on positional indices.
	class NATGUI_API DBComboBox : public BaseComboBox
	{
        cnt::PushBackVector<td::Variant> _keys; ///< Key values parallel to the display strings.
        std::function<void()> _onChangedSelection; ///< Callback invoked when the selected item changes.
    protected:
        DBComboBox(const DBComboBox&) = delete;
        DBComboBox& operator =(const DBComboBox&) = delete;

        /// @brief Default constructor; creates an empty combo box (for internal use).
        DBComboBox();

        /// @brief Constructs a combo box with a tooltip (for internal use).
        /// @param toolTip  Tooltip text displayed on hover.
        DBComboBox(const td::String& toolTip);

        /// @brief Returns the position of the given key in the internal key vector.
        /// @param key  Key value to search for.
        /// @return Zero-based index of the key, or -1 if not found.
        int getKeyPos(const td::Variant& key) const;
    public:
        /// @brief Constructs a DBComboBox that stores keys of the specified data type.
        /// @param dt  Data type of the key values (e.g. td::int4, td::string8, etc.).
        DBComboBox(td::DataType dt);

        /// @brief Returns the GUI object type identifier for this control.
        /// @return ObjType::DBComboBox.
        gui::ObjType getObjType() const override{ return ObjType::DBComboBox;}

        /// @brief Appends a display item with its associated key value.
        /// @param item  Display string for the new item.
        /// @param key   Key value associated with the item.
        /// @return true if the item was added successfully.
        bool addItem(const td::String& item, const td::Variant& key);

        /// @brief Appends a C-string display item with its associated key value.
        /// @param item  Display string for the new item (C-string).
        /// @param key   Key value associated with the item.
        /// @return true if the item was added successfully.
        bool addItem(const char* item, const td::Variant& key);

        /// @brief Removes the item at the specified position.
        /// @param pos  Zero-based index of the item to remove.
        void removeItem(int pos);

        /// @brief Selects the item whose key matches the supplied value.
        /// @param key          Key value identifying the item to select.
        /// @param sendMessage  If true, fires the selection-changed notification.
        /// @return true if an item with the given key was found and selected.
        bool setValue(const td::Variant& key, bool sendMessage=true) override;

        /// @brief Retrieves the key value of the currently selected item.
        /// @param key        Receives the key of the selected item.
        /// @param checkType  If true, performs a type check before assignment.
        /// @return true if a valid item is selected and its key was retrieved.
        bool getValue(td::Variant& key, bool checkType = false) const override;

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
