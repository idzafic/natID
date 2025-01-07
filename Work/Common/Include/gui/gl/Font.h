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
#include <gui/gl/natGL.h>
#include <cnt/SafeFullVector.h>
#include <cnt/PushBackVector.h>
#include <cnt/Dictionary.h>
#include <gui/Font.h>
#include <gui/gl/CommandList.h>
#include <gui/Image.h>
#include <td/Range.h>

namespace gui
{
namespace gl
{

class Program;
class Text;
//load font from .text file to texture keeping local info
class NATGL_API Font
{
    friend class Text;
    
    typedef struct _cmd
    {
        td::UINT4 from;
        td::UINT4 n;
        float z;
        td::ColorID colorID;
    } Txt;
    
    typedef struct _glyph
    {
        float left = 0;
        float advance = 0;
        float width = 0;
        float tx = 0;
    } Glyph;
    
    typedef struct _vd
    {
        float x;
        float y;
        float tx;
        float ty;
    } VertexEntry;
    
public:
    typedef struct _format
    {
        float scale = 1.0f;
        float z = 0.0f;
        td::ColorID colorID;
        td::HAlignment hAlignment = td::HAlignment::Left;
    } Format;
    
protected:
    cnt::SafeFullVector<td::UTF32>* _pUTF32 = nullptr;
    cnt::SafeFullVector<gui::gl::Font::Glyph> _glyphs;
    cnt::Dictionary<gui::Font::GlyphID, td::UINT2> _unicodeToPos;
    cnt::PushBackVector<VertexEntry> _vertexData;
    cnt::PushBackVector<Txt> _commands;
//    gui::Font::Metrics _fontMetrics;
    td::UINT4 _textureID = 0;
    float _heightLogPixels = 0;
    float _spaceBetweenGlyphs = 2.0;
//    float _scale = 1.0;
    float _backingFactor = 1.0f;
    float _additionalSpaceBetweenGlyphs = 0.0f;
    float _fontSize = 0;
    float _spaceWidth = 0;
    float _spaceBetweenLines = 1.0f;
    float _zMin = 0; //for zLevel = 0;
    float _zMax = 0; //for zLevel = 255;
    td::UINT4 _vao = 0;
    td::UINT4 _vbo = 0;
    td::BYTE _committed = 0;
private:
    bool loadBinaryData(const char* fileName, gui::Image::Data& imgData, float& widthLogPixels);
protected:
    bool setTexture(const gui::Texture& texture);
    void reserve(size_t nChars);
    void setZLevel(float zMin, float zMax);
    //transfer texture data to GPU
    gui::gl::Font* load(const char* fileName);
    gui::gl::Font* load(const td::String& fileName);
    bool commit();
    void draw(gui::gl::Program* program);
    gui::gl::Size addGlyphs(const gui::gl::Point2d& pt, const mu::UTF8* pStart, const mu::UTF8* pEnd, float scale);
    void release();
public:
    Font();
    ~Font();
    
    void setAdditionalSpaceBetweenGlyphs(float additionalSpaceBetweenGlyphs);
    void setSpaceBetweenLines(float vSpace);
    void reset();
    //returns occupied with and height
    gui::gl::Size addString(const gui::gl::Point2d& pt, const char* pStr, size_t strLen, td::ColorID color, float zLevel=0);
    gui::gl::Size addString(const gui::gl::Point2d& pt, const td::String& str, td::ColorID color, float zLevel=0);
    
    gui::gl::Size addString(const gui::gl::Point2d& pt, const char* pStr, size_t strLen, const Format& format);
    gui::gl::Size addString(const gui::gl::Point2d& pt, const td::String& str, const Format& format);
    
    gui::gl::Size measure(const char* pStr, size_t strLen, const gui::Range& range = gui::Range(0,0));
    gui::gl::Size measure(const td::String& str, const gui::Range& range = gui::Range(0,0));
};

} //namespace gl
} //namespace gui

