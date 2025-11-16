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

#ifdef MU_MACOS
// Forward declarations for Metal API
namespace MTL
{
    struct TextureSwizzleChannels;
}
#endif


namespace glx
{

using VertexType = float;
//using IndexType = td::UINT4;
typedef td::Size<VertexType> Size;
typedef td::Point<VertexType> Point2d;
typedef td::RectNormalized<VertexType> Rect;
typedef td::Circle<VertexType> Circle;
enum class PixelFormat : td::WORD
{
    Invalid = 0,
    A8Unorm = 1,

    R8Unorm = 10, R8Unorm_sRGB = 11, R8Snorm = 12,
    R8Uint = 13, R8Sint = 14,

    R16Unorm = 20, R16Snorm = 22, R16Uint = 23,
    R16Sint = 24, R16Float = 25,

    RG8Unorm = 30, RG8Unorm_sRGB = 31, RG8Snorm = 32,
    RG8Uint = 33, RG8Sint = 34,

    B5G6R5Unorm = 40, A1BGR5Unorm = 41,
    ABGR4Unorm = 42, BGR5A1Unorm = 43,

    R32Uint = 53, R32Sint = 54, R32Float = 55,

    RG16Unorm = 60, RG16Snorm = 62, RG16Uint = 63,
    RG16Sint = 64, RG16Float = 65,

    RGBA8Unorm = 70, RGBA8Unorm_sRGB = 71, RGBA8Snorm = 72,
    RGBA8Uint = 73, RGBA8Sint = 74,

    BGRA8Unorm = 80, BGRA8Unorm_sRGB = 81,

    RGB10A2Unorm = 90, RGB10A2Uint = 91,
    RG11B10Float = 92, RGB9E5Float = 93,
    BGR10A2Unorm = 94,

    RG32Uint = 103, RG32Sint = 104, RG32Float = 105,

    RGBA16Unorm = 110, RGBA16Snorm = 112,
    RGBA16Uint = 113, RGBA16Sint = 114, RGBA16Float = 115,

    RGBA32Uint = 123, RGBA32Sint = 124, RGBA32Float = 125,

    BC1_RGBA = 130, BC1_RGBA_sRGB = 131,
    BC2_RGBA = 132, BC2_RGBA_sRGB = 133,
    BC3_RGBA = 134, BC3_RGBA_sRGB = 135,
    BC4_RUnorm = 140, BC4_RSnorm = 141,
    BC5_RGUnorm = 142, BC5_RGSnorm = 143,
    BC6H_RGBFloat = 150, BC6H_RGBUfloat = 151,
    BC7_RGBAUnorm = 152, BC7_RGBAUnorm_sRGB = 153,

    PVRTC_RGB_2BPP = 160, PVRTC_RGB_2BPP_sRGB = 161,
    PVRTC_RGB_4BPP = 162, PVRTC_RGB_4BPP_sRGB = 163,
    PVRTC_RGBA_2BPP = 164, PVRTC_RGBA_2BPP_sRGB = 165,
    PVRTC_RGBA_4BPP = 166, PVRTC_RGBA_4BPP_sRGB = 167,

    EAC_R11Unorm = 170, EAC_R11Snorm = 172,
    EAC_RG11Unorm = 174, EAC_RG11Snorm = 176,
    EAC_RGBA8 = 178, EAC_RGBA8_sRGB = 179,

    ETC2_RGB8 = 180, ETC2_RGB8_sRGB = 181,
    ETC2_RGB8A1 = 182, ETC2_RGB8A1_sRGB = 183,

    ASTC_4x4_sRGB = 186, ASTC_5x4_sRGB = 187,
    ASTC_5x5_sRGB = 188, ASTC_6x5_sRGB = 189,
    ASTC_6x6_sRGB = 190, ASTC_8x5_sRGB = 192,
    ASTC_8x6_sRGB = 193, ASTC_8x8_sRGB = 194,
    ASTC_10x5_sRGB = 195, ASTC_10x6_sRGB = 196,
    ASTC_10x8_sRGB = 197, ASTC_10x10_sRGB = 198,
    ASTC_12x10_sRGB = 199, ASTC_12x12_sRGB = 200,

