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
    ViewGLX _viewCubeMap;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 600))
    {
        setDbgName("3D Animation Texture Main Window");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        setToolBar(_toolBar);
        setCentralView(&_viewCubeMap);
    }
    
protected:
    

    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == _toolBar.getSpeedSlider())
        {
            double val = pSlider->getValue();
            _viewCubeMap.updateSpeed((float)val);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == _toolBar.getTextureSwitch())
        {
            _viewCubeMap.switchTexture();
            return true;
        }
        return false;
    }
};
