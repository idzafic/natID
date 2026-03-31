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
#include <gui/Label.h>
#include "DrawableButtonIntern.h"
#include <gui/HorizontalLayout.h>

namespace gui
{

class NATGUI_API PropertyEditorTitle : public View
{
    
protected:
    Label _label;
    DrawableButtonIntern _apply;
    DrawableButtonIntern _restore;
    HorizontalLayout _layout;
    //td::BYTE _autoUpdate;
    
protected:
  
    void measure(CellInfo& cell) override;

    void reMeasure(CellInfo& cell) override;
    
    PropertyEditorTitle();
public:
    PropertyEditorTitle(const td::String& label, bool autoUpdate = false, td::HAlignment hAlign = td::HAlignment::Center);
    ~PropertyEditorTitle();
    
    //void setAlignment(td::HAlignment hAlign);
};

} //namespace gui