    ASTC_4x4_LDR = 204, ASTC_5x4_LDR = 205,
    ASTC_5x5_LDR = 206, ASTC_6x5_LDR = 207,
    ASTC_6x6_LDR = 208, ASTC_8x5_LDR = 210,
    ASTC_8x6_LDR = 211, ASTC_8x8_LDR = 212,
    ASTC_10x5_LDR = 213, ASTC_10x6_LDR = 214,
    ASTC_10x8_LDR = 215, ASTC_10x10_LDR = 216,
    ASTC_12x10_LDR = 217, ASTC_12x12_LDR = 218,

    ASTC_4x4_HDR = 222, ASTC_5x4_HDR = 223,
    ASTC_5x5_HDR = 224, ASTC_6x5_HDR = 225,
    ASTC_6x6_HDR = 226, ASTC_8x5_HDR = 228,
    ASTC_8x6_HDR = 229, ASTC_8x8_HDR = 230,
    ASTC_10x5_HDR = 231, ASTC_10x6_HDR = 232,
    ASTC_10x8_HDR = 233, ASTC_10x10_HDR = 234,
    ASTC_12x10_HDR = 235, ASTC_12x12_HDR = 236,

    GBGR422 = 240, BGRG422 = 241,

    Depth16Unorm = 250, Depth32Float = 252, Stencil8 = 253,
    Depth24Unorm_Stencil8 = 255,
    Depth32Float_Stencil8 = 260, X32_Stencil8 = 261, X24_Stencil8 = 262,

