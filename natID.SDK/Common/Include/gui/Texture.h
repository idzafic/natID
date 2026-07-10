// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Texture.h
    @brief Image texture class supporting load, edit, convolve, and save operations. */
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
/// @brief Manages a pixel buffer that can be loaded from files or resources, manipulated, and drawn on screen.
class NATGUI_API Texture
{
    friend class TextureHelper;

public:
    /// @brief Supported image file formats.
    enum class Type : td::BYTE
    {
        PNG=0, ///< Portable Network Graphics format.
        JPG,   ///< JPEG format.
        BMP,   ///< Windows Bitmap format.
        TGA    ///< Truevision TGA format.
    };

    /// @brief Individual color channels within a pixel.
    enum class Channel : td::BYTE
    {
        R = 0, ///< Red channel.
        G,     ///< Green channel.
        B,     ///< Blue channel.
        A      ///< Alpha channel.
    };
private:
    Handle _handle = nullptr; ///< Native platform handle for the texture.
    td::BYTE* _bytes = nullptr; ///< Pointer to the raw pixel byte buffer.
    td::UINT4 _noOfBytes = 0; ///< Number of bytes currently used by the pixel buffer.
    td::UINT4 _capacity = 0; ///< Allocated capacity of the pixel buffer in bytes.
    td::INT4 _width = 0; ///< Width of the texture in pixels.
    td::INT4 _height = 0; ///< Height of the texture in pixels.
    td::Color _backColor = {0,0,0,255}; ///< Background fill color used when creating or cleaning the texture.
    td::BYTE _bytesPerPixel = 4; ///< Number of bytes per pixel (e.g., 4 for RGBA).
    td::BYTE _fromRes = 0; ///< Non-zero if the texture was loaded from an application resource.
    td::BYTE _loadedFromFile = 0; ///< Non-zero if the texture was loaded from a file.
    td::BYTE _exportQuality = 100; ///< JPG export quality from 1 (lowest) to 100 (highest).
    td::BYTE _verticallyReversed = 0; ///< Non-zero if the pixel buffer is stored bottom-up.
private:
    /// @brief Performs a deep copy of another texture into this object.
    /// @param texture Source texture to copy.
    void copyTexture(const Texture& texture);

    /// @brief Releases the pixel buffer memory.
    void releaseBuffer();

    /// @brief Releases the native platform handle.
    void releaseHandle();

public:
    /// @brief Constructs an empty texture.
    Texture();

    /// @brief Constructs a texture by loading from a file or resource name.
    /// @param fileOrResName Path to a file or name of an application resource.
    Texture(const char* fileOrResName);

    /// @brief Copy constructor – performs a deep copy of the source texture.
    /// @param texture Source texture to copy.
    Texture(const Texture& texture);

    /// @brief Constructs a texture that wraps an externally owned byte buffer (not freed on destruction).
    /// @param bytes Pointer to the raw pixel data.
    /// @param size Width and height of the texture in pixels.
    /// @param bytesPerPixel Number of bytes per pixel.
    //bytes will not be released by the destructor
    Texture(td::BYTE* bytes, const td::Size<td::INT4>& size, td::BYTE bytesPerPixel);

    /// @brief Constructs a texture of the given size filled with a background color.
    /// @param size Width and height of the texture in pixels.
    /// @param backColor Fill color for the new texture.
    /// @param bytesPerPixel Number of bytes per pixel.
    Texture(const td::Size<td::INT4>& size, td::Color backColor = {0,0,0,255}, td::BYTE bytesPerPixel = 4);

    /// @brief Destructor; releases owned resources.
    ~Texture();

    /// @brief Resets the texture to an empty state, releasing all resources.
    void reset();

    /// @brief Pre-allocates the pixel buffer to the given byte capacity.
    /// @param capacity Desired buffer capacity in bytes.
    void reserve(td::UINT4 capacity);

