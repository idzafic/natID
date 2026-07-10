// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Types.h
    @brief Defines common GPU types, enumerations, and structures used throughout the GLX rendering API. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <td/Types.h>
#include <td/Point.h>
#include <td/RectNormalized.h>
#include <cstdint>

#ifdef MU_MACOS
// Forward declarations for Metal API
namespace MTL
{
    struct TextureSwizzleChannels;
}
#elif defined(MU_LINUX)
#ifdef None
#undef None
#endif

#ifdef Bool
#undef Bool
#endif

#ifdef Always
#undef Always
#endif

#endif


namespace glx
{

using VertexType = float; ///< Base scalar type used for vertex coordinates.
//using IndexType = td::UINT4;
typedef td::Size<VertexType> Size; ///< 2D size type using the vertex scalar type.
typedef td::Point<VertexType> Point2d; ///< 2D point type using the vertex scalar type.
typedef td::RectNormalized<VertexType> Rect; ///< Normalized 2D rectangle type using the vertex scalar type.
typedef td::Circle<VertexType> Circle; ///< Circle type using the vertex scalar type.

/// @brief Enumerates pixel formats supported by the GPU for textures, render targets, and buffers.
enum class PixelFormat : td::WORD
{
    Invalid = 0, ///< Invalid or unspecified pixel format.
    A8Unorm = 1, ///< 8-bit unsigned normalized alpha channel only.

    R8Unorm = 10,       ///< 8-bit unsigned normalized single red channel.
    R8Unorm_sRGB = 11,  ///< 8-bit unsigned normalized red in sRGB color space.
    R8Snorm = 12,       ///< 8-bit signed normalized single red channel.
    R8Uint = 13,        ///< 8-bit unsigned integer single red channel.
    R8Sint = 14,        ///< 8-bit signed integer single red channel.

    R16Unorm = 20,  ///< 16-bit unsigned normalized single red channel.
    R16Snorm = 22,  ///< 16-bit signed normalized single red channel.
    R16Uint = 23,   ///< 16-bit unsigned integer single red channel.
    R16Sint = 24,   ///< 16-bit signed integer single red channel.
    R16Float = 25,  ///< 16-bit floating point single red channel.

    RG8Unorm = 30,      ///< 8-bit unsigned normalized red-green channels.
    RG8Unorm_sRGB = 31, ///< 8-bit unsigned normalized red-green in sRGB color space.
    RG8Snorm = 32,      ///< 8-bit signed normalized red-green channels.
    RG8Uint = 33,       ///< 8-bit unsigned integer red-green channels.
    RG8Sint = 34,       ///< 8-bit signed integer red-green channels.

    B5G6R5Unorm = 40,   ///< Packed 5-6-5 unsigned normalized BGR format.
    A1BGR5Unorm = 41,   ///< Packed 1-5-5-5 unsigned normalized ABGR format.
    ABGR4Unorm = 42,    ///< Packed 4-4-4-4 unsigned normalized ABGR format.
    BGR5A1Unorm = 43,   ///< Packed 5-5-5-1 unsigned normalized BGR-Alpha format.

    R32Uint = 53,   ///< 32-bit unsigned integer single red channel.
    R32Sint = 54,   ///< 32-bit signed integer single red channel.
    R32Float = 55,  ///< 32-bit floating point single red channel.

    RG16Unorm = 60,     ///< 16-bit unsigned normalized red-green channels.
    RG16Snorm = 62,     ///< 16-bit signed normalized red-green channels.
    RG16Uint = 63,      ///< 16-bit unsigned integer red-green channels.
    RG16Sint = 64,      ///< 16-bit signed integer red-green channels.
    RG16Float = 65,     ///< 16-bit floating point red-green channels.

    RGBA8Unorm = 70,        ///< 8-bit unsigned normalized RGBA channels.
    RGBA8Unorm_sRGB = 71,   ///< 8-bit unsigned normalized RGBA in sRGB color space.
    RGBA8Snorm = 72,        ///< 8-bit signed normalized RGBA channels.
    RGBA8Uint = 73,         ///< 8-bit unsigned integer RGBA channels.
    RGBA8Sint = 74,         ///< 8-bit signed integer RGBA channels.

    BGRA8Unorm = 80,        ///< 8-bit unsigned normalized BGRA channels.
    BGRA8Unorm_sRGB = 81,   ///< 8-bit unsigned normalized BGRA in sRGB color space.

    RGB10A2Unorm = 90,  ///< 10-10-10-2 unsigned normalized RGB-Alpha.
    RGB10A2Uint = 91,   ///< 10-10-10-2 unsigned integer RGB-Alpha.
    RG11B10Float = 92,  ///< 11-11-10 floating point RG and B channels.
    RGB9E5Float = 93,   ///< Shared-exponent 9-9-9-5 floating point RGB.
    BGR10A2Unorm = 94,  ///< 10-10-10-2 unsigned normalized BGR-Alpha.

    RG32Uint = 103,     ///< 32-bit unsigned integer red-green channels.
    RG32Sint = 104,     ///< 32-bit signed integer red-green channels.
    RG32Float = 105,    ///< 32-bit floating point red-green channels.

    RGBA16Unorm = 110,  ///< 16-bit unsigned normalized RGBA channels.
    RGBA16Snorm = 112,  ///< 16-bit signed normalized RGBA channels.
    RGBA16Uint = 113,   ///< 16-bit unsigned integer RGBA channels.
    RGBA16Sint = 114,   ///< 16-bit signed integer RGBA channels.
    RGBA16Float = 115,  ///< 16-bit floating point RGBA channels.

