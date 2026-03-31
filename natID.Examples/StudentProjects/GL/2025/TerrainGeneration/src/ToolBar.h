#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

// Custom toolbar with a frequency icon button
class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgFrequency;

public:
    ToolBar()
        : gui::ToolBar("mainTB", 2)
        , _imgFrequency(":frequency")
    {
        addItem(tr("frequency"), &_imgFrequency, tr("frequencyTT"), 10, 0, 0, 10);
    }
};
