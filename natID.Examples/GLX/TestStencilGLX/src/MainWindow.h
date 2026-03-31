#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewGLX.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewGLX _viewGLX;
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10) // Application menu
        {
            if (actionID == 10) // Properties
            {
                return true;
            }
        }
        else if (menuID == 20) // Export
        {
            if (actionID == 10)
            {
                _viewGLX.exportFrame();
                return true;
            }
            
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == _toolBar.getSpeedSlider())
        {
            float val = (float) pSlider->getValue();
            _viewGLX.updateSpeed(val);
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::ColorPicker* pWidget) override
    {
        if (pWidget == _toolBar.getLightColorPicker())
        {
            auto colorID = pWidget->getValue();
            _viewGLX.updateLightColor(colorID);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pChBox) override
    {
        if (pChBox == _toolBar.getRotateSwitch())
        {
            _viewGLX.switchRotation();
            return true;
        }
        return false;
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 800))
    {
        setDbgName("GLX Light Demo Main Window");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        setToolBar(_toolBar);
        setCentralView(&_viewGLX);
    }
};
