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
    : gui::ToolBar("myToolBar", 8, 7)
    , _img1(":img1")
    , _img2(":img2")
    , _imgIncrease(":incSize")
    , _imgDecrease(":decSize")
    , _img3(":img3")
    {
        _allowedItems[0].init("Option1", tr("option1"), &_img1, tr("option1TT"), 10, 0, 0, 10);
        _defaultItems[0]=0;
        
        _allowedItems[1].init("Option2", tr("option2"), &_img2, tr("option2TT"), 10, 0, 0, 20);
        _defaultItems[1]=1;
        
        _allowedItems[2].initAsSpace();
        _defaultItems[2] = 2;
        
        _allowedItems[3].init("Increase", tr("incSize"), &_imgIncrease, tr("incSizeTT"), 255, 0, 0, 10);
        _defaultItems[3]=3;
        
        _allowedItems[4].init("Decrease", tr("decSize"), &_imgDecrease, tr("decSizeTT"), 255, 0, 0, 20);
        _defaultItems[4]=4;
        
        _allowedItems[5].initAsSpace();
        _defaultItems[5]=5;
        
        _allowedItems[6].init("Canvas", tr("canvas"), &_img3, tr("canvasTT"), 10, 0, 0, 30);
        _defaultItems[6]=6;
        
//        _allowedItems[7].init("Popover", &_symbolsPopover, 1024);
//        _defaultItems[7]=7;
        
        _allowedItems[7].initAsSpace();
    }
    
};
