//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewChessBoard.h"     //This is the main view
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
    
    ViewChessBoard _mainView;
    const td::UINT4 _cSettingsDlgID = 1000; //any unique id among dialogs
    
protected:
    
    void onInitialAppearance() override //will be called only once
    {
        _mainView.setFocus();
    }
    
    void updateMenuAndTB()
    {
        bool isRunning = _mainView.isRunning();
        gui::MenuItem* pMenuItem = _mainMenuBar.getItem(cMenuAnimation, 0, 0, 10);
        pMenuItem->setChecked(isRunning);
        
        gui::ToolBarItem* pTBItem = _toolBar.getItem(cMenuAnimation, 0, 0, 10);
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
                
                bool darkQueen = pSettingsDlg->useDarkQueen();
                _mainView.useDarkQueen(darkQueen);
                
                bool bPlaySound = pSettingsDlg->playSound();
                _mainView.playSound(bPlaySound);
//                _mainView.refresh();
            }
            return true;
        }
        return false;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
//        auto pActionItem = aiDesc.getActionItem();
        switch (menuID)
        {
            case cMenuApp:
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
                break;
            case cMenuAnimation:
                if (actionID == 10)
                {
                    _mainView.startStop();
                    return true;
                }
                break;
            default:
                assert(false);
        }
        return false;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(800, 800))
    , _imgStart(":start")
    , _imgStop(":stop")
    , _toolBar(&_imgStart)
    , _fnUpdateMenuAndTB(std::bind(&MainWindow::updateMenuAndTB, this))
    , _mainView(_fnUpdateMenuAndTB)
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_mainView);
    }
    
};
