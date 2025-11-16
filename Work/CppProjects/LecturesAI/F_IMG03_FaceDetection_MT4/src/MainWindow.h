//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include <functional>
#include "ToolBar.h"
#include "StatusBar.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    StatusBar _statusBar;
    ViewImageDetector _mainView;
protected:
    
    void onInitialAppearance() override //will be called only once
    {
        _mainView.setFocus();
    }
    
    void updateMenuAndTB()
    {
        bool isDetecting = _mainView.isDetecting();
        gui::MenuItem* pMenuItem = _mainMenuBar.getItem(cMenuDetect, 0, 0, cStartStopActionItem);
        pMenuItem->setChecked(isDetecting);
        
        //update start stop
        gui::ToolBarItem* pTBItem = _toolBar.getItem(cMenuDetect, 0, 0, cStartStopActionItem);
        if (pTBItem)
        {
            if (isDetecting)
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
        
        //update reset
        pTBItem = _toolBar.getItem(cMenuDetect, 0, 0, cResetActionItem);
        if (pTBItem)
        {
            if (isDetecting)
                pTBItem->disable();
            else
                pTBItem->enable();
        }
        
        _toolBar.updateSettingsPopoverTBI(!isDetecting);
        auto nFaces = _mainView.getNumberOfDetectedFaces();
        _statusBar.setNoOfFaces(nFaces);
        
        double timeInSec = _mainView.getTimeInSec();
        if (timeInSec > 0)
        {
            _statusBar.setMessage(tr("Completed"));
            _statusBar.setTime(timeInSec);
        }
        else
        {
//            _statusBar.setMessage(tr("Stopped/Uncomplete"));
            _statusBar.setTime(0);
        }
    }
    
    bool shouldClose() override
    {
        if (_mainView.isDetecting())
        {
            showAlert(tr("closeNOK"), tr("cloeErr"));
            return false;
        }
        return true;
    }

    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
//        auto pActionItem = aiDesc.getActionItem();
        switch (menuID)
        {
            case cMenuApp:  //nothing so far
                break;
            case cMenuDetect:
                switch(actionID)
                {
                    case cStartStopActionItem:
                    {
                        bool bStopping = false;
                        if (_mainView.isDetecting())
                        {
                            _statusBar.setMessage(tr("Stopping"));
                            bStopping = true;
                        }
                        else
                            _statusBar.setMessage(tr("Running"));
                        _mainView.startStop();
                        _statusBar.setNoOfFaces(0);
                        if (bStopping)
                            _statusBar.setMessage(tr("Stopped"));
                    }
                        return true;
                    case cResetActionItem:
                        _mainView.reset();
                        _statusBar.setMessage(tr("NewProblem"));
                        _statusBar.setProgressIndicator(0.0f);
                        return true;
                    default:
                        assert(false);
                }
                break;
            default:
                assert(false);
        }
        return false;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(600, 510))
    , _imgStart(":start")
    , _imgStop(":stop")
    , _mainView(std::bind(&MainWindow::updateMenuAndTB, this))
    , _toolBar(&_mainView, &_imgStart)
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setStatusBar(_statusBar);
        auto progressIndicator = _statusBar.getProgressIndicator();
        _mainView.setProgressIndicator(progressIndicator);
        setCentralView(&_mainView);
    }
    
};
