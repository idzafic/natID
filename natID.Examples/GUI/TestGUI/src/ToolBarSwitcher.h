#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>

class ToolBarSwitcher : public gui::ToolBar
{
protected:
    gui::Image _imgPrev;
    gui::Image _imgNext;
    gui::Image _imgRemove;
public:
    ToolBarSwitcher()
    : gui::ToolBar("Switcher", 3)
    , _imgPrev(":prev")
    , _imgNext(":next")
    , _imgRemove(":remove")
    {
        addItem("Prev view", &_imgPrev, "Prev View", 20, 0, 0, 10);
        
        addItem("Next view", &_imgNext, "Next View", 20, 0, 0, 11);
        
        addItem("Remove view", &_imgRemove, "Remove View", 20, 0, 0, 20);
    }
};
