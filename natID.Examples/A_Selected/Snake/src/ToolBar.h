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
    gui::Image _imgRun;
    gui::Image _imgOK;
    gui::Image _imgNOK;
    gui::Image _settings;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 7)
    , _imgRun(":start")
    , _imgOK(":ok")
    , _imgNOK(":nok")
    , _settings(":settings")
    {
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 10);
        addSpaceItem();
        addItem(tr("start"), &_imgRun, tr("startTT"), 20, 0, 0, 10);
        addSpaceItem();
        addItem(tr("Easy"), &_imgNOK, tr("Easy"), 20, 0, 0, 20);
        addItem(tr("Medium"), &_imgNOK, tr("Medium"), 20, 0, 0, 21);
        addItem(tr("Hard"), &_imgOK, tr("Hard"), 20, 0, 0, 22);

    }
};

