//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include "Constants.h"

class ToolBar : public gui::ToolBar
{
    gui::Image _settings;
public:
    ToolBar(gui::Image* imgRun)
    : gui::ToolBar("mainTB", 2)
    , _settings(":settings")
    {
        addItem(tr("settings"), &_settings, tr("settingsTT"), 10, 0, 0, 10);
        addItem(tr("start"), imgRun, tr("startTT"), cMenuAnimation, 0, 0, 10);
    }
};

