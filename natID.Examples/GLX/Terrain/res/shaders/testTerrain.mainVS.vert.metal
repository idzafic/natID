#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct TerrainUniforms
{
    float4x4 mvp;
};

struct mainVS_out
{
    float3 vColor [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 position [[attribute(0)]];
    float3 color [[attribute(1)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread float3& position, thread float3& vColor, thread float3& color, constant TerrainUniforms& uniforms)
{
    gl_Position = uniforms.mvp * float4(position, 1.0);
    vColor = color;
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant TerrainUniforms& uniforms [[buffer(3)]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, in.position, out.vColor, in.color, uniforms);
    return out;
}

