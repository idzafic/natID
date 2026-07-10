// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Image.h
    @brief Raster image class supporting PNG, JPG, and BMP formats with drawing and pixel access. */
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

/// @brief Represents a raster image that can be loaded from a file or resource and drawn within GUI controls.
class NATGUI_API Image //: public NatObject
{
    friend class Resources;
    friend class ImageHelper;

public:
    /// @brief Controls whether the original aspect ratio is preserved when drawing.
    enum class AspectRatio : unsigned char
    {
        No=0, ///< Do not preserve aspect ratio; stretch to fill the rectangle.
        Keep  ///< Preserve the original aspect ratio.
    };

    /// @brief Image file format type.
    enum class Type : unsigned char
    {
        PNG=0,   ///< Portable Network Graphics format.
        JPG,     ///< JPEG format.
        BMP,     ///< Bitmap format.
        Unknown  ///< Unknown or unsupported format.
    };

    /// @brief Pixel data format of the image.
    enum class Format : unsigned char
    {
        Error=0,  ///< Format could not be determined.
        RGBA,     ///< 32-bit RGBA (red, green, blue, alpha) format.
        RGB,      ///< 24-bit RGB (red, green, blue) format.
        Red,      ///< 8-bit red-channel-only format.
        Unknown   ///< Unknown pixel format.
    };

    /// @brief Raw pixel data container returned by getData().
    typedef struct _data
    {
        cnt::SafeFullVector<td::BYTE> data; ///< Raw pixel bytes ordered from top-left to bottom-right.
        td::UINT4 width; //in pixels ///< Image width in pixels.
        td::UINT4 height; //in pixels ///< Image height in pixels.
        td::UINT2 bitsPerPixel; ///< Number of bits per pixel.
        Image::Format format; ///< Pixel format of the raw data.
    } Data;

private:
    union
    {
        Handle _handle = nullptr; ///< Native platform handle for the image.
        Image* _pResImage; ///< Pointer to the original resource image (when used as a copy).
    };
    gui::Handle _disabled = nullptr; ///< Native handle for the greyed-out (disabled) version of the image.
    float _ratioHW = 1; ///< Height-to-width ratio of the image.
    td::BYTE _fromRes = 0; ///< Non-zero if the image was loaded from application resources.
    td::BYTE _fromDisplay = 0; ///< Non-zero if the image was captured from a display.
    td::BYTE _invertable = 0; ///< Non-zero if the image supports colour inversion (e.g. for dark mode).
    //enum class Type {FromRes, FromFile, FromDB};
    //Type _type = Type::FromRes;
private:
    /// @brief Calculates and stores the height-to-width ratio from an explicit size.
    /// @param sz Size to use for ratio calculation.
    void calcHWRatio(const gui::Size& sz);

    /// @brief Calculates and stores the height-to-width ratio from the native image size.
    void calcHWRatio();

    /// @brief Resets the native platform handle to nullptr.
    void resetHandle();
    //bool loadFromRes(const td::String& fn);
protected:
    /// @brief Returns the mutable native handle.
    /// @return Mutable native handle.
    gui::Handle getHandle();

    /// @brief Returns the immutable native handle.
    /// @return Const native handle.
    const gui::Handle getHandle() const;

    /// @brief Returns the native handle for the disabled (greyed) image variant.
    /// @return Handle for the disabled image.
    gui::Handle getDisabled();

    /// @brief Draws the image without managing the graphics context.
    /// @param rect Target rectangle to draw into.
    /// @param aspectRatio Whether to preserve the aspect ratio.
    /// @param hAlign Horizontal alignment within the rectangle.
    /// @param vAlign Vertical alignment within the rectangle.
    void drawWithoutContextManagement(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign = td::HAlignment::Center, td::VAlignment vAlign = td::VAlignment::Center) const;

    /// @brief Creates a drawing context backed by this image (used with Canvas::drawToImage).
    /// @param pCanvas Canvas that will draw into this image.
    /// @param clear If true, clears the image with clearColor before drawing.
    /// @param clearColor Colour used to clear the image if clear is true.
    void createDrawingContext(gui::Canvas* pCanvas, bool clear = false, td::ColorID clearColor = td::ColorID::Transparent);

    /// @brief Releases the drawing context previously created with createDrawingContext.
    void releaseDrawingContext();