    RGBA32Uint = 123,   ///< 32-bit unsigned integer RGBA channels.
    RGBA32Sint = 124,   ///< 32-bit signed integer RGBA channels.
    RGBA32Float = 125,  ///< 32-bit floating point RGBA channels.

    BC1_RGBA = 130,         ///< BC1 (DXT1) compressed RGBA.
    BC1_RGBA_sRGB = 131,    ///< BC1 (DXT1) compressed RGBA in sRGB color space.
    BC2_RGBA = 132,         ///< BC2 (DXT3) compressed RGBA.
    BC2_RGBA_sRGB = 133,    ///< BC2 (DXT3) compressed RGBA in sRGB color space.
    BC3_RGBA = 134,         ///< BC3 (DXT5) compressed RGBA.
    BC3_RGBA_sRGB = 135,    ///< BC3 (DXT5) compressed RGBA in sRGB color space.
    BC4_RUnorm = 140,       ///< BC4 compressed single channel unsigned normalized.
    BC4_RSnorm = 141,       ///< BC4 compressed single channel signed normalized.
    BC5_RGUnorm = 142,      ///< BC5 compressed two-channel unsigned normalized.
    BC5_RGSnorm = 143,      ///< BC5 compressed two-channel signed normalized.
    BC6H_RGBFloat = 150,    ///< BC6H compressed HDR RGB signed float.
    BC6H_RGBUfloat = 151,   ///< BC6H compressed HDR RGB unsigned float.
    BC7_RGBAUnorm = 152,    ///< BC7 compressed RGBA unsigned normalized.
    BC7_RGBAUnorm_sRGB = 153, ///< BC7 compressed RGBA unsigned normalized in sRGB color space.

    PVRTC_RGB_2BPP = 160,       ///< PVRTC compressed RGB at 2 bits per pixel.
    PVRTC_RGB_2BPP_sRGB = 161,  ///< PVRTC compressed RGB at 2 bpp in sRGB color space.
    PVRTC_RGB_4BPP = 162,       ///< PVRTC compressed RGB at 4 bits per pixel.
    PVRTC_RGB_4BPP_sRGB = 163,  ///< PVRTC compressed RGB at 4 bpp in sRGB color space.
    PVRTC_RGBA_2BPP = 164,      ///< PVRTC compressed RGBA at 2 bits per pixel.
    PVRTC_RGBA_2BPP_sRGB = 165, ///< PVRTC compressed RGBA at 2 bpp in sRGB color space.
    PVRTC_RGBA_4BPP = 166,      ///< PVRTC compressed RGBA at 4 bits per pixel.
    PVRTC_RGBA_4BPP_sRGB = 167, ///< PVRTC compressed RGBA at 4 bpp in sRGB color space.

    EAC_R11Unorm = 170,     ///< EAC compressed 11-bit unsigned normalized single channel.
    EAC_R11Snorm = 172,     ///< EAC compressed 11-bit signed normalized single channel.
    EAC_RG11Unorm = 174,    ///< EAC compressed 11-bit unsigned normalized two channels.
    EAC_RG11Snorm = 176,    ///< EAC compressed 11-bit signed normalized two channels.
    EAC_RGBA8 = 178,        ///< EAC compressed RGBA 8-bit.
    EAC_RGBA8_sRGB = 179,   ///< EAC compressed RGBA 8-bit in sRGB color space.

    ETC2_RGB8 = 180,        ///< ETC2 compressed RGB 8-bit.
    ETC2_RGB8_sRGB = 181,   ///< ETC2 compressed RGB 8-bit in sRGB color space.
    ETC2_RGB8A1 = 182,      ///< ETC2 compressed RGB 8-bit with 1-bit alpha.
    ETC2_RGB8A1_sRGB = 183, ///< ETC2 compressed RGB 8-bit with 1-bit alpha in sRGB color space.

    ASTC_4x4_sRGB = 186,    ///< ASTC 4x4 compressed in sRGB color space.
    ASTC_5x4_sRGB = 187,    ///< ASTC 5x4 compressed in sRGB color space.
    ASTC_5x5_sRGB = 188,    ///< ASTC 5x5 compressed in sRGB color space.
    ASTC_6x5_sRGB = 189,    ///< ASTC 6x5 compressed in sRGB color space.
    ASTC_6x6_sRGB = 190,    ///< ASTC 6x6 compressed in sRGB color space.
    ASTC_8x5_sRGB = 192,    ///< ASTC 8x5 compressed in sRGB color space.
    ASTC_8x6_sRGB = 193,    ///< ASTC 8x6 compressed in sRGB color space.
    ASTC_8x8_sRGB = 194,    ///< ASTC 8x8 compressed in sRGB color space.
    ASTC_10x5_sRGB = 195,   ///< ASTC 10x5 compressed in sRGB color space.
    ASTC_10x6_sRGB = 196,   ///< ASTC 10x6 compressed in sRGB color space.
    ASTC_10x8_sRGB = 197,   ///< ASTC 10x8 compressed in sRGB color space.
    ASTC_10x10_sRGB = 198,  ///< ASTC 10x10 compressed in sRGB color space.
    ASTC_12x10_sRGB = 199,  ///< ASTC 12x10 compressed in sRGB color space.
    ASTC_12x12_sRGB = 200,  ///< ASTC 12x12 compressed in sRGB color space.

