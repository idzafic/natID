//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
//#include "SymbolsPopover.h"

class ToolBar : public gui::ToolBar
{
protected:
//    gui::Image _img1;
//    gui::Image _img2;
    gui::Image _imgIncrease;
    gui::Image _imgDecrease;
    //gui::Image _img3;
    //SymbolsPopover _symbolsPopover;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 2)
    , _imgIncrease(":zoomIn")
    , _imgDecrease(":zoomOut")
    {
        addItem(tr("zoomIn"), &_imgIncrease, tr("zoomInTT"), 255, 0, 0, 10);
        addItem(tr("zoomOut"), &_imgDecrease, tr("zoomOutTT"), 255, 0, 0, 20);
    }
    
};
