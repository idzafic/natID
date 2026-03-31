#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct Positions
{
    float4 positions[1];
};

struct mainVS_out
{
    float4 vColor [[user(locn0)]];
    float4 gl_Position [[position]];
};

static inline __attribute__((always_inline))
void mainVS(thread uint& gl_VertexIndex, thread float4& gl_Position, thread float4& vColor, const device Positions& _14, const device Positions& _15)
{
    uint i = uint(int(gl_VertexIndex));
    gl_Position = _14.positions[i];
    vColor = _15.positions[i];
}

vertex mainVS_out mainVS(const device Positions& _14 [[buffer(0)]], const device Positions& _15 [[buffer(1)]], uint gl_VertexIndex [[vertex_id]])
{
    mainVS_out out = {};
    mainVS(gl_VertexIndex, out.gl_Position, out.vColor, _14, _15);
    return out;
}

