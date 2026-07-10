// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Renderer.h
    @brief Chart renderer that manages plot data, axes, legend, grid, and drawing operations. */
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

    class View;

/// @brief Core chart rendering engine that owns plot data series, axes, helper lines,
///        bands, and the legend, and draws them onto a canvas.
class NATPLOT_API Renderer
{
    friend class View;
public:
    static gui::Font font; ///< Default font shared across all Renderer instances.

    /// @brief Describes a horizontal or vertical helper line at a fixed axis position.
    using HelperLine = struct _HL
    {
        gui::CoordType position;              ///< Axis coordinate at which the helper line is drawn.
        td::MinMaxRange<gui::CoordType> range; ///< Range along the perpendicular axis over which the line is drawn.
        td::LineParams lineParams;             ///< Visual parameters (color, pattern, width) of the helper line.
    };

    /// @brief Describes a horizontal or vertical shaded band spanning an axis range.
    using Band = struct _Band
    {
        td::MinMaxRange<gui::CoordType> range; ///< Axis range covered by the band.
        td::BandParams bandParams;             ///< Visual parameters (fill color, transparency) of the band.
    };
private:
    static constexpr td::ColorID SELECT_COLOR = td::ColorID::Green; ///< Color used to highlight selected plot elements.

    /// @brief Index aliases for the four axis limits stored in _limits.
    enum class Limits : td::BYTE {
        xMin = 0, ///< Index of the minimum X-axis value.
        xMax,     ///< Index of the maximum X-axis value.
        yMin,     ///< Index of the minimum Y-axis value.
        yMax      ///< Index of the maximum Y-axis value.
    };
//    gui::CoordType* _Limits = nullptr;
    gui::CoordType _limits[4] = {math::maxVal<double>(),math::minVal<double>(),math::maxVal<double>(),math::minVal<double>()}; ///< Current axis extents: [xMin, xMax, yMin, yMax].

    Legend _legend; ///< Legend widget listing all active plots, bands, and helper lines.

    gui::Font* _font = nullptr; ///< Font used to render axis tick labels.
    gui::Size _size;            ///< Current pixel size of the drawing area.

    cnt::PushBackVector<Plot> _plots; ///< Ordered list of data plots managed by this renderer.
//    std::vector<td::ColorID> _pastColors;
//
//    std::unordered_set<td::ColorID> _disallowedColors;
//    std::queue<td::ColorID> _defaultColors;
//    std::queue<std::pair<td::LinePattern, td::DotPattern>> _defaultPatterns;

    cnt::PushBackVector<HelperLine> _horizontalLines; ///< Horizontal helper lines drawn across the plot area.
    cnt::PushBackVector<HelperLine> _verticalLines;   ///< Vertical helper lines drawn across the plot area.

    cnt::PushBackVector<Band> _horizontalBands; ///< Horizontal shaded bands spanning Y-axis ranges.
    cnt::PushBackVector<Band> _verticalBands;   ///< Vertical shaded bands spanning X-axis ranges.

    gui::Geometry _drawingWindow; ///< Viewport geometry defining the active drawing region.
    gui::Rect _drawingRect;       ///< Rectangle within which axis ticks, labels, and plots are drawn.

    /// @brief Internal margin values controlling spacing around the plot area.
    struct Margins {
        float marginTop = 20;    ///< Top margin in pixels.
        float marginRight = 20;  ///< Right margin in pixels.
        float marginBottom = 5;  ///< Bottom margin in pixels.
        float marginLeft = 5;    ///< Left margin in pixels.
    }_margins, _marginsZero; ///< Active margins and zero-margin fallback used when margins are hidden.

    gui::DrawableString _xAxisName; ///< Drawable label for the X axis.
    gui::DrawableString _yAxisName; ///< Drawable label for the Y axis.
    double _numberHeight;           ///< Height of a numeric tick label in pixels, used for layout.

    gui::Point _legendAnchorOffset; ///< Offset of the legend from its anchor point.

    //td::BYTE nLegendColumns = 1;
    //bool showLegend = false;

