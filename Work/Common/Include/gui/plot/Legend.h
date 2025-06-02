// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <deque>
#include <gui/DrawableString.h>
#include <vector>
#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/plot/Plot.h>
#include <td/LineParams.h>

namespace gui
{
namespace plot
{

class NATPLOT_API Legend
{
    using Desc = struct _Desc
    {
        gui::DrawableString lbl;
        td::LineParams lineParams;
        td::BYTE isLine = 1;
    };
    
    cnt::PushBackVector<Desc> _labels;
    gui::Size _parentSize = {0,0};
    gui::Rect _window; //place for legend entries
    
    std::vector<double> _lengths;
    gui::Font* _font;
    
    td::UINT2 _nColumns = 1;
    td::UINT2 _nBands = 0;
    td::UINT2 _nAritraryLines = 0;
    
    td::ColorID _textColor;
    td::BYTE _initialized = 0;
    td::BYTE _drawBackgroundRect = 1;
    
public:
    
    void changeLocation(const gui::Point &location);
    Legend(td::ColorID textColor, gui::Font *font, const gui::Point &location);
    
    int getNoOfEntriesPerColumn();
    
    void reserve(size_t nPlots)
    {
        _labels.reserve(nPlots);
        _lengths.reserve(nPlots);
    }
    
    void draw(td::ColorID backColor, bool bDarkMode);
    
    void changeName(const td::String& name, size_t poz);
    
    void changeColor(td::ColorID color, size_t poz);
    
    void changeColumnCnt(size_t columnCnt);
    
    size_t getColumnCnt() const;
    
    void addPlot(const Plot& f);
    
    void addBand(const td::String& bandName, td::ColorID bandColorL, td::ColorID bandColorD);
    
    void addHelperLine(const td::String& name, const td::LineParams& lineParams);
    
    const gui::Rect& getWindow();
    
    void setOrigin(const gui::Point &location);
    
    void setParentSize(const gui::Size& parentSize, bool updateOrigin);
    
    void setTextColor(const td::ColorID &color);
    
    void reset();
};

} //namepsace plot
} //namespace gui
