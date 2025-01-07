//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//

#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgSettings;
    gui::Image _imgOpen;
    gui::Image _imgSave;
    gui::Symbol _symbol;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 5)
    , _imgSettings(":settings")
    , _imgOpen(":imgOpen")
    , _imgSave(":imgSave")
    , _symbol(":roundRct")
    {
        addItem(tr("settings"), &_imgSettings, tr("settingsTT"), 10, 0, 0, 10);
        addItem(tr("open"), &_imgOpen, tr("openTT"), 20, 0, 0, 10);
        addItem(tr("save"), &_imgSave, tr("saveTT"), 20, 0, 0, 20);
        addSpaceItem();
        addItem(tr("rect"), &_symbol, tr("rectTT"), 20, 0, 0, 30);
    }
};

