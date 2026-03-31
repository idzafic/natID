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
    gui::Image _ctxRep;
    gui::Image _settings;
    //SymbolsPopover _symbolsPopover;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 10)
    , _img1(":normalRep")
    , _img2(":fallRep")
    , _imgIncrease(":incSize")
    , _imgDecrease(":decSize")
    , _ctxRep(":ctxRep")
    , _settings(":settings")
    {
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 1);
        
        addSpaceItem();
        
        addItem(tr("normalRep"), &_img1, tr("normalRepTT"), 10, 0, 0, 10);

        addItem(tr("normalRep2"), &_img1, tr("normalRep2TT"), 10, 0, 0, 11);
        
        addItem(tr("fallRep"), &_img2, tr("fallRepTT"), 10, 0, 0, 20);
        
        addItem(tr("fallRep2"), &_img2, tr("fallRep2TT"), 10, 0, 0, 21);

        addItem(tr("Rep"), &_ctxRep, tr("RepTT"), 10, 0, 0, 22);
      
        addSpaceItem();
        
        addItem(tr("incSize"), &_imgIncrease, tr("incSizeTT"), 255, 0, 0, 10);
        
        addItem(tr("decSize"), &_imgDecrease, tr("decSizeTT"), 255, 0, 0, 20);
        
    }
    
};
