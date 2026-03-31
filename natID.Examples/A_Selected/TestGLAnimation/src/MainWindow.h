//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
//#include "MenuBar.h"
#include "ViewGLAnimation.h"

class MainWindow : public gui::Window
{
private:
protected:
    //MenuBar _mainMenuBar;
    ViewGLAnimation _viewCubeMap;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        //_mainMenuBar.setAsMain(this);
//        _toolBar.forwardMessagesTo(this);
//        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
//    
//        setToolBar(_toolBar);
        mu::ILogger* pLog = mu::pApp()->getLogger();
        setCentralView(&_viewCubeMap);
    }
    
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
        {
            if (actionID == 10)
            {
                return true;
            }
        }
        return false;
    }
};
