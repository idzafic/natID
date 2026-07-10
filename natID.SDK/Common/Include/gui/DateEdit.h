// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DateEdit.h
    @brief Line-edit control specialised for date input. */
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
class DateEditHelper;

/// @brief A single-line text control that accepts and validates date values.
class NATGUI_API DateEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    friend class DateEditHelper;
private:
    td::Date _val;          ///< Current date value held by the control.
    td::Date _minDate;      ///< Minimum allowed date.
    td::Date _maxDate;      ///< Maximum allowed date.
    std::function<void()> _onChangedValue; ///< Callback invoked when the date value changes.
private:
    /// @brief Applies a validated date to the internal state and updates the display.
    /// @param dateValue New date value to store.
    void updateValue(td::Date dateValue);
protected:
    /// @brief Performs any post-processing adjustment on the final input string.
    /// @param pStrIn Raw input string from the line edit.
    /// @return Adjusted string suitable for date parsing.
    const char* adjustFinalString(const char* pStrIn);

    /// @brief Clamps a date value to the configured [minDate, maxDate] range.
    /// @param date Date to clamp in-place.
    void clip(td::Date& date) const;

    static bool _putLedingZeros; ///< Global flag controlling whether leading zeros are added to single-digit day/month fields.

    DateEdit(const DateEdit&) = delete;
    DateEdit& operator =(const DateEdit&) = delete;
public:
    /// @brief Default constructor. Creates an empty date edit control.
    DateEdit();

    /// @brief Constructs a date edit with an initial tooltip.
    /// @param toolTip Tooltip text to display on hover.
    DateEdit(const td::String& toolTip);

    /// @brief Destructor.
    ~DateEdit();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::DateEdit.
    gui::ObjType getObjType() const override { return ObjType::DateEdit;}

    /// @brief Sets the control value from a variant.
    /// @param val Variant containing the new date value.
    /// @param sendMessage If true, fires the value-changed notification.
    /// @return True if the value was accepted.
    bool setValue(const td::Variant& val, bool sendMessage=true) override;

    /// @brief Retrieves the current value as a variant.
    /// @param val Variant to fill with the current date.
    /// @param checkType If true, type compatibility is verified before assignment.
    /// @return True if the value was successfully retrieved.
    bool getValue(td::Variant& val, bool checkType = false) const override;

    /// @brief Returns the current date value directly.
    /// @return Current date.
    td::Date getValue() const;

    /// @brief Sets the control value from a Date object.
    /// @param d New date value.
    /// @param sendMessage If true, fires the value-changed notification.
    void setValue(const td::Date& d, bool sendMessage = false);

    /// @brief Globally enables or disables leading-zero formatting for day and month.
    /// @param bPutLeadingZeros True to pad single-digit values with a leading zero.
    static void putLeadingZeros(bool bPutLeadingZeros);

    /// @brief Returns the currently registered value-changed callback.
    /// @return Const reference to the callback function.
    const std::function<void()>& getChangedValueHandler() const;

    /// @brief Registers a callback to be invoked whenever the date value changes.
    /// @param fnToCall Callback function to register.
    void onChangedValue(const std::function<void()>& fnToCall);
};

}
