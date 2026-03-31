#pragma once
#include <gui/ViewScroller.h>
#include "ViewCanvas.h"

class ViewScroll : public gui::ViewScroller //gives the canvas view a scroller
{
private:
protected:
    std::function<void(td::Decimal3,td::Decimal3)> FsetXY;
    std::function<void(double, double, double, double, int)> FsetInfo;
    ViewCanvas canvas;
protected:

public:
    ViewScroll(std::function<void(td::Decimal3,td::Decimal3)> fsetxy, std::function<void(double, double, double, double, int)> fsetInfo)
        : gui::ViewScroller(gui::ViewScroller::Type::ScrollerAlwaysVisible, gui::ViewScroller::Type::NoScroll)
        ,FsetXY(fsetxy)
        ,FsetInfo(fsetInfo)
        ,canvas(FsetXY, FsetInfo)
    {
        setContentView(&canvas);
    }
    
    ViewCanvas& getView()
    {
        return canvas;
    }

    ViewCanvas& getCanvas(){return canvas;}

};
