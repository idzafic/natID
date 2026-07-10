// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CodeEditorStatusBar.h
    @brief A status bar for code editors that shows error counts, cursor position, and optional additional controls. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StatusBar.h>
#include <gui/Label.h>
#include <gui/CodeEdit.h>
#include <initializer_list>

namespace gui
{

/// @brief A status bar component for code editors displaying error count, cursor position, and custom controls.
class NATCODE_API CodeEditorStatusBar : public gui::StatusBar
{
protected:
//    gui::View* _pSwitcher = nullptr;
    gui::Label _lblMessage; ///< Label for displaying general status messages
    gui::Label _lblNoOfErrors; ///< Static label caption for the error count
    gui::Label _lblCursorPosition; ///< Static label caption for the cursor position

    gui::Label _cursorLineAndCol; ///< Dynamic label showing current cursor line and column
    gui::Label _noOfErrors; ///< Dynamic label showing the current number of errors

    CodeEditorStatusBar(const CodeEditorStatusBar&) = delete;
    CodeEditorStatusBar& operator =(const CodeEditorStatusBar&) = delete;
public:
    /// @brief Constructs a CodeEditorStatusBar with optional additional controls.
    /// @param pAdditionalControls Optional array of additional controls to place in the status bar.
    /// @param nAdditionalControls Number of additional controls.
    /// @param spaceBetweenCtrls Spacing in pixels between controls.
    CodeEditorStatusBar(gui::Control* pAdditionalControls = nullptr, size_t nAdditionalControls = 0, td::UINT2 spaceBetweenCtrls = 0);
    /// @brief Constructs a CodeEditorStatusBar with an initializer list of additional controls.
    /// @param additionalControls List of additional controls to include.
    /// @param spaceBetweenCtrls Spacing in pixels between controls.
    CodeEditorStatusBar(const std::initializer_list<gui::Control*>& additionalControls, td::UINT2 spaceBetweenCtrls=0);
    /// @brief Updates the displayed error count.
    /// @param nErrors The number of errors to show.
    void setNoOfErrors(td::UINT4 nErrors);
    /// @brief Updates the cursor position display from a CodeEdit selection.
    /// @param sel The current editor selection containing line and column info.
    void setCursorPosition(const CodeEdit::Selection& sel);
};

}