    ASTC_4x4_LDR = 204,     ///< ASTC 4x4 compressed LDR.
    ASTC_5x4_LDR = 205,     ///< ASTC 5x4 compressed LDR.
    ASTC_5x5_LDR = 206,     ///< ASTC 5x5 compressed LDR.
    ASTC_6x5_LDR = 207,     ///< ASTC 6x5 compressed LDR.
    ASTC_6x6_LDR = 208,     ///< ASTC 6x6 compressed LDR.
    ASTC_8x5_LDR = 210,     ///< ASTC 8x5 compressed LDR.
    ASTC_8x6_LDR = 211,     ///< ASTC 8x6 compressed LDR.
    ASTC_8x8_LDR = 212,     ///< ASTC 8x8 compressed LDR.
    ASTC_10x5_LDR = 213,    ///< ASTC 10x5 compressed LDR.
    ASTC_10x6_LDR = 214,    ///< ASTC 10x6 compressed LDR.
    ASTC_10x8_LDR = 215,    ///< ASTC 10x8 compressed LDR.
    ASTC_10x10_LDR = 216,   ///< ASTC 10x10 compressed LDR.
    ASTC_12x10_LDR = 217,   ///< ASTC 12x10 compressed LDR.
    ASTC_12x12_LDR = 218,   ///< ASTC 12x12 compressed LDR.

    ASTC_4x4_HDR = 222,     ///< ASTC 4x4 compressed HDR.
    ASTC_5x4_HDR = 223,     ///< ASTC 5x4 compressed HDR.
    ASTC_5x5_HDR = 224,     ///< ASTC 5x5 compressed HDR.
    ASTC_6x5_HDR = 225,     ///< ASTC 6x5 compressed HDR.
    ASTC_6x6_HDR = 226,     ///< ASTC 6x6 compressed HDR.
    ASTC_8x5_HDR = 228,     ///< ASTC 8x5 compressed HDR.
    ASTC_8x6_HDR = 229,     ///< ASTC 8x6 compressed HDR.
    ASTC_8x8_HDR = 230,     ///< ASTC 8x8 compressed HDR.
    ASTC_10x5_HDR = 231,    ///< ASTC 10x5 compressed HDR.
    ASTC_10x6_HDR = 232,    ///< ASTC 10x6 compressed HDR.
    ASTC_10x8_HDR = 233,    ///< ASTC 10x8 compressed HDR.
    ASTC_10x10_HDR = 234,   ///< ASTC 10x10 compressed HDR.
    ASTC_12x10_HDR = 235,   ///< ASTC 12x10 compressed HDR.
    ASTC_12x12_HDR = 236,   ///< ASTC 12x12 compressed HDR.

    GBGR422 = 240,  ///< GBGR 4:2:2 subsampled format.
    BGRG422 = 241,  ///< BGRG 4:2:2 subsampled format.

    Depth16Unorm = 250,             ///< 16-bit unsigned normalized depth.
    Depth32Float = 252,             ///< 32-bit floating point depth.
    Stencil8 = 253,                 ///< 8-bit stencil only.
    Depth24Unorm_Stencil8 = 255,    ///< 24-bit depth and 8-bit stencil combined.
    Depth32Float_Stencil8 = 260,    ///< 32-bit float depth with 8-bit stencil.
    X32_Stencil8 = 261,             ///< 32-bit unused + 8-bit stencil.
    X24_Stencil8 = 262,             ///< 24-bit unused + 8-bit stencil.

