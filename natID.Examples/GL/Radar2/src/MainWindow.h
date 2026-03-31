//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ViewGL.h"

template <class View>
class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    View _view;

public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        _mainMenuBar.setAsMain(this);
        setCentralView(&_view);
    }
};
