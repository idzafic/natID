//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <gui/Application.h>
#include "MainWindow.h"
#include "Constants.h"

class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow() override
    {
        auto appProperties = getProperties();
        td::BYTE initialModelID = appProperties->getValue("modelID", cInitialModel);
        return new MainWindow(initialModelID);
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }
};
