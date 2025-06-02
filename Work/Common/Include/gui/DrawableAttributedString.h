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
#include <gui/Types.h>
#include <gui/Font.h>
#include <td/ColorID.h>

namespace gui
{

using TabStop = struct Tabstop {
    int start;
    int nSteps;
    int delta;
};

class NATGUI_API DrawableAttributedString
{
public:
    using ColorRange = struct ClrRange
    {
        td::UINT4 len; //in glyphs
        td::ColorID color;
    };
//    enum class LineBreak : unsigned char {Spase, Word}; //Breaks on spaces for now
private:
    AttributedString _natString;
public:
    DrawableAttributedString();
//    DrawableAttributedString(const char*);
//    DrawableAttributedString(const td::String& str);
    ~DrawableAttributedString();
//    
    bool isInitialized() const;
//    void operator = (const char* pStr);
//    void operator = (const td::String& str);
    
//    void measure(gui::Font::ID, gui::Size& resultingSize) const;
//    void measure(gui::Font::ID, CoordType maxWidth, gui::Size& resultingSize) const;
    
//    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);
    
    void draw(const gui::Rect& r) const;
    
    //create using string, tabstrops, and color range
    void create(const td::String& str, DrawableAttributedString::ColorRange* range, size_t nSegments, td::UINT2 styleID = 0);
    
//    void draw(const gui::Rect& r) const;
    //gui::NatString getNatStrPtr();
    const gui::AttributedString getNatStrPtr() const;
    
    td::String toString() const;
    
    static void createNatAttributedStrings(const td::String& strHTML, cnt::PushBackVector<gui::AttributedStringBlock>& natStrings, gui::CoordType width, gui::CoordType& firstHeight, gui::CoordType& lastHeight, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign);
    
    //set new font to attributed string
    static void updateNatAttributedStrings(cnt::PushBackVector<gui::AttributedStringBlock>& natStrings, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign);
    
    static void measure(gui::AttributedString natStr, gui::CoordType width, gui::Size& resultingSize);
    
    static void draw(const AttributedString natStr, const gui::Rect& r);
    
    static void adjustHTMLString(td::String& strHTML);
    
    static void release(AttributedString& natStr);
    
//    static void setStyle(td::UINT2 styleID, const td::String fontName, float fontSize, const TabStop* pTabStops = nullptr, size_t nTabStops = 0);
//    
//    static void setStyle(td::UINT2 styleID, float fontSize, const td::String& fontName = "", const TabStop* pTabStops = nullptr, size_t nTabStops = 0);
};

} //namepace gui

