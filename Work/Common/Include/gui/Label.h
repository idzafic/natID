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
#include "Control.h"


namespace gui
{

class NATGUI_API Label : public Control
{
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    bool freeze() override;
    void unFreeze() override;
public:
    Label(const char* lblText=nullptr);
    Label(const td::String& lblText);
    Label(const td::String& lblText, gui::Font::ID fntID);
    gui::ObjType getObjType() const override { return ObjType::Label;}
    void setTitle(const td::String& title) override;
    void setTitle(const char* title) override;
    void setResizable(td::BYTE minChars=3, td::TextEllipsize ellips = td::TextEllipsize::End);
    void setSelectable();
    void operator = (const td::String& lblText);
    void operator = (const char* pLblText);
};

} //namespace gui
