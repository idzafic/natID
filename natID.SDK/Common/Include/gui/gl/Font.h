// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Font.h
 *  @brief OpenGL font renderer that loads glyph data from binary font files and renders text via GPU textures. */
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
/// @brief GPU-backed font that stores glyph metrics and renders Unicode text into an OpenGL texture.
class NATGL_API Font
{
    friend class Text;

    /// @brief Internal draw command describing a run of glyphs with a common color and depth.
    typedef struct _cmd
    {
        td::UINT4 from;      ///< Starting index in the vertex data array.
        td::UINT4 n;         ///< Number of vertices in this draw call.
        float z;             ///< Depth (z-level) for this text run.
        td::ColorID colorID; ///< Color applied to this text run.
    } Txt;

    /// @brief Metrics for a single glyph stored in the font texture atlas.
    typedef struct _glyph
    {
        float left = 0;    ///< Left bearing of the glyph in logical pixels.
        float advance = 0; ///< Horizontal advance width in logical pixels.
        float width = 0;   ///< Rendered width of the glyph in logical pixels.
        float tx = 0;      ///< Horizontal texture coordinate of the glyph in the atlas.
    } Glyph;

    /// @brief A single entry in the vertex buffer used to draw one quad corner.
    typedef struct _vd
    {
        float x;  ///< Horizontal position in logical pixels.
        float y;  ///< Vertical position in logical pixels.
        float tx; ///< Horizontal texture coordinate.
        float ty; ///< Vertical texture coordinate.
    } VertexEntry;

public:
    /// @brief Formatting parameters applied when adding a string to the font.
    typedef struct _format
    {
        float scale = 1.0f;                              ///< Uniform scale factor applied to glyph sizes.
        float z = 0.0f;                                  ///< Depth (z-level) at which the text is rendered.
        td::ColorID colorID;                             ///< Color used for the text.
        td::HAlignment hAlignment = td::HAlignment::Left; ///< Horizontal alignment of multi-line text.
    } Format;

protected:
    cnt::SafeFullVector<td::UTF32>* _pUTF32 = nullptr;          ///< Shared UTF-32 conversion buffer.
    cnt::SafeFullVector<gui::gl::Font::Glyph> _glyphs;          ///< Per-glyph metric table.
    cnt::Dictionary<gui::Font::GlyphID, td::UINT2> _unicodeToPos; ///< Maps Unicode code points to glyph indices.
    cnt::PushBackVector<VertexEntry> _vertexData;                ///< Interleaved position/UV vertex data.
    cnt::PushBackVector<Txt> _commands;                          ///< Draw command list for batched rendering.
//    gui::Font::Metrics _fontMetrics;
    td::UINT4 _textureID = 0;                    ///< OpenGL texture object ID for the glyph atlas.
    float _heightLogPixels = 0;                  ///< Line height in logical pixels.
    float _spaceBetweenGlyphs = 2.0;             ///< Default horizontal spacing between glyphs.
//    float _scale = 1.0;
    float _backingFactor = 1.0f;                 ///< HiDPI backing scale factor.
    float _additionalSpaceBetweenGlyphs = 0.0f;  ///< Extra spacing added between glyphs.
    float _fontSize = 0;                         ///< Nominal font size in points.
    float _spaceWidth = 0;                       ///< Width of the space character in logical pixels.
    float _spaceBetweenLines = 1.0f;             ///< Multiplier for vertical spacing between lines.
    float _zMin = 0; //for zLevel = 0;           ///< Minimum depth value mapped to z-level 0.
    float _zMax = 0; //for zLevel = 255;         ///< Maximum depth value mapped to z-level 255.
    td::UINT4 _vao = 0;                          ///< OpenGL Vertex Array Object (VAO) handle.
    td::UINT4 _vbo = 0;                          ///< OpenGL Vertex Buffer Object (VBO) handle.
    td::BYTE _committed = 0;                     ///< Non-zero after vertex data has been uploaded to the GPU.
private:
    /// @brief Reads raw glyph binary data and the associated image from a font file.
    /// @param fileName Path to the binary font file.
    /// @param imgData Output image data containing the glyph atlas bitmap.
    /// @param widthLogPixels Output width of the atlas in logical pixels.
    /// @return True if the file was read successfully.
    bool loadBinaryData(const char* fileName, gui::Image::Data& imgData, float& widthLogPixels);
protected:
    /// @brief Uploads a glyph atlas texture to the GPU.
    /// @param texture The texture descriptor.
    /// @return True if the texture was set successfully.
    bool setTexture(const gui::Texture& texture);

    /// @brief Pre-allocates vertex buffer space for the given number of characters.
    /// @param nChars Expected number of characters to render.
    void reserve(size_t nChars);

    /// @brief Sets the depth range used to map integer z-levels to clip-space depth values.
    /// @param zMin Depth corresponding to z-level 0.
    /// @param zMax Depth corresponding to z-level 255.
    void setZLevel(float zMin, float zMax);