    /// @brief Copy-assignment operator; performs a deep copy.
    /// @param texture Source texture to copy.
    /// @return Const reference to this texture.
    const Texture&  operator = (const Texture& texture);

    /// @brief Loads the texture from a file path given as a C-string.
    /// @param fileName Null-terminated path to the image file.
    /// @return True if loading succeeded.
    bool loadFromFile(const char* fileName);

    /// @brief Loads the texture from a file path given as a String.
    /// @param fileName Path to the image file.
    /// @return True if loading succeeded.
    bool loadFromFile(const td::String& fileName);

    /// @brief Opens an MNIST image collection file and returns metadata.
    /// @param fileName Path to the MNIST image binary file.
    /// @param[out] f Open file stream positioned at the first image.
    /// @param[out] sizeOfEachImage Width and height of each image in the collection.
    /// @param[out] bytesPerPixel Bytes per pixel for each image.
    /// @return Number of images in the collection, or -1 on failure.
    static int openMNISTImageCollection(const td::String& fileName, std::ifstream& f, td::Size<td::INT4>& sizeOfEachImage, td::BYTE& bytesPerPixel);  //if file is MNIST f will be open, it will return number of images inside; -1 if fails

    /// @brief Opens an MNIST label collection file and returns the label count.
    /// @param fileName Path to the MNIST label binary file.
    /// @param[out] f Open file stream positioned at the first label.
    /// @return Number of labels in the collection, or -1 on failure.
    static int openMNISTLabelCollection(const td::String& fileName, std::ifstream& f);  //if file is MNIST f will be open, it will return number of labels inside; -1 if fails

    /// @brief Loads a single image from an already-open MNIST image collection.
    /// @param f Open file stream for the MNIST image file.
    /// @param iPos Zero-based index of the image to load.
    /// @return True if the image was loaded successfully.
    bool loadMNISTImage(std::ifstream& f, int iPos);

    /// @brief Resizes the texture to the given dimensions, reallocating the buffer.
    /// @param size New width and height in pixels.
    void resize(const td::Size<td::INT4>& size);

    /// @brief Copies pixels from another texture into this texture at the specified position.
    /// @param positionInThisTexture Top-left destination position in this texture.
    /// @param fromOtherTexture Source texture to copy from.
    void replace(const td::Point<td::INT4>& positionInThisTexture, const Texture& fromOtherTexture);

    /// @brief Adds random noise to the texture using separate per-channel noise levels.
    /// @param noiseLevel Maximum noise amplitude per channel.
    void addNoise(td::Color noiseLevel); //noise can be added using separate channels for RGB

    /// @brief Fills the entire texture with the specified color.
    /// @param withColor Color to use as the fill.
    void clean(td::Color withColor); //replaces all pixels with the specified color

    /// @brief Computes the normalized cross-correlation between this texture and a kernel at a given position (all channels combined).
    /// @param positionInThisTexture Top-left position in this texture for the comparison window.
    /// @param kernel Kernel texture.
    /// @return Correlation value in [0, 1].
    float calculateNormalizedCrossCorrelation(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel) const;

    /// @brief Computes the standard convolution between this texture and a kernel at a given position (all channels combined).
    /// @param positionInThisTexture Top-left position in this texture for the computation window.
    /// @param kernel1 Kernel texture.
    /// @return Convolution result.
    float calculateStandardConvolution(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel1) const;

    /// @brief Computes the normalized cross-correlation for a single channel.
    /// @param positionInThisTexture Top-left position in this texture for the comparison window.
    /// @param kernel Kernel texture.
    /// @param channel Channel to use for the computation.
    /// @return Correlation value in [0, 1].
    //Method for single-channel convolution
    float calculateNormalizedCrossCorrelation(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel, Channel channel) const;

    /// @brief Computes the standard convolution for a single channel.
    /// @param positionInThisTexture Top-left position in this texture for the computation window.
    /// @param kernel Kernel texture.
    /// @param channel Channel to use for the computation.
    /// @return Convolution result.
    float calculateStandardConvolution(const td::Point<td::INT4>& positionInThisTexture, const Texture& kernel, Channel channel) const;

