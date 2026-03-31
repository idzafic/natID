#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/ComboBox.h>
#include <gui/ColorPicker.h>

class ToolBarShapes : public gui::ToolBar
{
protected:
    gui::Image _img1;
    gui::Image _img2;
    gui::ComboBox _elements;
    gui::ColorPicker _colorPicker;
public:
    ToolBarShapes()
    : gui::ToolBar("Shapes", 5)
    , _img1(":poly128")
    , _img2(":rect12")
    {
        _elements.addItem("Polyline");
        _elements.addItem("Wired Polygon");
        _elements.addItem("Filled Polygon");
        _elements.addItem("Filled Polygon with wire");
        _elements.addItem("Rectangle");
        _elements.addItem("Filled Rectangle");
        _elements.addItem("Filled Rectangle with wire");
        _elements.selectIndex(0);
        _elements.forwardMessagesTo(this);
        _colorPicker.forwardMessagesTo(this);
        
        
        addItem("Polygon wired", &_img1, "Wired polygon", 10, 0, 0, 10);
        
        addItem("Edit rectangle", &_img2, "Tooltip for rect", 10, 0, 0, 20);
        
        addSpaceItem();
        
        addItem("Select shape", "Tooltip for shape selector", &_elements, 10000);
        
        addItem("Color", "Tooltip for Color Picker", &_colorPicker, 10001);

    }
};
