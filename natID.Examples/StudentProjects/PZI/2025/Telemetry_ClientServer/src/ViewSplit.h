#pragma once
#include <gui/SplitterLayout.h>
#include <gui/View.h>

#include "ViewScroll.h"
#include "ViewGrid.h"



class ViewSplit : public gui::View //puts the canvas view and the grid layout for the regular line graph into a split leyout and handles comunication between them
{
    gui::SplitterLayout _splitter;
    std::function<void(td::Decimal3,td::Decimal3)> fcnsetXY;
    std::function<void(double, double, double, double, int)> fcnsetInfo;
    ViewScroll scroll;
    std::function<void(int)> fcnChangeX;
    std::function<void(int)> fcnChangeY;
    std::function<void(int)> fcnChangeRun;
    std::function<void(double)> fcnChangeScale;
    std::function<void(double)> fcnChangeThickness;
    std::function<void(td::ColorID)> fcnChangeColor;
    std::function<void(td::LinePattern)> fcnChangePattern;
    ViewGrid grid;


public:
    ViewSplit():_splitter(gui::SplitterLayout::Orientation::Vertical, gui::SplitterLayout::AuxiliaryCell::Second)
        ,fcnsetXY(std::bind(&ViewGrid::setXY, &grid, std::placeholders::_1, std::placeholders::_2))
        ,fcnsetInfo(std::bind(&ViewGrid::setInfo, &grid, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))
        ,scroll(fcnsetXY, fcnsetInfo)
        ,fcnChangeX(std::bind(&ViewCanvas::changeX, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeY(std::bind(&ViewCanvas::changeY, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeRun(std::bind(&ViewCanvas::changeRun, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeScale(std::bind(&ViewCanvas::changeScale, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeThickness(std::bind(&ViewCanvas::changeThickness, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeColor(std::bind(&ViewCanvas::changeColor, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangePattern(std::bind(&ViewCanvas::changePattern, &scroll.getCanvas(), std::placeholders::_1))
        ,grid(fcnChangeX, fcnChangeY, fcnChangeRun, fcnChangeScale, fcnChangeThickness, fcnChangeColor, fcnChangePattern)
    {
        _splitter.setContent(scroll, grid);
        setLayout(&_splitter);
    }
};
