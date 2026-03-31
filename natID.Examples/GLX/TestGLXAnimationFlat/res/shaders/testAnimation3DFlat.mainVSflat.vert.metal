#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 mvp;
};

struct mainVSflat_out
{
    float3 color [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVSflat_in
{
    float3 pos [[attribute(0)]];
    float3 vertex_color [[attribute(1)]];
};

static inline __attribute__((always_inline))
void mainVSflat(thread float4& gl_Position, thread float3& pos, thread float3& color, thread float3& vertex_color, constant UniformBufferObject& ubo)
{
    gl_Position = ubo.mvp * float4(pos, 1.0);
    color = vertex_color;
}

vertex mainVSflat_out mainVSflat(mainVSflat_in in [[stage_in]], constant UniformBufferObject& ubo [[buffer(2)]])
{
    mainVSflat_out out = {};
    mainVSflat(out.gl_Position, in.pos, out.color, in.vertex_color, ubo);
    return out;
}

