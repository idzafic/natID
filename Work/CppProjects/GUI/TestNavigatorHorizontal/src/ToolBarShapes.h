#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/ComboBox.h>

class ToolBarShapes : public gui::ToolBar
{
protected:
    gui::Image _img1;
    gui::Image _img2;
    gui::ComboBox _elements;
public:
    ToolBarShapes()
    : gui::ToolBar("Shapes", 5, 4)
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
        
        _allowedItems[0].init("Polygon", "Polygon wired", &_img1, "Wired polygon", 10, 0, 0, 10);
        _defaultItems[0]=0;
        
        _allowedItems[1].init("Rectangle", "Edit rectangle", &_img2, "Tooltip for rect", 10, 0, 0, 20);
        _defaultItems[1]=1;
        
        _allowedItems[2].initAsSpace();
        _defaultItems[2]=2;
        
        _allowedItems[3].init("ComboShapes", "Select shape", "Tooltip for shape selector", &_elements, 10000);
        _defaultItems[3]=3;
        
        _allowedItems[4].initAsSpace();
    }
};
