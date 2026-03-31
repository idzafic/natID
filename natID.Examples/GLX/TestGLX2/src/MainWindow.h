#pragma once
#include <gui/Window.h>
#include "MenuBar.h"

template <class View>
class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    View _view;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 800))
    {
        setDbgName("Radar Main Window");
        _mainMenuBar.setAsMain(this);
        setCentralView(&_view);
    }
};
