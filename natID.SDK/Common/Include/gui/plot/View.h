// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file View.h
    @brief Interactive plot view that combines canvas rendering with the chart Renderer and user input handling. */
#pragma once
#include <gui/plot/natPlotLib.h>
#include <gui/plot/Renderer.h>
#include <gui/Canvas.h>
#include <gui/Transformation.h>
#include <td/ColorID.h>

namespace gui
{
namespace plot
{

class Settings;

/// @brief A fully interactive chart view that inherits both gui::Canvas and gui::plot::Renderer,
///        providing zoom, pan, selection, legend dragging, annotation, and file I/O.
class NATPLOT_API View : public gui::Canvas, public gui::plot::Renderer
{
public:

    using Pattern = Plot::Pattern; ///< Convenience alias for the plot line pattern type.
    /// @brief High-level user-facing actions that can be triggered programmatically or via toolbar buttons.
    enum class Action : td::BYTE {
        Reset=0,          ///< Reset the chart to its initial data extents.
        GridOnOf,         ///< Toggle the background grid on or off.
        LegendOnOff,      ///< Toggle the legend visibility.
        Export,           ///< Export the chart to an image file.
        Settings,         ///< Open the chart settings dialog.
        MinMarginOnOff,   ///< Toggle between full margins and minimal margins.
        ShowInfo,         ///< Show a data-point annotation overlay.
        DeleteInfo,       ///< Remove all data-point annotations.
        Help              ///< Show the chart help dialog.
    };
private:
    /// @brief Internal state machine actions driven by mouse/touch events.
    enum class InternAction : td::BYTE {
        None=0,          ///< No active interaction.
        Select,          ///< User is drawing a selection rectangle.
        SecondaryClick,  ///< Secondary (right) button was pressed.
        Drag,            ///< User is panning the chart.
        PointPeek,       ///< User is hovering to inspect a data point.
        MoveLegend       ///< User is dragging the legend to a new position.
    };
    friend class gui::plot::Settings;
    static const std::initializer_list<gui::InputDevice::Event> inputs;   ///< Default set of input events handled by the view.
    static const std::initializer_list<gui::InputDevice::Event> noInputs; ///< Empty input event set used when user interaction is disabled.
    gui::Point _lastMousePos;         ///< Last recorded mouse cursor position in view coordinates.
    gui::Transformation _tr;          ///< Coordinate transformation applied to the chart drawing.
    gui::Point _trPoint;              ///< Reference point for the current transformation.
    gui::Size _realSize;              ///< Actual pixel size of the view.
    gui::Rect _selectRect;            ///< Current rubber-band selection rectangle in view coordinates.
    td::String _txtPut, _lastPath;    ///< Annotation text buffer and last used file path for open/save dialogs.
//    struct ImageSaveSettings
//    {
//        enum class ImageSaveStyle{WindowSize=0, FixedWidth, FixedHeight, Fixed};
//        double _width, _height;
//        ImageSaveStyle _mode;
//    } _imageSaveSettings;
    //td::Point<double> _legendAnchorOffset;
    //td::Anchor _legendAnchor = td::Anchor::Fixed;
    InternAction _internAction = InternAction::None; ///< Currently active internal interaction state.
    td::ColorID _backgroundColor;                    ///< Background fill color of the chart view.
    static gui::Point prevPosition;                  ///< Previous mouse position used to compute drag delta.
private:
//    void setColorsAndPatternStack();
    /// @brief Resets the chart to its initial axis extents (overrides Renderer::resetGraph).
    void resetGraph() override;

protected:

    /// @brief Handles primary (left) mouse button press to begin selection or annotation.
    /// @param inputDevice Input device state at the time of the event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    /// @brief Handles primary mouse button release to finalize selection or zoom.
    /// @param inputDevice Input device state at the time of the event.
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
    /// @brief Handles secondary (right) mouse button press to begin context-menu preparation.
    /// @param inputDevice Input device state at the time of the event.
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    /// @brief Handles secondary mouse button release, typically to show a context menu.
    /// @param inputDevice Input device state at the time of the event.
    void onSecondaryButtonReleased(const gui::InputDevice& inputDevice) override;
    /// @brief Handles mouse cursor movement for data-point peeking or legend dragging.
    /// @param inputDevice Input device state including current cursor position.
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
    /// @brief Handles mouse drag events for panning or legend repositioning.
    /// @param inputDevice Input device state including current cursor position and button state.
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    /// @brief Handles pinch or scroll-wheel zoom gestures.
    /// @param inputDevice Input device state including zoom scale factor.
    /// @return True if the zoom event was consumed.
    bool onZoom(const gui::InputDevice& inputDevice) override;
    /// @brief Handles keyboard key press events (e.g., Escape to cancel selection).
    /// @param key The key that was pressed.
    /// @return True if the key event was consumed.
    bool onKeyPressed(const gui::Key& key) override;
    /// @brief Called when the cursor leaves the view area.
    /// @param inputDevice Input device state at the time of the event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the cursor enters the view area.
    /// @param inputDevice Input device state at the time of the event.
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    /// @brief Handles primary button double-click to reset zoom to fit all data.
    /// @param inputDevice Input device state at the time of the event.
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
    /// @brief Handles scroll events for panning or zooming.
    /// @param inputDevice Input device state including scroll delta.
    /// @return True if the scroll event was consumed.
    bool onScroll(const gui::InputDevice& inputDevice) override;

