#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "Renderer.h"
#include <mu/DebugConsoleLog.h>

template <class View>
class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    View _view;
    

    
    void toggleRadarDirection()
    {
        Renderer* pRadarRenderer = _view.getRadarRenderer();
        if (pRadarRenderer) {
            pRadarRenderer->toggleRadarDirection();
            bool isReversed = pRadarRenderer->isRadarReversed();

            // Update checkbox state
            gui::CheckBox* pCheckBox = _toolBar.getReverseCheckBox();
            if (pCheckBox) {
                pCheckBox->setValue(isReversed, false);
            }
        }
    }
    
    
public:
    MainWindow()
    : gui::Window(gui::Size(800, 800))
    {
        setDbgName("Radar Main Window");
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_view);
        // Forward toolbar checkbox change events to handle state changes
        _toolBar.forwardMessagesTo(this);
        
    }
    

    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        toggleRadarDirection();
        return true;
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == _toolBar.getReverseCheckBox())
        {
            toggleRadarDirection();
            return true;
        }
        return false;
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isASCII()) {
            char ch = key.getChar();
            
            // Keyboard shortcut to reverse radar direction
            if (ch == 'r' || ch == 'R') {
                toggleRadarDirection();
                return true;
            }
        }
        return false;
    }
};
