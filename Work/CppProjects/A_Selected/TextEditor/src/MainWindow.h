#pragma once
#include <gui/Window.h>
#include "TextEditor.h"
#include "MenuBar.h"

class MainWindow : public gui::Window
{
protected:
    MenuBar _menu;
    TextEditor _mainView;
public:
    MainWindow()
    : gui::Window(gui::Size(400, 280))
    {
        setTitle(tr("textEditor"));
        _menu.setAsMain(this);
        setCentralView(&_mainView);
    }
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pActionItem = aiDesc.getActionItem();
        
        if (menuID == 10)
        {
            if (actionID == 10)
            {
                //open HTML
                _mainView.loadHTML();
                return true;
            }
            else if (actionID == 20)
            {
                //save HTML
                _mainView.saveHTML();
                return true;
            }
        }
        return false;
    }
    
 
};
