//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/ColorPicker.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgExport;
    gui::ColorPicker _colorPicker;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 2)
    , _imgExport(":export")
    {
        _colorPicker.setValue(td::ColorID::Blue);
        
        addItem(tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 10);
        addItem(tr("ColorE"), tr("ColorETT"), &_colorPicker, 1);
    }
};
