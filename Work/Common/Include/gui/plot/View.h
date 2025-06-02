// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATPLOT_API View : public gui::Canvas, public gui::plot::Renderer
{
public:
    
    
    using Pattern = Plot::Pattern;
    enum class Action : td::BYTE {Reset=0, GridOnOf, LegendOnOff, Export, Settings, MinMarginOnOff, ShowInfo, DeleteInfo, Help};
private:
    enum class InternAction : td::BYTE { None=0, Select, SecondaryClick, Drag, PointPeek, MoveLegend };
    friend class gui::plot::Settings;
    static const std::initializer_list<gui::InputDevice::Event> inputs;
    static const std::initializer_list<gui::InputDevice::Event> noInputs;
    gui::Point _lastMousePos;
    gui::Transformation _tr;
    gui::Point _trPoint;
    gui::Size _realSize;
    gui::Rect _selectRect;
    td::String _txtPut, _lastPath;
//    struct ImageSaveSettings
//    {
//        enum class ImageSaveStyle{WindowSize=0, FixedWidth, FixedHeight, Fixed};
//        double _width, _height;
//        ImageSaveStyle _mode;
//    } _imageSaveSettings;
    
    InternAction _internAction = InternAction::None;
    td::ColorID _backgroundColor;
private:
//    void setColorsAndPatternStack();
    void resetGraph() override;

protected:
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onSecondaryButtonReleased(const gui::InputDevice& inputDevice) override;
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    bool onZoom(const gui::InputDevice& inputDevice) override;
    bool onKeyPressed(const gui::Key& key) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice) override;
    bool onScroll(const gui::InputDevice& inputDevice) override;
    
    void onDraw(const gui::Rect& rect) override;
    void onResize(const gui::Size& newSize) override;
    
public:
    
    
    View(gui::Font *fontAxis, gui::Font *fontLegend, td::ColorScheme colorScheme, bool takeUserInput = true);
    
    View(const View&) = delete;
    View& operator=(const View&) = delete;
    
    void setBackgroundColor(td::ColorID color);
    td::ColorID getBackgroundColor() { return _backgroundColor; }
    void drawButtons(bool draw);
    
    void showInformation();
    void showHelp();
    void saveMenu();
    void readTXT(const td::String& path);
    void readXML(const td::String& path, bool onlyData);
    
    void handleAction(Action actionID);
    static inline const char * buttonsImages[] = {":@PlotFullScreen", ":@PlotGrid", ":@PlotLegend", ":@PlotSave", ":@PlotReset", ":@PlotSettings", ":@PlotShowAnotation", ":@PlotDeleteAnnotation", ":@PlotHelp"};
    bool saveXML(const td::String& path);
    void saveTXT(const td::String& path);
    bool saveTXT(const td::String& path, bool horizontal);
    bool openFile(td::String path, bool readOnlyFunctions = false);
    bool save(const td::String& path);
    
    void drawAgain() override
    {
        reDraw();
    }
    
    static double ExportToImageResolutionScale;
    void systemColorModeChanged(bool bDarkMode) override;
};

} //namepsace plot
} //namespace gui