    BGRA10_XR = 552,        ///< 10-bit BGRA extended range.
    BGRA10_XR_sRGB = 553,   ///< 10-bit BGRA extended range in sRGB color space.
    BGR10_XR = 554,         ///< 10-bit BGR extended range.
    BGR10_XR_sRGB = 555,    ///< 10-bit BGR extended range in sRGB color space.
};


//enum class DataType : td::BYTE {flt=0, uint, uvec1, uvec2, uvec3, uvec4, vec1, vec2, vec3, vec4, norm1, norm2, norm3, norm4, mat3x3, mat4x4, none};
/// @brief Enumerates shader data types used in vertex descriptors and function arguments.
enum class DataType : td::BYTE {
    None = 0,               ///< No data type.
    Struct = 1,             ///< Struct compound type.
    Array = 2,              ///< Array compound type.
    Float = 3,              ///< Single-precision float (alias: flt).
    flt = 3,                ///< Alias for Float.
    Float2 = 4,             ///< Two-component float vector (alias: vec2).
    vec2 = 4,               ///< Alias for Float2.
    Float3 = 5,             ///< Three-component float vector (alias: vec3).
    vec3 = 5,               ///< Alias for Float3.
    Float4 = 6,             ///< Four-component float vector (alias: vec4).
    vec4 = 6,               ///< Alias for Float4.
    Float2x2 = 7,           ///< 2x2 float matrix.
    Float2x3 = 8,           ///< 2x3 float matrix.
    Float2x4 = 9,           ///< 2x4 float matrix.
    Float3x2 = 10,          ///< 3x2 float matrix.
    Float3x3 = 11,          ///< 3x3 float matrix.
    Float3x4 = 12,          ///< 3x4 float matrix.
    Float4x2 = 13,          ///< 4x2 float matrix.
    Float4x3 = 14,          ///< 4x3 float matrix.
    Float4x4 = 15,          ///< 4x4 float matrix.
    Half = 16,              ///< Half-precision float scalar.
    Half2 = 17,             ///< Two-component half-precision vector.
    Half3 = 18,             ///< Three-component half-precision vector.
    Half4 = 19,             ///< Four-component half-precision vector.
    Half2x2 = 20,           ///< 2x2 half-precision matrix.
    Half2x3 = 21,           ///< 2x3 half-precision matrix.
    Half2x4 = 22,           ///< 2x4 half-precision matrix.
    Half3x2 = 23,           ///< 3x2 half-precision matrix.
    Half3x3 = 24,           ///< 3x3 half-precision matrix.
    Half3x4 = 25,           ///< 3x4 half-precision matrix.
    Half4x2 = 26,           ///< 4x2 half-precision matrix.
    Half4x3 = 27,           ///< 4x3 half-precision matrix.
    Half4x4 = 28,           ///< 4x4 half-precision matrix.
    Int = 29,               ///< 32-bit signed integer scalar.
    Int2 = 30,              ///< Two-component signed integer vector.
    Int3 = 31,              ///< Three-component signed integer vector.
    Int4 = 32,              ///< Four-component signed integer vector.
    UInt = 33,              ///< 32-bit unsigned integer scalar (alias: uint).
    uint = 33,              ///< Alias for UInt.
    UInt2 = 34,             ///< Two-component unsigned integer vector (alias: uvec2).
    uvec2 = 34,             ///< Alias for UInt2.
    UInt3 = 35,             ///< Three-component unsigned integer vector (alias: uvec3).
    uvec3 = 35,             ///< Alias for UInt3.
    UInt4 = 36,             ///< Four-component unsigned integer vector (alias: uvec4).
    uvec4 = 36,             ///< Alias for UInt4.
    Short = 37,             ///< 16-bit signed integer scalar.
    Short2 = 38,            ///< Two-component 16-bit signed integer vector.
    Short3 = 39,            ///< Three-component 16-bit signed integer vector.
    Short4 = 40,            ///< Four-component 16-bit signed integer vector.
    UShort = 41,            ///< 16-bit unsigned integer scalar.
    UShort2 = 42,           ///< Two-component 16-bit unsigned integer vector.
    UShort3 = 43,           ///< Three-component 16-bit unsigned integer vector.
    UShort4 = 44,           ///< Four-component 16-bit unsigned integer vector.
    Char = 45,              ///< 8-bit signed integer scalar.
    Char2 = 46,             ///< Two-component 8-bit signed integer vector.
    Char3 = 47,             ///< Three-component 8-bit signed integer vector.
    Char4 = 48,             ///< Four-component 8-bit signed integer vector.
    UChar = 49,             ///< 8-bit unsigned integer scalar.
    UChar2 = 50,            ///< Two-component 8-bit unsigned integer vector.
    UChar3 = 51,            ///< Three-component 8-bit unsigned integer vector.
    UChar4 = 52,            ///< Four-component 8-bit unsigned integer vector.
    Bool = 53,              ///< Boolean scalar.
    Bool2 = 54,             ///< Two-component boolean vector.
    Bool3 = 55,             ///< Three-component boolean vector.
    Bool4 = 56,             ///< Four-component boolean vector.
    Texture = 58,           ///< Texture resource type.
    Sampler = 59,           ///< Sampler resource type.
    Pointer = 60,           ///< Pointer type.
    R8Unorm = 62,           ///< 8-bit unsigned normalized single channel as data type.
    R8Snorm = 63,           ///< 8-bit signed normalized single channel as data type.
    R16Unorm = 64,          ///< 16-bit unsigned normalized single channel as data type.
    R16Snorm = 65,          ///< 16-bit signed normalized single channel as data type.
    RG8Unorm = 66,          ///< 8-bit unsigned normalized two channels as data type.
    RG8Snorm = 67,          ///< 8-bit signed normalized two channels as data type.
    RG16Unorm = 68,         ///< 16-bit unsigned normalized two channels as data type.
    RG16Snorm = 69,         ///< 16-bit signed normalized two channels as data type.
    RGBA8Unorm = 70,        ///< 8-bit unsigned normalized RGBA as data type.
    RGBA8Unorm_sRGB = 71,   ///< 8-bit unsigned normalized RGBA (sRGB) as data type.
    RGBA8Snorm = 72,        ///< 8-bit signed normalized RGBA as data type.
    RGBA16Unorm = 73,       ///< 16-bit unsigned normalized RGBA as data type.
    RGBA16Snorm = 74,       ///< 16-bit signed normalized RGBA as data type.
    RGB10A2Unorm = 75,      ///< Packed 10-10-10-2 unsigned normalized as data type.
    RG11B10Float = 76,      ///< 11-11-10 float as data type.
    RGB9E5Float = 77,       ///< Shared-exponent 9-9-9-5 float as data type.
    RenderPipeline = 78,    ///< Render pipeline object type.
    ComputePipeline = 79,   ///< Compute pipeline object type.
    IndirectCommandBuffer = 80, ///< Indirect command buffer type.
    Long = 81,              ///< 64-bit signed integer scalar.
    Long2 = 82,             ///< Two-component 64-bit signed integer vector.
    Long3 = 83,             ///< Three-component 64-bit signed integer vector.
    Long4 = 84,             ///< Four-component 64-bit signed integer vector.
    ULong = 85,             ///< 64-bit unsigned integer scalar.
    ULong2 = 86,            ///< Two-component 64-bit unsigned integer vector.
    ULong3 = 87,            ///< Three-component 64-bit unsigned integer vector.
    ULong4 = 88,            ///< Four-component 64-bit unsigned integer vector.
    VisibleFunctionTable = 115,             ///< Visible function table resource type.
    IntersectionFunctionTable = 116,        ///< Intersection function table resource type.
    PrimitiveAccelerationStructure = 117,   ///< Primitive acceleration structure resource type.
    InstanceAccelerationStructure = 118     ///< Instance acceleration structure resource type.
};

constexpr td::BYTE FloatUnits[] = {1, 1, 1, 2, 3, 4, 1, 2, 3, 4, 9, 16, 0}; ///< Number of float units per DataType entry (indexed by DataType value offset).

/// @brief Specifies how a source or destination color/alpha factor is computed during blending.
enum class BlendFactor : td::BYTE {
    Zero = 0,                       ///< Factor is zero.
    One = 1,                        ///< Factor is one.
    SourceColor = 2,                ///< Factor is the source color.
    OneMinusSourceColor = 3,        ///< Factor is one minus the source color.
    SourceAlpha = 4,                ///< Factor is the source alpha.
    OneMinusSourceAlpha = 5,        ///< Factor is one minus the source alpha.
    DestinationColor = 6,           ///< Factor is the destination color.
    OneMinusDestinationColor = 7,   ///< Factor is one minus the destination color.
    DestinationAlpha = 8,           ///< Factor is the destination alpha.
    OneMinusDestinationAlpha = 9,   ///< Factor is one minus the destination alpha.
    SourceAlphaSaturated = 10,      ///< Factor is the source alpha, clamped to [0,1].
    BlendColor = 11,                ///< Factor is the constant blend color.
    OneMinusBlendColor = 12,        ///< Factor is one minus the constant blend color.
    BlendAlpha = 13,                ///< Factor is the constant blend alpha.
    OneMinusBlendAlpha = 14,        ///< Factor is one minus the constant blend alpha.
    Source1Color = 15,              ///< Factor is the second source color (dual-source blending).
    OneMinusSource1Color = 16,      ///< Factor is one minus the second source color.
    Source1Alpha = 17,              ///< Factor is the second source alpha.
    OneMinusSource1Alpha = 18       ///< Factor is one minus the second source alpha.
};

/// @brief Specifies the arithmetic operation used when combining source and destination values during blending.
enum class BlendOperation : td::BYTE {
    Add = 0,            ///< Source + Destination.
    Subtract = 1,       ///< Source - Destination.
    ReverseSubtract = 2, ///< Destination - Source.
    Min = 3,            ///< Minimum of Source and Destination.
    Max = 4             ///< Maximum of Source and Destination.
};

/// @brief Bitmask specifying which color channels are written during rendering.
enum class ColorWriteMask : td::BYTE {
    None = 0,   ///< No channels are written.
    Alpha = 1,  ///< Alpha channel is written.
    Blue = 2,   ///< Blue channel is written.
    Green = 4,  ///< Green channel is written.
    Red = 8,    ///< Red channel is written.
    All = 15    ///< All channels (RGBA) are written.
};

/// @brief Categorizes the class of geometric primitive for pipeline input topology.
enum class PrimitiveClass : td::BYTE {
    Unspecified = 0, ///< Primitive class is not specified.
    Point = 1,       ///< Point primitives.
    Line = 2,        ///< Line primitives.
    Triangle = 3     ///< Triangle primitives.
}; //PrimitiveTopologyClass

/// @brief Specifies the type of geometric primitive drawn by a draw call.
enum class PrimitiveType : td::BYTE {
    Point = 0,          ///< Individual points.
    Line = 1,           ///< Individual line segments.
    LineStrip = 2,      ///< Connected line strip.
    Triangle = 3,       ///< Individual triangles.
    TriangleStrip = 4   ///< Connected triangle strip.
};

//enum class Primitive : td::BYTE {Points=0, Lines, LineStrip, LineLoop, Triangles, TriangleFan, TriangleStrip, Error};

/// @brief Specifies the partitioning mode used by the tessellator.
enum class TessellationPartitionMode : td::BYTE {
    Pow2 = 0,    ///< Subdivide using powers of two.
    Integer = 1, ///< Subdivide using integer values.
    Odd = 2,     ///< Subdivide using odd fractional values.
    Even = 3     ///< Subdivide using even fractional values.
};

/// @brief Specifies how the tessellator reads per-patch or per-instance tessellation factors.
enum class TessellationFactorStepFunction : td::BYTE {
    Constant = 0,               ///< Constant tessellation factor for all patches.
    PerPatch = 1,               ///< One tessellation factor per patch.
    PerInstance = 2,            ///< One tessellation factor per instance.
    nPerPatchAndPerInstance = 3 ///< Per-patch factor multiplied by per-instance factor.
};

/// @brief Specifies the data format of tessellation factors stored in the tessellation factor buffer.
enum class TessellationFactorFormat : td::BYTE {
    Half = 0 ///< Tessellation factors are stored as 16-bit half-precision floats.
};

/// @brief Specifies the index type for tessellation control point indexing.
enum class TessellationControlPointIndexType : td::BYTE {
    None = 0,   ///< No index type (unindexed).
    UInt16 = 1, ///< 16-bit unsigned integer indices.
    UInt32 = 2  ///< 32-bit unsigned integer indices.
};

/// @brief Specifies which triangle faces are culled during rasterization.
enum class CullMode : td::BYTE {
    None = 0,   ///< No culling; both front and back faces are rendered.
    Front = 1,  ///< Front-facing triangles are culled.
    Back = 2    ///< Back-facing triangles are culled.
};

/// @brief Specifies the winding order used to determine front-facing triangles.
enum class Winding : td::BYTE {
    Clockwise = 0,          ///< Clockwise winding indicates a front face.
    CounterClockwise = 1    ///< Counter-clockwise winding indicates a front face.
};

/// @brief Specifies how depth values outside the clipping planes are handled.
enum class DepthClipMode : td::BYTE {
    Clip = 0,   ///< Fragments beyond the clipping planes are clipped.
    Clamp = 1   ///< Depth values are clamped to the near/far planes instead of clipping.
};

/// @brief Specifies how triangles are filled during rasterization.
enum class TriangleFillMode : td::BYTE {
    Fill = 0,  ///< Triangles are filled (solid rendering).
    Lines = 1  ///< Only triangle edges are drawn (wireframe rendering).
};

/// @brief Bitmask specifying the render pipeline stages.
enum class RenderStage : td::BYTE {
    Vertex = 1,   ///< Vertex shader stage.
    Fragment = 2, ///< Fragment shader stage.
    Tile = 4      ///< Tile shader stage.
};

/// @brief Specifies the mode of visibility result queries.
enum class VisibilityResultMode : td::BYTE {
    Disabled = 0, ///< Visibility result queries are disabled.
    Boolean = 1,  ///< Returns a boolean indicating if any samples passed.
    Counting = 2  ///< Returns the count of samples that passed.
};

/// @brief Specifies the action taken on a render target attachment when a render pass begins.
enum class LoadAction : td::BYTE {
    DontCare = 0, ///< Contents are undefined at the start of the render pass.
    Load = 1,     ///< Existing contents are preserved.
    Clear = 2     ///< Contents are cleared to a specified value.
};

/// @brief Specifies the action taken on a render target attachment when a render pass ends.
enum class StoreAction : td::BYTE {
    DontCare = 0,                   ///< Rendered contents are discarded.
    Store = 1,                      ///< Rendered contents are stored.
    MultisampleResolve = 2,         ///< Multisample contents are resolved to a single-sample texture.
    StoreAndMultisampleResolve = 3, ///< Contents are both stored and resolved.
    Unknown = 4,                    ///< Store action is unknown.
    CustomSampleDepthStore = 5      ///< Custom per-sample depth data is stored.
};

/// @brief Bitmask options for store actions.
enum class StoreActionOptions : td::BYTE {
    None = 0,                   ///< No options.
    ValidMask = 1,              ///< Valid options mask.
    CustomSamplePositions = 1   ///< Custom sample positions are used.
};

/// @brief Specifies the data type used for index buffers.
enum class IndexType : td::BYTE {
    UInt16 = 0, ///< 16-bit unsigned integer indices.
    UInt32 = 1  ///< 32-bit unsigned integer indices.
};

/// @brief Specifies the operation performed on a stencil buffer value when a stencil test passes or fails.
enum class StencilOperation : uint8_t {
    Keep = 0,           ///< Keep the current stencil value.
    Zero = 1,           ///< Set the stencil value to zero.
    Replace = 2,        ///< Replace the stencil value with the reference value.
    IncrementClamp = 3, ///< Increment the stencil value, clamped to the maximum.
    DecrementClamp = 4, ///< Decrement the stencil value, clamped to zero.
    Invert = 5,         ///< Bitwise invert the stencil value.
    IncrementWrap = 6,  ///< Increment the stencil value, wrapping to zero on overflow.
    DecrementWrap = 7   ///< Decrement the stencil value, wrapping on underflow.
};

/// @brief Defines a rectangular scissor region in pixel coordinates for clipping rendering output.
struct ScissorRect
{
    td::UINT4 x;      ///< X coordinate of the scissor rectangle's top-left corner.
    td::UINT4 y;      ///< Y coordinate of the scissor rectangle's top-left corner.
    td::UINT4 width;  ///< Width of the scissor rectangle in pixels.
    td::UINT4 height; ///< Height of the scissor rectangle in pixels.
};

using Integer = std::intptr_t;   ///< Signed pointer-size integer type.
using UInteger = std::uintptr_t; ///< Unsigned pointer-size integer type.

//inline bool isNormalized(glx::DataType dt)
//{
//    if (dt >= glx::DataType::norm1 && dt <= glx::DataType::norm4)
//        return true;
//    return false;
//}

/// @brief A generic 3D volume (width x height x depth) for use with tile and threadgroup operations.
/// @tparam T The numeric type for the dimensions.
template <typename T>
class Volume
{
public:
    T width = 0;  ///< Width dimension of the volume.
    T height = 0; ///< Height dimension of the volume.
    T depth = 0;  ///< Depth dimension of the volume.

