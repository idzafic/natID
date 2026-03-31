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
    : gui::ToolBar("myToolBar", 2)
    , _imgExport(":export")
    {
        addItem(tr("speedAndType"), tr("speedAndTypeTT"), &_sliderView);
        addItem(tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 10);
    }
    
    gui::CheckBox* getShadingModeSwitch()
    {
        return _sliderView.getCheckBox();
    }
    
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer)
    {
        Base::forwardMessagesTo(pConsumer);
        _sliderView.forwardMessagesTo(pConsumer);
    }
};