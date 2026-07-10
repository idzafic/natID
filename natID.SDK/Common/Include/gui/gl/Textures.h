// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Textures.h
    @brief OpenGL texture manager for loading, binding, and querying multiple textures. */
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
/// @brief Manages a collection of OpenGL textures and their binding to texture units.
class NATGL_API Textures
{
    /// @brief Internal descriptor holding a texture object's GPU ID and type.
    class TextureInfo
    {
    public:
        td::UINT4 ID = 0;                                      ///< OpenGL texture object ID.
        gui::gl::Texture::Type type = gui::gl::Texture::Type::None; ///< Texture type (2D, cube map, etc.).
    };

public:
    //Texture units
    /// @brief Enumeration of the 32 available OpenGL texture units (T0–T31).
    enum class Unit : td::BYTE {
        T0=0,  ///< Texture unit 0.
        T1,    ///< Texture unit 1.
        T2,    ///< Texture unit 2.
        T3,    ///< Texture unit 3.
        T4,    ///< Texture unit 4.
        T5,    ///< Texture unit 5.
        T6,    ///< Texture unit 6.
        T7,    ///< Texture unit 7.
        T8,    ///< Texture unit 8.
        T9,    ///< Texture unit 9.
        T10,   ///< Texture unit 10.
        T11,   ///< Texture unit 11.
        T12,   ///< Texture unit 12.
        T13,   ///< Texture unit 13.
        T14,   ///< Texture unit 14.
        T15,   ///< Texture unit 15.
        T16,   ///< Texture unit 16.
        T17,   ///< Texture unit 17.
        T18,   ///< Texture unit 18.
        T19,   ///< Texture unit 19.
        T20,   ///< Texture unit 20.
        T21,   ///< Texture unit 21.
        T22,   ///< Texture unit 22.
        T23,   ///< Texture unit 23.
        T24,   ///< Texture unit 24.
        T25,   ///< Texture unit 25.
        T26,   ///< Texture unit 26.
        T27,   ///< Texture unit 27.
        T28,   ///< Texture unit 28.
        T29,   ///< Texture unit 29.
        T30,   ///< Texture unit 30.
        T31    ///< Texture unit 31.
    };
private:
    cnt::PushBackVector<TextureInfo> _textureIDs; ///< List of loaded texture descriptors.
    td::UINT2 _nMaxUnits = 0;                     ///< Maximum number of texture image units available.
    td::UINT2 _maxCombined = 0;                   ///< Maximum number of combined texture units.
private:
    Textures(const Textures&) = delete;
    Textures& operator =(const Textures&) = delete;
public:
    /// @brief Default constructor. Queries the driver for texture unit limits.
	Textures();
    /// @brief Destructor. Releases all GPU texture objects.
    ~Textures();

    /// @brief Reserves capacity for the given number of textures.
    /// @param nTextures Number of texture slots to pre-allocate.
    void reserve(td::UINT2 nTextures);

    /// @brief Returns the number of textures currently loaded.
    /// @return Current texture count.
    td::UINT2 size() const;
    /// @brief Returns the reserved capacity of the texture storage.
    /// @return Capacity in number of textures.
    td::UINT2 capacity() const;

    /// @brief Loads and appends textures from a list of gui::Texture pointers.
    /// @param textures Initializer list of pointers to gui::Texture objects.
    /// @param textureType The OpenGL texture type for all supplied textures.
    /// @return True if all textures were appended successfully, false otherwise.
    bool append(const std::initializer_list<gui::Texture*>& textures, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);
    /// @brief Loads and appends textures from a list of resource name strings.
    /// @param textureNames Initializer list of texture resource name strings.
    /// @param textureType The OpenGL texture type for all supplied textures.
    /// @return True if all textures were appended successfully, false otherwise.
    bool append(const std::initializer_list<const char*>& textureNames, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);
    /// @brief Loads and appends a cube map texture from six face image files.
    /// @param timgFaceNames Array of six image file names in order: Left, Front, Right, Back, Top, Bottom.
    /// @return True if the cube map was loaded successfully, false otherwise.
    bool appendCubeMap(const cnt::Array<const char*, 6>& timgFaceNames); //in order Left, Front, Right, Back, Top, Bottom

    /// @brief Activates the given texture unit as the current texture unit.
    /// @param unit The texture unit to activate.
    void setActiveUnit(Unit unit);
    //iPos is position of texture within Textures (position in _textureIDs)
    /// @brief Binds the texture at the given internal position to the currently active unit.
    /// @param iPos Zero-based index into the internal texture list.
    void bindToActiveUnit(td::UINT2 iPos);
    /// @brief Activates a texture unit and binds the texture at the given position to it.
    /// @param unit The texture unit to activate.
    /// @param iPos Zero-based index into the internal texture list.
    void setActive(Unit unit, td::UINT2 iPos);
    /// @brief Sets a sampler uniform to the specified texture unit and binds a texture to it.
    /// @param uniformLocation The GLSL uniform location of the sampler variable.
    /// @param unit The texture unit to use.
    /// @param iPos Zero-based index into the internal texture list.
    void setUniform(td::INT4 uniformLocation, gui::gl::Textures::Unit unit, td::UINT2 iPos);

    /// @brief Binds an existing OpenGL texture ID directly to a sampler unit.
    /// @param textureID The raw OpenGL texture object ID.
    /// @param unit The texture unit to bind to.
    /// @param textureType The OpenGL texture target type.
    static void bindToSamplerUnit(td::UINT4 textureID, gui::gl::Textures::Unit unit, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::Regular2D);

    /// @brief Returns the raw OpenGL texture ID for the texture at the given position.
    /// @param iPos Zero-based index into the internal texture list.
    /// @return OpenGL texture object ID.
    td::UINT4 getID(td::UINT2 iPos) const;
    /// @brief Returns the texture type for the texture at the given position.
    /// @param iPos Zero-based index into the internal texture list.
    /// @return The texture type enumeration value.
    gui::gl::Texture::Type getType(td::UINT2 iPos) const;
    /// @brief Returns both the ID and type for the texture at the given position.
    /// @param iPos Zero-based index into the internal texture list.
    /// @return A tuple containing the texture ID and its type.
    std::tuple<td::UINT4, gui::gl::Texture::Type> getIDAndType(td::UINT2 iPos) const;

    /// @brief Returns the number of texture units currently allocated.
    /// @return Count of allocated texture units as a single-byte unsigned integer.
    td::UINT1 getAllocatedUnits() const;
    /// @brief Returns the maximum number of texture image units reported by the driver.
    /// @return Maximum texture unit count.
    td::UINT2 getMaximumUnits() const;
    /// @brief Returns the maximum number of combined texture units reported by the driver.
    /// @return Maximum combined texture unit count.
    td::UINT2 getMaximumCombinedUnits() const;

    /// @brief Converts a Unit enum value to the corresponding OpenGL native texture unit ID.
    /// @param pos The texture unit enum value.
    /// @return The native OpenGL texture unit constant (e.g., GL_TEXTURE0 + index).
    static td::UINT4 getNatPosID(Unit pos);

    /// @brief Releases all GPU texture objects managed by this instance.
    void release();


};

} //namespace gl
} //namespace gui
