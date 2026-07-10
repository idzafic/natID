// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Types.h
    @brief Core type aliases, enumerations, and utility functions used throughout the gui::gl subsystem. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <td/Point.h>
#include <td/RectNormalized.h>

namespace gui
{
namespace gl
{

using VertexType = float;   ///< Scalar type used for vertex coordinate components.
using IndexType = td::UINT4; ///< Scalar type used for index buffer elements.
typedef td::Size<VertexType> Size;       ///< 2D size type using the vertex scalar type.
typedef td::Point<VertexType> Point2d;  ///< 2D point type using the vertex scalar type.
typedef td::RectNormalized<VertexType> Rect; ///< Normalized rectangle type using the vertex scalar type.
typedef td::Circle<VertexType> Circle;  ///< Circle type using the vertex scalar type.

/// @brief Describes the per-vertex data format for OpenGL vertex attributes.
enum class DataType : td::BYTE {
    flt=0,   ///< Single float.
    uint,    ///< Single unsigned integer.
    uvec1,   ///< 1-component unsigned integer vector.
    uvec2,   ///< 2-component unsigned integer vector.
    uvec3,   ///< 3-component unsigned integer vector.
    uvec4,   ///< 4-component unsigned integer vector.
    vec1,    ///< 1-component float vector.
    vec2,    ///< 2-component float vector.
    vec3,    ///< 3-component float vector.
    vec4,    ///< 4-component float vector.
    norm1,   ///< 1-component normalized float.
    norm2,   ///< 2-component normalized float.
    norm3,   ///< 3-component normalized float.
    norm4,   ///< 4-component normalized float.
    mat3x3,  ///< 3x3 float matrix.
    mat4x4,  ///< 4x4 float matrix.
    none     ///< Unspecified or placeholder type.
};
constexpr td::BYTE FloatUnits[] = {1, 1, 1, 2, 3, 4, 1, 2, 3, 4, 9, 16, 0}; ///< Number of float units per DataType entry.

/// @brief Specifies the OpenGL drawing primitive used in a draw call.
enum class Primitive : td::BYTE {
    Points=0,       ///< GL_POINTS — individual points.
    Lines,          ///< GL_LINES — line segments.
    LineStrip,      ///< GL_LINE_STRIP — connected line strip.
    LineLoop,       ///< GL_LINE_LOOP — closed line loop.
    Triangles,      ///< GL_TRIANGLES — independent triangles.
    TriangleFan,    ///< GL_TRIANGLE_FAN — triangle fan.
    TriangleStrip,  ///< GL_TRIANGLE_STRIP — triangle strip.
    Patches,        ///< GL_PATCHES — tessellation patches.
    Error           ///< Sentinel value indicating an invalid or unknown primitive.
};

/// @brief Returns true if the given DataType represents a normalized attribute format.
/// @param dt The data type to test.
/// @return True if dt is one of norm1–norm4, false otherwise.
inline bool isNormalized(gui::gl::DataType dt)
{
    if (dt >= gui::gl::DataType::norm1 && dt <= gui::gl::DataType::norm4)
        return true;
    return false;
}

/// @brief Types and functions related to the OpenGL depth buffer configuration.
namespace DepthBuffer
{
/// @brief Bit-depth options for the depth buffer.
enum class Size : td::BYTE {
    B0=0, ///< No depth buffer (alias 0).
    b0,   ///< No depth buffer (alias 1).
    B1,   ///< Depth buffer size variant 1.
    b8,   ///< 8-bit depth buffer.
    B2,   ///< Depth buffer size variant 2.
    b16,  ///< 16-bit depth buffer.
    B3,   ///< Depth buffer size variant 3.
    b24,  ///< 24-bit depth buffer.
    B4,   ///< Depth buffer size variant 4.
    b32   ///< 32-bit depth buffer.
};
/// @brief Depth comparison functions for the depth test.
enum class Function : td::BYTE {
    Always=0,        ///< Depth test always passes.
    Never,           ///< Depth test never passes.
    Less,            ///< Pass if incoming depth < stored depth.
    Equal,           ///< Pass if incoming depth == stored depth.
    LessOrEqual,     ///< Pass if incoming depth <= stored depth.
    Greater,         ///< Pass if incoming depth > stored depth.
    NotEqual,        ///< Pass if incoming depth != stored depth.
    GreaterOrEqual   ///< Pass if incoming depth >= stored depth.
};
}

/// @brief Types and functions related to the OpenGL stencil buffer configuration.
namespace StencilBuffer
{
/// @brief Bit-depth options for the stencil buffer.
enum class Size : td::BYTE {
    B0=0, ///< No stencil buffer (alias 0).
    b0,   ///< No stencil buffer (alias 1).
    B1,   ///< Stencil buffer size variant 1.
    b8,   ///< 8-bit stencil buffer.
    B2,   ///< Stencil buffer size variant 2.
    b16,  ///< 16-bit stencil buffer.
    B3,   ///< Stencil buffer size variant 3.
    b24,  ///< 24-bit stencil buffer.
    B4,   ///< Stencil buffer size variant 4.
    b32   ///< 32-bit stencil buffer.
};
/// @brief Operations that can be applied to the stencil buffer value.
enum class Operation : td::BYTE {
    Keep=0,            ///< Keep the current stencil value.
    Zero,              ///< Set the stencil value to zero.
    Replace,           ///< Replace the stencil value with the reference value.
    Increase,          ///< Increment the stencil value by one (clamp at max).
    IncreaseAndWrap,   ///< Increment the stencil value by one (wrap to zero at max).
    Decrease,          ///< Decrement the stencil value by one (clamp at zero).
    DecreaseAndWrap,   ///< Decrement the stencil value by one (wrap at zero).
    Invert             ///< Bitwise-invert the stencil value.
};
/// @brief Stencil comparison functions for the stencil test.
enum class Function : td::BYTE {
    Never = 0,    ///< Stencil test never passes.
    Less,         ///< Pass if (ref & mask) < (stencil & mask).
    LessEqual,    ///< Pass if (ref & mask) <= (stencil & mask).
    Greater,      ///< Pass if (ref & mask) > (stencil & mask).
    GreaterEqual, ///< Pass if (ref & mask) >= (stencil & mask).
    Equal,        ///< Pass if (ref & mask) == (stencil & mask).
    NotEqual,     ///< Pass if (ref & mask) != (stencil & mask).
    Always        ///< Stencil test always passes.
};
}

/// @brief Types related to OpenGL blending configuration.
namespace Blend
{
/// @brief Blending factor operands for the source and destination blend equation.
enum class Factor : td::BYTE {
    Zero=0,               ///< Factor is (0, 0, 0, 0).
    One,                  ///< Factor is (1, 1, 1, 1).
    SrcColor,             ///< Factor is the source color.
    OneMinusSrcColor,     ///< Factor is (1 - source color).
    DstColor,             ///< Factor is the destination color.
    OneMinusDstColor,     ///< Factor is (1 - destination color).
    SrcAlpha,             ///< Factor is the source alpha.
    OneMinusSrcAlpha,     ///< Factor is (1 - source alpha).
    DstAlpha,             ///< Factor is the destination alpha.
    OneMinusDstAlpha,     ///< Factor is (1 - destination alpha).
    ConstColor,           ///< Factor is the constant blend color.
    OneMinusConstColor,   ///< Factor is (1 - constant blend color).
    ConstAlpha,           ///< Factor is the constant blend alpha.
    OneMinusConstAlpha,   ///< Factor is (1 - constant blend alpha).
    SrcAlphaSaturate,     ///< Factor is min(src alpha, 1 - dst alpha).
    Src1Color,            ///< Factor is the second source color (dual-source blending).
    OneMinusSrc1Color,    ///< Factor is (1 - second source color).
    Src1Alpha,            ///< Factor is the second source alpha (dual-source blending).
    OneMinusSrc1Alpha     ///< Factor is (1 - second source alpha).
};
}

/// @brief Types related to OpenGL texture configuration.
namespace Texture
{
/// @brief Specifies the dimensionality and layout of an OpenGL texture object.
enum class Type : td::BYTE {
    None=0,          ///< No texture / uninitialized.
    Regular1D,       ///< Standard 1D texture (GL_TEXTURE_1D).
    Regular2D,       ///< Standard 2D texture (GL_TEXTURE_2D).
    Atlas,           ///< 2D texture atlas (multiple images packed into one texture).
    CubeMapAllInOne, ///< Cube map stored as a single composite image.
    CubeMap          ///< Standard cube map texture (GL_TEXTURE_CUBE_MAP).
};
}

} //namespace gl
} //namespace gui

