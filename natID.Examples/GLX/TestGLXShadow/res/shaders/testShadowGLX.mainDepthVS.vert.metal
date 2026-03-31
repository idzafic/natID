#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct DepthPassUBO
{
    float4x4 depthMVP;
};

struct LightingUBO
{
    float4 lightColor;
};

struct SceneUBO
{
    float4x4 v;
    float4x4 mv;
    float4x4 n;
    float4x4 mvp;
    float4x4 DepthBiasMVP;
    float4 lightPos;
};

struct mainDepthVS_out
{
    float4 gl_Position [[position]];
};

struct mainDepthVS_in
{
    float3 vertexPosition [[attribute(0)]];
};

static inline __attribute__((always_inline))
void mainDepthVS(thread float4& gl_Position, thread float3& vertexPosition, constant DepthPassUBO& ubo)
{
    gl_Position = ubo.depthMVP * float4(vertexPosition, 1.0);
}

vertex mainDepthVS_out mainDepthVS(mainDepthVS_in in [[stage_in]], constant DepthPassUBO& ubo [[buffer(8)]])
{
    mainDepthVS_out out = {};
    mainDepthVS(out.gl_Position, in.vertexPosition, ubo);
    return out;
}

