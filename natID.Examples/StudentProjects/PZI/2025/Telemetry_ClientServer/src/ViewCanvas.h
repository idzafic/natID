#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Transformation.h>
#include <gui/DrawableString.h>


#include "LineGraph.h"


class ViewCanvas : public gui::Canvas{  //displays the line graph from the LineGraph class
protected:
    LineGraph grafik;
    gui::DrawableString xOsa;
    gui::DrawableString yOsa;
    gui::Point translate;
    double scaleX = 1.0;
    double scaleY = 1.0;
    double xAtMouse, yAtMouse;
    std::function<void(td::Decimal3,td::Decimal3)> setXY;
    std::function<void(double, double, double, double, int)> setInfo;
    double thickness=2.0;
    td::ColorID color=td::ColorID::Yellow;
    td::LinePattern pattern=td::LinePattern::Solid;

protected:

    bool getModelSize(gui::Size& modelSize)const override{ //for scroller to work
        double a=grafik.getXmax()*scaleX;
        modelSize.width=a*1+50;
        modelSize.height=550;
        return true;
    }
    
    void onDraw(const gui::Rect& rect) override{ //draws the graph and calls for the data being shown in the grid layout to be updated
        gui::Size sz;
        getSize(sz);
        grafik.Draw(sz,scaleX, thickness, color, pattern);
        setInfo(grafik.getXmax(), grafik.getXmin(), grafik.getYmax(), grafik.getYmin(), grafik.getRun());
    }

public:
    ViewCanvas(std::function<void(td::Decimal3,td::Decimal3)> FsetXY, std::function<void(double, double, double, double, int)> FsetInfo)
        : gui::Canvas({gui::InputDevice::Event::Zoom, gui::InputDevice::Event::CursorMove})
        ,setXY(FsetXY)
        ,setInfo(FsetInfo)
    {
        enableResizeEvent(true);
        setClipsToBounds();
        registerForScrollEvents();
    }

    void changeX(int xid) //changes the data type on the x axis
    {
        grafik.setX(xid);
        mu::dbgLog("cx");
        reDraw();
    }

    void changeY(int yid) //changes the data type on the y axis
    {
        grafik.setY(yid);
        mu::dbgLog("cy");
        reDraw();
    }

    void changeRun(int run){    //changes the run which is being shown
        grafik.setRun(run);
        reDraw();
    }

    void changeScale(double scale){     //changes the scaling of the x axis
        scaleX=scale;
        reDraw();
    }

    void changeThickness(double thick){ //changes line thickness
        thickness=thick;
        reDraw();
    }

    void changeColor(td::ColorID c){    //changes line color
        color=c;
        reDraw();
    }

    void changePattern(td::LinePattern p){ //changes line pattern
        pattern=p;
        reDraw();
    }

    LineGraph& getGraf(){return grafik;}

    void onCursorMoved(const gui::InputDevice& inputDevice)override{ //tracks cursor position and converts it to coordinates to be shown
        gui::Point position= inputDevice.getModelPoint();
        td::Decimal3 y=position.y;
        td::Decimal3 x=(position.x-5)/scaleX;
        gui::Size sz;
        getSize(sz);
        double maxVal = grafik.getYmax();
        double minVal = grafik.getYmin();
        double B = sz.height;

        double range  = maxVal - minVal;
        double outMin = (maxVal <= 0) ? 25.0 : 0.0;
        double outMax = (minVal >= 0) ? (B - 25.0) : B;

        y = maxVal - (y - outMin) * range / (outMax - outMin);
        setXY(x,y);
    }
};
