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

class View;
class BaseView;
class ViewHelper;

class NATGUI_API Layout : public Control
{
    friend class View;
    friend class BaseView;
    friend class ViewHelper;
protected:
    Layout();
    void addCtrlToView(Control* pCtrl, View* pView) const;
    
    virtual void initialMeasure(CellInfo&);
public:
    virtual void populateView(View* pView, const Cell& cell, const Size& szMargins);
    void hide(bool bHide);
    virtual bool isHidden() const override;
};

} //namespace gui
