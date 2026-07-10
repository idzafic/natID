// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Command.h
 * @brief Represents a single renderable GPU draw or uniform-setter command.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <td/Types.h>
#include <gui/gl/GL.h>
#include <functional>
#include <td/ColorID.h>
#include <td/Color.h>
//#include <gui/gl/Program.h>
#include <tuple>
#include <gui/gl/Textures.h>

namespace gui
{
namespace gl
{

class CommandList;

/// @brief Encapsulates a single OpenGL draw or uniform-setter command that can be encoded and executed.
class NATGL_API Command
{
    friend class CommandList;
public:
    /// @brief Enumerates the high-level type of an encoded command.
    enum class Type : td::BYTE {None = 0, DrawArrays, DrawArrayPointsWithSize, DrawArrayPatchesWithSize, DrawElementPointsWithSize, DrawElementPatchesWithSize, DrawElements, DrawArraysInstanced, DrawElementsInstanced, MVP, Uniforms, Custom};
    /// @brief Enumerates the drawing sub-type used to select the appropriate rendering path.
    enum class DrawSubType : td::BYTE {None=0, DrawWithClr3, DrawWithClr3z, DrawWithClr4, DrawWithClr4z, DrawWithTexture, DrawWithTexturez, DrawInstanced, DrawWithTextureInstanced};

    /// @brief Enumerates the kinds of additional uniform values that can be attached to a command.
    enum class Uniform : td::BYTE {None=0, ClrID, Z, Byte, Clr3, Clr4, Mat4Ptr, Flt, Uint, Vec2, Vec3, Uvec2, Uvec3, Vec1Ptr, Vec2Ptr, Vec3Ptr, Vec4Ptr, Uvec1Ptr, Uvec2Ptr, Uvec3Ptr, Uvec4Ptr};

    /// @brief Holds an additional uniform value together with its type and shader binding position.
    typedef struct _additinalUniform
    {
        Uniform type;   ///< Type of the uniform value.
        td::BYTE pos;   ///< Shader uniform location index.
        union
        {
            void* ptrVal;        ///< Generic pointer value.
            glm::vec3 vec3;      ///< Three-component float vector.
            glm::uvec3 uvec3;    ///< Three-component unsigned integer vector.
            glm::vec2 vec2;      ///< Two-component float vector.
            glm::uvec2 uvec2;    ///< Two-component unsigned integer vector.
            float fVal;          ///< Single float scalar.
            td::UINT4 u4Val;     ///< Unsigned 32-bit integer scalar.
            td::BYTE clr[4];     ///< RGBA color stored as four bytes.
            td::ColorID clrID;   ///< Color identified by a color-ID enum value.
            td::BYTE byteVal;    ///< Single byte value.
            td::UINT2 u2Val;     ///< Unsigned 16-bit integer scalar.
        };
    } AdditionalUniform;

    // Define a typedef for the function pointer type
    typedef void (*CommandPtr)(const Command*); ///< Function pointer type for command execution callbacks.
    /// @brief Describes a contiguous range of vertices or indices used by a draw call.
    typedef struct _Range
    {
        td::UINT4 startPos; ///< Starting index or vertex position.
        td::UINT4 len;      ///< Number of elements in the range.
    }Range;

    /// @brief Stores a color identified by a color-ID and an explicit alpha channel.
    typedef struct _Color
    {
        td::ColorID colorID = td::ColorID::Transparent; ///< Color identifier.
        td::BYTE alpha = 255;                           ///< Alpha transparency value (0-255).
    } Color;

    /// @brief Internal layout helper used to zero-initialise the command union.
    typedef struct _cleaner
    {
        td::LUINT8 first;  ///< First 8-byte clearing field.
        td::UINT4 second;  ///< Second 4-byte clearing field.
        td::UINT2 third;   ///< Third 2-byte clearing field.
        td::BYTE forth;    ///< Fourth 1-byte clearing field.
    } Cleaner;

