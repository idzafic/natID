// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Label.h
 * @brief Non-interactive text label control for displaying static or dynamic text.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"


namespace gui
{

/// @brief A read-only text label control that can optionally be made resizable or selectable.
class NATGUI_API Label : public Control
{
protected:
    /// @brief Performs the initial measurement of the label for layout purposes.
    /// @param cellInfo Cell information to be filled with size data.
    void measure(CellInfo&) override;

    /// @brief Re-measures the label when its content changes.
    /// @param cellInfo Cell information to be updated.
    void reMeasure(CellInfo&) override;

    /// @brief Freezes the label to prevent further layout updates.
    /// @return True if the freeze operation succeeded.
    bool freeze() override;

    /// @brief Unfreezes the label to allow layout updates.
    void unFreeze() override;

    Label(const Label&) = delete;
    Label& operator =(const Label&) = delete;

public:
    /// @brief Constructs a Label with optional initial text.
    /// @param lblText Optional C-string text to display.
    Label(const char* lblText=nullptr);

    /// @brief Constructs a Label with a td::String text.
    /// @param lblText Text to display.
    Label(const td::String& lblText);

    /// @brief Constructs a Label with text and an explicit font.
    /// @param lblText Text to display.
    /// @param fntID Font identifier to use for rendering.
    Label(const td::String& lblText, gui::Font::ID fntID);

    /// @brief Returns the GUI object type identifier for this control.
    /// @return ObjType::Label.
    gui::ObjType getObjType() const override { return ObjType::Label;}

    /// @brief Sets the displayed text from a td::String.
    /// @param title New label text.
    void setTitle(const td::String& title) override;

    /// @brief Sets the displayed text from a C-string.
    /// @param title New label text.
    void setTitle(const char* title) override;

    /// @brief Makes the label resizable with text truncation when too narrow.
    /// @param minChars Minimum number of characters to display before truncating.
    /// @param ellips Truncation mode to apply.
    void setResizable(td::BYTE minChars=3, td::TextEllipsize ellips = td::TextEllipsize::End);

    /// @brief Allows the user to select and copy the label text.
    void setSelectable();

    /// @brief Sets the horizontal alignment of the label text (meaningful only when resizable).
    /// @param hAlign Horizontal alignment value.
    void setAlignment(td::HAlignment hAlign); // make sense only if it resizible

    /// @brief Assigns new text to the label from a td::String.
    /// @param lblText New text to display.
    void operator = (const td::String& lblText);

    /// @brief Assigns new text to the label from a C-string.
    /// @param pLblText New text to display.
    void operator = (const char* pLblText);
};

} //namespace gui
