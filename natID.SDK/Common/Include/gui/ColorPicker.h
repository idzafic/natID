// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ColorPicker.h
    @brief A popover button control that allows the user to select a color. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "PopoverButton.h"
#include <gui/ColorPopoverView.h>
//#include <td/Color.h>


namespace gui
{

class ColorPopoverView;

/// @brief A button-style control that opens a color selection popover, allowing the user to pick a ColorID.
class NATGUI_API ColorPicker : public PopoverButton
{
private:
    std::function<void()> _onChangedValue; ///< Callback function invoked when the selected color changes
protected:
    gui::ColorPopoverView _colorPopoverView; ///< The popover view used to display color choices

    ColorPicker(const ColorPicker&) = delete;
    ColorPicker& operator =(const ColorPicker&) = delete;
public:
    /// @brief Default constructor.
    ColorPicker();
    /// @brief Constructs a ColorPicker with a tooltip.
    /// @param toolTip Tooltip text to display on hover.
    ColorPicker(const td::String& toolTip);
    //~ColorPicker();
    //virtual gui::ObjType getObjType() const override{ return ObjType::ColorPicker;}
    /// @brief Sets the selected color from a Variant.
    /// @param val The variant containing the ColorID value.
    /// @param sendMessage Whether to trigger change notifications.
    /// @return True if the value was applied successfully.
    bool setValue(const td::Variant& val, bool sendMessage=true) override;
    /// @brief Retrieves the currently selected color as a Variant.
    /// @param val Output variant to receive the ColorID.
    /// @param checkType Whether to enforce type compatibility.
    /// @return True if the value was retrieved successfully.
    bool getValue(td::Variant& val, bool checkType = false) const override;
    /// @brief Returns the currently selected ColorID.
    /// @return The selected color identifier.
    td::ColorID getValue() const;
    /// @brief Sets the selected color by ColorID.
    /// @param colorID The color identifier to select.
    /// @param sendMessage Whether to trigger change notifications.
    void setValue(td::ColorID colorID, bool sendMessage = false);

    /// @brief Returns the currently registered value-changed callback.
    /// @return Const reference to the callback function.
    const std::function<void()>& getChangedValueHandler() const;
    /// @brief Registers a callback function invoked when the selected color changes.
    /// @param fnToCall The function to invoke on color change.
    void onChangedValue(const std::function<void()>& fnToCall);
    void onChangedSelection(const std::function<void()>& fnToCall) = delete;
};
}
