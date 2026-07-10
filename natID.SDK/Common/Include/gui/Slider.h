// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Slider.h
 * @brief Provides the Slider control for selecting a value within a range.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DataCtrl.h"

namespace gui
{
	/// @brief A slider control that allows the user to select a numeric value within a defined range.
	class NATGUI_API Slider : public DataCtrl
	{
    private:
        std::function<void()> _onChangedValue; ///< Callback invoked when the slider value changes.
    protected:
        /// @brief Measures the preferred size of the slider.
        /// @param cell Cell info to be filled with size data.
        void measure(CellInfo&) override;

        /// @brief Re-measures the slider after layout changes.
        /// @param cell Cell info to be updated.
        void reMeasure(CellInfo&) override;

        Slider(const Slider&) = delete;
        Slider& operator =(const Slider&) = delete;
    public:
        /// @brief Constructs a Slider with orientation and optional value display.
        /// @param orientation Horizontal or vertical orientation.
        /// @param bShowValues Whether to display tick values.
        Slider(DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);

        /// @brief Constructs a Slider with a tooltip, orientation, and optional value display.
        /// @param toolTip Tooltip text.
        /// @param orientation Horizontal or vertical orientation.
        /// @param bShowValues Whether to display tick values.
        Slider(const td::String& toolTip, DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);

        /// @brief Destructor.
        ~Slider();

        /// @brief Returns the GUI object type identifier.
        /// @return ObjType::Slider.
        gui::ObjType getObjType() const override { return ObjType::Slider;}

        /// @brief Sets the slider value from a variant.
        /// @param val New value as a Variant.
        /// @param sendMessage Whether to send a change notification.
        /// @return True if the value was set successfully.
        bool setValue(const td::Variant& val, bool sendMessage=true) override;

        /// @brief Gets the current slider value into a variant.
        /// @param val Output variant to receive the value.
        /// @param checkType Whether to enforce type checking.
        /// @return True if the value was retrieved successfully.
        bool getValue(td::Variant& val, bool checkType = false) const override;

        /// @brief Sets the slider value as a double.
        /// @param val New numeric value.
        /// @param sendMessage Whether to send a change notification.
        void setValue(double val, bool sendMessage=true);

        /// @brief Returns the current slider value as a double.
        /// @return Current value.
        double getValue() const;

        /// @brief Sets the valid range and tick count for the slider.
        /// @param minVal Minimum value.
        /// @param maxVal Maximum value.
        /// @param nTicks Number of tick marks (-1 for automatic).
        void setRange(double minVal, double maxVal, int nTicks=-1);

        /// @brief Returns the registered value-changed callback.
        /// @return Reference to the callback function.
        const std::function<void()>& getChangedValueHandler() const;

        /// @brief Registers a callback to invoke when the slider value changes.
        /// @param fnToCall Callback function.
        void onChangedValue(const std::function<void()>& fnToCall);
	};
}
