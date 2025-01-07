//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//

#pragma once
#include <gui/Application.h>
#include "MainWindow.h"

class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow() override
    {
        setInitialFrameSize(gui::Window::FrameSize::Maximized);
        return new MainWindow();
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }
};
