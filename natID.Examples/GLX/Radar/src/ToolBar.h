#pragma once
#include <gui/ToolBar.h>
#include "TBSliderView.h"

class ToolBar : public gui::ToolBar
{
    using Base = gui::ToolBar;
protected:
    TBReverseView _reverseView;
public:
    ToolBar()
    : gui::ToolBar("radarToolBar", 1)
    {
        addItem("", tr("Reverse radar direction"), &_reverseView);
    }
    
    gui::CheckBox* getReverseCheckBox()
    {
        return _reverseView.getCheckBox();
    }
    
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer)
    {
        Base::forwardMessagesTo(pConsumer);
        _reverseView.forwardMessagesTo(pConsumer);
    }
    
};
