#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewOrtho.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewOrtho _viewOrtho;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 600))
    {
        setDbgName("Orthographic Viewer");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        setToolBar(_toolBar);
        setCentralView(&_viewOrtho);
    }
};