    Image(const Image&) = delete;
    Image& operator =(const Image&) = delete;
public:
    /// @brief Default constructor; creates an empty image with no loaded data.
    Image();

    /// @brief Constructs an Image from an existing native platform handle (used for resources).
    /// @param handle Native platform image handle.
    Image(gui::Handle handle); //for resource

    /// @brief Constructs an Image and loads it from a file path or resource name.
    /// @param fileOrResName Null-terminated string with file path or resource identifier.
    Image(const char* fileOrResName);

    /// @brief Constructs an Image and loads it from a file path or resource name.
    /// @param fileOrResName String with file path or resource identifier.
    Image(const td::String& fileOrResName);

    /// @brief Constructs an empty Image of the given size (for off-screen rendering).
    /// @param sz Dimensions of the image in pixels.
    Image(const gui::Size& sz);
//    Image(const Image& img); //for resource images only

    /// @brief Constructs a lightweight reference copy from a resource image.
    /// @param pResImage Pointer to the original resource image.
    Image(const Image* pResImage); //for resource images only

    /// @brief Destructor; releases the native image handle.
    ~Image();
//    virtual gui::ObjType getObjType() const override { return ObjType::Image; }

    //draws in a specific rectangle
    //if pResultRect != nullptr, it will be used to return bounding rectangle of image placement
    /// @brief Draws the image into the given rectangle.
    ///        If pPlacementRect is not null, it receives the actual bounding rectangle of the placed image.
    /// @param rect Target drawing rectangle.
    /// @param aspectRatio Whether to preserve the aspect ratio.
    /// @param hAlign Horizontal alignment within the rectangle.
    /// @param vAlign Vertical alignment within the rectangle.
    /// @param pPlacementRect Optional output parameter for the actual placement rectangle.
    void draw(const gui::Rect& rect, AspectRatio aspectRatio = AspectRatio::Keep, td::HAlignment hAlign=td::HAlignment::Center, td::VAlignment vAlign=td::VAlignment::Center, gui::Rect* pPlacementRect = nullptr) const;

    /// @brief Retrieves the dimensions of the image.
    /// @param sz Output size structure filled with the image width and height.
    void getSize(gui::Size& sz) const;

    /// @brief Loads an image from a file path or resource name.
    /// @param fileOrResName Null-terminated string with the file path or resource identifier.
    /// @return True if the image was loaded successfully.
    bool load(const char* fileOrResName);

    /// @brief Loads an image from a file path or resource name.
    /// @param fileOrResName String with the file path or resource identifier.
    /// @return True if the image was loaded successfully.
    bool load(const td::String& fileOrResName);

    /// @brief Checks whether the image has been loaded successfully and is ready to use.
    /// @return True if the image is valid.
    bool isOK() const;

    /// @brief Retrieves the raw pixel data of the image.
    /// @param data Output structure to fill with pixel data, dimensions, and format.
    /// @return True if data was retrieved successfully.
    bool getData(Data& data) const;

    /// @brief Returns the height-to-width ratio of the image.
    /// @return Ratio of image height to image width.
    float getHWRatio() const;

    /// @brief Indicates whether the image was captured from a display surface.
    /// @return True if the image originates from a display.
    bool isFromDisplay() const;

    /// @brief Marks the image as invertible (for dark-mode colour inversion support).
    /// @param bInvertable True to enable colour inversion support.
    void setInvertible(bool bInvertable);

    /// @brief Returns whether colour inversion is supported for this image.
    /// @return True if the image is invertible.
    bool isInvertible() const;

    /// @brief Inverts all colours in the image.
    void invertColors();
    //get color from (x,y) pixel where (0,0) is top-left corner of the image
    //td::Color getColor(unsigned int x, unsigned int y) const;
    //get data content (bytes are ordered from top-left->top right up to bottom->right corner)

    /// @brief Retrieves pixel colours as an array ordered from top-left to bottom-right.
    /// @param content Output vector filled with pixel colour values.
    void getContent(cnt::SafeFullVector<td::Color>& content) const;

    /// @brief Saves the image to a file.
    /// @param fileName Destination file path.
    /// @param type Image format to use when saving.
    /// @return True if the file was saved successfully.
    bool saveToFile(const td::String& fileName, Image::Type type = Image::Type::PNG) const;

};

} //namespace gui
