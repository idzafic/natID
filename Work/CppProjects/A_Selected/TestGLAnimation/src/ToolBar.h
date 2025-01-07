//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include "TBSliderView.h"

class ToolBar : public gui::ToolBar
{
    using Base = gui::ToolBar;
protected:
    gui::Image _imgExport;
    TBSliderView _sliderView;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 2, 2)
    , _imgExport(":export")
    {
        td::WORD i = 0;
        initItem(i++, tr("scaleTex"), tr("scaleTexTT"), &_sliderView);
        initItem(i++, tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 10);
    }
    
    gui::Slider* getTextureSlider()
    {
        return _sliderView.getSlider();
    }
    
    gui::CheckBox* getTextureSwitch()
    {
        return _sliderView.getCheckBox();
    }
    
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer)
    {
        Base::forwardMessagesTo(pConsumer);
        _sliderView.forwardMessagesTo(pConsumer);
    }
    
};
