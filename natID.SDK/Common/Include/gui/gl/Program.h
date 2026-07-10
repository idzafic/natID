// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Program.h
 *  @brief GLSL shader program wrapper that manages linking, uniform locations, and command execution. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <gui/gl/GL.h>
#include <td/String.h>
#include <glm/glm.hpp>
#include <td/ColorID.h>
#include <cnt/SafeFullVector.h>
#include <gui/gl/Textures.h>
#include <gui/gl/CommandList.h>

namespace gui
{
namespace gl
{

class View;
class Buffer;
//class Command;

//class Textures;
class ProgHelper;
class SkyBox;

/// @brief Encapsulates a linked GLSL shader program with helpers for uniform management, buffer binding, and draw command encoding.
class NATGL_API Program
{
    friend class View;
    friend class Command;
    friend class ProgHelper;
    friend class SkyBox;
protected:
    gui::gl::CommandList _commandList;               ///< Ordered list of draw commands owned by this program; if empty, commands from _pBuffer are executed.
    gui::gl::Buffer* _pBuffer = nullptr;             ///< External GPU buffer activated together with this program.
    gui::gl::Textures* _pTextures = nullptr;         ///< Texture set bound when this program is activated.
    cnt::SafeFullVector<td::INT4> _uniformLocations; ///< Cached uniform locations indexed by user-defined slot (max 255).
    cnt::SafeFullVector<td::INT4> _uniformLocationsForMatrices; ///< Cached uniform locations for matrix uniforms (max 255).
    td::UINT4 _programID = 0;                        ///< OpenGL program object identifier.
    td::INT4 _color3Location = -1;                   ///< Uniform location of the vec3 color uniform (-1 if unused).
    td::INT4 _color4Location = -1;                   ///< Uniform location of the vec4 color uniform (-1 if unused).
    td::INT4 _matrixLocation = -1;                   ///< Uniform location of the MVP matrix uniform (-1 if unused).
private:
    /// @brief Internal helper that compiles and links vertex, fragment, and optional additional shaders.
    /// @param vertexResIDOrText Vertex shader resource ID or source text.
    /// @param fragmentResIDOrText Fragment shader resource ID or source text.
    /// @param tessCtrlShaderResID Optional tessellation control shader resource ID.
    /// @param tessEvalShaderResID Optional tessellation evaluation shader resource ID.
    /// @param geometryShaderID Optional geometry shader resource ID.
    /// @return The linked OpenGL program ID, or 0 on failure.
    static td::UINT4 _link(const char* vertexResIDOrText, const char* fragmentResIDOrText, const char* tessCtrlShaderResID = nullptr, const char* tessEvalShaderResID = nullptr, const char* geometryShaderID = nullptr);

protected:
    //check shader compilation
    /// @brief Checks whether a shader compiled without errors and logs any diagnostics.
    /// @param glslType The type of shader that was compiled.
    /// @param shaderID OpenGL shader object ID.
    /// @param shaderFileName Source file name used in error messages.
    /// @return True if compilation succeeded.
    static bool isCompilationOK(GLSL::Type glslType, td::UINT4 shaderID, const char* shaderFileName);

    //check GLSL shader linking into a program
    /// @brief Checks whether the program linked without errors and logs any diagnostics.
    /// @param programID OpenGL program object ID.
    /// @param vertexShaderFileName Vertex shader file name used in error messages.
    /// @param fragmentShaderFileName Fragment shader file name used in error messages.
    /// @return True if linking succeeded.
    static bool isLinkingOK(td::UINT4 programID, const char* vertexShaderFileName, const char* fragmentShaderFileName);

    //flloating point square matrices
    /// @brief Sets a mat4x4 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param mat The 4x4 matrix value.
    void setUniformValue(td::UINT4 uniformIndex, const glm::mat4x4& mat);

    /// @brief Sets a mat3x3 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param mat The 3x3 matrix value.
    void setUniformValue(td::UINT4 uniformIndex, const glm::mat3x3& mat);

    //floating point vectors
    /// @brief Sets a vec1 (float scalar) uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The scalar value wrapped in a glm::vec1.
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec1& vec);

    /// @brief Sets a vec2 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 2-component float vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec2& vec);

    /// @brief Sets a vec3 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 3-component float vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec3& vec);

    /// @brief Sets a vec4 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 4-component float vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec4& vec);

    //unisgned int (4bytes) vectors
    /// @brief Sets a uvec1 (uint scalar) uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The unsigned integer wrapped in a glm::uvec1.
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec1& vec);

