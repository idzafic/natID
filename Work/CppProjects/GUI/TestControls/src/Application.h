//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Application.h>
#include "MainWindow.h"
#include "ContextMenus.h"

class Application : public gui::Application
{
protected:
    ContextMenus _ctxMenus;
    
    gui::Window* createInitialWindow() override
    {
        return new MainWindow();
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
        setContextMenus(&_ctxMenus);
    }
};