    /// @brief Default constructor; initializes all dimensions to zero.
    Volume() = default;
    /// @brief Constructs a volume with the specified dimensions.
    /// @param w Width.
    /// @param h Height.
    /// @param d Depth.
    constexpr Volume(T w, T h, T d) : width(w), height(h), depth(d) {}
};

/// @brief Describes a 3D viewport region used during rasterization.
struct Viewport
{
    double originX; ///< X coordinate of the viewport's top-left corner.
    double originY; ///< Y coordinate of the viewport's top-left corner.
    double width;   ///< Width of the viewport.
    double height;  ///< Height of the viewport.
    double znear;   ///< Near depth clipping plane.
    double zfar;    ///< Far depth clipping plane.
};

typedef double TimeInterval; ///< Type alias for time intervals in seconds.

/// @brief Namespace containing tessellation patch type enumeration.
namespace Patch
{
/// @brief Specifies the type of tessellation patch.
enum class Type : td::BYTE {
    None = 0,     ///< No patch type.
    Triangle = 1, ///< Triangular patch.
    Quad = 2      ///< Quad patch.
};
} //namespace Patch

/// @brief Represents an RGBA clear color used when clearing a render target attachment.
struct ClearColor
{
    double red;   ///< Red component of the clear color (0.0 to 1.0).
    double green; ///< Green component of the clear color (0.0 to 1.0).
    double blue;  ///< Blue component of the clear color (0.0 to 1.0).
    double alpha; ///< Alpha component of the clear color (0.0 to 1.0).

