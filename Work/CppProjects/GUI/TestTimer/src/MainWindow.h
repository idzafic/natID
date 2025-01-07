//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "MainView.h"
#include <functional>

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    
    std::function<void()> _fnUpdateMenuAndTB;
    
    MainView _mainView;
protected:
    
    void updateMenuAndTB()
    {
        bool isRunning = _mainView.isRunning();
        gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 10);
        pMenuItem->setChecked(isRunning);
        
        gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 10);
        if (pTBItem)
        {
            if (isRunning)
            {
                pTBItem->setImage(&_imgStop);
                pTBItem->setLabel(tr("stop"));
                pTBItem->setTooltip(tr("stopTT"));
            }
            else
            {
                pTBItem->setImage(&_imgStart);
                pTBItem->setLabel(tr("start"));
                pTBItem->setTooltip(tr("startTT"));
            }
        }
    }
    
    bool shouldClose() override
    {
        if (_mainView.isRunning())
        {
            showAlert("Timer is running!", "First stop the timer and then close the application!");
            return false;
        }
        return true;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {

        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pActionItem = aiDesc.getActionItem();
        
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    _mainView.startStop();
                    return true;
                }
            }
        }
        return false;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 800, 480))
    , _imgStart(":start")
    , _imgStop(":stop")
    , _toolBar(&_imgStart)
    , _fnUpdateMenuAndTB(std::bind(&MainWindow::updateMenuAndTB, this))
    , _mainView(&_fnUpdateMenuAndTB)
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_mainView);
//        composeContent();
    }
    
    ~MainWindow()
    {
    }
    
};
