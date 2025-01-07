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
#include <deque>
#include <gui/DrawableString.h>
#include <vector>
#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/plot/Function.h>

namespace gui
{
namespace plot
{

class NATPLOT_API Legend {
    std::deque<gui::DrawableString> imena;
    std::vector<td::ColorID> colors;
    gui::Rect _window;
    size_t _columns = 2;
    std::vector<double> _lengths;
    gui::Font *_font;
    
    static constexpr double offsetToName = 13;
    static constexpr double offsetColumn = 18;
    static constexpr double offsetHeight = 10;
    static constexpr double rectSize = 20;
    
public:
    td::ColorID textColor;
    
    void changeLocation(const gui::Point &location);
    Legend(td::ColorID textColor, gui::Font *font, const gui::Point &location);
    
    int getPerColumn();
    
    void draw();
    
    void changeName(const td::String& name, size_t poz);
    
    void changeColor(td::ColorID color, size_t poz);
    
    void changeColumnCnt(size_t columnCnt);
    
    void addFunction(const Function& f);
    
    const gui::Rect &getWindow();
    
    void setOrigin(const gui::Point &location);
    
    void setTextColor(const td::ColorID &color);
    
    void reset();
};

} //namepsace plot
} //namespace gui
