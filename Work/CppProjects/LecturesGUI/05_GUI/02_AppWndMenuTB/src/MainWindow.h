//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 480))
    {
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
    
        setToolBar(_toolBar);        
    }
    
    
protected:
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    td::String msgText(tr("mOpen"));
                    td::String informativeText;
                    informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
                   showAlert(msgText, informativeText);
                    return true;
                }
                    
                case 20:
                {
                    //PRIMJER snimanja fajla
                    td::String msgText(tr("mSave"));
                    td::String informativeText;
                    informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)",
                                           menuID, firstSubMenuID, lastSubMenuID, actionID);
                    
                   showAlert(msgText, informativeText);
                    return true;
                }
                
                case 30:
                {
                    td::String msgText(tr("mRect"));
                    td::String informativeText;
                    informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
                    
                   showAlert(msgText, informativeText);
                    return true;
                }
                default:
                    return false;
            }
        }
        
        td::String msgText("Unhandled onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
};