/// @brief Types and utility functions for GLSL shader objects.
namespace GLSL
{

/// @brief Identifies the type of a GLSL shader stage or the linked program.
enum class Type : unsigned char {
    VertexShader = 0,          ///< Vertex shader stage.
    FragmentShader,            ///< Fragment shader stage.
    TessellationCtrlShader,    ///< Tessellation control shader stage.
    TessellationEvalShader,    ///< Tessellation evaluation shader stage.
    GeometryShader,            ///< Geometry shader stage.
    Program                    ///< Linked shader program.
};

/// @brief Converts a GLSL::Type value to a human-readable string.
/// @param type The GLSL shader stage or program type.
/// @return A null-terminated string representation of the type.
inline const char* toString(GLSL::Type type)
{
    switch (type)
    {
    case GLSL::Type::VertexShader: return "VertexShader";
    case GLSL::Type::FragmentShader: return "FragmentShader";
    case GLSL::Type::TessellationCtrlShader: return "TessellationCtrlShader";
    case GLSL::Type::TessellationEvalShader: return "TessellationEvalShader";
    case GLSL::Type::GeometryShader: return "GeometryShader";
    case GLSL::Type::Program: return "Program";
    default:
        assert(false);
        return "WrongGLSLType";
    }
}



} //namespace GLSL


