#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ViewGLX.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ViewGLX _viewGLX;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 800))
    {
        setDbgName("Main Wnd");
        _mainMenuBar.setAsMain(this);
        setCentralView(&_viewGLX);
    }
};
