#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ViewTerrainGen.h"
#include "ToolBar.h"

// Main application window with menu bar, toolbar, and central terrain generation view
class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ViewTerrainGen _view;
    ToolBar _toolBar;

public:
    // Constructor: sets up geometry, attaches toolbar and view
    MainWindow()
        : gui::Window(gui::Geometry(50, 50, 1600, 900))
    {
        _mainMenuBar.setAsMain(this);
        setCentralView(&_view);
        setToolBar(_toolBar);
        // Forward toolbar actions directly to the central view for handling
        _toolBar.forwardMessagesTo(&_view);
    }

protected:
    // Called when window first appears to load toolbar settings
    virtual void onInitialAppearance() override
    {
        auto settings = getApplication()->getProperties();
        _toolBar.show(settings->getValue("TBVisible", true));
        _toolBar.setIconSize(static_cast<gui::ToolBar::IconSize>(settings->getValue("TBIconSz", 2)));
        _toolBar.showLabels(settings->getValue("TBLbls", true));
    }

    // Action handling is now done in ViewTerrainGen since messages are forwarded
};