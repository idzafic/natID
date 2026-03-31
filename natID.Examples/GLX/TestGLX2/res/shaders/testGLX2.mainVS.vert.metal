#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct SimpleUniforms
{
    float zoomFactor;
    float pad0;
    float pad1;
    float pad2;
};

struct mainVS_out
{
    float4 vertexColor [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float2 position [[attribute(0)]];
    float3 color [[attribute(1)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread float2& position, thread float4& vertexColor, thread float3& color, constant SimpleUniforms& uniforms)
{
    gl_Position = float4(position * uniforms.zoomFactor, 0.0, 1.0);
    vertexColor = float4(color, 1.0);
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant SimpleUniforms& uniforms [[buffer(1)]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, in.position, out.vertexColor, in.color, uniforms);
    return out;
}