    float _yAxisNameSeperation = 60; //150; ///< Distance in pixels between the Y-axis line and its name label.
    float _xAxisNameSeperation = 60; //86;  ///< Distance in pixels between the X-axis line and its name label.

    td::Anchor _legendAnchor = td::Anchor::Fixed; ///< Anchor point that determines legend repositioning behavior.
    bool _drawMargins;              ///< True if plot area margins are currently visible.
    bool _drawNumbersOutside = true; ///< True to draw axis tick numbers outside the drawing rectangle.
    bool _initalDraw = false;       ///< True after the first draw call has completed.

    bool _drawLegend = false;           ///< True if the legend should be drawn.
    bool _legendPositionChanged = false; ///< True when the legend position has been modified since the last draw.
    bool _active = false;               ///< True if the renderer is in an interactive (active) state.
    bool _drawGrid = true;              ///< True if the background grid should be drawn.

    td::ColorScheme _colorScheme = td::ColorScheme::Auto; ///< Active color scheme (Auto, Light, or Dark).
    td::ColorID _axisColor = td::ColorID::SysText;       ///< Color used for axis lines and tick labels.
    td::ColorID _borderLineColor = td::ColorID::Gray;    ///< Color used for the plot border rectangle.
    td::ColorID _gridColor = td::ColorID::Gray;          ///< Color used for the background grid lines.

    td::BYTE _findXMin = 0; ///< Non-zero when the X-axis minimum should be auto-computed from data.
    td::BYTE _findXMax = 0; ///< Non-zero when the X-axis maximum should be auto-computed from data.
    td::BYTE _findYMin = 0; ///< Non-zero when the Y-axis minimum should be auto-computed from data.
    td::BYTE _findYMax = 0; ///< Non-zero when the Y-axis maximum should be auto-computed from data.


//    bool _yAxisNameIsNotNull = false;
    /// @brief Returns the next auto-selected plot color from the default color sequence.
    /// @return The next available color ID.
    td::ColorID nextColor();

    /// @brief Draws the X and Y axis lines, ticks, and numeric labels.
    void drawAxis();

//    td::String to_string(gui::CoordType x);
    /// @brief Returns true if the given index is out of range of the plots vector.
    /// @param number Zero-based index to check.
    /// @return True if the index exceeds the number of plots.
    inline bool checkRange(const size_t& number) { return number > _plots.size(); }
    /// @brief Recalculates the drawing window and margins based on the current size and axis labels.
    void setUpDrawingWindow();
    /// @brief Finalizes a newly added plot by updating limits and the legend.
    /// @param newFun The plot that was just added.
    void finishAddingFunction(Plot& newFun);

    /// @brief Expands the axis limits to accommodate the data range of the given plot.
    /// @param newFun The plot whose data range is used to update the limits.
    void updateLimits(const Plot& newFun);
    /// @brief Zooms the view to display exactly the given geometry window.
    /// @param window The target geometry window in data coordinates.
    void zoomToWindow(const gui::Geometry& window);
    /// @brief Translates the visible graph region by the given data-coordinate offsets.
    /// @param x Horizontal translation in data coordinates.
    /// @param y Vertical translation in data coordinates.
    void moveGraph(const gui::CoordType &x, const gui::CoordType& y);
    /// @brief Called when the renderer's pixel size changes; recomputes layout.
    void onSizeChanged();
    /// @brief Returns true if the application is currently in dark color mode.
    /// @return True when dark mode is active.
    bool isDarkMode() const;
//    Function::Pattern checkDefaultPattern(const Function::Pattern &pattern);
    /// @brief Override hook called to reset the graph to its initial state; default is no-op.
    virtual void resetGraph(){}

protected:
    /// @brief Sets the data-coordinate bounding box for the visible axis range.
    /// @param bounds The rectangle defining xMin, xMax, yMin, yMax in data coordinates.
    void setBounds(const gui::Rect& bounds);
    /// @brief Validates and clamps the current axis limits to avoid degenerate ranges.
    void checkLimits();
    /// @brief Repositions the legend relative to the current drawing window.
    void updateLegendLocation();
public:
    using Pattern = Plot::Pattern; ///< Convenience alias for the plot line pattern type.
    /// @brief Override hook to request a canvas redraw; default is no-op.
    virtual void drawAgain(){}

