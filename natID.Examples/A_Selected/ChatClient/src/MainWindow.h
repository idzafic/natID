#pragma once
#include <gui/Window.h>
#include "MainView.h" 
#include "MenuBar.h"
#include "ToolBar.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgInProcess;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    
    MainView _mainView;
protected:
    bool shouldClose() override
    {
//        if (_mainView.isConnected())
//        {
//            showAlert(tr("closeNOK"), tr("cloeErr"));
//            return false;
//        }
        return true;
    }
    
    void updateMenuAndTB()
    {
        MainView::Connection status = _mainView.getStatus();
        gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 10);
        pMenuItem->setChecked(status == MainView::Connection::Connected);
        
        gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 10);
        if (pTBItem)
        {
            if (status == MainView::Connection::Connected)
            {
                pTBItem->setImage(&_imgStop);
                pTBItem->setLabel(tr("disconnect"));
                pTBItem->setTooltip(tr("disconnectTT"));
            }
            else if (status == MainView::Connection::Disconnected)
            {
                pTBItem->setImage(&_imgStart);
                pTBItem->setLabel(tr("connect"));
                pTBItem->setTooltip(tr("connectTT"));
            }
            else //InProcess
            {
                pTBItem->setImage(&_imgInProcess);
                pTBItem->setLabel(tr("inProcess"));
                pTBItem->setTooltip(tr("inProcessTT"));
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
                _mainView.connectDisconnect();
                return true;
            }
        }
        return false;
    }
    
    bool onClientSocket(const gui::no::Notification& notification) override
    {
        auto clientMessage = notification.getClientMessage();
        switch (clientMessage)
        {
            case gui::no::Notification::ClientMessage::ConnectionFailed:
            case gui::no::Notification::ClientMessage::ConnectionSuccess:
            case gui::no::Notification::ClientMessage::Disconnected:
                updateMenuAndTB();
                break;
            default:
                break;
        }
        return true;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(600, 480))
    , _imgStart(":start")
    , _imgInProcess(":inProcess")
    , _imgStop(":stop")
    , _toolBar(&_imgStart)
    {
        _mainView.setTBUpdater(std::bind(&MainWindow::updateMenuAndTB, this));
        setTitle(tr("Client"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_mainView);
    }
    
};
