//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "NavigatorView.h"


class NavigatorWindow : public gui::Window
{
private:
protected:
    NavigatorView _myView;
public:
    NavigatorWindow(gui::Window* pParent = nullptr, td::UINT4 wndID = 0)
    : gui::Window(gui::Geometry(10, 300, 1000, 600), pParent, wndID)
    {
        gui::Size sz;
        _myView.getSize(sz);
        setCentralView(&_myView, gui::Frame::FixSizes::No);
    }
    
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {
        //gui::Window::onClose();
        //_fnCloseSwitcher();
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        
        td::String msgText("Handling onActionItem NavigatorWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
};
