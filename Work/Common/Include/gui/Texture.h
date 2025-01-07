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
#include <gui/natGUI.h>
#include <td/Types.h>
#include <td/String.h>
#include <td/Size.h>

namespace gui
{

class TextureHelper;
class NATGUI_API Texture
{
    friend class TextureHelper;
    
public:
    enum class Type : td::BYTE {PNG=0, JPG, BMP, TGA};
private:
    td::BYTE* _bytes = nullptr;
    td::UINT4 _noOfBytes = 0;
    td::UINT4 _capacity = 0;
    td::INT4 _width = 0;
    td::INT4 _height = 0;
    td::BYTE _bytesPerPixel = 4;
    td::BYTE _fromRes = 0;
    td::BYTE _loadedFromFile = 0;
    td::BYTE _exportQuality = 100; //for JPG
    td::BYTE _verticallyReversed = 0;
private:
    void copyTexture(const Texture& texture);
    void releaseBuffer();
    
public:
    Texture();
    Texture(const char* fileOrResName);
    Texture(const Texture& texture);
    //bytes will not be released by the destructor
    Texture(td::BYTE* bytes, const td::Size<td::INT4>& size, td::BYTE bytesPerPixel);
    
    ~Texture();
    
    void reset();
    void reserve(td::UINT4 capacity);
    
    void operator = (const Texture& texture);
    
    bool loadFromFile(const char* fileName);
    bool loadFromFile(const td::String& fileName);
    void setExportJPGQuality(unsigned int quality); //from 1-100
    
    //use Type to define output type
    bool saveToFile(const td::String& fileName, Type fileType);
    //use fileName extension to define output type
    bool saveToFile(const td::String& fileName);
    
    const td::BYTE* getBytes() const;
    td::UINT4 getNoOfBytes() const;
    td::UINT4 getCapacity() const;
    td::INT4 getWidth() const;
    td::INT4 getHeight() const;
    td::BYTE getBytesPerPixel() const;
    bool isResource() const;
    bool isMirroredVertically() const;
    void mirrorVertically();
    bool isOk() const;
    
    void release();
};

} //namespace gui

