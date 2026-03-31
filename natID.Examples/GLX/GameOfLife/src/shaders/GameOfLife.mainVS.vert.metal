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

struct mainVS_out
{
    float4 gl_Position [[position]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread uint& gl_VertexIndex)
{
    gl_Position = float4(_99[int(gl_VertexIndex)], 0.0, 1.0);
}

vertex mainVS_out mainVS(uint gl_VertexIndex [[vertex_id]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, gl_VertexIndex);
    return out;
}