    /// @brief Parameters for a basic draw call (DrawArrays or DrawElements).
    typedef struct _draw
    {
        Range range;              ///< Vertex or index range to draw.
        Primitive primType;       ///< OpenGL primitive topology.
        Uniform firstUniform;     ///< Type of the first additional uniform.
        td::BYTE firstUniformPos; ///< Shader location of the first additional uniform.
        td::BYTE uniforms[4];     ///< Packed additional uniform data bytes.
    } Draw;

    /// @brief Parameters for a point draw call with a custom point size.
    typedef struct _drawPointsWithPtSize
    {
        Range range;              ///< Vertex or index range to draw.
        Primitive primType;       ///< OpenGL primitive topology (typically Points).
        float pointSize;          ///< GL point size in pixels.
        Uniform firstUniform;     ///< Type of the first additional uniform.
        td::BYTE firstUniformPos; ///< Shader location of the first additional uniform.
    } DrawPointsWithPtSize;

    /// @brief Parameters for a patch draw call with a specified vertices-per-patch count.
    typedef struct _drawPatchesWithSize
    {
        Range range;              ///< Vertex or index range to draw.
        Primitive primType;       ///< OpenGL primitive topology (Patches).
        td::UINT4 patchesPerVertex; ///< Number of control points per patch.
        Uniform firstUniform;     ///< Type of the first additional uniform.
        td::BYTE firstUniformPos; ///< Shader location of the first additional uniform.
    } DrawPatchesWithSize;

    /// @brief Draw parameters with an RGB (3-component) color uniform.
    typedef struct _clrDraw3
    {
        Range range;              ///< Vertex or index range to draw.
        Primitive primType;       ///< OpenGL primitive topology.
        td::BYTE rgb[3];          ///< RGB color components (red, green, blue).
        Uniform firstUniform;     ///< Type of the first additional uniform.
        td::BYTE firstUniformPos; ///< Shader location of the first additional uniform.
        td::BYTE uniformVal;      ///< Packed value for the first uniform.
    } ClrDraw3;

    /// @brief Draw parameters with an RGB color and a depth (z) value.
    typedef struct _clrDraw3z
    {
        Range range;         ///< Vertex or index range to draw.
        Primitive primType;  ///< OpenGL primitive topology.
        td::BYTE rgb[3];     ///< RGB color components.
        td::BYTE z;          ///< Encoded depth/z level.
        td::BYTE dummy[2];   ///< Padding bytes.
    } ClrDraw3z;

    /// @brief Draw parameters with an RGBA (4-component) color uniform.
    typedef struct _clrDraw4
    {
        Range range;         ///< Vertex or index range to draw.
        Primitive primType;  ///< OpenGL primitive topology.
        td::Color rgba;      ///< RGBA color value.
        td::BYTE dummy[2];   ///< Padding bytes.
    } ClrDraw4;

    /// @brief Draw parameters with an RGBA color and a depth (z) value.
    typedef struct _clrDraw4z
    {
        Range range;         ///< Vertex or index range to draw.
        Primitive primType;  ///< OpenGL primitive topology.
        td::Color rgba;      ///< RGBA color value.
        td::BYTE z;          ///< Encoded depth/z level.
        td::BYTE dummy;      ///< Padding byte.
    } ClrDraw4z;

    /// @brief Draw parameters with a texture sampler binding.
    typedef struct _clrDrawTexture
    {
        Range range;                           ///< Vertex or index range to draw.
        Primitive primType;                    ///< OpenGL primitive topology.
        gui::gl::Textures::Unit texturePosition; ///< GPU texture unit slot.
        td::BYTE textureID;                    ///< Index into the Textures container.
        Uniform firstUniform;                  ///< Type of the first additional uniform.
        td::BYTE firstUniformPos;              ///< Shader location of the first additional uniform.
        td::BYTE uniforms[2];                  ///< Additional packed uniform data.
    } ClrDrawTexture;

