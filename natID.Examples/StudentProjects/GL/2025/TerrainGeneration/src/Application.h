#pragma once
#include <gui/Application.h>
#include "MainWindow.h"

class Application : public gui::Application
{
protected:
    // Creates and returns the initial window shown at startup
    gui::Window* createInitialWindow() override
    {
		setInitialFrameSize(gui::Window::FrameSize::FullScreen); // Set the initial frame size to full screen
        return new MainWindow();
    }

public:
    Application(int argc, const char** argv)
        : gui::Application(argc, argv)
    {
    }
};