    // Match Metal's MTLClearColorMake style
    /// @brief Creates a ClearColor with the specified RGBA components.
    /// @param r Red component.
    /// @param g Green component.
    /// @param b Blue component.
    /// @param a Alpha component.
    /// @return A ClearColor instance with the given components.
    static constexpr ClearColor Make(double r, double g, double b, double a) noexcept
    {
        return { r, g, b, a };
    }
};

/// @brief Bitmask specifying how a texture can be used within the rendering pipeline.
enum class TextureUsage : uint32_t {
    Unknown = 0,                    ///< Usage is unknown or unspecified.
    ShaderRead = 1u << 0,           ///< Texture can be sampled or read in shaders.
    ShaderWrite = 1u << 1,          ///< Texture can be written in compute or fragment shaders.
    RenderTarget = 1u << 2,         ///< Texture can be used as a color, depth, or stencil render target.
    // (bit 3 intentionally unused in Metal)
    PixelFormatView = 1u << 4,      ///< Allow creating texture views with different pixel formats.
    ShaderAtomic = 1u << 5,         ///< Allow image atomic operations (Metal 3.1+).
};

/// @brief Specifies the dimensionality and structure of a texture resource.
enum class TextureType : uint32_t {
    Type1D = 0,                 ///< One-dimensional texture.
    Type1DArray = 1,            ///< Array of one-dimensional textures.
    Type2D = 2,                 ///< Two-dimensional texture.
    Type2DArray = 3,            ///< Array of two-dimensional textures.
    Type2DMultisample = 4,      ///< Two-dimensional multisampled texture.
    TypeCube = 5,               ///< Cube texture (6 faces).
    TypeCubeArray = 6,          ///< Array of cube textures.
    Type3D = 7,                 ///< Three-dimensional (volume) texture.
    Type2DMultisampleArray = 8, ///< Array of two-dimensional multisampled textures.
    TypeTextureBuffer = 9       ///< Buffer-backed texture.
};


/// @brief Enumerates vertex attribute formats that define how vertex data is laid out in memory.
enum class VertexFormat : uint32_t
{
    Invalid = 0,   ///< MTLVertexFormatInvalid