    BGRA10_XR = 552, BGRA10_XR_sRGB = 553,
    BGR10_XR = 554, BGR10_XR_sRGB = 555,
};


//enum class DataType : td::BYTE {flt=0, uint, uvec1, uvec2, uvec3, uvec4, vec1, vec2, vec3, vec4, norm1, norm2, norm3, norm4, mat3x3, mat4x4, none};
enum class DataType : td::BYTE {
    None = 0,
    Struct = 1,
    Array = 2,
    Float = 3,
    flt = 3,
    Float2 = 4,
    vec2 = 4,
    Float3 = 5,
    vec3 = 5,
    Float4 = 6,
    vec4 = 6,
    Float2x2 = 7,
    Float2x3 = 8,
    Float2x4 = 9,
    Float3x2 = 10,
    Float3x3 = 11,
    Float3x4 = 12,
    Float4x2 = 13,
    Float4x3 = 14,
    Float4x4 = 15,
    Half = 16,
    Half2 = 17,
    Half3 = 18,
    Half4 = 19,
    Half2x2 = 20,
    Half2x3 = 21,
    Half2x4 = 22,
    Half3x2 = 23,
    Half3x3 = 24,
    Half3x4 = 25,
    Half4x2 = 26,
    Half4x3 = 27,
    Half4x4 = 28,
    Int = 29,
    Int2 = 30,
    Int3 = 31,
    Int4 = 32,
    UInt = 33,
    uint = 33,
    UInt2 = 34,
    uvec2 = 34,
    UInt3 = 35,
    uvec3 = 35,
    UInt4 = 36,
    uvec4 = 36,
    Short = 37,
    Short2 = 38,
    Short3 = 39,
    Short4 = 40,
    UShort = 41,
    UShort2 = 42,
    UShort3 = 43,
    UShort4 = 44,
    Char = 45,
    Char2 = 46,
    Char3 = 47,
    Char4 = 48,
    UChar = 49,
    UChar2 = 50,
    UChar3 = 51,
    UChar4 = 52,
    Bool = 53,
    Bool2 = 54,
    Bool3 = 55,
    Bool4 = 56,
    Texture = 58,
    Sampler = 59,
    Pointer = 60,
    R8Unorm = 62,
    R8Snorm = 63,
    R16Unorm = 64,
    R16Snorm = 65,
    RG8Unorm = 66,
    RG8Snorm = 67,
    RG16Unorm = 68,
    RG16Snorm = 69,
    RGBA8Unorm = 70,
    RGBA8Unorm_sRGB = 71,
    RGBA8Snorm = 72,
    RGBA16Unorm = 73,
    RGBA16Snorm = 74,
    RGB10A2Unorm = 75,
    RG11B10Float = 76,
    RGB9E5Float = 77,
    RenderPipeline = 78,
    ComputePipeline = 79,
    IndirectCommandBuffer = 80,
    Long = 81,
    Long2 = 82,
    Long3 = 83,
    Long4 = 84,
    ULong = 85,
    ULong2 = 86,
    ULong3 = 87,
    ULong4 = 88,
    VisibleFunctionTable = 115,
    IntersectionFunctionTable = 116,
    PrimitiveAccelerationStructure = 117,
    InstanceAccelerationStructure = 118
};

constexpr td::BYTE FloatUnits[] = {1, 1, 1, 2, 3, 4, 1, 2, 3, 4, 9, 16, 0};

enum class BlendFactor : td::BYTE {
    Zero = 0,
    One = 1,
    SourceColor = 2,
    OneMinusSourceColor = 3,
    SourceAlpha = 4,
    OneMinusSourceAlpha = 5,
    DestinationColor = 6,
    OneMinusDestinationColor = 7,
    DestinationAlpha = 8,
    OneMinusDestinationAlpha = 9,
    SourceAlphaSaturated = 10,
    BlendColor = 11,
    OneMinusBlendColor = 12,
    BlendAlpha = 13,
    OneMinusBlendAlpha = 14,
    Source1Color = 15,
    OneMinusSource1Color = 16,
    Source1Alpha = 17,
    OneMinusSource1Alpha = 18
};

enum class BlendOperation : td::BYTE { Add = 0, Subtract = 1, ReverseSubtract = 2, Min = 3, Max = 4};

enum class ColorWriteMask : td::BYTE { None = 0, Alpha = 1, Blue = 2, Green = 4, Red = 8, All = 15};

enum class PrimitiveClass : td::BYTE { Unspecified = 0, Point = 1, Line = 2, Triangle = 3}; //PrimitiveTopologyClass

enum class PrimitiveType : td::BYTE { Point = 0, Line = 1, LineStrip = 2, Triangle = 3, TriangleStrip = 4};

//enum class Primitive : td::BYTE {Points=0, Lines, LineStrip, LineLoop, Triangles, TriangleFan, TriangleStrip, Error};

enum class TessellationPartitionMode : td::BYTE { Pow2 = 0, Integer = 1, Odd = 2,Even = 3 };

enum class TessellationFactorStepFunction : td::BYTE { Constant = 0, PerPatch = 1, PerInstance = 2, nPerPatchAndPerInstance = 3};

enum class TessellationFactorFormat: td::BYTE {Half = 0};

enum class TessellationControlPointIndexType: td::BYTE { None = 0, UInt16 = 1, UInt32 = 2};
enum class CullMode : td::BYTE { None = 0, Front = 1, Back = 2 };

enum class Winding : td::BYTE { Clockwise = 0, CounterClockwise = 1};

enum class DepthClipMode : td::BYTE {Clip = 0, Clamp = 1};

enum class TriangleFillMode : td::BYTE { Fill = 0, Lines = 1};

enum class RenderStage : td::BYTE { Vertex = 1, Fragment = 2, Tile = 4 };

enum class VisibilityResultMode : td::BYTE { Disabled = 0, Boolean = 1, Counting = 2 };

enum class LoadAction : td::BYTE { DontCare = 0, Load = 1, Clear = 2 };

enum class StoreAction : td::BYTE { DontCare = 0,Store = 1,MultisampleResolve = 2,StoreAndMultisampleResolve = 3,Unknown = 4,CustomSampleDepthStore = 5};

enum class StoreActionOptions : td::BYTE { None = 0, ValidMask = 1, CustomSamplePositions = 1};

enum class IndexType : td::BYTE { UInt16 = 0, UInt32 = 1};

enum class StencilOperation : uint8_t {
    Keep = 0,
    Zero = 1,
    Replace = 2,
    IncrementClamp = 3,
    DecrementClamp = 4,
    Invert = 5,
    IncrementWrap = 6,
    DecrementWrap = 7
};

struct ScissorRect
{
    td::UINT4 x;
    td::UINT4 y;
    td::UINT4 width;
    td::UINT4 height;
};

using Integer = std::intptr_t;
using UInteger = std::uintptr_t;

//inline bool isNormalized(glx::DataType dt)
//{
//    if (dt >= glx::DataType::norm1 && dt <= glx::DataType::norm4)
//        return true;
//    return false;
//}

template <typename T>
class Volume
{
public:
    T width = 0;
    T height = 0;
    T depth = 0;
};

struct Viewport
{
    double originX;
    double originY;
    double width;
    double height;
    double znear;
    double zfar;
};

typedef double TimeInterval;

namespace Patch
{
enum class Type : td::BYTE { None = 0, Triangle = 1, Quad = 2};
} //namespace Patch

struct ClearColor
{
    double red;
    double green;
    double blue;
    double alpha;

