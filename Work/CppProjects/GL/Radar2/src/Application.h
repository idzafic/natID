//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Application.h>
#include "MainWindow.h"

template <class View>
class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow() override
    {
        return new MainWindow<View>();
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }
};