    /// @brief Sets a uvec2 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 2-component unsigned integer vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec2& vec);

    /// @brief Sets a uvec3 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 3-component unsigned integer vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec3& vec);

    /// @brief Sets a uvec4 uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param vec The 4-component unsigned integer vector.
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec4& vec);

    //scalars
    /// @brief Sets an unsigned integer (uint) uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param val The unsigned integer value.
    void setUniformValue(td::UINT4 uniformIndex, td::UINT4 val);

    /// @brief Sets a float uniform at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param val The float value.
    void setUniformValue(td::UINT4 uniformIndex, float val);

    //sets RGB values from clrID using glm::vec3
    /// @brief Sets a vec3 RGB color uniform from a ColorID at the given slot index.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param clrID The color whose RGB components are extracted.
    void setUniformValue(td::UINT4 uniformIndex, td::ColorID clrID);

    //sets RGB values from clrID and A from alpha using glm::vec4
    /// @brief Sets a vec4 RGBA color uniform from a ColorID and an explicit alpha value.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param clrID The color whose RGB components are extracted.
    /// @param alpha The alpha value (0–255).
    void setUniformValue(td::UINT4 uniformIndex, td::ColorID clrID, td::BYTE alpha);

    /// @brief Binds a texture to the given sampler unit and sets the corresponding uniform.
    /// @param uniformIndex Index into _uniformLocations.
    /// @param textureType The type of the texture (2D, cube map, etc.).
    /// @param textureID OpenGL texture object ID.
    /// @param samplerUnit The texture unit slot to bind to.
    void setUniformValue(td::UINT4 uniformIndex, gui::gl::Texture::Type textureType, td::UINT4 textureID, gui::gl::Textures::Unit samplerUnit);

    //common (most used) setters
    /// @brief Sets the vec3 color uniform via the pre-cached _color3Location.
    /// @param color3 RGB color as a 3-component float vector.
    void setColor3(const glm::vec3& color3); //use _colorSetter3

    /// @brief Sets the vec4 color uniform via the pre-cached _color4Location.
    /// @param color4 RGBA color as a 4-component float vector.
    void setColor4(const glm::vec4& color4); //use _colorSetter4

    /// @brief Sets the MVP (model-view-projection) matrix uniform via the pre-cached _matrixLocation.
    /// @param mat4 The combined MVP transformation matrix.
    void setMVP(const glm::mat4x4& mat4); //use _matrixSetter

    //returns index of a uniform
    /// @brief Queries the OpenGL driver for a uniform location by name.
    /// @param uniformName GLSL uniform variable name.
    /// @return The uniform location, or -1 if not found.
    td::INT4 getUniformLocation(const char* uniformName);

    /// @brief Retrieves and caches the locations of multiple matrix uniforms by name.
    /// @param pMatrixNames Array of null-terminated uniform names.
    /// @param nMatrices Number of names in the array.
    /// @return True if all locations were found successfully.
    bool getUniformLocationsForMatrices(const char** pMatrixNames, size_t nMatrices);

    /// @brief Sets multiple matrix uniforms using the cached matrix locations.
    /// @param pMatrices Array of pointers to mat4x4 matrices.
    /// @param nMatrices Number of matrices.
    void setUniformValuesForMatrices(const glm::mat4x4** pMatrices, size_t nMatrices);

    /// @brief Sets multiple matrix uniforms from a contiguous array of mat4x4 values.
    /// @param pMatrices Pointer to the first matrix.
    /// @param nMatrices Number of matrices.
    void setUniformValuesForMatrices(const glm::mat4x4* pMatrices, size_t nMatrices);

    /// @brief Sets matrix uniforms from a SafeFullVector, optionally limiting how many are set.
    /// @param matrices Vector of mat4x4 matrices.
    /// @param nToSet Number of matrices to set (0 = all).
    void setUniformValuesForMatrices(const cnt::SafeFullVector<glm::mat4x4>& matrices, size_t nToSet = 0); //0-means all, otherwise first nToSet

    /// @brief Returns the Textures object associated with this program.
    /// @return Pointer to the Textures object, or nullptr if none is set.
    gui::gl::Textures* getTextures();

    /// @brief Pre-allocates storage for the given number of uniform slots.
    /// @param nUniforms Maximum number of uniforms this program will use (max 255).
    void reserveUniforms(td::BYTE nUniforms);

    /// @brief Checks that all cached uniform locations are valid (>= 0).
    /// @return True if every cached location is valid.
    bool uniformLocationsAreOk() const;

    //if linking is ok onLinked is called
    /// @brief Called after successful program linking; override to cache uniform locations.
    virtual void onLinked();

    //if linking is NOT ok onError is called
    /// @brief Called after a failed program link; override to handle errors.
    virtual void onError();

    /// @brief Deleted copy constructor — Program objects are not copyable.
    Program(const Program&) = delete;
    /// @brief Deleted copy assignment operator — Program objects are not copyable.
    Program& operator =(const Program&) = delete;
