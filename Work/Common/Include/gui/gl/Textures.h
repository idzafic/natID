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
#include <gui/gl/natGL.h>
#include <cnt/PushBackVector.h>
#include <gui/Texture.h>
#include <gui/gl/Types.h>
#include <tuple>
#include <cnt/Array.h>

namespace gui
{
namespace gl
{
class NATGL_API Textures
{
    class TextureInfo
    {
    public:
        td::UINT4 ID = 0;
        gui::gl::Texture::Type type = gui::gl::Texture::Type::None;
    };
    
public:
    //Texture units
    enum class Unit : td::BYTE {T0=0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31};
private:
    cnt::PushBackVector<TextureInfo> _textureIDs;
    td::UINT2 _nMaxUnits = 0;
    td::UINT2 _maxCombined = 0;
private:
    
public:
	Textures();
    ~Textures();
    
    void reserve(td::UINT2 nTextures);
    
    td::UINT2 size() const;
    td::UINT2 capacity() const;
    
    bool append(const std::initializer_list<gui::Texture*>& textures, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);
    bool append(const std::initializer_list<const char*>& textureNames, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);
    bool appendCubeMap(const cnt::Array<const char*, 6>& timgFaceNames); //in order Left, Front, Right, Back, Top, Bottom
    
    void setActiveUnit(Unit unit);
    //iPos is position of texture within Textures (position in _textureIDs)
    void bindToActiveUnit(td::UINT2 iPos);
    void setActive(Unit unit, td::UINT2 iPos);
    void setUniform(td::INT4 uniformLocation, gui::gl::Textures::Unit unit, td::UINT2 iPos);
    
    static void bindToSamplerUnit(td::UINT4 textureID, gui::gl::Textures::Unit unit, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);
    
    td::UINT4 getID(td::UINT2 iPos) const;
    gui::gl::Texture::Type getType(td::UINT2 iPos) const;
    std::tuple<td::UINT4, gui::gl::Texture::Type> getIDAndType(td::UINT2 iPos) const;
    
    td::UINT1 getAllocatedUnits() const;
    td::UINT2 getMaximumUnits() const;
    td::UINT2 getMaximumCombinedUnits() const;
    
    static td::UINT4 getNatPosID(Unit pos);
    
    void release();
    
    
};

} //namespace gl
} //namespace gui