    UChar2 = 1,    ///< MTLVertexFormatUChar2
    UChar3 = 2,    ///< MTLVertexFormatUChar3
    UChar4 = 3,    ///< MTLVertexFormatUChar4
    Char2 = 4,     ///< MTLVertexFormatChar2
    Char3 = 5,     ///< MTLVertexFormatChar3
    Char4 = 6,     ///< MTLVertexFormatChar4

    UChar2Norm = 7,  ///< MTLVertexFormatUChar2Normalized
    UChar3Norm = 8,  ///< MTLVertexFormatUChar3Normalized
    UChar4Norm = 9,  ///< MTLVertexFormatUChar4Normalized
    Char2Norm = 10,  ///< MTLVertexFormatChar2Normalized
    Char3Norm = 11,  ///< MTLVertexFormatChar3Normalized
    Char4Norm = 12,  ///< MTLVertexFormatChar4Normalized

    UShort2 = 13,  ///< MTLVertexFormatUShort2
    UShort3 = 14,  ///< MTLVertexFormatUShort3
    UShort4 = 15,  ///< MTLVertexFormatUShort4
    Short2 = 16,   ///< MTLVertexFormatShort2
    Short3 = 17,   ///< MTLVertexFormatShort3
    Short4 = 18,   ///< MTLVertexFormatShort4

    UShort2Norm = 19, ///< MTLVertexFormatUShort2Normalized
    UShort3Norm = 20, ///< MTLVertexFormatUShort3Normalized
    UShort4Norm = 21, ///< MTLVertexFormatUShort4Normalized
    Short2Norm = 22,  ///< MTLVertexFormatShort2Normalized
    Short3Norm = 23,  ///< MTLVertexFormatShort3Normalized
    Short4Norm = 24,  ///< MTLVertexFormatShort4Normalized

    Half2 = 25,    ///< MTLVertexFormatHalf2
    Half3 = 26,    ///< MTLVertexFormatHalf3
    Half4 = 27,    ///< MTLVertexFormatHalf4

    Float = 28,    ///< MTLVertexFormatFloat
    Float2 = 29,   ///< MTLVertexFormatFloat2
    Float3 = 30,   ///< MTLVertexFormatFloat3
    Float4 = 31,   ///< MTLVertexFormatFloat4

    Int = 32,      ///< MTLVertexFormatInt
    Int2 = 33,     ///< MTLVertexFormatInt2
    Int3 = 34,     ///< MTLVertexFormatInt3
    Int4 = 35,     ///< MTLVertexFormatInt4

