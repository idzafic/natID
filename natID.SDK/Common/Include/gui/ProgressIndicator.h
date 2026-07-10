// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ProgressIndicator.h @brief Declares the ProgressIndicator control for displaying task progress. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DataCtrl.h"

namespace gui
{
/// @brief A control that visually indicates the progress of an ongoing operation as a value in [0, 1].
	class NATGUI_API ProgressIndicator : public DataCtrl
	{
    protected:
        /// @brief Measures the preferred cell size for this control.
        void measure(CellInfo&) override;

        /// @brief Re-measures the cell size after layout changes.
        void reMeasure(CellInfo&) override;
        ProgressIndicator(const ProgressIndicator&) = delete;
        ProgressIndicator& operator =(const ProgressIndicator&) = delete;
    public:
        /// @brief Constructs a ProgressIndicator with given orientation and optional value labels.
        /// @param orientation Horizontal or vertical orientation.
        /// @param bShowValues True to display numeric progress values alongside the indicator.
        ProgressIndicator(DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);

        /// @brief Constructs a ProgressIndicator with a tooltip.
        /// @param toolTip Tooltip text.
        /// @param orientation Horizontal or vertical orientation.
        /// @param bShowValues True to display numeric progress values.
        ProgressIndicator(const td::String& toolTip, DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);

        /// @brief Destructor.
        ~ProgressIndicator();

        /// @brief Returns the GUI object type identifier.
        /// @return ObjType::ProgressIndicator.
        gui::ObjType getObjType() const override { return ObjType::ProgressIndicator;}

        //val must be double in range [0,1]
        /// @brief Sets the progress value from a Variant (must be a double in [0, 1]).
        /// @param val Variant containing a double progress value.
        /// @param sendMessage Whether to send a change notification message.
        /// @return True if the value was set successfully.
        bool setValue(const td::Variant& val, bool sendMessage=true) override;

        /// @brief Retrieves the current progress value into a Variant.
        /// @param val Output Variant to receive the value.
        /// @param checkType Whether to validate the variant type.
        /// @return True if retrieval succeeded.
        bool getValue(td::Variant& val, bool checkType = false) const override;

        /// @brief Sets the progress value directly as a double in [0, 1].
        /// @param val Progress value between 0.0 and 1.0.
        void setValue(double val); //val must be double in range [0,1]

        /// @brief Returns the current progress value as a double.
        /// @return Progress value in [0, 1].
        double getValue() const;
	};
}
