//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ViewScroller.h>
#include "ViewCanvas.h"

class ScrolledCanvasView : public gui::ViewScroller
{
private:
protected:
    ViewCanvas _canvas;
protected:

public:
    ScrolledCanvasView()
    : gui::ViewScroller(gui::ViewScroller::Type::ScrollAndAutoHide, gui::ViewScroller::Type::ScrollAndAutoHide)
    {
        setContentView(&_canvas);
    }
    
    ViewCanvas& getView()
    {
        return _canvas;
    }
};
