// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BaseComboBox.h
 * @brief Abstract base class for combo-box controls.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DataCtrl.h"

namespace gui
{
    /// @brief Abstract base for combo-box controls that manages selection state and item layout.
    class NATGUI_API BaseComboBox : public DataCtrl
	{
    protected:
        /// @brief Measures the preferred size of the combo box during layout.
        /// @param ci Cell info structure to fill with size data.
        void measure(CellInfo&) override;

        /// @brief Re-measures the combo box after a layout change.
        /// @param ci Cell info structure to update with new size data.
        void reMeasure(CellInfo&) override;

        /// @brief Constructs a combo box with a specific data type.
        /// @param dt The data type the combo box stores.
        BaseComboBox(td::DataType dt);

        /// @brief Constructs a combo box with the default data type.
        BaseComboBox();

        BaseComboBox(const BaseComboBox&) = delete;
        BaseComboBox& operator =(const BaseComboBox&) = delete;

//        void removeItem(int pos);
    public:
        /// @brief Destructor.
        ~BaseComboBox();

        /// @brief Selects the item at the given index.
        /// @param index Zero-based index of the item to select.
        /// @param bSendMessage Whether to send a selection-changed notification.
        void selectIndex(int index, bool bSendMessage = true);

        /// @brief Returns the index of the currently selected item.
        /// @return Zero-based index of the selected item, or -1 if nothing is selected.
        int getSelectedIndex() const;

        /// @brief Returns the display text of the currently selected item.
        /// @return The text of the selected item.
        td::String getSelectedText() const;

        /// @brief Returns the total number of items in the combo box.
        /// @return Number of items.
        int getNoOfItems() const;

        /// @brief Calculates the width needed to display the longest item text.
        /// @return The required width in coordinate units.
        gui::CoordType getWidthToFitLongestItem() const;

        /// @brief Resizes the control to fit its content.
        void sizeToFit() override;

	};
} //namespace gui
