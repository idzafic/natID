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
    gui::Image _img1;
    gui::Image _img2;
    gui::Image _imgIncrease;
    gui::Image _imgDecrease;
    gui::Image _img3;
    //SymbolsPopover _symbolsPopover;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 7)
    , _img1(":img1")
    , _img2(":img2")
    , _imgIncrease(":incSize")
    , _imgDecrease(":decSize")
    , _img3(":img3")
    {
        
        addItem(tr("option1"), &_img1, tr("option1TT"), 10, 0, 0, 10);

        addItem(tr("option2"), &_img2, tr("option2TT"), 10, 0, 0, 20);
        
        addSpaceItem();
        
        addItem(tr("incSize"), &_imgIncrease, tr("incSizeTT"), 255, 0, 0, 10);
        
        addItem(tr("decSize"), &_imgDecrease, tr("decSizeTT"), 255, 0, 0, 20);
        
        addSpaceItem();
        
        addItem(tr("canvas"), &_img3, tr("canvasTT"), 10, 0, 0, 30);
    }
    
};
