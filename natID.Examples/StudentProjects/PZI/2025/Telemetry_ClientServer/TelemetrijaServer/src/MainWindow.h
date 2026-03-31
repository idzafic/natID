#pragma once
#include <gui/Window.h>
#include "MainView.h" 
#include "MenuBar.h"
#include "ToolBar.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    MainView _mainView;
protected:
    
    bool shouldClose() override
    {
        if (_mainView.isRunning())
        {
            showAlert(tr("closeNOK"), tr("closeErr"));
            return false;
        }
        return true;
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
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        //        auto pActionItem = aiDesc.getActionItem();
        if (menuID == 10)
        {
            //App menu
            if (actionID == 10)
            {
                //show settings
                _mainView.adjustSettings();
                return true;
            }
        }
        else if (menuID == 20)
        {
            //Server menu
            if (actionID == 10)
            {
                //start stop
                _mainView.startStop();
                
                updateMenuAndTB();
                return true;
            }
        }
        return false;
    }
public:
    MainWindow()
    : gui::Window(gui::Size(500, 380))
    , _imgStart(":start")
    , _imgStop(":stop")
    , _toolBar(&_imgStart)
    {
        setTitle(tr("Server"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_mainView);
    }
};
