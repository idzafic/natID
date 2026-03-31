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

struct mainVS_out
{
    float2 fragTexCoord [[user(locn0)]];
    float4 v_normal [[user(locn1)]];
    float3 FragPos [[user(locn2)]];
    float3 LightPos [[user(locn3)]];
    float4 ShadowCoord [[user(locn4)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 vertexPosition [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
    float3 vertexNormal [[attribute(2)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float3& LightPos, thread float4& v_normal, thread float3& vertexNormal, thread float3& FragPos, thread float3& vertexPosition, thread float2& fragTexCoord, thread float2& texCoord, thread float4& gl_Position, thread float4& ShadowCoord, constant SceneUBO& ubo)
{
    LightPos = float3((ubo.v * float4(ubo.lightPos.xyz, 1.0)).xyz);
    v_normal = fast::normalize(ubo.n * float4(vertexNormal, 1.0));
    FragPos = float3((ubo.mv * float4(vertexPosition, 1.0)).xyz);
    fragTexCoord = texCoord;
    gl_Position = ubo.mvp * float4(vertexPosition, 1.0);
    ShadowCoord = ubo.DepthBiasMVP * float4(vertexPosition, 1.0);
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant SceneUBO& ubo [[buffer(2)]])
{
    mainVS_out out = {};
    mainVS(out.LightPos, out.v_normal, in.vertexNormal, out.FragPos, in.vertexPosition, out.fragTexCoord, in.texCoord, out.gl_Position, out.ShadowCoord, ubo);
    return out;
}