    /// @brief Draw parameters with a texture sampler binding and a depth (z) value.
    typedef struct _clrDrawTexturez
    {
        Range range;                           ///< Vertex or index range to draw.
        Primitive primType;                    ///< OpenGL primitive topology.
        gui::gl::Textures::Unit texturePosition; ///< GPU texture unit slot.
        td::BYTE textureID;                    ///< Index into the Textures container.
        td::BYTE z;                            ///< Encoded depth/z level.
        Uniform firstUniform;                  ///< Type of the first additional uniform.
        td::BYTE firstUniformPos;              ///< Shader location of the first additional uniform.
        td::BYTE uniform;                      ///< Packed additional uniform byte.
    } ClrDrawTexturez;

    /// @brief Parameters for an instanced draw call.
    typedef struct _drawInstanced
    {
        Range range;             ///< Vertex or index range to draw per instance.
        Primitive primType;      ///< OpenGL primitive topology.
        td::UINT4 noOfInstances; ///< Number of instances to render.
        td::BYTE dummy[2];       ///< Padding bytes.
    } DrawInstanced;

    /// @brief Parameters for an instanced draw call with a texture sampler.
    typedef struct _drawTextureInstanced
    {
        Range range;               ///< Vertex or index range to draw per instance.
        Primitive primType;        ///< OpenGL primitive topology.
        td::UINT2 noOfInstances;   ///< Number of instances to render.
        td::BYTE textureID;        ///< Index into the Textures container.
        Uniform firstUniform;      ///< Type of the first additional uniform.
        td::BYTE firstUniformPos;  ///< Shader location of the first additional uniform.
        td::BYTE uniform;          ///< Packed additional uniform byte.
    } DrawTextureInstanced;

    /// @brief Parameters for setting a Model-View-Projection matrix uniform.
    typedef struct _mvp
    {
        const glm::mat4x4* pMatrix; ///< Pointer to the MVP transformation matrix.
        Uniform firstUniform;       ///< Type of the first additional uniform.
        td::BYTE firstUniformPos;   ///< Shader location of the first additional uniform.
        td::BYTE uniforms[5];       ///< Additional packed uniform data.
    } MVP;

    /// @brief Parameters for a standalone uniform-setter command (no draw call).
    typedef struct _uniforms
    {
        Uniform firstUniform;     ///< Type of the first uniform to set.
        td::BYTE firstUniformPos; ///< Shader location of the first uniform.
        td::BYTE uniforms[13];    ///< Additional packed uniform data bytes.
    } Uniforms;



private:
    CommandPtr _command = nullptr; ///< Pointer to the compiled command execution function.
public:
    union
    {
        Cleaner cleaner;                             ///< Used for zero-initialisation of the union.
        Draw draw;                                   ///< Basic draw call parameters.
        DrawPointsWithPtSize drawPointsWithPointSize; ///< Point draw call with point-size parameter.
        DrawPatchesWithSize drawPatchesWithSize;      ///< Patch draw call with patch-size parameter.
        ClrDraw3 clrDraw3;                           ///< Draw call with RGB color uniform.
        ClrDraw3z clrDraw3z;                         ///< Draw call with RGB color and depth.
        ClrDraw4 clrDraw4;                           ///< Draw call with RGBA color uniform.
        ClrDraw4z clrDraw4z;                         ///< Draw call with RGBA color and depth.
        ClrDrawTexture clrDrawTexture;               ///< Draw call with texture binding.
        ClrDrawTexturez clrDrawTexturez;             ///< Draw call with texture binding and depth.
        DrawInstanced drawInstanced;                 ///< Instanced draw call parameters.
        DrawTextureInstanced drawTextureInstanced;   ///< Instanced draw call with texture.
        MVP mvp;                                     ///< MVP matrix setter parameters.
        Uniforms uniforms;                           ///< Standalone uniform setter parameters.
    };

private:
    td::BYTE _type : 4 = 0;        ///< Packed command type (4 bits).
    td::BYTE _drawSubType : 4 = 0; ///< Packed draw sub-type (4 bits).
private:
//    std::tuple<Type, DrawSubType> getTypeInfo() const;
    /// @brief Returns the high-level command type.
    /// @return The Type enum value for this command.
    inline Type getType() const
    {
        return (Type) _type;
    }