    UInt = 36,     ///< MTLVertexFormatUInt
    UInt2 = 37,    ///< MTLVertexFormatUInt2
    UInt3 = 38,    ///< MTLVertexFormatUInt3
    UInt4 = 39,    ///< MTLVertexFormatUInt4

    Int1010102Norm = 40,  ///< MTLVertexFormatInt1010102Normalized
    UInt1010102Norm = 41, ///< MTLVertexFormatUInt1010102Normalized
    UChar4NormBgra = 42,  ///< MTLVertexFormatUChar4Normalized_bgra
};


/// @brief Specifies how vertex buffer data advances between vertex shader invocations.
enum class VertexStepFunction : uint32_t
{
    Constant = 0,           ///< MTLVertexStepFunctionConstant
    PerVertex = 1,          ///< MTLVertexStepFunctionPerVertex
    PerInstance = 2,        ///< MTLVertexStepFunctionPerInstance
    PerPatch = 3,           ///< MTLVertexStepFunctionPerPatch
    PerPatchControlPoint = 4, ///< MTLVertexStepFunctionPerPatchControlPoint
};

// Texture swizzle support (mirrors Metal's MTLSwizzle / MTLTextureSwizzleChannels)
/// @brief Specifies the swizzle component mapping for individual texture channels.
enum class TextureSwizzle : uint8_t {
    Zero = 0,  ///< Channel always reads as zero.
    One = 1,   ///< Channel always reads as one.
    Red = 2,   ///< Channel reads from the red component.
    Green = 3, ///< Channel reads from the green component.
    Blue = 4,  ///< Channel reads from the blue component.
    Alpha = 5  ///< Channel reads from the alpha component.
};

/// @brief Defines the swizzle mapping for all four texture channels (RGBA).
struct TextureSwizzleChannels
{
    TextureSwizzle red   = TextureSwizzle::Red;   ///< Swizzle source for the red output channel.
    TextureSwizzle green = TextureSwizzle::Green; ///< Swizzle source for the green output channel.
    TextureSwizzle blue  = TextureSwizzle::Blue;  ///< Swizzle source for the blue output channel.
    TextureSwizzle alpha = TextureSwizzle::Alpha; ///< Swizzle source for the alpha output channel.

    /// @brief Default constructor; initializes channels to identity swizzle (RGBA -> RGBA).
    TextureSwizzleChannels() = default;

    /// @brief Returns a TextureSwizzleChannels with the default identity mapping.
    /// @return Default swizzle channels.
    static TextureSwizzleChannels Default() noexcept
    {
        return TextureSwizzleChannels{};
    }

#ifdef MU_MACOS
    // Automatic conversion to MTL::TextureSwizzleChannels on macOS
    /// @brief Converts this swizzle channels descriptor to a Metal MTL::TextureSwizzleChannels.
    operator MTL::TextureSwizzleChannels() const;
    /// @brief Constructs TextureSwizzleChannels from a Metal MTL::TextureSwizzleChannels.
    /// @param mtlSwizzle The Metal swizzle channels to convert from.
	TextureSwizzleChannels(const MTL::TextureSwizzleChannels& mtlSwizzle);
#endif
};


/// @brief Specifies the comparison function used in depth, stencil, and sampler tests.
enum class CompareFunction : uint8_t {
    Never = 0,       ///< Test never passes.
    Less = 1,        ///< Test passes if source < reference.
    Equal = 2,       ///< Test passes if source == reference.
    LessEqual = 3,   ///< Test passes if source <= reference.
    Greater = 4,     ///< Test passes if source > reference.
    NotEqual = 5,    ///< Test passes if source != reference.
    GreaterEqual = 6, ///< Test passes if source >= reference.
    Always = 7       ///< Test always passes.
};

/// @brief Specifies the border color used when a sampler addresses outside the [0,1] texture coordinate range.
enum class SamplerBorderColor : uint8_t {
    TransparentBlack = 0, ///< Border returns transparent black (0, 0, 0, 0).
    OpaqueBlack = 1,      ///< Border returns opaque black (0, 0, 0, 1).
    OpaqueWhite = 2       ///< Border returns opaque white (1, 1, 1, 1).
};

/// @brief Specifies how texture coordinates are resolved when they fall outside the [0,1] range.
enum class SamplerAddressMode : uint8_t {
    ClampToEdge = 0,        ///< Texture coordinate is clamped to the edge texel.
    MirrorClampToEdge = 1,  ///< Mirrors once then clamps to the edge.
    Repeat = 2,             ///< Texture coordinate wraps (tiles).
    MirrorRepeat = 3,       ///< Texture coordinate mirrors and wraps.
    ClampToZero = 4,        ///< Returns zero (transparent black) outside [0,1].
    ClampToBorderColor = 5  ///< Returns the configured border color outside [0,1].
};

/// @brief Specifies the texture filtering used when a texture is minified or magnified.
enum class Filter : uint8_t {
    Nearest = 0, ///< Nearest-neighbor filtering; no interpolation.
    Linear = 1   ///< Linear interpolation between texels.
};

/// @brief Specifies the mipmap filtering mode used when sampling between mipmap levels.
enum class MipFilter : uint8_t {
    NotMipmapped = 0, ///< Mipmapping is disabled; only the base level is sampled.
    Nearest = 1,      ///< The nearest mipmap level is selected.
    Linear = 2        ///< Linearly interpolates between the two nearest mipmap levels.
};


} //namespace glx
