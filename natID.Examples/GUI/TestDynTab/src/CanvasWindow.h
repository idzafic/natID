//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBarShapes.h"
//#include "ViewCanvas.h"
#include "ViewSymbols.h"

//#include <gui/priv/CanvasColorPopoverView.h>

class CanvasWindow : public gui::Window
{
private:
protected:
    ToolBarShapes _toolBar;
    //ViewCanvas _myView;
    ViewSymbols _myView;
    //gui::CanvasColorPopoverView _myView;
public:
    CanvasWindow(Window* pParent = nullptr, td::LUINT8 wndID = 0)
    : gui::Window(gui::Geometry(200, 200, 800, 600), pParent, wndID)
    {
        //_mainMenuBar.setAsMain();
        //_mainMenuBar.forwardMessagesTo(this);
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
        
        setToolBar(_toolBar);
        setCentralView(&_myView);
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
//        auto pAI = aiDesc.getActionItem();
        
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
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
    ~CanvasWindow()
    {
    }
};