public:
    /// @brief Default constructor.
    Program();

    /// @brief Destructor — releases the OpenGL program object.
    ~Program();

    //compile and link using text resources for vertex and fragment shader and optional geometry shader
    /// @brief Compiles and links a program from vertex and fragment shader resources.
    /// @param vertexShaderResID Vertex shader resource identifier or source text.
    /// @param fragmentShaderResID Fragment shader resource identifier or source text.
    /// @param geometryShaderResID Optional geometry shader resource identifier.
    /// @return True if linking succeeded.
    bool link(const char* vertexShaderResID, const char* fragmentShaderResID, const char* geometryShaderResID = nullptr);

    //compile and link using text resources for vertex, fragment, tesselation, and tess control shader and optional geometry shader
    /// @brief Compiles and links a program from vertex, fragment, tessellation, and optional geometry shader resources.
    /// @param vertexShaderResID Vertex shader resource identifier.
    /// @param fragmentShaderResID Fragment shader resource identifier.
    /// @param tessCtrlShaderResID Tessellation control shader resource identifier.
    /// @param tessEvalShaderResID Tessellation evaluation shader resource identifier.
    /// @param geometryShaderResID Optional geometry shader resource identifier.
    /// @return True if linking succeeded.
    bool link(const char* vertexShaderResID, const char* fragmentShaderResID, const char* tessCtrlShaderResID, const char* tessEvalShaderResID, const char* geometryShaderResID = nullptr);

    /// @brief Returns the OpenGL program object ID.
    /// @return The program ID, or 0 if not linked.
    td::UINT4 getID() const;

    //check if program is ok
    /// @brief Returns whether the program was linked successfully.
    /// @return True if the program is ready to use.
    bool isOK() const;

    //get accumulated error
    /// @brief Returns the accumulated GLSL error string from the most recent failed link attempt.
    /// @return The error string.
    static td::String getLastError();

    //set buffer which will be activated when Program activates
    /// @brief Associates an external GPU buffer that is activated together with this program.
    /// @param pBuffer Pointer to the buffer.
    void setBuffer(gui::gl::Buffer* pBuffer);

    /// @brief Associates a Textures set that is bound when this program is activated.
    /// @param pTextures Pointer to the Textures object.
    void setTextures(gui::gl::Textures* pTextures);

    //buffer layoutID to be used with
    /// @brief Activates the program (and its associated buffer if set) for rendering.
    void activate() const;

    /// @brief Executes all commands in the internal command list (or from the buffer if the list is empty). Requires a previous call to activate().
    void execute(); //requires previous call to activate, execute own commands if any, otherwise execute commands from buffer

    /// @brief Executes a sub-range of commands from the associated buffer. Requires a previous call to activate().
    /// @param startPos Index of the first command to execute.
    /// @param count Number of commands to execute.
    void execute(size_t startPos, size_t count); //requires previous call to activate, it will execute commands from buffer

    //commands
    //reserve number of commands
    /// @brief Pre-allocates space in the internal command list.
    /// @param nCommands Expected number of commands.
    void reserveCommands(size_t nCommands);

    //places command on the list and links function pointers
    /// @brief Encodes a Command into the internal command list.
    /// @param pComm Pointer to the command to encode.
    /// @return True if the command was added successfully.
    bool encode(gui::gl::Command* pComm);

    //removes pComm from list if it is last in the list
    /// @brief Removes the last command from the list if it matches pComm.
    /// @param pComm Pointer to the command to remove.
    /// @return True if the command was removed.
    bool remove(gui::gl::Command* pComm);

    //creates empy command
    /// @brief Allocates a new empty Command and appends it to the list.
    /// @return Pointer to the newly created Command.
    gui::gl::Command* createCommand();

    /// @brief Deactivates this program and restores the default program state.
    void deActivate() const;

    /// @brief Releases the OpenGL program object and resets internal state.
    void release();

    /// @brief Unbinds any currently active program (calls glUseProgram(0)).
    static void useNone();
};

} //namespace gl
} //namespace gui
