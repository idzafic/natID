//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewChess2.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewChess2 _viewChess2;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
//        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
//    
        setToolBar(_toolBar);
        setCentralView(&_viewChess2);
        
        gui::CheckBox* pTextureSwitch = _toolBar.getTextureSwitch();
        pTextureSwitch->onClick([this](){
            _viewChess2.switchTexture();
        });
    }
};