    /// @brief Constructs a Renderer with the specified fonts, color scheme, and margin setting.
    /// @param axisFont Font used to render axis tick labels.
    /// @param legendFont Font used to render legend entry labels.
    /// @param colorScheme Initial color scheme (Auto, Light, or Dark).
    /// @param startWithMargins True to begin rendering with margins enabled.
    Renderer(gui::Font *axisFont, gui::Font *legendFont, td::ColorScheme colorScheme, bool startWithMargins);
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

//    ~Renderer();
    /// @brief Clears all plots, helper lines, bands, and resets axis limits.
    void reset();

    /// @brief Sets the color used for axis lines, ticks, and labels.
    /// @param boja The new axis color ID.
    void setAxisColor(td::ColorID boja);
    /// @brief Returns the current axis color.
    /// @return The axis color ID.
    td::ColorID getAxisColor() { return _axisColor; }
    /// @brief Sets the label text for the X axis.
    /// @param xName New X-axis name string.
    /// @param bRedraw True to trigger a redraw after setting the name.
    void setxAxisName(const td::String& xName, bool bRedraw = true);
    /// @brief Sets the label text for the Y axis.
    /// @param yName New Y-axis name string.
    /// @param bRedraw True to trigger a redraw after setting the name.
    void setyAxisName(const td::String& yName, bool bRedraw = true);
    /// @brief Returns the current X-axis label string.
    /// @return The X-axis name as a td::String.
    const td::String getxAxisName() { return _xAxisName.toString(); }
    /// @brief Returns the current Y-axis label string.
    /// @return The Y-axis name as a td::String.
    const td::String getyAxisName() { return _yAxisName.toString(); }
    /// @brief Sets the pixel distance between each axis line and its name label.
    /// @param xNameDistanceFromAxis Distance in pixels for the X-axis label.
    /// @param yNameDistanceFromAxis Distance in pixels for the Y-axis label.
    void setAxisNameDistance(float xNameDistanceFromAxis, float yNameDistanceFromAxis);
    /// @brief Retrieves the current pixel distances between axis lines and their name labels.
    /// @param xNameDistanceFromAxis Output: distance for the X-axis label.
    /// @param yNameDistanceFromAxis Output: distance for the Y-axis label.
    void getAxisNameDistance(float& xNameDistanceFromAxis, float& yNameDistanceFromAxis);

    /// @brief Pre-allocates storage for the expected number of plots, helper lines, bands, and legend entries.
    /// @param nPlots Expected number of data plots.
    /// @param nHorizontalLines Expected number of horizontal helper lines.
    /// @param nVerticalLines Expected number of vertical helper lines.
    /// @param nHorizontalBands Expected number of horizontal bands.
    /// @param nVerticalBands Expected number of vertical bands.
    /// @param nLegends Expected number of legend entries.
    void reservePlots(size_t nPlots, size_t nHorizontalLines, size_t nVerticalLines, size_t nHorizontalBands, size_t nVerticalBands, size_t nLegends);

    /// @brief Changes the font size used for axis tick labels.
    /// @param fs New font size in points.
    /// @param bRedraw True to trigger a redraw after changing the font size.
    void setAxisFontSize(float fs, bool bRedraw);

    /// @brief Sets the pixel margins around the plot drawing area.
    /// @param left Left margin in pixels.
    /// @param right Right margin in pixels.
    /// @param bottom Bottom margin in pixels.
    /// @param top Top margin in pixels.
    void setMargins(float left, float right, float bottom, float top);
    /// @brief Retrieves the current pixel margins around the plot drawing area.
    /// @param left Output: left margin in pixels.
    /// @param right Output: right margin in pixels.
    /// @param bottom Output: bottom margin in pixels.
    /// @param top Output: top margin in pixels.
    void getMargins(float &left, float &right, float &bottom, float &top);
    /// @brief Shows or hides the plot legend.
    /// @param draw True to draw the legend, false to hide it.
    void showLegend(bool draw);
    /// @brief Shows or hides the background grid.
    /// @param draw True to draw the grid, false to hide it.
    void showGrid(bool draw) { _drawGrid = draw; drawAgain(); }

