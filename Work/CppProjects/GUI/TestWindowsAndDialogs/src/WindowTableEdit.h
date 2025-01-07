//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "ViewTableEdit.h"
#include "ToolBar.h"

class WindowTableEdit : public gui::Window
{
protected:
    ViewTableEdit _viewTableEdit;
    ToolBar _toolBar;
public:
    WindowTableEdit(gui::Window* parentWnd, td::UINT4 wndID = 0)
    : gui::Window(gui::Size(800, 600), parentWnd, wndID)
    {
        setTitle(tr("Table Edit Window"));
        setToolBar(_toolBar);
        setCentralView(&_viewTableEdit);
//        composeContent();
    }
    
    ~WindowTableEdit()
    {
    }
};
