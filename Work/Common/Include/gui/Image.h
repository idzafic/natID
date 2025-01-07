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
#include "NatObject.h"
#include <cnt/SafeFullVector.h>
#include <td/String.h>

namespace gui
{

class NATGUI_API Image : public NatObject
{
public:
    enum class AspectRatio : unsigned char {No=0, Keep};
    enum class Type : unsigned char {PNG=0, JPG, BMP};
    enum class Format : unsigned char {Error=0, RGBA, RGB, Red, Un};
    typedef struct _data
    {
        cnt::SafeFullVector<td::BYTE> data;
        td::UINT4 width; //in pixels
        td::UINT4 height; //in pixels
        td::UINT2 bitsPerPixel;
        Image::Format format;
    } Data;
    
private:
    gui::Handle _disabled = nullptr;
    float _ratioHW = 1;
    td::BYTE _fromRes = 0;
    td::BYTE _fromDisplay = 0;
    //enum class Type {FromRes, FromFile, FromDB};
    //Type _type = Type::FromRes;
private:
    void calcHWRatio(const gui::Size& sz);
    void calcHWRatio();
public:
    Image();
    Image(gui::Handle handle);
    Image(const char* fileOrResName);
    Image(const td::String& fileOrResName);
    Image(const gui::Size& sz);
    ~Image();
    virtual gui::ObjType getObjType() const override { return ObjType::Image; }
    
    void draw(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign=td::HAlignment::Center, td::VAlignment vAlign=td::VAlignment::Center) const;
    void drawWithoutContextManagement(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign=td::HAlignment::Center, td::VAlignment vAlign=td::VAlignment::Center) const;
    void getSize(gui::Size& sz) const;
    
    bool load(const char* fileOrResName);
    bool load(const td::String& fileOrResName);
    
    bool isOK() const;
    bool getData(Data& data) const;
    
    //for drawing to Image
    void startDrawingContext(bool clear = false, td::ColorID clearColor = td::ColorID::Transparent);
    void releaseDrawingContext();
    float getHWRatio() const;
    bool isFromDisplay() const;
    void invertColors();
    bool saveToFile(const td::String& fileName, Image::Type type = Image::Type::PNG) const;
    gui::Handle getDisabled();
};

} //namespace gui
