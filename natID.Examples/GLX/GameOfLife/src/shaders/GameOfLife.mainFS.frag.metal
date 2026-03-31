#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wmissing-braces"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

template<typename T, size_t Num>
struct spvUnsafeArray
{
    T elements[Num ? Num : 1];
    
    thread T& operator [] (size_t pos) thread
    {
        return elements[pos];
    }
    constexpr const thread T& operator [] (size_t pos) const thread
    {
        return elements[pos];
    }
    
    device T& operator [] (size_t pos) device
    {
        return elements[pos];
    }
    constexpr const device T& operator [] (size_t pos) const device
    {
        return elements[pos];
    }
    
    constexpr const constant T& operator [] (size_t pos) const constant
    {
        return elements[pos];
    }
    
    threadgroup T& operator [] (size_t pos) threadgroup
    {
        return elements[pos];
    }
    constexpr const threadgroup T& operator [] (size_t pos) const threadgroup
    {
        return elements[pos];
    }
};

struct GameBoardData
{
    uint width;
    uint height;
    uint data[1];
};

constant uint3 gl_WorkGroupSize [[maybe_unused]] = uint3(32u, 32u, 1u);

struct GameBoardData_1
{
    uint width;
    uint height;
    uint data[1];
};

struct ViewportUBO
{
    float4 extentPx;
};

constant spvUnsafeArray<float2, 4> _99 = spvUnsafeArray<float2, 4>({ float2(-1.0), float2(1.0, -1.0), float2(-1.0, 1.0), float2(1.0) });

struct mainFS_out
{
    float4 outColor [[color(0)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float4& gl_FragCoord, thread float4& outColor, const device GameBoardData_1& gameBoard)
{
    uint gridWidth = 25u;
    uint borderSize = gridWidth / 25u;
    uint2 index = uint2(int2(gl_FragCoord.xy / float2(float(gridWidth))));
    uint2 pxInCell = uint2(gl_FragCoord.xy) - (index * uint2(gridWidth));
    bool _257 = pxInCell.x < borderSize;
    bool _265;
    if (!_257)
    {
        _265 = pxInCell.y < borderSize;
    }
    else
    {
        _265 = _257;
    }
    if (_265)
    {
        outColor = float4(0.800000011920928955078125, 0.800000011920928955078125, 0.800000011920928955078125, 1.0);
        return;
    }
    bool _272 = index.x >= gameBoard.width;
    bool _281;
    if (!_272)
    {
        _281 = index.y >= gameBoard.height;
    }
    else
    {
        _281 = _272;
    }
    if (_281)
    {
        outColor = float4(1.0, 0.0, 0.0, 0.5);
        return;
    }
    float v = float(gameBoard.data[(index.y * gameBoard.width) + index.x] != 0u);
    outColor = float4(v, v, v, 1.0);
}

fragment mainFS_out mainFS(const device GameBoardData_1& gameBoard [[buffer(0)]], float4 gl_FragCoord [[position]])
{
    mainFS_out out = {};
    mainFS(gl_FragCoord, out.outColor, gameBoard);
    return out;
}

