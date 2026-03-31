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
        setDbgName("3D Animation Flat Shading Main Window");
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        setToolBar(_toolBar);
        setCentralView(&_viewCubeMap);
    }
    
protected:
    

    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == _toolBar.getShadingModeSwitch())
        {
            _viewCubeMap.switchColorShadingMode();
            return true;
        }
        return false;
    }
};
