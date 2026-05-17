//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include "SymbolsPopover.h"

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgOpen;
    gui::Image _imgSave;
    SymbolsPopover _symbolsPopover;
    gui::Image _settings;
public:
    ToolBar()
    : gui::ToolBar("mainToolBar", 6, 6)
    , _imgOpen(":imgOpen")
    , _imgSave(":imgSave")
    , _settings(":settings")
    {
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 10);
        
        addSpaceItem();
        
        addItem(tr("Open"), &_imgOpen, tr("openTT"), 20, 0, 0, 10);
        
        addItem(tr("Save"), &_imgSave, tr("SaveTT"), 20, 0, 0, 20);
        
        addSpaceItem();

        addItem(&_symbolsPopover, 1024);
    }
    
};
