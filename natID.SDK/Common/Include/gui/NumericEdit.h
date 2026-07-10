// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file NumericEdit.h
    @brief Line-edit control restricted to numeric input with optional formatting and range constraints. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "LineEdit.h"
#include <tuple>
#include <td/Concepts.h>

namespace gui
{
/// @brief Forward declaration of the LineEdit helper template.
template <class TBASE>
class LineEditHelper;

/// @brief A LineEdit control restricted to numeric values with range, decimal-place, and format settings.
class NATGUI_API NumericEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    double _minValue;                           ///< Minimum allowed numeric value.
    double _maxValue;                           ///< Maximum allowed numeric value.
    td::FormatFloat _format = td::FormatFloat::Decimal; ///< Display format used for floating-point values.
protected:
    /// @brief Adjusts the raw input string during editing to conform to the numeric format.
    /// @param pStrIn Pointer to the current input string.
    /// @param buff Temporary buffer available for constructing an adjusted string.
    /// @param buffLen Size of the temporary buffer in bytes.
    /// @return A tuple of (isValid, adjustedCString) where adjustedCString may point into buff.
    std::tuple<bool, const char*> adjustWorkingString(const char* pStrIn, char* buff, size_t buffLen);

    /// @brief Post-processes the string when editing is committed, enforcing final format rules.
    /// @param pStrIn Pointer to the committed string.
    /// @return Pointer to the final adjusted string (may be pStrIn itself).
    const char* adjustFinalString(const char* pStrIn);

    /// @brief Default constructor for use by sub-classes; does not create the platform control.
    NumericEdit();

    NumericEdit(const NumericEdit&) = delete;
    NumericEdit& operator =(const NumericEdit&) = delete;
public:
    /// @brief Constructs a NumericEdit control with the specified data type and display options.
    /// @param dataType The td::DataType that determines which numeric type the control accepts.
    /// @param sendMsg Specifies whether value-changed messages are sent during editing.
    /// @param bShowThSep If true, thousands separators are displayed.
    /// @param toolTip Optional tooltip string shown on hover.
    /// @param nDec Number of decimal places; -1 means use a type-appropriate default.
    NumericEdit(td::DataType dataType, LineEdit::Messages sendMsg = LineEdit::Messages::DoNotSend, bool bShowThSep = true, const td::String& toolTip = "", int nDec = -1);

    /// @brief Destroys the NumericEdit control and releases associated resources.
    ~NumericEdit();

    /// @brief Retrieves the current numeric value into a variant.
    /// @param val Output variant that receives the current value.
    /// @param checkType If true, a type check is performed before writing val.
    /// @return True if the value was successfully retrieved.
    bool getValue(td::Variant& val, bool checkType = false) const override;

    /// @brief Returns the current numeric value as a variant.
    /// @return A td::Variant holding the current numeric value.
    td::Variant getValue() const;

    /// @brief Retrieves the current value into a typed numeric variable.
    /// @tparam T A numeric (non-bool) type that matches the control's data type.
    /// @param val Output variable that receives the current value.
    template <td::conc::NumericNotBool T>
    void getValue(T& val) const
    {
        td::Variant varVal = getValue();
        varVal.getValue(val);
    }

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::NumericEdit.
    gui::ObjType getObjType() const override { return ObjType::NumericEdit;}

    /// @brief Sets the number of digits displayed after the decimal point.
    /// @param nDecPlace The number of decimal places to show.
    void setNumberOfDigitsAfterDecimalPoint(td::BYTE nDecPlace);

    /// @brief Returns the number of digits currently displayed after the decimal point.
    /// @return The number of decimal places.
    td::BYTE getNumberOfDigitsAfterDecimalPoint() const;

    /// @brief Shows or hides the thousands separator in the displayed value.
    /// @param bShowThSep True to display thousands separators, false to hide them.
    void showThSep(bool bShowThSep);

    /// @brief Returns whether the thousands separator is currently visible.
    /// @return True if thousands separators are shown.
    bool isThSepVisible() const;

    /// @brief Sets the minimum allowed value for the control.
    /// @param minVal The minimum numeric value the user can enter.
    void setMinValue(double minVal);

    /// @brief Sets the maximum allowed value for the control.
    /// @param maxVal The maximum numeric value the user can enter.
    void setMaxValue(double maxVal);

    /// @brief Returns the minimum allowed value.
    /// @return The minimum numeric bound as a double.
    double getMinValue() const;

    /// @brief Returns the maximum allowed value.
    /// @return The maximum numeric bound as a double.
    double getMaxValue() const;

    /// @brief Sets the floating-point display format (e.g. Decimal, Scientific).
    /// @param fmt The td::FormatFloat value to apply.
    void setFormat(td::FormatFloat fmt);

    /// @brief Returns the current floating-point display format.
    /// @return The active td::FormatFloat setting.
    td::FormatFloat getFormat() const;
};

} //namespace gui
