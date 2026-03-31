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
#include <td/Types.h>
#include <td/Point.h>
#include <td/RectNormalized.h>

namespace gui
{
namespace gl
{

using VertexType = float;
using IndexType = td::UINT4;
typedef td::Size<VertexType> Size;
typedef td::Point<VertexType> Point2d;
typedef td::RectNormalized<VertexType> Rect;
typedef td::Circle<VertexType> Circle;

enum class DataType : td::BYTE {flt=0, uint, uvec1, uvec2, uvec3, uvec4, vec1, vec2, vec3, vec4, norm1, norm2, norm3, norm4, mat3x3, mat4x4, none};
constexpr td::BYTE FloatUnits[] = {1, 1, 1, 2, 3, 4, 1, 2, 3, 4, 9, 16, 0};
enum class Primitive : td::BYTE {Points=0, Lines, LineStrip, LineLoop, Triangles, TriangleFan, TriangleStrip, Patches, Error};

inline bool isNormalized(gui::gl::DataType dt)
{
    if (dt >= gui::gl::DataType::norm1 && dt <= gui::gl::DataType::norm4)
        return true;
    return false;
}

namespace DepthBuffer
{
enum class Size : td::BYTE {B0=0, b0, B1, b8, B2, b16, B3, b24, B4, b32};
enum class Function : td::BYTE {Always=0, Never, Less, Equal, LessOrEqual, Greater, NotEqual, GreaterOrEqual};
}

namespace StencilBuffer
{
enum class Size : td::BYTE {B0=0, b0, B1, b8, B2, b16, B3, b24, B4, b32};
enum class Operation : td::BYTE {Keep=0, Zero, Replace, Increase, IncreaseAndWrap, Decrease, DecreaseAndWrap, Invert};
enum class Function : td::BYTE { Never = 0, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual, Always };
}

namespace Blend
{
enum class Factor : td::BYTE {Zero=0, One, SrcColor, OneMinusSrcColor, DstColor, OneMinusDstColor, SrcAlpha, OneMinusSrcAlpha, DstAlpha, OneMinusDstAlpha, ConstColor, OneMinusConstColor, ConstAlpha, OneMinusConstAlpha, SrcAlphaSaturate, Src1Color, OneMinusSrc1Color, Src1Alpha, OneMinusSrc1Alpha};
}

namespace Texture
{
enum class Type : td::BYTE {None=0, Regular1D, Regular2D, Atlas, CubeMapAllInOne, CubeMap};
}

} //namespace gl
} //namespace gui

namespace GLSL
{

enum class Type : unsigned char { VertexShader = 0, FragmentShader, TessellationCtrlShader, TessellationEvalShader, GeometryShader, Program };
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



