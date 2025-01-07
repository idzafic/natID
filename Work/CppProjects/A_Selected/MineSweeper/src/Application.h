
#pragma once
#include <gui/Application.h>
#include "MainWindow.h"
#include "GlobalEvents.h"

class Application : public gui::Application
{
protected:
    
    gui::Window* createInitialWindow() override
    {
        return new MainWindow();
    }

    virtual void systemAppearanceChanged(bool bDarkMode) override
    {
        gui::Application::systemAppearanceChanged(bDarkMode);
        for(auto &fun : systemAppearanceChangedEvent)
            fun(bDarkMode);
    }
    

public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
    }
};
