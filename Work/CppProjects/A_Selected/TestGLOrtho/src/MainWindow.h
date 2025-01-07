//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "StatusBar.h"
#include "ViewGLOrtho.h"
#include "DialogSettings.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    StatusBar _statusBar;
    ViewGLOrtho _viewOrtho;
    const td::UINT4 _cSettingsDlgID = 17; //any unique id among dialogs
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    , _viewOrtho(&_statusBar)
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
    
        setToolBar(_toolBar);
        setStatusBar(_statusBar);
        setCentralView(&_viewOrtho);
    }
    
    ~MainWindow()
    {
    }
    
protected:
    
    void onInitialAppearance() override
    {
        auto appProperties = getAppProperties();
        bool showStatusBar = appProperties->getValue("showStatusBar", true);
        if (!showStatusBar)
        {
            _statusBar.show(false);
        }
        _statusBar.resetValues(); //clean initial patterns for size detection with initial values
        _viewOrtho.setFocus();
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
        {
            if (actionID == 10)
            {
                auto pDlg = getAttachedWindow(_cSettingsDlgID);
                if (pDlg)
                    pDlg->setFocus();
                else
                {
                    gui::Window* pWnd = getParentWindow();
                    auto statusBar = pWnd->getStatusBar();
                    DialogSettings* pSettingsDlg = new DialogSettings(this, _cSettingsDlgID);
                    pSettingsDlg->keepOnTopOfParent();
                    
                    auto mainTB = pWnd->getToolBar();
                    pSettingsDlg->setMainTB(mainTB);
                    
                    pSettingsDlg->open();
                }
                return true;
            }
            else if (actionID == 20)
            {
                showInfo(tr("UsageInfoHeader"), tr("UsageInfo"));                
                return true;
            }
            else if (actionID == 30)
            {
                //export to image
                _viewOrtho.exportFrame();
                return true;
            }
        }
        return false;
    }
    
};
