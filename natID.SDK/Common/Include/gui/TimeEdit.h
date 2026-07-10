// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TimeEdit.h
    @brief Line-edit control specialised for time-of-day input. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "LineEdit.h"
#include <td/DateTime.h>

namespace gui
{
template <class TBASE>
class LineEditHelper;
class TimeEditHelper;

/// @brief A single-line text control that accepts and validates time-of-day values.
class NATGUI_API TimeEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    friend class TimeEditHelper;
private:
    td::Time _val;          ///< Current time value held by the control.
    td::Time _minTime;      ///< Minimum allowed time.
    td::Time _maxTime;      ///< Maximum allowed time.
    std::function<void()> _onChangedValue; ///< Callback invoked when the time value changes.
private:
    /// @brief Applies a validated time value to the internal state and updates the display.
    /// @param val New time value to store.
    void updateValue(td::Time val);
protected:
    /// @brief Performs any post-processing adjustment on the final input string.
    /// @param pStrIn Raw input string from the line edit.
    /// @return Adjusted string suitable for time parsing.
    const char* adjustFinalString(const char* pStrIn);

    /// @brief Clamps a time value to the configured [minTime, maxTime] range.
    /// @param time Time value to clamp in-place.
    void clip(td::Time& time) const;

    static bool _putLedingZeros; ///< Global flag controlling whether leading zeros are added to single-digit hour/minute/second fields.

    TimeEdit(const TimeEdit&) = delete;
    TimeEdit& operator =(const TimeEdit&) = delete;
public:
    /// @brief Default constructor. Creates an empty time edit control.
    TimeEdit();

    /// @brief Constructs a time edit with an initial tooltip.
    /// @param toolTip Tooltip text to display on hover.
    TimeEdit(const td::String& toolTip);

    /// @brief Destructor.
    ~TimeEdit();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::TimeEdit.
    gui::ObjType getObjType() const override { return ObjType::TimeEdit;}

    /// @brief Sets the control value from a variant.
    /// @param val Variant containing the new time value.
    /// @param sendMessage If true, fires the value-changed notification.
    /// @return True if the value was accepted.
    bool setValue(const td::Variant& val, bool sendMessage=true) override;

    /// @brief Retrieves the current value as a variant.
    /// @param val Variant to fill with the current time.
    /// @param checkType If true, type compatibility is verified before assignment.
    /// @return True if the value was successfully retrieved.
    bool getValue(td::Variant& val, bool checkType = false) const override;

    /// @brief Returns the current time value directly.
    /// @return Current time.
    td::Time getValue() const;

    /// @brief Sets the control value from a Time object.
    /// @param t New time value.
    /// @param sendMessage If true, fires the value-changed notification.
    void setValue(const td::Time& t, bool sendMessage = false);

    /// @brief Globally enables or disables leading-zero formatting for hour, minute, and second fields.
    /// @param bPutLeadingZeros True to pad single-digit fields with a leading zero.
    static void putLeadingZeros(bool bPutLeadingZeros);

    /// @brief Returns the currently registered value-changed callback.
    /// @return Const reference to the callback function.
    const std::function<void()>& getChangedValueHandler() const;

    /// @brief Registers a callback to be invoked whenever the time value changes.
    /// @param fnToCall Callback function to register.
    void onChangedValue(const std::function<void()>& fnToCall);
};

}
