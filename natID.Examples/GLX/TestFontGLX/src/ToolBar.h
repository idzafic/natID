#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>


class ToolBar : public gui::ToolBar
{
    using Base = gui::ToolBar;
protected:
    gui::Image _imgExport;
 
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 1)
    , _imgExport(":export")
    {
        addItem(tr("export"), &_imgExport, tr("exportTT"), 20, 0, 0, 10);
    }
    

   
    
};
