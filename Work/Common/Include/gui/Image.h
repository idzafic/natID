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
class Resources;
class ImageHelper;
class Canvas;

class NATGUI_API Image //: public NatObject
{
    friend class Resources;
    friend class ImageHelper;
    
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
    union
    {
        Handle _handle = nullptr;
        Image* _pResImage;
    };
    gui::Handle _disabled = nullptr;
    float _ratioHW = 1;
    td::BYTE _fromRes = 0;
    td::BYTE _fromDisplay = 0;
    td::BYTE _invertable = 0;
    //enum class Type {FromRes, FromFile, FromDB};
    //Type _type = Type::FromRes;
private:
    void calcHWRatio(const gui::Size& sz);
    void calcHWRatio();
    void resetHandle();
    //bool loadFromRes(const td::String& fn);
protected:
    gui::Handle getHandle();
    const gui::Handle getHandle() const;
    gui::Handle getDisabled();
    void drawWithoutContextManagement(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign = td::HAlignment::Center, td::VAlignment vAlign = td::VAlignment::Center) const;

    //for drawing to Image used by Canvas::drawToImage)
    void createDrawingContext(gui::Canvas* pCanvas, bool clear = false, td::ColorID clearColor = td::ColorID::Transparent);
    void releaseDrawingContext();

public:
    Image();
    Image(gui::Handle handle); //for resource
    Image(const char* fileOrResName);
    Image(const td::String& fileOrResName);
    Image(const gui::Size& sz);
//    Image(const Image& img); //for resource images only
    Image(const Image* pResImage); //for resource images only
    ~Image();
//    virtual gui::ObjType getObjType() const override { return ObjType::Image; }
    
    //draws in a specific rectangle
    //if pResultRect != nullptr, it will be used to return bounding rectangle of image placement
    void draw(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign=td::HAlignment::Center, td::VAlignment vAlign=td::VAlignment::Center, gui::Rect* pPlacementRect = nullptr) const;
    
    void getSize(gui::Size& sz) const;
    
    bool load(const char* fileOrResName);
    bool load(const td::String& fileOrResName);
    
    bool isOK() const;
    bool getData(Data& data) const;
    
    float getHWRatio() const;
    bool isFromDisplay() const;
    void setInvertible(bool bInvertable);
    bool isInvertible() const;
    void invertColors();
    //get color from (x,y) pixel where (0,0) is top-left corner of the image
    //td::Color getColor(unsigned int x, unsigned int y) const;
    //get data content (bytes are ordered from top-left->top right up to bottom->right corner)
    void getContent(cnt::SafeFullVector<td::Color>& content) const;
    bool saveToFile(const td::String& fileName, Image::Type type = Image::Type::PNG) const;
    
};

} //namespace gui
