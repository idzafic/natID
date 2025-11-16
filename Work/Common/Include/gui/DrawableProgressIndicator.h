// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/DrawableString.h>

namespace gui
{

class NATGUI_API DrawableProgressIndicator
{
public:
    enum  class Type : unsigned char {NA=0, Rect, Circle, FillCircle};
    
protected:
    gui::Shape _shapeEmpty;
    gui::Shape _shapeFull;
    gui::DrawableString _str;
    gui::Size _size;
    double _progres;
    gui::Font* _pFont = nullptr;
    float _lineWidth = 3.0f;
    gui::Font::ID _fontID = gui::Font::ID::NA;
    Type _type;
    td::ColorID _colorEmpty;
    td::ColorID _colorFull;
    td::BYTE _prevDisplayed = 255;
    //td::BYTE _systemColors = 1;
protected:
    DrawableProgressIndicator(const DrawableProgressIndicator& pi) = delete;
    DrawableProgressIndicator& operator=(const DrawableProgressIndicator&) = delete;
    void createShapes();
public:
    DrawableProgressIndicator(const gui::Size& sz, Type type, gui::Font::ID fntID = gui::Font::ID::SystemNormal);
    DrawableProgressIndicator(const gui::Size& sz, Type type, gui::Font* pFont);
    
    // Move constructor
    DrawableProgressIndicator(DrawableProgressIndicator&& pi) noexcept;
    // Move assignment operator
    DrawableProgressIndicator& operator=(DrawableProgressIndicator&& dpi) noexcept;
//    
//    ~DrawableProgressIndicator();

    void setLineWidth(float lw);
    
    void setColors(td::ColorID colorEmpty, td::ColorID _colorFull);
    
    void draw(const gui::Point& tl);
    
    //value has to be between 0 and 1
    void setValue(double value);
    double getValue() const;
    
    const gui::Size& getSize() const;
};

} //namepace gui

