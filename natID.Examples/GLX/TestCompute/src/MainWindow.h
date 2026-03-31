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
    : gui::Window(gui::Size(1000, 800))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        setToolBar(_toolBar);
        setCentralView(&_viewGLX);
    }
    
protected:

    
};
