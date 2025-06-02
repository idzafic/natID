// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <gui/plot/Plot.h>
#include <gui/plot//Legend.h>
#include <gui/Canvas.h>
#include <gui/Image.h>
#include <vector>
#include <unordered_set>
#include <gui/DrawableString.h>
#include <gui/Font.h>
#include <queue>
#include <td/LineParams.h>
#include <td/ColorID.h>
#include <gui/Application.h>
#include <td/MinMaxRange.h>
#include <td/BandParams.h>

class Graph;

namespace gui
{
namespace plot
{


#ifdef MU_WINDOWS
const char* const cg_DefFontName = "Times New Roman";
#else
#ifdef MU_LINUX
const char* const cg_DefFontName = "Liberation Serif";
#else
//MACOS
const char* const cg_DefFontName = "Times New Roman";
#endif
#endif

    class View;

class NATPLOT_API Renderer
{
    friend class View;
public:
    static gui::Font font;
    using HelperLine = struct _HL
    {
        gui::CoordType position;
        td::MinMaxRange<gui::CoordType> range;
        td::LineParams lineParams;
    };
    
    using Band = struct _Band
    {
        td::MinMaxRange<gui::CoordType> range;
        td::BandParams bandParams;
    };
private:
    static constexpr td::ColorID SELECT_COLOR = td::ColorID::Green;

    enum class Limits : td::BYTE {xMin = 0, xMax, yMin, yMax};
//    gui::CoordType* _Limits = nullptr;
    gui::CoordType _limits[4] = {math::maxVal<double>(),math::minVal<double>(),math::maxVal<double>(),math::minVal<double>()};

    Legend _legend;

    gui::Font* _font = nullptr;
    gui::Size _size;
    
    cnt::PushBackVector<Plot> _plots;
//    std::vector<td::ColorID> _pastColors;
//
//    std::unordered_set<td::ColorID> _disallowedColors;
//    std::queue<td::ColorID> _defaultColors;
//    std::queue<std::pair<td::LinePattern, td::DotPattern>> _defaultPatterns;
    
    cnt::PushBackVector<HelperLine> _horizontalLines;
    cnt::PushBackVector<HelperLine> _verticalLines;
    
    cnt::PushBackVector<Band> _horizontalBands;
    cnt::PushBackVector<Band> _verticalBands;
    
    gui::Geometry _drawingWindow;
    gui::Rect _drawingRect;
    
    struct Margins {
        float marginTop = 20;
        float marginRight = 20;
        float marginBottom = 5;
        float marginLeft = 40;
    }_margins, _marginsZero;

    gui::DrawableString _xAxisName;
    gui::DrawableString _yAxisName;
    double _numberHeight;
    
    float _yAxisNameSeperation = 40; //150;
    float _xAxisNameSeperation = 40; //86;
    
    bool _drawMargins;
    bool _drawNumbersOutside = true;
    bool _initalDraw = false;

    bool _drawLegend = false;
    bool _legendPositionChanged = false;
    bool _active = false;
    bool _drawGrid = true;
    
    td::ColorScheme _colorScheme = td::ColorScheme::Auto;
    td::ColorID _axisColor = td::ColorID::SysText;
    td::ColorID _borderLineColor = td::ColorID::Gray;
    td::ColorID _gridColor = td::ColorID::Gray;
    
    td::BYTE _findXMin = 0;
    td::BYTE _findXMax = 0;
    td::BYTE _findYMin = 0;
    td::BYTE _findYMax = 0;
    
    
//    bool _yAxisNameIsNotNull = false;
    td::ColorID nextColor();
    
    void drawAxis();
    
//    td::String to_string(gui::CoordType x);
    inline bool checkRange(const size_t& number) { return number > _plots.size(); }
    void setUpDrawingWindow();
    void finishAddingFunction(Plot& newFun);
    
    void updateLimits(const Plot& newFun);
    void zoomToWindow(const gui::Geometry& window);
    void moveGraph(const gui::CoordType &x, const gui::CoordType& y);
    void onSizeChanged();
    bool isDarkMode() const;
//    Function::Pattern checkDefaultPattern(const Function::Pattern &pattern);
    virtual void resetGraph(){}
    
protected:
    void setBounds(const gui::Rect& bounds);
    
public:
    using Pattern = Plot::Pattern;
    virtual void drawAgain(){}

    Renderer(gui::Font *axisFont, gui::Font *legendFont, td::ColorScheme colorScheme, bool startWithMargins);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
//    ~Renderer();
    void reset();
    
    void setAxisColor(td::ColorID boja);
    td::ColorID getAxisColor() { return _axisColor; }
    void setxAxisName(const td::String& xName, bool bRedraw = true);
    void setyAxisName(const td::String& yName, bool bRedraw = true);
    const td::String getxAxisName() { return _xAxisName.toString(); }
    const td::String getyAxisName() { return _yAxisName.toString(); }
    void setAxisNameDistance(float xNameDistanceFromAxis, float yNameDistanceFromAxis);
    void getAxisNameDistance(float& xNameDistanceFromAxis, float& yNameDistanceFromAxis);

    void reservePlots(size_t nPlots, size_t nHorizontalLines, size_t nVerticalLines, size_t nHorizontalBands, size_t nVerticalBands, size_t nLegends);

    void setAxisFontSize(float fs, bool bRedraw);

    void setMargins(float left, float right, float bottom, float top);
    void getMargins(float &left, float &right, float &bottom, float &top);
    void showLegend(bool draw);
    void showGrid(bool draw) { _drawGrid = draw; drawAgain(); }
    
    void addFunction2(const gui::CoordType* x, const gui::CoordType* y, size_t length,  const td::String& name, const td::LineParams& lineParams);
    
    void addFunction3(const gui::Point* points, size_t length, const td::String& name, const td::LineParams& lineParams);
    
    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, td::ColorID color, float lineWidth = 2, Pattern pattern = Pattern(), td::String name = "line");
    
    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, float lineWidth = 2, Pattern pattern = Pattern(), td::String name = "line");
//    void addFunction(Function && fun);
    
    void addHorizontalBand(const td::String& name, const td::BandParams bandParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    void addVerticalBand(const td::String& name, const td::BandParams bandParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    void addHorizontalLine(const td::String& name, gui::CoordType pos, const td::LineParams& lineParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    void addVerticalLine(const td::String& name, gui::CoordType pos, const td::LineParams& lineParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    
    
    void showAsInitial();

    const cnt::PushBackVector<Plot>& getFunctions(){return _plots;}
    void changeWidth(double width, size_t function);
    void changeName(const td::String& name, size_t function);
    void changePattern(Pattern pattern, size_t function);
    void changeColor(td::ColorID color, size_t function);
    void setLegendLocation(const gui::Point& location);
    void setLegendCols(size_t cols);
    size_t getLegendCols() const;
//    void removeColorFromAutopicker(td::ColorID color);
    
    void Zoom(const gui::CoordType& scale);
    void ZoomToArea(gui::CoordType *minX, gui::CoordType *maxX, gui::CoordType *minY, gui::CoordType *maxY);
    void setGraphSize(const gui::Size &sz);
    void draw(td::ColorID backColor);
    
    void getProps(gui::plot::Plot::Props& props) const;
//    void setProps(const gui::Plot::Props& props);
};

} //namepsace plot
} //namespace gui

