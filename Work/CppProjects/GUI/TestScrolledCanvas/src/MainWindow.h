//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include <gui/ActionItem.h>
#include "MenuBar.h"
#include "MainView.h"
#include "ToolBar.h"


class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    MainView _mainView;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 400, 300))
    {
        setTitle("Test Scrolled Canvs");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        //_toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
        setToolBar(_toolBar);
        
        setCentralView(&_mainView);
    }
    
    ~MainWindow()
    {
    }
    
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {
        gui::Window::onClose();
    }
    
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                auto& view = _mainView.getView();
                view.toolbarZoomIn();
                return true;
            }
            if (actionID == 20)
            {
                auto& view = _mainView.getView();
                view.toolbarZoomOut();
                return true;
            }
            if (actionID == 30)
            {
                auto& canvas = _mainView.getView();
                
                for (auto i = std::underlying_type<gui::Image::Type>::type{0}; i <= static_cast<std::underlying_type<gui::Image::Type>::type>(gui::Image::Type::BMP); i++)
                {
                    gui::Image::Type imgType = (gui::Image::Type)i;
                    
                    {
                        double resScaling = 2;
                        bool onlyVisiblePart = true;
                        gui::Image img;
                        bool bOK = canvas.drawToImage(img, resScaling, onlyVisiblePart);
                        if (!bOK)
                            return true;
                        
        #ifdef MU_WINDOWS
                        td::String strFn("R://CanvasVP");
        #else
                        td::String strFn("/Volumes/RAMDisk/CanvasVP");
        #endif
                        bOK = img.saveToFile(strFn, imgType);
            
                    }
                    
                    {
                        double resScaling = 1;
                        bool onlyVisiblePart = false;
                        gui::Image img;
                        bool bOK = canvas.drawToImage(img, resScaling, onlyVisiblePart);
                        if (!bOK)
                            return true;

        #ifdef MU_WINDOWS
                        td::String strFn("R://CanvasWhole");
        #else
                        td::String strFn("/Volumes/RAMDisk/CanvasWhole");
        #endif
                        bOK = img.saveToFile(strFn, imgType);

                    }
                    
                    {
                        double resScaling = 4;
                        gui::Rect r(98, 198, 502, 502);
                        gui::Image img;
                        bool bOK = canvas.drawToImage(img, r, resScaling);
                        if (!bOK)
                            return true;

        #ifdef MU_WINDOWS
                        td::String strFn("R://CanvasRect");
        #else
                        td::String strFn("/Volumes/RAMDisk/CanvasRect");
        #endif
                        bOK = img.saveToFile(strFn, imgType);

                    }
                        
                    
                }
                
                return true;
            }
            
            if (actionID == 40)
            {
                auto& canvas = _mainView.getView();
                {
#ifdef MU_WINDOWS
                    td::String strFn("R://CanvasVP");
#else
                    td::String strFn("/Volumes/RAMDisk/CanvasVP");
#endif
                    bool onlyVisiblePart = true;
                    bool bOKEPS = canvas.exportToEPS(strFn, onlyVisiblePart);
                    bool bOKPDF = canvas.exportToPDF(strFn, onlyVisiblePart);
                    bool bOKSVG = canvas.exportToSVG(strFn, onlyVisiblePart);
                }
                
                {
#ifdef MU_WINDOWS
                    td::String strFn("R://CanvasWhole");
#else
                    td::String strFn("/Volumes/RAMDisk/CanvasWhole");
#endif
                    bool onlyVisiblePart = false;
                    bool bOKEPS = canvas.exportToEPS(strFn, onlyVisiblePart);
                    bool bOKPDF = canvas.exportToPDF(strFn, onlyVisiblePart);
                    bool bOKSVG = canvas.exportToSVG(strFn, onlyVisiblePart);
                }
                
                {
#ifdef MU_WINDOWS
                    td::String strFn("R://CanvasRect");
#else
                    td::String strFn("/Volumes/RAMDisk/CanvasRect");
#endif
                    gui::Rect r(98, 198, 502, 502);
                    bool bOKEPS = canvas.exportToEPS(r, strFn);
                    bool bOKPDF = canvas.exportToPDF(r, strFn);
                    bool bOKSVG = canvas.exportToSVG(r, strFn);
                }
                
                return true;
            }
            
            
            //tolbar
            return true;
        }
        
        if (menuID)
        {
            if (actionID == 10)
            {
                bool bVisible = _toolBar.isVisible();
                _toolBar.show(!bVisible);
                pAI->setChecked(!bVisible);
                return true;
            }
            
            if (actionID == 20)
            {
                bool bVisible = _toolBar.areLabelsVisible();
                _toolBar.showLabels(!bVisible);
                pAI->setChecked(!bVisible);
                return true;
            }
            
            if (actionID == 30)
            {
                gui::ToolBar::IconSize iconSize = _toolBar.getIconSize();
                if ( (iconSize == gui::ToolBar::IconSize::Small) || (iconSize == gui::ToolBar::IconSize::SystemDefault) )
                {
                    iconSize = gui::ToolBar::IconSize::Regular;
                    pAI->setChecked(true);
                }
                else
                {
                    iconSize = gui::ToolBar::IconSize::Small;
                    pAI->setChecked(false);
                }
                _toolBar.setIconSize(iconSize);
                return true;
            }
            
            return false;
        }
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            return false;
        }
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            if (actionID == 20)
            {
//                ScrollCanvasWindow* canvasWnd = new ScrollCanvasWindow(this);
//                canvasWnd->show(gui::Window::InitialSize::Maximized);
                return true;
            }
            
            if (actionID == 40)
            {
                //Show switcher (Prikazi switcher)
//                if (_pSwitcher == nullptr)
//                {
//                    std::function<void (void)> fnCloseSwitcher( std::bind( &MainWindow::handleClosingSwitcher, this) );
//                    _pSwitcher = new SwitcherWindow(fnCloseSwitcher, this);
//                    _pSwitcher->show(gui::Window::InitialSize::Maximized);
//                    pAI->setChecked(true);
//                    pAI->disable();
//                    
//                    //gui::ActionItem = _mainMenuBar.getItem(
//                }
                return true;
            }
            
            if (actionID == 50)
            {
                //Show navigator
//                NavigatorWindow* pNavWindow = new NavigatorWindow(this);
//                pNavWindow->show(gui::Window::InitialSize::Normal);
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
    
};