    // Match Metal's MTLClearColorMake style
    static constexpr ClearColor Make(double r, double g, double b, double a) noexcept
    {
        return { r, g, b, a };
    }
};

enum class TextureUsage : uint32_t {
    Unknown = 0,
    ShaderRead = 1u << 0,   // sample/read in shaders
    ShaderWrite = 1u << 1,   // write in compute/fragment (Metal permits FP/INT formats that support writes)
    RenderTarget = 1u << 2,   // color/depth/stencil render target
    // (bit 3 intentionally unused in Metal)
    PixelFormatView = 1u << 4,   // allow creating texture views with different pixel formats/layouts
    ShaderAtomic = 1u << 5,   // allow image atomic ops (Metal 3.1+)
};

enum class TextureType : uint32_t {
    Type1D = 0,
    Type1DArray = 1,
    Type2D = 2,
    Type2DArray = 3,
    Type2DMultisample = 4,
    TypeCube = 5,
    TypeCubeArray = 6,
    Type3D = 7,
    Type2DMultisampleArray = 8,
    TypeTextureBuffer = 9
};


enum class VertexFormat : uint32_t
{
    Invalid = 0,   // MTLVertexFormatInvalid

    UChar2 = 1,    // MTLVertexFormatUChar2
    UChar3 = 2,    // MTLVertexFormatUChar3
    UChar4 = 3,    // MTLVertexFormatUChar4
    Char2 = 4,     // MTLVertexFormatChar2
    Char3 = 5,     // MTLVertexFormatChar3
    Char4 = 6,     // MTLVertexFormatChar4

    UChar2Norm = 7,  // MTLVertexFormatUChar2Normalized
    UChar3Norm = 8,  // MTLVertexFormatUChar3Normalized
    UChar4Norm = 9,  // MTLVertexFormatUChar4Normalized
    Char2Norm = 10,  // MTLVertexFormatChar2Normalized
    Char3Norm = 11,  // MTLVertexFormatChar3Normalized
    Char4Norm = 12,  // MTLVertexFormatChar4Normalized

    UShort2 = 13,  // MTLVertexFormatUShort2
    UShort3 = 14,  // MTLVertexFormatUShort3
    UShort4 = 15,  // MTLVertexFormatUShort4
    Short2 = 16,   // MTLVertexFormatShort2
    Short3 = 17,   // MTLVertexFormatShort3
    Short4 = 18,   // MTLVertexFormatShort4

