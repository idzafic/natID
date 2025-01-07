//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
//#include "ToolBar.h"
#include "ToolBarShapes.h"
#include "ViewVert.h"
#include "ViewVertFixed.h"
#include "ViewHoriz.h"
#include "ViewVertWithHor.h"
#include "ViewGrid.h"
#include "ViewSpanGrid.h"
#include "ViewComplex.h"
#include "ViewWithTable.h"
//#include "ViewCanvas.h"
#include "CanvasWindow.h"
#include "ScrollCanvasWindow.h"
#include <gui/FileDialog.h>
#include "ViewWithTabs.h"
#include "SwitcherWindow.h"
//#include <functional>
#include "NavigatorWindow.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBarShapes _toolBar;
    //ViewVertFixed _myView;
    //ViewVert _myView;
    //ViewVertWithHor _myView;
    //ViewGrid _myView;
    //ViewSpanGrid _myView;
    //ViewComplex _myView;
    //ViewWithTable _myView;
    //ViewCanvas _myView;
    //MainViewWithTabs _myView;
    //CanvasWindow* _canvasWnd = nullptr;
    NavigatorView _myView;
    SwitcherWindow* _pSwitcher = nullptr;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        _mainMenuBar.setAsMain(this);
        //_mainMenuBar.forwardMessagesTo(this);
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
    
        setToolBar(_toolBar);
        setCentralView(&_myView);
        
        //composeContent();
    }
    
    ~MainWindow()
    {
//        if (_canvasWnd)
//        {
//            delete _canvasWnd;
//        }
    }
    
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {
        gui::Window::onClose();
    }
    
    void handleClosingSwitcher()
    {
        _pSwitcher = nullptr;
        
        gui::ActionItem* pAI = _mainMenuBar.getItem(20, 0, 0, 40); //enable menu option
        assert(pAI);
        pAI->setChecked(false);
        pAI->enable();
    }
    
//    void saveFile(gui::FileDialog::Status status, const td::String& fileName, gui::ActionItem* pAI)
//    {
//        pAI->enable();
//        if (status == gui::FileDialog::Status::OK)
//            mu::dbgLog("User pressed OK! Selected file to save =%s", fileName.c_str());
//        else
//            mu::dbgLog("User cancelled saving!");
//
//    }
//
//    void openFile(gui::FileDialog::Status status, const td::String& fileName, gui::ActionItem* pAI)
//    {
//        pAI->enable();
//        if (status == gui::FileDialog::Status::OK)
//            mu::dbgLog("User pressed OK! Selected file to open =%s", fileName.c_str());
//        else
//            mu::dbgLog("User cancelled opening!");
//    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 10 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    //PRIMJER snimanja fajla
//                    pAI->disable();
                    
                    //gui::SaveFileDialog dlg("Save something");
                    td::String strNameLbl(tr("Snimi"));
                    strNameLbl += ":";
                    td::String dlgTitle("Snimi nesto");
                    const char* filter = ".xml";
                    const char* defaultFileName ="ImeFajla";
                    
                    //Primjer sa pointerom na member funkciju koristenjem bind i placeholder-a
//                    td::String selectedFileName;
//                    gui::FileDialog::CallBack callBackFn(std::bind(&MainWindow::saveFile, this, std::placeholders::_1, std::placeholders::_2, pAI));
//                    gui::FileDialog::saveFileAsync(this, callBackFn, dlgTitle, strNameLbl, filter, defaultFileName);
                    return true;
                }
                case 20:
                {
                    //PRIMJER Otvaranja fajla
//                    pAI->disable();
                    td::String dlgTitle("Otvori nesto");
                    
//                    gui::FileDialog::CallBack callBackFn(std::bind(&MainWindow::openFile, this, std::placeholders::_1, std::placeholders::_2, pAI));
//
//                    auto allowedExtensions = {"xml", "7z"};
//                    gui::FileDialog::openFileAsync(this, callBackFn, dlgTitle, allowedExtensions);
                    
                    //Hint: To open folder uncomment line below
                    //gui::FileDialog::openFolder(callBackFn, dlgTitle);
                    return true;
                }
                case 30:
                {
                    //TODO - Vratiti
                    
                    CanvasWindow* canvasWnd = new CanvasWindow(this);
                    canvasWnd->open(gui::Window::FrameSize::Maximized);
                    return true;
                }
                default:
                    return false;
            }
        }
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            if (actionID == 20)
            {
                //td::String msgText("Handling To je to");
                //td::String infoText("To je to! Sad nastavi ovdje");
                //showAlert(msgText, infoText);
                ScrollCanvasWindow* canvasWnd = new ScrollCanvasWindow(this);
                canvasWnd->open(gui::Window::FrameSize::Maximized);
                return true;
            }
            
            if (actionID == 40)
            {
                //Show switcher (Prikazi switcher)
                if (_pSwitcher == nullptr)
                {
                    std::function<void (void)> fnCloseSwitcher( std::bind( &MainWindow::handleClosingSwitcher, this) );
                    _pSwitcher = new SwitcherWindow(fnCloseSwitcher, this);
                    _pSwitcher->open(gui::Window::FrameSize::Maximized);
                    pAI->setChecked(true);
                    pAI->disable();
                    
                    //gui::ActionItem = _mainMenuBar.getItem(
                }
                return true;
            }
            
            if (actionID == 50)
            {
                //Show navigator
                NavigatorWindow* pNavWindow = new NavigatorWindow(this);
                pNavWindow->open(gui::Window::FrameSize::UseSpecified);
                return true;
            }
        }
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //increase window size
                gui::Geometry g;
                getGeometry(g);
                g.size.width *= 1.2f;
                g.size.height *= 1.2f;
                setGeometry(g, true, gui::Frame::Animation::Yes);
                return true;
            }
            else if (actionID == 20)
            {
                //decrease window size
                gui::Geometry g;
                getGeometry(g);
                g.size.width *= 0.8f;
                g.size.height *= 0.8f;
                setGeometry(g, true, gui::Frame::Animation::Yes);
                return true;
            }
        }
        
        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
    bool onChangedSelection(gui::ComboBox* pCmb) override
    {
        if (pCmb->getControlID() == 10000)
        {
            td::String msgText("Handling onChangedSelection");
            td::String informativeText("Toolbar combo box changed value!");

            showAlert(msgText, informativeText);
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::ColorPicker* pCP) override
    {
        if (pCP->getControlID() == 10001)
        {
            td::String msgText("Handling onChangedValue");
            td::String informativeText("Toolbar color picker changed value!");

            showAlert(msgText, informativeText);
            return true;
        }
        return false;
    }

};
