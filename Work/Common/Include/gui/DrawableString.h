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

namespace gui
{

class NATGUI_API DrawableString
{
public:
//    enum class LineBreak : unsigned char {Spase, Word}; //Breaks on spaces for now
private:
    NatString _natString;
public:
    DrawableString();
    DrawableString(const char*);
    DrawableString(const td::String& str);
    ~DrawableString();
    
    bool isInitialized() const;
    void operator = (const char* pStr);
    void operator = (const td::String& str);
    
    void measure(gui::Font::ID, gui::Size& resultingSize) const;
    void measure(gui::Font::ID, CoordType maxWidth, gui::Size& resultingSize) const;

    void measure(const gui::Font* pFont, gui::Size& resultingSize) const;
    void measure(const gui::Font* pFont, CoordType maxWidth, gui::Size& resultingSize) const;
    
    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);
    
    void draw(const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End) const;
    void draw(const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID) const;
    
    void draw(const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End) const;
    void draw(const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID) const;
    
    static void draw(const td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);
    static void draw(const td::String& txt, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);
    
    static void draw(const char* pStr, size_t nChars, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);
    static void draw(const char* pStr, size_t nChars, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);
    
    static void draw(const td::String& txt, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left,  td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End);
    static void draw(const td::String& txt, const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID);
    
    static void draw(const char* pStr, size_t nChars, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left,  td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End);
    static void draw(const char* pStr, size_t nChars, const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID);
    
    static void drawHTML(const td::String& txtHTML, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left);
//    static void drawHTML(const char* txtHTML, size_t nChars, const gui::Rect& r, const gui::Font* pFont);
    
    //gui::NatString getNatStrPtr();
    const gui::NatString getNatStrPtr() const;
    
    td::String toString() const;
};

} //namepace gui

