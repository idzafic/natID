#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewGLX.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewGLX _viewGLX;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 700))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        setToolBar(_toolBar);
        setCentralView(&_viewGLX);
    }
    
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
        {
            if (actionID == 10)
            {
                //export to image
                _viewGLX.exportFrame();
                return true;
            }
        }
        return false;
    }
    
};
