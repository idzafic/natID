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
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/View.h>
#include <cnt/SafeFullVector.h>
#include <gui/Button.h>

namespace gui
{

class Dialog;

class NATGUI_API DialogView : public View
{
    friend class Dialog;
protected:
    gui::Dialog* _pDialog;
private:
    bool onClick(gui::Button* pButton) override;
    bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const override;
public:
    DialogView(gui::Dialog* pDlg);
    ~DialogView();
};

} //namespace gui
