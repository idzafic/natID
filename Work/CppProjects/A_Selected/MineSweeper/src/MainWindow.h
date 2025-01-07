
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewMinesweeper.h"
#include "StatusBar.h"

#include "GlobalEvents.h"

class MainWindow : public gui::Window
{
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewMinesweeper _mainView;
    StatusBar _statusBar;
protected:
    
    void onInitialAppearance() override
    {
        _mainView.setDifficulty(ViewMinesweeper::Difficulty::Medium);
        auto appProperties = getAppProperties();
        bool showStatusBar = appProperties->getValue("showStatusBar", true);
        if (!showStatusBar)
        {
            _statusBar.show(false);
            gui::Size sz;
            _mainView.getFieldSize(sz);
            adjustSizeToContentSize(sz);
        }
        
        systemAppearanceChangedEvent.at(0)(true); //workaround da se isDarkMode() ne mora pozivati u konstruktoru ViewMinesweeper.h
        // izbaciti kasnije

        _mainView.setFocus();
    }
public:
    MainWindow()
    : gui::Window(gui::Size(320, 320)) //show minimum size first
    , _mainView(&_statusBar)
    {
        mu::dbgLog("INFO: MainWindow");
        setResizable(false);
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        _mainMenuBar.forwardMessagesTo(&_mainView);
        setToolBar(_toolBar);
        _toolBar.forwardMessagesTo(&_mainView);
        setStatusBar(_statusBar);
        setCentralView(&_mainView);
    }
    
    ~MainWindow()
    {
    }
    
};
