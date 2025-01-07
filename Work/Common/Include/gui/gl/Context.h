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
#include "natGL.h"
//#include <gui/NatObject.h>
#include <td/Types.h>
#include <tuple>
#include <td/ColorID.h>
#include "Textures.h"

namespace gui
{
namespace gl
{

class ContextHelper;

class NATGL_API Context //: public NatObject
{
    friend class ContextHelper;
public:
    enum class Animation : td::BYTE {Yes, No};
    enum class Profile : td::BYTE {Core3_2=0, Core4_1};
    enum class Clear : td::BYTE {Color=1, Depth, Stencil};
    enum class Flag : td::BYTE {DepthTest=0, CullFace, StencilTest, Blend};
    enum class PolygonMode : td::BYTE{Fill=0, Line, Point};
private:
    Profile _profile; //min required
    DepthBuffer::Size _depthBufferSize = DepthBuffer::Size::B0; //0-bytes (no depth buffer)
    StencilBuffer::Size _stencilBufferSize = StencilBuffer::Size::B0; //0-bytes (no stencil buffer)
    float _minFPS = 0;
    float _maxFPS = 0;
    td::BYTE _animation = 0;
    td::BYTE _rgbSize = 24;
    td::BYTE _alphaSize = 8;
    
protected:
public:
    Context(Animation animation, DepthBuffer::Size depthBufferSize = DepthBuffer::Size::B0, StencilBuffer::Size stencilBufferSize = StencilBuffer::Size::B0, Context::Profile profile = Profile::Core4_1);
    Context(DepthBuffer::Size depthBufferSize = DepthBuffer::Size::B0, StencilBuffer::Size stencilBufferSize = StencilBuffer::Size::B0, Context::Profile profile = Profile::Core4_1);
    Context::Profile getProfile() const;
    void setRGBSize(td::BYTE numberOfBitsForFGB, td::BYTE numberOfBitsForAlpha);
//    void setAnimation(bool bAnimation);
    bool isAnimation() const;
    std::tuple<td::BYTE, td::BYTE> getRGBSize() const;
    td::BYTE getDepthBufferSize() const; //in bits
    td::BYTE getStencilBufferSize() const; //in bits
    void setPreferredFrameRateRange(float minFPS, float maxFPS);
    std::tuple<float, float> getPreferredFrameRateRange() const;
    
    static void clear(Clear clearBit);
    static void clear(const std::initializer_list<Clear>& clearList);
    static void clear(td::ColorID clr);
    static void enable(Flag flag);
    static void disable(Flag flag);
    static void maskDepthBuffer(bool bMask);
    static void setDepthBufferFunction(gui::gl::DepthBuffer::Function fn);
    static void setStencilBufferMask(td::UINT4 mask);
    static void setStencilBufferFunction(gui::gl::StencilBuffer::Function fn, td::INT4 ref, td::UINT4 mask);
    static void setStencilBufferOperation(gui::gl::StencilBuffer::Operation opStencilFail, gui::gl::StencilBuffer::Operation opStencilOkDepthFail,gui::gl::StencilBuffer::Operation opStencilAndDepthOK);
    
    static void setBlendFunction(gui::gl::Blend::Factor sourceFactor, gui::gl::Blend::Factor destFactor);
    static void setPointSize(float pointSize);
    static void setPolygonMode(gui::gl::Context::PolygonMode mode);
//    static void setStencilBufferMask(td::UINT4 mask);
//        static void setStencilBufferFunction(gui::gl::StencilBuffer::Fn fn, td::INT4 ref, td::UINT4 mask);
//        static void setStencilBufferOperation(gui::gl::StencilBuffer::Op fnStencilFail, gui::gl::StencilBuffer::Op fnStencilOkDepthFail, gui::gl::StencilBuffer::Op fnStencilAndDepthOK);
    
    static void setActiveTextureUnit(gui::gl::Textures::Unit unit);
    static void bindTextureToCurrentlyActiveTextureUnit(td::UINT4 textureID, gui::gl::Texture::Type textureType);
    
    ~Context();
};

} //namespace gl
} //namespace gui
