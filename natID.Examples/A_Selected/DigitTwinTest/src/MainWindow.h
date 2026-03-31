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
#include "DialogSettings.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    
    std::function<void()> _fnUpdateMenuAndTB;
    
    MainView _mainView;
    int* _pInt; //mem leak test
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
            showAlert(tr("closeNOK"), tr("cloeErr"));
            return false;
        }
        return true;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10 & actionID == 10)
        {
            td::UINT4 settingsID = 728289;
            auto pDlg = getAttachedWindow(settingsID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings* pSettingsDlg = new DialogSettings(this, settingsID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->open();
            }
            return true;
        }
        
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
    : gui::Window(gui::Size(1200, 800))
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
        _pInt = new int[2]; //mem leak
    }
    
    ~MainWindow()
    {
    }
    
};
