#pragma once
#include <gui/Window.h>
#include "MainView.h"

class MainWindow : public gui::Window
{
protected:
    MainView _mainView;

protected:
    void onInitialAppearance() override
    {
        _mainView.focusOnCanvas();
    }

public:
    MainWindow()
        : gui::Window(gui::Size(800, 620))
        , _mainView()
    {
        setTitle("Super Mario - Test");
        setCentralView(&_mainView);
    }
};