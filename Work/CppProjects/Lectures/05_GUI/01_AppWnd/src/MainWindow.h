//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>

class MainWindow : public gui::Window
{
public:
    MainWindow()
    : gui::Window(gui::Size(800, 480))
    {
        setTitle("First App");
    }  
};