    /// @brief Adds a plot from separate X and Y coordinate arrays with explicit line parameters.
    /// @param x Pointer to the array of X coordinate values.
    /// @param y Pointer to the array of Y coordinate values.
    /// @param length Number of data points.
    /// @param name Display name for the plot (shown in the legend).
    /// @param lineParams Visual line parameters (color, pattern, width).
    void addFunction2(const gui::CoordType* x, const gui::CoordType* y, size_t length,  const td::String& name, const td::LineParams& lineParams);

    /// @brief Adds a plot from an array of points with an optional original-data array and line parameters.
    /// @param pts Pointer to the display points array.
    /// @param ptsOrig Pointer to the original (un-transformed) points array.
    /// @param length Number of data points.
    /// @param name Display name for the plot.
    /// @param lineParams Visual line parameters.
    void addFunction2(gui::Point* pts, gui::Point* ptsOrig, size_t length, const td::String& name, const td::LineParams& lineParams);

    /// @brief Adds a plot from a read-only array of points with line parameters.
    /// @param points Pointer to the const array of 2D data points.
    /// @param length Number of data points.
    /// @param name Display name for the plot.
    /// @param lineParams Visual line parameters.
    void addFunction3(const gui::Point* points, size_t length, const td::String& name, const td::LineParams& lineParams);

    /// @brief Adds a plot with full control over color, width, pattern, and name.
    /// @param x Pointer to the array of X coordinate values.
    /// @param y Pointer to the array of Y coordinate values.
    /// @param length Number of data points.
    /// @param color Line color ID.
    /// @param lineWidth Line width in pixels.
    /// @param pattern Line dash/dot pattern.
    /// @param name Display name for the plot.
    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, td::ColorID color, float lineWidth = 2, Pattern pattern = Pattern(), td::String name = "line");

    /// @brief Adds a plot using the next auto-selected color.
    /// @param x Pointer to the array of X coordinate values.
    /// @param y Pointer to the array of Y coordinate values.
    /// @param length Number of data points.
    /// @param lineWidth Line width in pixels.
    /// @param pattern Line dash/dot pattern.
    /// @param name Display name for the plot.
    void addFunction(gui::CoordType* x, gui::CoordType* y, size_t length, float lineWidth = 2, Pattern pattern = Pattern(), td::String name = "line");