    /// @brief Returns the draw sub-type for this command.
    /// @return The DrawSubType enum value for this command.
    inline DrawSubType getDrawSubType() const
    {
        return (DrawSubType) _drawSubType;
    }

    /// @brief Checks whether this command is a draw command.
    /// @return true if the command issues a draw call.
    inline bool isDraw() const;

    /// @brief Sets the type and sub-type fields together.
    /// @param type High-level command type.
    /// @param drawSubType Draw sub-type for this command.
    void setTypeInfo(Type type, DrawSubType drawSubType);

    /// @brief Encodes the command as a DrawElements call.
    void encodeDrawElements();
    /// @brief Encodes the command as a DrawArrays call.
    void encodeDrawArrays();
    /// @brief Encodes the command as a DrawElementsInstanced call.
    void encodeDrawElementsInstanced();
    /// @brief Encodes the command as a DrawArraysInstanced call.
    void encodeDrawArraysInstanced();
    /// @brief Encodes the command as an MVP matrix setter.
    void encodeMatrix();

    /// @brief Executes the encoded command against the currently bound program.
    void execute();
protected:
    /// @brief Resets the command to its default (unencoded) state.
    void reset();
public:
    /// @brief Default constructor; creates an unencoded command.
    Command();

    //command creation
    /// @brief Creates a DrawElements command.
    /// @param primType OpenGL primitive topology to use.
    /// @param indexStartPos Starting offset in the index buffer.
    /// @param nIndices Number of indices to draw.
    /// @return Reference to this command for chaining.
    Command& createDrawElements(gui::gl::Primitive primType, size_t indexStartPos, td::UINT4 nIndices);

    /// @brief Creates a DrawArrays command.
    /// @param primType OpenGL primitive topology to use.
    /// @param vertexStartPos Starting offset in the vertex buffer.
    /// @param nVertices Number of vertices to draw.
    /// @return Reference to this command for chaining.
    Command& createDrawArrays(gui::gl::Primitive primType, size_t vertexStartPos, td::UINT4 nVertices);

    /// @brief Creates a DrawElementsInstanced command.
    /// @param primType OpenGL primitive topology to use.
    /// @param indexStartPos Starting offset in the index buffer.
    /// @param nIndices Number of indices per instance.
    /// @param nInstances Number of instances to render.
    /// @return Reference to this command for chaining.
    Command& createDrawElementsInstanced(gui::gl::Primitive primType, size_t indexStartPos, td::UINT4 nIndices, td::UINT4 nInstances);

    /// @brief Creates a DrawArraysInstanced command.
    /// @param primType OpenGL primitive topology to use.
    /// @param vertexStartPos Starting offset in the vertex buffer.
    /// @param nVertices Number of vertices per instance.
    /// @param nInstances Number of instances to render.
    /// @return Reference to this command for chaining.
    Command& createDrawArraysInstanced(gui::gl::Primitive primType, size_t vertexStartPos, td::UINT4 nVertices, td::UINT4 nInstances);

    /// @brief Creates an MVP matrix setter command.
    /// @param pUniformMatrix Pointer to the mat4 MVP matrix.
    /// @param pAdditionalUniforms Optional array of additional uniform descriptors.
    /// @param nUniforms Number of additional uniforms in the array.
    /// @return Reference to this command for chaining.
    Command& createMVPSetter(const glm::mat4x4* pUniformMatrix, AdditionalUniform* pAdditionalUniforms = nullptr, td::BYTE nUniforms=0);

    /// @brief Creates a standalone uniform setter command.
    /// @param pUniforms Array of uniform descriptors to set.
    /// @param nUniforms Number of uniforms in the array.
    /// @return true on success, false if the uniform list is too large.
    bool createUniformSetter(AdditionalUniform* pUniforms, td::BYTE nUniforms);

