//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewGLX.h"
#include <mu/DebugConsoleLog.h>

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewGLX _viewGLX;
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10) // Application menu
        {
            if (actionID == 10) // Properties
            {
                return true;
            }
        }
        else if (menuID == 20) // Export
        {
            if (actionID == 20) // Export frame
            {
                return true;
            }
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == _toolBar.getSpeedSlider())
        {
            MU_DEBUG_LOG_INFO("Slideer changed!");
            float val = (float) pSlider->getValue();
            _viewGLX.updateSpeed(val);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pChBox) override
    {
        if (pChBox == _toolBar.getRotateSwitch())
        {
            bool bAnimation = pChBox->isChecked();
            _viewGLX.animate(bAnimation);
            return true;
        }
        return false;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(1200, 600))
    {
        setDbgName("GLX Shadow Demo Main Window");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        setToolBar(_toolBar);
        setCentralView(&_viewGLX);
    }
};
