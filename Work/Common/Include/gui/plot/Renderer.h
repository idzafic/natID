// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

// ##################################################################################
// # native Interface Design (natID) ver. 3.2.7 (2024-12-16)
// # Licensed under the Academic Free License, version 3
// # Copyright (C) 2024 Prof. dr Izudin Dzafic 
// # 
// # You may use this code under the terms of the Academic Free License, version 3.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ###################################################################################

#pragma once
#include <gui/plot/Function.h>
#include <gui/plot//Legend.h>
#include <gui/Canvas.h>
#include <gui/Image.h>
#include <vector>
#include <unordered_set>
#include <gui/DrawableString.h>
#include <gui/Font.h>
#include <queue>

class Graph;

namespace gui
{
namespace plot
{

class NATPLOT_API Renderer
{
    friend class View;

    static constexpr td::ColorID SELECT_COLOR = td::ColorID::Green;

    std::vector<Function> _funkcije;
    std::vector<td::ColorID> _pastColors;
    td::ColorID nextColor();
    td::ColorID _axisColor = td::ColorID::SysText;
    std::unordered_set<td::ColorID> _disallowedColors;
    std::queue<td::ColorID> _defaultColors;
    
    gui::Geometry drawingWindow;
    gui::Rect _drawingRect;
    bool _drawMargins;
    bool _drawNumbersOutside = true;

    struct Margins {
        double marginTop = 20;
        double marginRight = 20;
        double marginBottom = 5;
        double marginLeft = 10;
    }_margins, _marginsZero;


    bool _active = false;
    bool _drawGrid = false;
    gui::DrawableString xAxisName = "", yAxisName = "";
    void drawAxis();
    double _numberHeight;
    td::String to_string(gui::CoordType x);
    inline bool checkRange(const size_t& number) { return number > _funkcije.size(); }

    
    void setUpDrawingWindow();
    void finishAddingFunction(Function& newFun);
    void updateLimits(const Function& newFun);
    void ZoomToWindow(const gui::Geometry& window);
    void moveGraph(const gui::CoordType &x, const gui::CoordType& y);

    enum class limits{xMin = 0, xMax, yMin, yMax};
    gui::CoordType* _Limits = nullptr;
    bool _initalDraw = false;

    

    bool _drawLegend = false;
    bool _legendPositionChanged = false;

    Legend _legenda;


    double _yAxisNameSeperation = 40; //150;
    double _xAxisNameSeperation = 40; //86;

    gui::Font *_font;
    gui::Size _size;
    void onSizeChanged();


protected:




public:

    std::vector<gui::CoordType> verticals, horizontals;
    virtual void drawAgain(){}

    Renderer(td::ColorID axisColor, gui::Font *axisFont, gui::Font *legendFont, bool startWithMargins = false);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    ~Renderer();
    void reset();
    
    void setAxisColor(td::ColorID boja);
    td::ColorID getAxisColor() { return _axisColor; }
    void setxAxisName(const td::String& xName);
    void setyAxisName(const td::String& yName);
    const td::String getxAxisName() { return xAxisName.toString(); }
    const td::String getyAxisName() { return yAxisName.toString(); }
    void setAxisNameDistance(double xNameDistanceFromAxis, double yNameDistanceFromAxis);
    void getAxisNameDistance(double &xNameDistanceFromAxis, double &yNameDistanceFromAxis);

    
    void setMargins(double left, double right, double bottom, double top);
    void getMargins(double &left, double &right, double &bottom, double &top);
    void showLegend(bool draw);
    void showGrid(bool draw) { _drawGrid = draw; drawAgain(); }

    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, td::ColorID color, double lineWidth = 2, td::LinePattern pattern = td::LinePattern::Solid, td::String name = "line");
    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, double lineWidth = 2, td::LinePattern pattern = td::LinePattern::Solid, td::String name = "line");
    void addFunction(Function && fun);
    
    void fitToWindow();

    

    const std::vector<Function>& getFunctions(){return _funkcije;}
    void changeWidth(double width, size_t function);
    void changeName(const td::String& name, size_t function);
    void changePattern(td::LinePattern pattern, size_t function);
    void changeColor(td::ColorID color, size_t function);
    void setLegendLocation(const gui::Point& location);
    void setLegendCols(int cols);
    void removeColorFromAutopicker(td::ColorID color);
    
    void Zoom(const gui::CoordType& scale);
    void ZoomToArea(gui::CoordType* minX, gui::CoordType* maxX, gui::CoordType* minY, gui::CoordType* maxY);

    void setGraphSize(const gui::Size &sz);
    void draw();

protected:
    //virtual td::LinePattern checkDefaultPattern(const td::LinePattern& pattern);
    virtual void resetGraph(){}

};

} //namepsace plot
} //namespace gui

