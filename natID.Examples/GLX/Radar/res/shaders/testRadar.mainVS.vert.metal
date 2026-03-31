#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBuffer
{
    float2 leftClick;
    float2 rightClick;
    float2 viewportSize;
    float animStep;
};

struct Positions
{
    float4 positions[1];
};

struct mainVS_out
{
    float4 gl_Position [[position]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread uint& gl_VertexIndex, const device Positions& _92)
{
    gl_Position = _92.positions[int(gl_VertexIndex)];
}

vertex mainVS_out mainVS(const device Positions& _92 [[buffer(3)]], uint gl_VertexIndex [[vertex_id]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, gl_VertexIndex, _92);
    return out;
}

