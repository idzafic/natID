// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
class NATCODE_API CodeEditor : public View
{
protected:
    gui::LineNumbers _lineNumbers;
    gui::CodeEditorStatusBar _statusBar;
    gui::CodeEdit* _pCodeEdit;
    gui::HorizontalLayout _hLayout;
    gui::VerticalLayout _vLayout;
    td::UINT2 _namePos = 0;
    CodeEditor() = delete;
public:
    CodeEditor(CodeEdit& codeEdit); //no status bar
    
    //with status bar
    //View* pSwitcher is optional and will be placed on status bar to control additional views
    CodeEditor(CodeEdit* pCodeEdit, gui::Control* pAdditionalControls = nullptr, size_t nAdditionalControls = 0, td::UINT2 spaceBetweenCtrls=0);
    CodeEditor(CodeEdit* pCodeEdit, const std::initializer_list<gui::Control*>& additionalControls, td::UINT2 spaceBetweenCtrls=0);
    CodeEdit* getCodeEdit();
    const CodeEdit* getCodeEdit() const;
    void systemColorModeChanged(bool bDarkMode) override;
};

}