    // Adjusts brightness by factor in range (-1.0f to +1.0f)
    // Positive values make image lighter, negative values make it darker
    /// @brief Adjusts the brightness of all pixels by the given factor.
    /// @param factor Value in [-1.0, +1.0]; positive values lighten, negative values darken.
    void adjustBrightness(float factor);

    // Apply a user-defined filter to each pixel
    // The callback receives the pixel color (modifiable) and its (x,y) coordinates (pixel position in texture).
    /// @brief Applies a user-defined per-pixel filter function to every pixel in the texture.
    /// @param func Callback receiving the pixel x/y coordinates and a modifiable color reference.
    void applyFilter(const std::function<void(int x, int y, td::Color& color)>& func);

    //draw it on a 2D element (Canvas, or another image)
    /// @brief Draws the texture at the specified point, optionally scaled.
    /// @param pt Top-left drawing position.
    /// @param scale Uniform scale factor (default 1.0).
    void draw(gui::Point& pt, float scale = 1.0f);

    /// @brief Sets the quality level used when exporting to JPEG.
    /// @param quality Value from 1 (lowest) to 100 (highest).
    void setExportJPGQuality(unsigned int quality); //from 1-100

    //use Type to define output type
    /// @brief Saves the texture to a file, using the explicitly specified format.
    /// @param fileName Destination file path.
    /// @param fileType Output image format.
    /// @return True if saving succeeded.
    bool saveToFile(const td::String& fileName, Type fileType);

    //use fileName extension to define output type
    /// @brief Saves the texture to a file, inferring the format from the file extension.
    /// @param fileName Destination file path (extension determines the format).
    /// @return True if saving succeeded.
    bool saveToFile(const td::String& fileName);

    //takes content from another texture (retrieving texture cannot be from resources)
    /// @brief Copies pixel content from another (non-resource) texture into this one.
    /// @param texture Source texture to copy from.
    void copyFrom(const Texture& texture);

    /// @brief Returns a const pointer to the raw pixel byte buffer.
    /// @return Pointer to the pixel data.
    const td::BYTE* getBytes() const;

    /// @brief Returns the number of bytes currently used by the pixel buffer.
    /// @return Byte count.
    td::UINT4 getNoOfBytes() const;

    /// @brief Returns the total number of pixels in the texture.
    /// @return Pixel count (width * height).
    td::UINT4 getNoOfPixels() const;

    /// @brief Returns the allocated capacity of the pixel buffer in bytes.
    /// @return Buffer capacity in bytes.
    td::UINT4 getCapacity() const; //in bytes

    /// @brief Returns the width of the texture in pixels.
    /// @return Width in pixels.
    td::INT4 getWidth() const;      //in pixels

    /// @brief Returns the height of the texture in pixels.
    /// @return Height in pixels.
    td::INT4 getHeight() const;     //in pixels

    /// @brief Returns the texture size as a floating-point Size.
    /// @param[out] sz Receives the width and height in pixels.
    void getSize(gui::Size& sz) const; //in pixels, floating point

    /// @brief Returns the number of bytes per pixel.
    /// @return Bytes per pixel (e.g., 4 for RGBA).
    td::BYTE getBytesPerPixel() const;

    /// @brief Returns whether the texture was loaded from an application resource.
    /// @return True if the texture is a resource.
    bool isResource() const;

    /// @brief Returns whether the pixel buffer is stored in a bottom-up (vertically mirrored) order.
    /// @return True if mirrored vertically.
    bool isMirroredVertically() const;

    /// @brief Flips the pixel buffer vertically in place.
    void mirrorVertically();

    /// @brief Returns whether the texture holds valid pixel data.
    /// @return True if the texture is valid.
    bool isOk() const;

    /// @brief Releases all resources held by the texture, returning it to an empty state.
    void release();
};

} //namespace gui

