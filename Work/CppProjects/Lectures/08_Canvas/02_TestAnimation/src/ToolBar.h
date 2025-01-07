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
public:
    ToolBar(gui::Image* imgRun)
    : gui::ToolBar("mainTB", 1)
    {
        addItem(tr("start"), imgRun, tr("startTT"), 20, 0, 0, 10);
    }
};

