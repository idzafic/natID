#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class ToolBar : public gui::ToolBar
{
    gui::Image _imgBomb;
public:
    ToolBar(gui::Image* imgRun)
        : gui::ToolBar("mainTB", 2)
        , _imgBomb(":bomb")
    {
        addItem(tr("start"), imgRun, tr("startTT"), 20, 0, 0, 10);
        addItem(tr("exp"), &_imgBomb, tr("expTT"), 20, 0, 0, 20);
    }
};
