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
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 600))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        setToolBar(_toolBar);
        setCentralView(&_viewGLX);
    }
    
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();
        if (menuID == 20)
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
        if (pSlider == _toolBar.getTextureSlider())
        {
            double val = pSlider->getValue();
            _viewGLX.updateTextScale((float)val);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == _toolBar.getTextureSwitch())
        {
            _viewGLX.switchTexture();
            return true;
        }
        return false;
    }
};
