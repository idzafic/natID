// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Text.h
    @brief OpenGL text rendering class for drawing font-based text in a GL context. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Buffer.h"
#include "Program.h"
#include "Font.h"

namespace gui
{
namespace gl
{

/// @brief Manages GPU-based text rendering using OpenGL shader programs and font atlases.
class NATGL_API Text
{
    friend class Font;
public:
    /// @brief Controls which OpenGL states are enabled or disabled during text rendering.
    enum class Ctrl : td::BYTE {
        Nothing=0,               ///< No special GL state changes.
        Depth = 1,               ///< Enable depth testing.
        CullFace = 2,            ///< Enable face culling.
        DepthAndCullFace = 3,    ///< Enable both depth testing and face culling.
        Blend = 4,               ///< Enable alpha blending.
        DepthAndBlend = 5,       ///< Enable depth testing and alpha blending.
        CullFaceAndBlend=6,      ///< Enable face culling and alpha blending.
        DepthCullFaceAndBlend = 7 ///< Enable depth testing, face culling, and alpha blending.
    };

protected:
    /// @brief Internal GLSL program used for rendering text glyphs.
    class Prog : public gui::gl::Program
    {
        td::INT4 _samplerPos = -1; ///< Uniform location for the texture sampler.
        td::INT4 _colorPos = -1;   ///< Uniform location for the text color.
        td::INT4 _zPos = -1;       ///< Uniform location for the Z-depth position.
    protected:
        /// @brief Called after the shader program has been successfully linked.
        void onLinked() override;
        /// @brief Called when a shader linking error occurs.
        void onError() override;
    public:
        /// @brief Constructs the text shader program.
        Prog();
        /// @brief Sets the projection matrix uniform.
        /// @param projection The orthographic or perspective projection matrix.
        void setProjection(const glm::mat4& projection);
        /// @brief Sets the texture sampler unit uniform.
        /// @param unit The texture unit to bind the font atlas to.
        void setSampler(gui::gl::Textures::Unit unit);
        /// @brief Sets the text color uniform.
        /// @param clr RGBA color vector for the rendered text.
        void setColor(const glm::vec4& clr);
        /// @brief Sets the Z-depth position uniform.
        /// @param z Depth value used for layering text in 3D space.
        void setZPos(float z);
    };

protected:
    Prog _program;                                   ///< The internal GLSL shader program for text rendering.
    glm::mat4 _projectionMatrix;                     ///< Projection matrix applied to text geometry.
    cnt::SafeFullVector<gui::gl::Font> _fonts;        ///< Collection of loaded font objects.
    cnt::SafeFullVector<td::UTF32> _utf32;            ///< Buffer of UTF-32 encoded characters to render.
    gui::gl::Textures::Unit _textureUnit = gui::gl::Textures::Unit::T0; ///< Active texture unit used for font atlas binding.

protected:
    Text(const Text&) = delete;
    Text& operator =(const Text&) = delete;
public:
    /// @brief Default constructor. Initializes internal state without allocating GPU resources.
    Text();
    /// @brief Destructor. Releases all GPU and CPU resources.
    ~Text();

    //set initial frame size and set texture unit
    /// @brief Initializes the text renderer with the given frame dimensions and texture unit.
    /// @param frameSize The initial size of the rendering frame in pixels.
    /// @param tu The texture unit to use for the font atlas.
    void init(const gui::Size& frameSize, gui::gl::Textures::Unit tu);

    //should be called on frame resize
    /// @brief Updates the projection matrix when the frame is resized.
    /// @param frameSize The new size of the rendering frame in pixels.
    void setFrameSize(const gui::Size& frameSize);

    //allocate space for fonts
    /// @brief Reserves storage for a given number of fonts.
    /// @param nFonts The number of font slots to allocate.
    void allocFonts(td::BYTE nFonts);
    /// @brief Loads a font from a file into the specified slot.
    /// @param fontPos Index of the font slot to populate.
    /// @param fntFileName Path to the font file (null-terminated string).
    /// @return Pointer to the loaded Font object, or nullptr on failure.
    gui::gl::Font* loadFont(td::BYTE fontPos, const char* fntFileName);
    /// @brief Loads a font from a file into the specified slot.
    /// @param fontPos Index of the font slot to populate.
    /// @param fntFileName Path to the font file as a td::String.
    /// @return Pointer to the loaded Font object, or nullptr on failure.
    gui::gl::Font* loadFont(td::BYTE fontPos, const td::String& fntFileName);

    /// @brief Returns a reference to the font at the given slot index.
    /// @param fontPos Index of the desired font slot.
    /// @return Reference to the Font object at the specified position.
    gui::gl::Font& getFont(td::BYTE fontPos);

    //transfer data to GPU (must have active context)
    /// @brief Uploads all buffered text data to the GPU. Requires an active OpenGL context.
    void commit();

    //draw text
    /// @brief Renders all committed text using the specified GL state control.
    /// @param ctrl OpenGL state flags to apply during rendering.
    void render(Text::Ctrl ctrl = Text::Ctrl::DepthCullFaceAndBlend);

    //release all memory allocated by the Buffer
    /// @brief Releases all CPU and GPU memory allocated by the text renderer.
    void release();
};

} //namespace gl
} //namespace gui
