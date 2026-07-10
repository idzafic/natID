// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CodeEditor.h
    @brief A composite view combining line numbers, a CodeEdit control, and an optional status bar. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/CodeEdit.h>
#include <gui/CodeEditorStatusBar.h>
#include <gui/_aux/LineNumbers.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <unordered_set>
#include <initializer_list>

namespace gui
{

//this is a view that displays line numbers and code edit control
/// @brief A composite view that displays a line number gutter, a CodeEdit control, and optionally a status bar.
class NATCODE_API CodeEditor : public View
{
protected:
    gui::LineNumbers _lineNumbers; ///< Line number display gutter on the left side
    gui::CodeEditorStatusBar _statusBar; ///< Status bar shown below the editor
    gui::CodeEdit* _pCodeEdit; ///< Pointer to the CodeEdit control managed by this view
    gui::HorizontalLayout _hLayout; ///< Horizontal layout combining line numbers and code edit
    gui::VerticalLayout _vLayout; ///< Vertical layout combining the horizontal layout and the status bar
    td::UINT2 _namePos = 0; ///< Position index used for naming/symbol registry resolution

    CodeEditor() = delete;
    CodeEditor(const CodeEditor&) = delete;
    CodeEditor& operator =(const CodeEditor&) = delete;
public:
    /// @brief Constructs a CodeEditor without a status bar.
    /// @param codeEdit Reference to the CodeEdit control to embed.
    CodeEditor(CodeEdit& codeEdit); //no status bar

    //with status bar
    //View* pSwitcher is optional and will be placed on status bar to control additional views
    /// @brief Constructs a CodeEditor with an optional status bar and additional toolbar controls.
    /// @param pCodeEdit Pointer to the CodeEdit control to embed.
    /// @param pAdditionalControls Optional array of additional controls to place on the status bar.
    /// @param nAdditionalControls Number of additional controls.
    /// @param spaceBetweenCtrls Spacing in pixels between additional controls.
    CodeEditor(CodeEdit* pCodeEdit, gui::Control* pAdditionalControls = nullptr, size_t nAdditionalControls = 0, td::UINT2 spaceBetweenCtrls=0);
    /// @brief Constructs a CodeEditor with an initializer list of additional status bar controls.
    /// @param pCodeEdit Pointer to the CodeEdit control to embed.
    /// @param additionalControls List of additional controls to show on the status bar.
    /// @param spaceBetweenCtrls Spacing in pixels between additional controls.
    CodeEditor(CodeEdit* pCodeEdit, const std::initializer_list<gui::Control*>& additionalControls, td::UINT2 spaceBetweenCtrls=0);
    /// @brief Returns a pointer to the embedded CodeEdit control.
    /// @return Pointer to the CodeEdit.
    CodeEdit* getCodeEdit();
    /// @brief Returns a const pointer to the embedded CodeEdit control.
    /// @return Const pointer to the CodeEdit.
    const CodeEdit* getCodeEdit() const;
    /// @brief Notifies the editor that the system color mode has changed.
    /// @param bDarkMode True if the system switched to dark mode.
    void systemColorModeChanged(bool bDarkMode) override;
    /// @brief Returns the line numbers display component.
    /// @return Const pointer to the ILineNumbers interface.
    const gui::ILineNumbers* getLineNumbers() const;
};

}
