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
    gui::Image _imgOpen;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 1)
    , _imgOpen(":imgOpen")
    {
        addItem(tr("open"), &_imgOpen, tr("openTT"), 20, 0, 0, 10);
    }
};

