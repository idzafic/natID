// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LinePatternPicker.h
    @brief Button control that lets the user pick a line pattern from a popover. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "PopoverButton.h"
#include <gui/LinePatternPopoverView.h>

namespace gui
{

/// @brief A popover-based button that allows the user to select a line pattern.
class NATGUI_API LinePatternPicker : public PopoverButton
{
private:
    std::function<void()> _onChangedValue; ///< Callback invoked when the selected line pattern changes.
protected:
    LinePatternPopoverView _patternPopoverView; ///< The popover canvas that displays available line patterns.

    LinePatternPicker(const LinePatternPicker&) = delete;
    LinePatternPicker& operator =(const LinePatternPicker&) = delete;
public:
    /// @brief Constructs a default LinePatternPicker control.
    LinePatternPicker();
    //LinePatternPicker(const td::String& toolTip);
    /// @brief Destroys the LinePatternPicker and releases associated resources.
    ~LinePatternPicker();
    //gui::ObjType getObjType() const override { return ObjType::LinePatternPicker;}

    /// @brief Sets the current value from a variant and optionally sends a change message.
    /// @param val The variant containing the line pattern value to set.
    /// @param sendMessage If true, a value-changed message is dispatched after setting.
    /// @return True if the value was successfully applied, false otherwise.
    bool setValue(const td::Variant& val, bool sendMessage=true) override;

    /// @brief Retrieves the current value into a variant.
    /// @param val Output variant that receives the current line pattern.
    /// @param checkType If true, performs a type check before populating val.
    /// @return True if the value was successfully retrieved, false otherwise.
    bool getValue(td::Variant& val, bool checkType = false) const override;

    /// @brief Returns the currently selected line pattern.
    /// @return The current td::LinePattern selection.
    td::LinePattern getValue() const;

    /// @brief Sets the line pattern directly.
    /// @param lnPattern The line pattern to apply.
    /// @param sendMessage If true, a value-changed message is dispatched after setting.
    void setValue(td::LinePattern lnPattern, bool sendMessage = false);

    /// @brief Returns a reference to the registered value-changed callback.
    /// @return Const reference to the std::function called on value change.
    const std::function<void()>& getChangedValueHandler() const;

    /// @brief Registers a callback to be invoked when the selected line pattern changes.
    /// @param fnToCall The function to call when the value changes.
    void onChangedValue(const std::function<void()>& fnToCall);
};

} //namespace gui