    /// @brief Draws the chart into the given dirty rectangle.
    /// @param rect The rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;
    /// @brief Called when the view is resized; updates the renderer and redraws.
    /// @param newSize The new pixel size of the view.
    void onResize(const gui::Size& newSize) override;

public:


    /// @brief Constructs a plot View with the specified fonts, color scheme, and optional user input.
    /// @param fontAxis Font used to render axis tick labels.
    /// @param fontLegend Font used to render legend entry labels.
    /// @param colorScheme Initial color scheme (Auto, Light, or Dark).
    /// @param takeUserInput True to register mouse/keyboard/touch event handlers.
    View(gui::Font *fontAxis, gui::Font *fontLegend, td::ColorScheme colorScheme, bool takeUserInput = true);

    View(const View&) = delete;
    View& operator=(const View&) = delete;

    /// @brief Sets the background fill color of the chart view.
    /// @param color The color ID to use as the background.
    void setBackgroundColor(td::ColorID color);
    /// @brief Returns the current background fill color.
    /// @return The background color ID.
    td::ColorID getBackgroundColor() { return _backgroundColor; }
    /// @brief Shows or hides the toolbar overlay buttons.
    /// @param draw True to render the overlay buttons.
    void drawButtons(bool draw);

    /// @brief Opens a dialog displaying information about the currently selected data point.
    void showInformation();
    /// @brief Opens the chart help dialog.
    void showHelp();
    /// @brief Opens the file save dialog for exporting the chart.
    void saveMenu();
    /// @brief Reads plot data from a plain-text file.
    /// @param path Path to the text file containing plot data.
    void readTXT(const td::String& path);
    /// @brief Reads plot data from an XML file, optionally importing only data without style.
    /// @param path Path to the XML file.
    /// @param onlyData True to import data series only, ignoring visual settings.
    void readXML(const td::String& path, bool onlyData);

    /// @brief Dispatches a high-level chart action by its Action ID.
    /// @param actionID The action to execute.
    void handleAction(Action actionID);
    static inline const char * buttonsImages[] = {":@PlotFullScreen", ":@PlotGrid", ":@PlotLegend", ":@PlotSave", ":@PlotReset", ":@PlotSettings", ":@PlotShowAnotation", ":@PlotDeleteAnnotation", ":@PlotHelp"}; ///< Resource IDs for the overlay toolbar button images.
    /// @brief Saves the chart data to an XML file.
    /// @param path Destination file path.
    /// @return True if the file was saved successfully.
    bool saveXML(const td::String& path);
    /// @brief Saves the chart data to a plain-text file.
    /// @param path Destination file path.
    void saveTXT(const td::String& path);
    /// @brief Saves the chart data to a plain-text file, optionally in horizontal layout.
    /// @param path Destination file path.
    /// @param horizontal True to write data in row-per-series (horizontal) format.
    /// @return True if the file was saved successfully.
    bool saveTXT(const td::String& path, bool horizontal);
    /// @brief Opens and loads a chart data file (auto-detects XML or text format).
    /// @param path Path to the file to open.
    /// @param readOnlyFunctions True to import data series only without changing visual settings.
    /// @return True if the file was opened and parsed successfully.
    bool openFile(td::String path, bool readOnlyFunctions = false);
    /// @brief Saves the chart to a file using a format inferred from the path extension.
    /// @param path Destination file path.
    /// @return True if the file was saved successfully.
    bool save(const td::String& path);

    /// @brief Requests a canvas redraw by delegating to reDraw().
    void drawAgain() override
    {
        reDraw();
    }

    static double ExportToImageResolutionScale; ///< Scale factor applied when exporting the chart to an image file.
    /// @brief Called when the system color mode (light/dark) changes; triggers a full redraw.
    /// @param bDarkMode True if the system switched to dark mode.
    void systemColorModeChanged(bool bDarkMode) override;
};

} //namepsace plot
} //namespace gui
