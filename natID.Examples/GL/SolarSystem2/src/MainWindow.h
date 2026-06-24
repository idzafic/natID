//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "SolarSystemView.h"

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;
    SolarSystemView _solarView;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 800))
    {
        _mainMenuBar.setAsMain(this);
        setCentralView(&_solarView);
    }
};
