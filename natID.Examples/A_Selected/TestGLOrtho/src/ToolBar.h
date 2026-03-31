//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgSettings;
    gui::Image _imgInfo;
    gui::Image _imgExport;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 3, 3)
    , _imgSettings(":settings")
    , _imgInfo(":info")
    , _imgExport(":export")
    {
        addItem(tr("settings"), &_imgSettings, tr("settingsTT"), 20, 0, 0, 10);
        addItem(tr("info"), &_imgInfo, tr("infoTT"), 20, 0, 0, 20);
        addItem(tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 30);
    }
    
};
