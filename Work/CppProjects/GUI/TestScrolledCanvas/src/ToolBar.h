//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include "SymbolsPopover.h"

class ToolBar : public gui::ToolBar
{
protected:
//    gui::Image _img1;
//    gui::Image _img2;
    gui::Image _imgIncrease;
    gui::Image _imgDecrease;
    gui::Image _imgFromFile;
    gui::Symbol _xfmr;
    //gui::Image _img3;
    SymbolsPopover _symbolsPopover;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 6)
    , _imgIncrease(":zoomIn")
    , _imgDecrease(":zoomOut")
//    , _imgFromFile("/Users/idzafic/Work/Common/Icons/bh_flag.png")
    , _imgFromFile(":bh_flag")
    , _xfmr(":Xfmr4")
    {
        addItem(tr("zoomIn"), &_imgIncrease, tr("zoomInTT"), 255, 0, 0, 10);
        addItem(tr("zoomOut"), &_imgDecrease, tr("zoomOutTT"), 255, 0, 0, 20);

        addItem(tr("fileLbl"), &_imgFromFile, tr("file"), 255, 0, 0, 30);

        addSpaceItem();

        addItem(tr("SymbolLBL"), &_xfmr, tr("symbolTT"), 255, 0, 0, 40);
        
        td::UINT2 ctrlPopoverID = 100;
        addItem( &_symbolsPopover, ctrlPopoverID);
    }
    
};
