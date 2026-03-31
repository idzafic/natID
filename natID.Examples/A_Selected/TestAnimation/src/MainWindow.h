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
    const td::UINT4 _cSettingsDlgID = 76521; //any unique id among dialogs
protected:
    
    void onInitialAppearance() override //will be called only once
    {
        _mainView.setFocusToAnimation();
    }
    
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
    
    bool onClick(gui::Dialog* dlg, td::UINT4 dlgID) override
    {
        //get notification from dialog
        if (dlgID == _cSettingsDlgID)
        {
            if (dlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
            {
                DialogSettings* pSettingsDlg = (DialogSettings*) dlg;
                bool bShouldRestart = pSettingsDlg->shouldRestartFromInitialPosition();
                _mainView.setRestartFromInitialPosition(bShouldRestart);
            }
            return true;
        }
        return false;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
//        auto pActionItem = aiDesc.getActionItem();
        
        if (menuID == 10 & actionID == 10)
        {
            auto pDlg = getAttachedWindow(_cSettingsDlgID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings* pSettingsDlg = new DialogSettings(this, _cSettingsDlgID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->setMainTB(&_toolBar);
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
    }
    
    ~MainWindow()
    {
    }
    
};