//    void addFunction(Function && fun);

    /// @brief Adds a horizontal shaded band to the plot area.
    /// @param name Display name for the band (shown in the legend if showInLegend is true).
    /// @param bandParams Visual fill parameters for the band.
    /// @param range Y-axis range covered by the band.
    /// @param showInLegend True to add an entry for this band in the legend.
    void addHorizontalBand(const td::String& name, const td::BandParams bandParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    /// @brief Adds a vertical shaded band to the plot area.
    /// @param name Display name for the band.
    /// @param bandParams Visual fill parameters for the band.
    /// @param range X-axis range covered by the band.
    /// @param showInLegend True to add an entry for this band in the legend.
    void addVerticalBand(const td::String& name, const td::BandParams bandParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    /// @brief Adds a horizontal helper line at a fixed Y position.
    /// @param name Display name for the line.
    /// @param pos Y-axis value at which the line is drawn.
    /// @param lineParams Visual line parameters.
    /// @param range X-axis range over which the line is drawn.
    /// @param showInLegend True to add an entry for this line in the legend.
    void addHorizontalLine(const td::String& name, gui::CoordType pos, const td::LineParams& lineParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);
    /// @brief Adds a vertical helper line at a fixed X position.
    /// @param name Display name for the line.
    /// @param pos X-axis value at which the line is drawn.
    /// @param lineParams Visual line parameters.
    /// @param range Y-axis range over which the line is drawn.
    /// @param showInLegend True to add an entry for this line in the legend.
    void addVerticalLine(const td::String& name, gui::CoordType pos, const td::LineParams& lineParams, const td::MinMaxRange<gui::CoordType>& range, bool showInLegend);


    /// @brief Resets the view to the initial data extents as if viewing for the first time.
    void showAsInitial();

    /// @brief Returns a const reference to the list of all managed plot objects.
    /// @return Const reference to the plots vector.
    const cnt::PushBackVector<Plot>& getFunctions(){return _plots;}
    /// @brief Changes the line width of the plot at the given index.
    /// @param width New line width in pixels.
    /// @param function Zero-based index of the plot to modify.
    void changeWidth(double width, size_t function);
    /// @brief Changes the display name of the plot at the given index.
    /// @param name New display name string.
    /// @param function Zero-based index of the plot to modify.
    void changeName(const td::String& name, size_t function);
    /// @brief Changes the line pattern of the plot at the given index.
    /// @param pattern New line pattern.
    /// @param function Zero-based index of the plot to modify.
    void changePattern(Pattern pattern, size_t function);
    /// @brief Changes the color of the plot at the given index.
    /// @param color New color ID.
    /// @param function Zero-based index of the plot to modify.
    void changeColor(td::ColorID color, size_t function);
    /// @brief Moves the legend to an absolute position.
    /// @param location New top-left position of the legend in view coordinates.
    void setLegendLocation(const gui::Point& location);
    /// @brief Adjusts a drag delta to keep the legend within its anchor constraints.
    /// @param delta In/out drag delta that may be clamped by the anchor.
	void adjustDeltaToAnchor(gui::Point& delta);
    /// @brief Moves the legend by the given delta relative to its current position.
    /// @param delta Translation vector in view coordinates.
	void incrementLegendLocation(gui::Point& delta);
    /// @brief Sets the number of columns in the legend layout.
    /// @param cols Desired column count.
    void setLegendCols(size_t cols);
    /// @brief Configures legend layout with position, column count, anchor, and visibility.
    /// @param offset Offset from the anchor point.
    /// @param cols Number of legend columns.
    /// @param anchor Anchor type controlling automatic repositioning.
    /// @param bShowLegend True to make the legend visible.
    void setLegendCols(const gui::Point& offset, size_t cols, td::Anchor anchor, bool bShowLegend);
    /// @brief Returns the current number of legend columns.
    /// @return Column count.
    size_t getLegendCols() const;
//    void removeColorFromAutopicker(td::ColorID color);

    /// @brief Scales the current axis limits by the given factor, zooming in or out around the center.
    /// @param scale Zoom factor (> 1 zooms in, < 1 zooms out).
    void Zoom(const gui::CoordType& scale);
    /// @brief Zooms the view to exactly fit the specified data-coordinate bounding box.
    /// @param minX Pointer to the minimum X value (or nullptr to keep current).
    /// @param maxX Pointer to the maximum X value (or nullptr to keep current).
    /// @param minY Pointer to the minimum Y value (or nullptr to keep current).
    /// @param maxY Pointer to the maximum Y value (or nullptr to keep current).
    void ZoomToArea(gui::CoordType *minX, gui::CoordType *maxX, gui::CoordType *minY, gui::CoordType *maxY);
    /// @brief Updates the pixel size of the drawing area and triggers a layout recompute.
    /// @param sz New size of the graph area in pixels.
    void setGraphSize(const gui::Size &sz);
    /// @brief Performs a full draw of all chart elements onto the current canvas.
    /// @param backColor Background color to fill before drawing.
    void draw(td::ColorID backColor);

    /// @brief Populates a Props structure with the current axis limits.
    /// @param props Output structure to fill with axis range properties.
    void getProps(gui::plot::Plot::Props& props) const;

    /// @brief Returns the current legend anchor offset.
    /// @return Const reference to the legend anchor offset point.
    const gui::Point& getLegendAnchorOffset() const;

    /// @brief Returns the current legend anchor type.
    /// @return The td::Anchor value controlling legend repositioning.
    td::Anchor getLegendAnchor() const;
//    void setProps(const gui::Plot::Props& props);
};

} //namepsace plot
} //namespace gui
