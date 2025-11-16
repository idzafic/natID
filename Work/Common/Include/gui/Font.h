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
#include <cnt/SafeFullVector.h>
#include <cnt/PushBackVector.h>

namespace gui
{

#ifdef MU_WINDOWS
const char* const cg_DefFontName = "Times New Roman";
#else
#ifdef MU_LINUX
const char* const cg_DefFontName = "Liberation Serif";
#else
//MACOS
const char* const cg_DefFontName = "Times New Roman";
#endif
#endif

class DrawableString;
class FontHelper;

class NATGUI_API Font
{
    friend class FontHelper;
    
public:
    using GlyphID = td::UTF32;
    enum class Unit : unsigned char {LogicalPixel=0, Point};
    enum class Style : unsigned char {Normal=0, Bold, Italic, BoldItalic};
    enum class Markup : unsigned char {Subscript=0, Superscript, Underline, NA};
    
    enum class ID : unsigned char
        {   SystemSmallest=0, SystemSmallestBold, SystemSmallestItalic, SystemSmallestBoldItalic,
            SystemSmaller, SystemSmallerBold, SystemSmallerItalic, SystemSmallerBoldItalic,
            SystemNormal, SystemBold, SystemItalic, SystemBoldItalic,
            SystemLarger, SystemLargerBold, SystemLargerItalic, SystemLargerBoldItalic,
            SystemLargest, SystemLargestBold, SystemLargestItalic, SystemLargestBoldItalic,
            SystemControl, SystemControlTitle,
            Custom, NA};
    typedef struct _metrics
    {
        float ascender;
        float descender;
        float xHeight;
        float capHeight;
        float defaultLineHeight;
        float leading;
    }Metrics;
    
    typedef struct _glyphMetrics
    {
        gui::Point bottomLeft;
        gui::Size boundingSize;
        gui::Size advancement;
    } GlyphMetrics;
    
protected:
    td::String _familyName;
    float _size;
    td::UINT2 _customID = 0;
    ID _id = ID::NA;
    Style _style = Style::Normal;
    td::BYTE _dynamic = 0;
public:
    Font();
    // Move constructor
    Font(Font&& fnt) noexcept;
    // Move assignment operator
    Font& operator=(Font&& fnt) noexcept;
    ~Font();
    
    bool create(const td::String& fntName, float size, gui::Font::Style style, gui::Font::Unit sizeUnit);
    bool create(const char* fntName, float size, gui::Font::Style style, gui::Font::Unit sizeUnit);
    
    td::UINT2 getCustomID() const;
    
    Style getStyle() const;
    ID getType() const;
    bool isBold() const;
    bool isItalic() const;
    const td::String& getFamilyName() const;
    float getSize() const;
    float getScaledSize() const;
    void setDynamic();
    bool isDynamic() const;
    
    bool isOk() const;
    bool isCustom() const;
    
    //metrics
    void getMetrics(Metrics& metrics) const;
//    void getGlyphMetrics(const char* pStr, cnt::PushBackVector<GlyphMetrics>& cntMetrics) const;
//    void getGlyphMetrics(const td::String& str, cnt::PushBackVector<GlyphMetrics>& cntMetrics) const;
    void getGlyphMetrics(GlyphID glyph, GlyphMetrics& glyphMetrics) const;
    void getGlyphMetrics(const GlyphID* pGlyphs, size_t nGlyphs, cnt::PushBackVector<GlyphMetrics>& cntMetrics) const;
    
    //strin dymensions with this font
    void measureString(const char* pStr, size_t strLen, gui::Size& resultingSize) const;
    void measureString(const td::String& str, gui::Size& resultingSize) const;
    
    static void getAverageSize(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);
    static float getHeight(gui::Font::ID fontID);
    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);
    
    static const char* toString(gui::Font::Style style);
    static const char* toString(gui::Font::Markup markup);
    static const cnt::SafeFullVector<td::String>& getSystemFamilyNames();
    static const cnt::SafeFullVector<td::String>& getSystemMonospacedFamilyNames();
    
    //returns system font height
    static float getSystemHeight();
};


} //namespace gui
