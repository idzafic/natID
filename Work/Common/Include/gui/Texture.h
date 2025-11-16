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
#include <gui/Types.h>
#include <td/String.h>
#include <td/Size.h>
#include <fstream>
#include <td/Color.h>
#include <functional>

namespace gui
{

class TextureHelper;
class NATGUI_API Texture
{
    friend class TextureHelper;
    
public:
    enum class Type : td::BYTE {PNG=0, JPG, BMP, TGA};
    enum class Channel : td::BYTE {R = 0, G, B, A};
private:
    Handle _handle = nullptr;
    td::BYTE* _bytes = nullptr;
    td::UINT4 _noOfBytes = 0;
    td::UINT4 _capacity = 0;
    td::INT4 _width = 0;
    td::INT4 _height = 0;
    td::Color _backColor = {0,0,0,255};
    td::BYTE _bytesPerPixel = 4;
    td::BYTE _fromRes = 0;
    td::BYTE _loadedFromFile = 0;
    td::BYTE _exportQuality = 100; //for JPG
    td::BYTE _verticallyReversed = 0;
private:
    void copyTexture(const Texture& texture);
    void releaseBuffer();
    void releaseHandle();
    
public:
    Texture();
    Texture(const char* fileOrResName);
    Texture(const Texture& texture);
    //bytes will not be released by the destructor
    Texture(td::BYTE* bytes, const td::Size<td::INT4>& size, td::BYTE bytesPerPixel);
    
    Texture(const td::Size<td::INT4>& size, td::Color backColor = {0,0,0,255}, td::BYTE bytesPerPixel = 4);
    
    ~Texture();
    
    void reset();
    void reserve(td::UINT4 capacity);
    
    const Texture&  operator = (const Texture& texture);
    
    bool loadFromFile(const char* fileName);
    bool loadFromFile(const td::String& fileName);
    
    static int openMNISTImageCollection(const td::String& fileName, std::ifstream& f, td::Size<td::INT4>& sizeOfEachImage, td::BYTE& bytesPerPixel);  //if file is MNIST f will be open, it will return number of images inside; -1 if fails
    static int openMNISTLabelCollection(const td::String& fileName, std::ifstream& f);  //if file is MNIST f will be open, it will return number of labels inside; -1 if fails
    
    bool loadMNISTImage(std::ifstream& f, int iPos);
    
    void resize(const td::Size<td::INT4>& size);
    void replace(const td::Point<td::INT4>& positionInThisTexture, const Texture& fromOtherTexture);
    void addNoise(td::Color noiseLevel); //noise can be added using separate channels for RGB
    void clean(td::Color withColor); //replaces all pixels with the specified color
    
    //calculates normalized cross correlation: returns values from 0 to 1 (all channels combined)
    float calculateNormalizedCrossCorrelation(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel) const;
    float calculateStandardConvolution(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel1) const;
    
    //Method for single-channel convolution
    float calculateNormalizedCrossCorrelation(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel, Channel channel) const;
    float calculateStandardConvolution(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel, Channel channel) const;

    // Adjusts brightness by factor in range (-1.0f to +1.0f)
    // Positive values make image lighter, negative values make it darker
    void adjustBrightness(float factor);

    // Apply a user-defined filter to each pixel
    // The callback receives the pixel color (modifiable) and its (x,y) coordinates (pixel position in texture).
    void applyFilter(const std::function<void(int x, int y, td::Color& color)>& func);

    //draw it on a 2D element (Canvas, or another image)
    void draw(gui::Point& pt, float scale = 1.0f);
    
    void setExportJPGQuality(unsigned int quality); //from 1-100
    
    //use Type to define output type
    bool saveToFile(const td::String& fileName, Type fileType);
    //use fileName extension to define output type
    bool saveToFile(const td::String& fileName);
    
    //takes content from another texture (retrieving texture cannot be from resources)
    void copyFrom(const Texture& texture);
    
    const td::BYTE* getBytes() const;
    td::UINT4 getNoOfBytes() const;
    td::UINT4 getNoOfPixels() const;
    
    td::UINT4 getCapacity() const; //in bytes
    td::INT4 getWidth() const;      //in pixels
    td::INT4 getHeight() const;     //in pixels
    
    void getSize(gui::Size& sz) const; //in pixels, floating point
    
    td::BYTE getBytesPerPixel() const;
    bool isResource() const;
    bool isMirroredVertically() const;
    void mirrorVertically();
    bool isOk() const;
    
    void release();
};

} //namespace gui

