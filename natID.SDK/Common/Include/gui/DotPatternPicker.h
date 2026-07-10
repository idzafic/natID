// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DotPatternPicker.h
 * @brief Popover-button control for selecting a dot pattern value.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "PopoverButton.h"
#include <gui/DotPatternPopoverView.h>

namespace gui
{

/// @brief A popover button that lets the user pick a dot-pattern style.
class NATGUI_API DotPatternPicker : public PopoverButton
{
private:
    std::function<void()> _onChangedValue; ///< Callback invoked when the selected dot pattern changes.
protected:
    DotPatternPopoverView _patternPopoverView; ///< The popover canvas that displays the available dot patterns.

    DotPatternPicker(const DotPatternPicker&) = delete;
    DotPatternPicker& operator =(const DotPatternPicker&) = delete;
public:
    /// @brief Constructs a DotPatternPicker with default settings.
    DotPatternPicker();

    /// @brief Constructs a DotPatternPicker with the given tooltip text.
    /// @param toolTip Tooltip string shown on hover.
    DotPatternPicker(const td::String& toolTip);

    //gui::ObjType getObjType() const override { return ObjType::DotPatternPicker;}

    /// @brief Sets the picker value from a generic Variant.
    /// @param val  The variant holding a td::DotPattern value.
    /// @param sendMessage Whether to notify observers of the change.
    /// @return True if the value was accepted and applied.
    bool setValue(const td::Variant& val, bool sendMessage=true) override;

    /// @brief Retrieves the current value as a generic Variant.
    /// @param val      Output variant that receives the current dot pattern.
    /// @param checkType Whether to enforce type checking.
    /// @return True if the value was successfully read.
    bool getValue(td::Variant& val, bool checkType = false) const override;

    /// @brief Returns the currently selected dot pattern.
    /// @return The current td::DotPattern value.
    td::DotPattern getValue() const;

    /// @brief Sets the selected dot pattern directly.
    /// @param dotPattern The dot pattern to select.
    /// @param sendMessage Whether to notify observers of the change.
    void setValue(td::DotPattern dotPattern, bool sendMessage = false);

    /// @brief Returns the registered value-changed callback.
    /// @return Reference to the stored std::function callback.
    const std::function<void()>& getChangedValueHandler() const;

    /// @brief Registers a callback to be invoked when the selected value changes.
    /// @param fnToCall The function to call on value change.
    void onChangedValue(const std::function<void()>& fnToCall);
};

} //namespace gui
