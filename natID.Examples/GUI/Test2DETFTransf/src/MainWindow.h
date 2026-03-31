//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "MainView.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    MenuBar _mainMenuBar;
    
    MainView _mainView;
protected:
    
    bool shouldClose() override
    {
        return true;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(1200, 800))
    {
        setTitle("2D TRANSFORMACIJE");
        _mainMenuBar.setAsMain(this);
        setCentralView(&_mainView);
    }
    
    ~MainWindow()
    {
    }
    
};
