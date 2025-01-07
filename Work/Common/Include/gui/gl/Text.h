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
#include "Buffer.h"
#include "Program.h"
#include "Font.h"

namespace gui
{
namespace gl
{

class NATGL_API Text
{
    friend class Font;
public:
    enum class Ctrl : td::BYTE {Nothing=0, Depth = 1, CullFace = 2, DepthAndCullFace = 3, Blend = 4, DepthAndBlend = 5, CullFaceAndBlend=6, DepthCullFaceAndBlend = 7};
    
protected:
    class Prog : public gui::gl::Program
    {
        td::INT4 _samplerPos = -1;
        td::INT4 _colorPos = -1;
        td::INT4 _zPos = -1;
    protected:
        void onLinked() override;
        void onError() override;
    public:
        Prog();
        void setProjection(const glm::mat4& projection);
        void setSampler(gui::gl::Textures::Unit unit);
        void setColor(const glm::vec4& clr);
        void setZPos(float z);
    };
    
protected:
    Prog _program;
    glm::mat4 _projectionMatrix;
    cnt::SafeFullVector<gui::gl::Font> _fonts;
    cnt::SafeFullVector<td::UTF32> _utf32;
    gui::gl::Textures::Unit _textureUnit = gui::gl::Textures::Unit::T0;
private:
protected:
    
public:
    Text();
    ~Text(); 
    
    //set initial frame size and set texture unit
    void init(const gui::Size& frameSize, gui::gl::Textures::Unit tu);
    
    //should be called on frame resize
    void setFrameSize(const gui::Size& frameSize);
    
    //allocate space for fonts
    void allocFonts(td::BYTE nFonts);
    //load fonts
    gui::gl::Font* loadFont(td::BYTE fontPos, const char* fntFileName);
    gui::gl::Font* loadFont(td::BYTE fontPos, const td::String& fntFileName);
    
    gui::gl::Font& getFont(td::BYTE fontPos);
    
    //transfer data to GPU (must have active context)
    void commit();

    //draw text
    void render(Text::Ctrl ctrl = Text::Ctrl::DepthCullFaceAndBlend);

    //release all memory allocated by the Buffer
    void release();
};

} //namespace gl
} //namespace gui

