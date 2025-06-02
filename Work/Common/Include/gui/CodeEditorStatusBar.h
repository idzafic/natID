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
#include <gui/StatusBar.h>
#include <gui/Label.h>
#include <gui/CodeEdit.h>
#include <initializer_list>

namespace gui
{

class NATCODE_API CodeEditorStatusBar : public gui::StatusBar
{
protected:
//    gui::View* _pSwitcher = nullptr;
    gui::Label _lblMessage;
    gui::Label _lblNoOfErrors;
    gui::Label _lblCursorPosition;
    
    gui::Label _cursorLineAndCol;
    gui::Label _noOfErrors;
    
public:
    CodeEditorStatusBar(gui::Control* pAdditionalControls = nullptr, size_t nAdditionalControls = 0, td::UINT2 spaceBetweenCtrls = 0);
    CodeEditorStatusBar(const std::initializer_list<gui::Control*>& additionalControls, td::UINT2 spaceBetweenCtrls=0);
    void setNoOfErrors(td::UINT4 nErrors);
    void setCursorPosition(const CodeEdit::Selection& sel);
};

}

