#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct LightingUBO
{
    float3 lightPos;
    float3 lightColor;
};

struct MatrixUBO
{
    float4x4 v;
    float4x4 mv;
    float4x4 n;
    float4x4 mvp;
};

struct OutlineMatrixUBO
{
    float4x4 m;
    float4x4 v;
    float4x4 p;
};

struct mainVS_outline_out
{
    float4 gl_Position [[position]];
};

struct mainVS_outline_in
{
    float3 vertexPosition [[attribute(0)]];
};

static inline __attribute__((always_inline))
void mainVS_outline(thread float4& gl_Position, thread float3& vertexPosition, constant OutlineMatrixUBO& matrices)
{
    gl_Position = ((matrices.p * matrices.v) * matrices.m) * float4(vertexPosition, 1.0);
}

vertex mainVS_outline_out mainVS_outline(mainVS_outline_in in [[stage_in]], constant OutlineMatrixUBO& matrices [[buffer(7)]])
{
    mainVS_outline_out out = {};
    mainVS_outline(out.gl_Position, in.vertexPosition, matrices);
    return out;
}