    /// @brief Creates a DrawArrays command for point primitives with a custom point size.
    /// @param vertexStartPos Starting offset in the vertex buffer.
    /// @param nVertices Number of point vertices to draw.
    /// @param pointSize GL point size in pixels.
    /// @return Reference to this command for chaining.
    Command& createDrawArrayPoints(size_t vertexStartPos, td::UINT4 nVertices, float pointSize);

    /// @brief Creates a DrawElements command for point primitives with a custom point size.
    /// @param indexStartPos Starting offset in the index buffer.
    /// @param nIndices Number of indices to draw.
    /// @param pointSize GL point size in pixels.
    /// @return Reference to this command for chaining.
    Command& createDrawElementPoints(size_t indexStartPos, td::UINT4 nIndices, float pointSize);

    /// @brief Creates a DrawArrays command for tessellation patches with a specified patch size.
    /// @param vertexStartPos Starting offset in the vertex buffer.
    /// @param nVertices Total number of patch control-point vertices.
    /// @param verticesPerPatch Number of control points per patch.
    /// @return Reference to this command for chaining.
    Command& createDrawArrayPatches(size_t vertexStartPos, td::UINT4 nVertices, int verticesPerPatch);

    /// @brief Creates a DrawElements command for tessellation patches with a specified patch size.
    /// @param indexStartPos Starting offset in the index buffer.
    /// @param nIndices Total number of patch indices.
    /// @param verticesPerPatch Number of control points per patch.
    /// @return Reference to this command for chaining.
    Command& createDrawElementPatches(size_t indexStartPos, td::UINT4 nIndices, int verticesPerPatch);

    //instanced drawing (command previously created with createDrawElements or createDrawArrays)
    /// @brief Overrides the instance count for a previously created instanced draw command.
    /// @param nInstances New number of instances to render.
    /// @return Reference to this command for chaining.
    Command& setNumberOfInstances(td::UINT4 nInstances);

    //Additional uniforms
    /// @brief Attaches additional uniform values to an existing draw command.
    /// @param pUniforms Array of uniform descriptors.
    /// @param nUniforms Number of uniforms in the array.
    /// @return true on success, false if the values could not be stored.
    bool setAdditionalUniforms(AdditionalUniform* pUniforms, td::BYTE nUniforms=1);

    /// @brief Binds a texture to the command's texture sampler slot.
    /// @param gpuPosition GPU texture unit to bind to.
    /// @param textureLoc Index of the texture within the Textures container.
    /// @return Reference to this command for chaining.
    Command& setTexture(gui::gl::Textures::Unit gpuPosition, td::INT1 textureLoc);

    //RGB used in color uniform (color3 or vec3)
    /// @brief Sets the RGB color uniform using a color identifier.
    /// @param newColor Color identifier to resolve to RGB values.
    /// @return Reference to this command for chaining.
    Command& setColor(td::ColorID newColor);

    //RGBA used in color uniform (color4 or vec4)
    /// @brief Sets the RGBA color uniform using a td::Color value.
    /// @param colorRGBA Full RGBA color value.
    /// @return Reference to this command for chaining.
    Command& setColor(td::Color colorRGBA);

    /// @brief Sets the RGBA color uniform using a color identifier and an explicit alpha.
    /// @param newColor Color identifier to resolve to RGB values.
    /// @param alpha Alpha transparency value (0-255).
    /// @return Reference to this command for chaining.
    Command& setColor(td::ColorID newColor, td::BYTE alpha);

    /// @brief Sets the RGBA color uniform using a color identifier and a floating-point alpha.
    /// @param newColor Color identifier to resolve to RGB values.
    /// @param alpha Alpha transparency in the range [0.0, 1.0].
    /// @return Reference to this command for chaining.
    Command& setColor(td::ColorID newColor, float alpha);

    /// @brief Finalizes encoding of the command, resolving function pointers for execution.
    /// @return true if the command was encoded successfully.
    bool encode();
};


} //namespace gl
} //namespace gui
