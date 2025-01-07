//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Controls.h>
#include <gui/Navigator.h>
#include "SwitcherView.h"

class NavigatorView: public gui::View
{
private:
protected:
    gui::HorizontalLayout _hl;
    gui::Navigator _navigator;
    gui::Image _navImg1;
    gui::Image _navImg2;
    gui::Image _navImg3;
    gui::Symbol _fourWindingTransf;
    gui::Symbol _inductor;
    gui::Symbol _testPie;
    SwitcherView _switcherView;
public:
    NavigatorView()
    : _hl(2)
    , _navigator(6, true)
    , _navImg1(":Home")
    , _navImg2(":TransCost")
    , _navImg3(":ETF")
    , _fourWindingTransf(":Xfmr4")
    , _inductor(":inductor")
    , _testPie(":TestPie")
    {
        setMargins(0,0,0,0);
        _navigator.setItem(0, &_navImg1, "Demo sa kontrolama");
        _navigator.setItem(1, &_navImg2, "Canvas Demo");
        _navigator.setItem(2, &_navImg3, "TableEdit Demo");
        _navigator.setItem(3, &_fourWindingTransf, "Horizonal Layout");
        _navigator.setItem(4, &_inductor, "Vertiacal Layout", 90);
        _navigator.setItem(5, &_testPie, "Standard Tab");
        _hl.append(_navigator);
        
        //_imgView.setImage(_navImg1);
        _hl.append(_switcherView);
        setLayout(&_hl);
    }
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
//        auto pAI = aiDesc.getActionItem();
        
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return false;
    }
        
    bool onChangedSelection(gui::Navigator* pNav) override
    {
        td::UINT2 currSelection = pNav->getCurrentSelection();
        _switcherView.showView((int) currSelection);
        //mu::dbgLog("Navigator selChanged. New Sel =%d", currSelection);
        return true;
    }
};