    UShort2Norm = 19, // MTLVertexFormatUShort2Normalized
    UShort3Norm = 20, // MTLVertexFormatUShort3Normalized
    UShort4Norm = 21, // MTLVertexFormatUShort4Normalized
    Short2Norm = 22,  // MTLVertexFormatShort2Normalized
    Short3Norm = 23,  // MTLVertexFormatShort3Normalized
    Short4Norm = 24,  // MTLVertexFormatShort4Normalized

    Half2 = 25,    // MTLVertexFormatHalf2
    Half3 = 26,    // MTLVertexFormatHalf3
    Half4 = 27,    // MTLVertexFormatHalf4

    Float = 28,    // MTLVertexFormatFloat
    Float2 = 29,   // MTLVertexFormatFloat2
    Float3 = 30,   // MTLVertexFormatFloat3
    Float4 = 31,   // MTLVertexFormatFloat4

    Int = 32,      // MTLVertexFormatInt
    Int2 = 33,     // MTLVertexFormatInt2
    Int3 = 34,     // MTLVertexFormatInt3
    Int4 = 35,     // MTLVertexFormatInt4

    UInt = 36,     // MTLVertexFormatUInt
    UInt2 = 37,    // MTLVertexFormatUInt2
    UInt3 = 38,    // MTLVertexFormatUInt3
    UInt4 = 39,    // MTLVertexFormatUInt4

    Int1010102Norm = 40, // MTLVertexFormatInt1010102Normalized
    UInt1010102Norm = 41, // MTLVertexFormatUInt1010102Normalized
    UChar4NormBgra = 42, // MTLVertexFormatUChar4Normalized_bgra
};


enum class VertexStepFunction : uint32_t
{
    Constant = 0, // MTLVertexStepFunctionConstant
    PerVertex = 1, // MTLVertexStepFunctionPerVertex
    PerInstance = 2, // MTLVertexStepFunctionPerInstance
    PerPatch = 3, // MTLVertexStepFunctionPerPatch
    PerPatchControlPoint = 4, // MTLVertexStepFunctionPerPatchControlPoint
};

// Texture swizzle support (mirrors Metal's MTLSwizzle / MTLTextureSwizzleChannels)
enum class TextureSwizzle : uint8_t { Zero = 0, One = 1, Red = 2, Green = 3, Blue = 4, Alpha = 5 };
struct TextureSwizzleChannels
{
    TextureSwizzle red   = TextureSwizzle::Red;
    TextureSwizzle green = TextureSwizzle::Green;
    TextureSwizzle blue  = TextureSwizzle::Blue;
    TextureSwizzle alpha = TextureSwizzle::Alpha;
    
    TextureSwizzleChannels() = default;
    
    static TextureSwizzleChannels Default() noexcept
    {
        return TextureSwizzleChannels{};
    }

#ifdef MU_MACOS
    // Automatic conversion to MTL::TextureSwizzleChannels on macOS
    operator MTL::TextureSwizzleChannels() const;
	TextureSwizzleChannels(const MTL::TextureSwizzleChannels& mtlSwizzle);
#endif
};


enum class CompareFunction : uint8_t {
    Never = 0,
    Less = 1,
    Equal = 2,
    LessEqual = 3,
    Greater = 4,
    NotEqual = 5,
    GreaterEqual = 6,
    Always = 7
};

enum class SamplerBorderColor : uint8_t {
    TransparentBlack = 0,
    OpaqueBlack = 1,
    OpaqueWhite = 2
};

enum class SamplerAddressMode : uint8_t {
    ClampToEdge = 0,
    MirrorClampToEdge = 1,
    Repeat = 2,
    MirrorRepeat = 3,
    ClampToZero = 4,
    ClampToBorderColor = 5
};

enum class Filter : uint8_t {
    Nearest = 0,
    Linear = 1
};

enum class MipFilter : uint8_t {
    NotMipmapped = 0,
    Nearest = 1,
    Linear = 2
};


} //namespace glx

















