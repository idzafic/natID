// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Property.h
    @brief Defines a single property entry used in property editors. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <td/Variant.h>
#include <td/String.h>
#include <tuple>

namespace gui
{

/// @brief Represents a single property with a label, value, type, and presentation options.
class NATGUI_API Property
{
public:
    /// @brief Indicates the role of the property in the editor.
    enum class Type : td::BYTE  {Group=0, ///< Property acts as a group header
                                  ReadOnly, ///< Property is read-only
                                  Normal    ///< Property is a normal editable entry
                                 };
    /// @brief Controls how the property value is presented in the editor.
    enum class Presentation : td::BYTE {Default=0, ///< Default presentation
                                         Slider,    ///< Presented as a slider control
                                         Combo      ///< Presented as a combo box
                                        };
protected:
    td::String _lbl;          ///< Display label of the property
    td::Variant _value;       ///< Current value of the property
    td::String _ctrlToolTip;  ///< Tooltip text for the property control
    union
    {
        double _minValue;              ///< Minimum allowed value (used when type is numeric)
        const td::String* _pStringList; ///< Pointer to string list (used when presentation is Combo)
    };
    union
    {
        double _maxValue;  ///< Maximum allowed value (used when type is numeric)
        size_t _nStrings;  ///< Number of strings in the string list
    };

    td::UINT4 _key;                        ///< Unique key identifying this property
    Type _type;                            ///< Type of this property (group, read-only, normal)
    Presentation _presentation;            ///< Visual presentation style of this property
    char _noDec: 4;                        ///< Number of decimal places to display
    td::BYTE _thSeparator : 1;             ///< Whether to display thousands separator
    td::BYTE _useCallbackOnValueChange : 1;///< Whether to invoke callback on value change
protected:

public:
    /// @brief Default constructor. Initializes an empty property.
    Property();
    /// @brief Destructor. Releases property resources.
    ~Property();
    /// @brief Sets the key, label, value, and optional tooltip for this property.
    /// @param key Unique identifier for this property.
    /// @param lbl Display label shown next to the control.
    /// @param val Initial value variant.
    /// @param ctrlToolTip Optional tooltip text for the control.
    void set(td::UINT4 key, const td::String& lbl, const td::Variant& val, const td::String& ctrlToolTip="");
    /// @brief Marks this property as read-only so its value cannot be edited.
    void setReadOnly();
    /// @brief Configures the number of decimal places and optional thousands separator display.
    /// @param noDec Number of decimal digits to show.
    /// @param showThSep If true, a thousands separator is displayed.
    void setDecimalPointsAndThSep(int noDec, bool showThSep=true);
    /// @brief Sets the visual presentation style of this property.
    /// @param presentation The desired presentation (Default, Slider, or Combo).
    void setPresentation(Presentation presentation);
    /// @brief Configures this property as a group header with the given label.
    /// @param lbl Label for the group header.
    void setGroup(const td::String& lbl);
    /// @brief Returns the unique key of this property.
    /// @return The property key.
    td::UINT4 getKey() const;
    /// @brief Returns a const reference to the current value.
    /// @return Const reference to the value variant.
    const td::Variant& getValue() const;
    /// @brief Returns a mutable reference to the current value.
    /// @return Mutable reference to the value variant.
    td::Variant& getValue();
    /// @brief Returns the type of this property.
    /// @return The property type.
    Property::Type getType() const;
    /// @brief Returns the presentation style of this property.
    /// @return The presentation style.
    Property::Presentation getPresentation() const;
    /// @brief Returns whether the thousands separator should be shown.
    /// @return True if the thousands separator is visible.
    bool showThSeparator() const;
    /// @brief Returns the configured number of decimal places.
    /// @return Number of decimal places.
    int getNoDec() const;
    /// @brief Returns the display label of this property.
    /// @return Const reference to the label string.
    const td::String& getLabel() const;
    /// @brief Returns the tooltip text for the property control.
    /// @return Const reference to the tooltip string.
    const td::String& getCtrlToolTip() const;
    /// @brief Sets the minimum allowed numeric value.
    /// @param minValue The minimum value.
    void setMinValue(double minValue);
    /// @brief Sets the maximum allowed numeric value.
    /// @param maxValue The maximum value.
    void setMaxValue(double maxValue);
    /// @brief Returns the minimum allowed numeric value.
    /// @return Minimum value.
    double getMinValue() const;
    /// @brief Returns the maximum allowed numeric value.
    /// @return Maximum value.
    double getMaxValue() const;

    /// @brief Assigns a list of strings for combo-box presentation.
    /// @param pStrings Pointer to the first string in the array.
    /// @param nStrings Number of strings in the array.
    void setStringList(const td::String* pStrings, size_t nStrings);
    /// @brief Returns the string list and its count as a tuple.
    /// @return Tuple containing pointer to first string and number of strings.
    std::tuple<const td::String*, size_t> getStringList() const;
};

} //namepace gui