    //transfer texture data to GPU
    /// @brief Loads font data from a C-string file path.
    /// @param fileName Path to the binary font file.
    /// @return Pointer to this Font object, or nullptr on failure.
    gui::gl::Font* load(const char* fileName);

    /// @brief Loads font data from a td::String file path.
    /// @param fileName Path to the binary font file.
    /// @return Pointer to this Font object, or nullptr on failure.
    gui::gl::Font* load(const td::String& fileName);

    /// @brief Uploads all accumulated vertex data to the GPU.
    /// @return True if the commit succeeded.
    bool commit();

    /// @brief Issues OpenGL draw calls using the given shader program.
    /// @param program The GLSL program used to render the text.
    void draw(gui::gl::Program* program);

    /// @brief Appends glyph quads for a UTF-8 string starting at the given position.
    /// @param pt Top-left origin in model coordinates.
    /// @param pStart Pointer to the first byte of the UTF-8 string.
    /// @param pEnd One-past-the-end pointer of the UTF-8 string.
    /// @param scale Uniform scale applied to glyph sizes.
    /// @return The width and height of the bounding box occupied by the rendered glyphs.
    gui::gl::Size addGlyphs(const gui::gl::Point2d& pt, const mu::UTF8* pStart, const mu::UTF8* pEnd, float scale);

    /// @brief Releases all GPU resources (VAO, VBO, texture) and resets state.
    void release();

    /// @brief Deleted copy constructor — Font objects are not copyable.
    Font(const Font&) = delete;
    /// @brief Deleted copy assignment operator — Font objects are not copyable.
    Font& operator =(const Font&) = delete;
public:
    /// @brief Default constructor.
    Font();

    /// @brief Move constructor.
    /// @param other Font to move from.
    Font(Font&& other);

    /// @brief Destructor — releases GPU resources.
    ~Font();

    /// @brief Sets extra horizontal spacing inserted between consecutive glyphs.
    /// @param additionalSpaceBetweenGlyphs Extra spacing in logical pixels.
    void setAdditionalSpaceBetweenGlyphs(float additionalSpaceBetweenGlyphs);

    /// @brief Sets the vertical line-spacing multiplier.
    /// @param vSpace Multiplier applied to the line height (1.0 = single-spaced).
    void setSpaceBetweenLines(float vSpace);

    /// @brief Clears all accumulated vertex data and draw commands without releasing GPU resources.
    void reset();

    //returns occupied with and height
    /// @brief Appends a C-string to the rendering buffer at the given position.
    /// @param pt Top-left origin in model coordinates.
    /// @param pStr Pointer to a null-terminated C string.
    /// @param strLen Number of bytes in pStr.
    /// @param color Text color.
    /// @param zLevel Depth level (0–255) at which the text is rendered.
    /// @return The width and height of the occupied bounding box.
    gui::gl::Size addString(const gui::gl::Point2d& pt, const char* pStr, size_t strLen, td::ColorID color, float zLevel=0);

    /// @brief Appends a td::String to the rendering buffer at the given position.
    /// @param pt Top-left origin in model coordinates.
    /// @param str The string to render.
    /// @param color Text color.
    /// @param zLevel Depth level (0–255) at which the text is rendered.
    /// @return The width and height of the occupied bounding box.
    gui::gl::Size addString(const gui::gl::Point2d& pt, const td::String& str, td::ColorID color, float zLevel=0);

    /// @brief Appends a C-string using detailed format parameters.
    /// @param pt Top-left origin in model coordinates.
    /// @param pStr Pointer to a null-terminated C string.
    /// @param strLen Number of bytes in pStr.
    /// @param format Formatting parameters (scale, depth, color, alignment).
    /// @return The width and height of the occupied bounding box.
    gui::gl::Size addString(const gui::gl::Point2d& pt, const char* pStr, size_t strLen, const Format& format);

    /// @brief Appends a td::String using detailed format parameters.
    /// @param pt Top-left origin in model coordinates.
    /// @param str The string to render.
    /// @param format Formatting parameters (scale, depth, color, alignment).
    /// @return The width and height of the occupied bounding box.
    gui::gl::Size addString(const gui::gl::Point2d& pt, const td::String& str, const Format& format);

    /// @brief Measures the bounding box of a C-string without adding it to the render buffer.
    /// @param pStr Pointer to a null-terminated C string.
    /// @param strLen Number of bytes in pStr.
    /// @param range Optional character range to measure (0,0 means the entire string).
    /// @return Width and height of the text if rendered.
    gui::gl::Size measure(const char* pStr, size_t strLen, const gui::Range& range = gui::Range(0,0));

    /// @brief Measures the bounding box of a td::String without adding it to the render buffer.
    /// @param str The string to measure.
    /// @param range Optional character range to measure (0,0 means the entire string).
    /// @return Width and height of the text if rendered.
    gui::gl::Size measure(const td::String& str, const gui::Range& range = gui::Range(0,0));
};

} //namespace gl
} //namespace gui
