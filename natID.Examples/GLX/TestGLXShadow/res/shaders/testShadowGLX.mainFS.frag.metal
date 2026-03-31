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

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

struct mainFS_in
{
    float2 fragTexCoord [[user(locn0)]];
    float4 v_normal [[user(locn1)]];
    float3 FragPos [[user(locn2)]];
    float3 LightPos [[user(locn3)]];
    float4 ShadowCoord [[user(locn4)]];
};

static inline __attribute__((always_inline))
float3 phong(thread const float3& modelColor, thread float3& LightPos, thread float4& v_normal, thread float3& FragPos, constant LightingUBO& lighting)
{
    float3 LightPosition = LightPos;
    float mKa = 0.100000001490116119384765625;
    float mKd = 1.0;
    float mKs = 0.4000000059604644775390625;
    float3 Ka = lighting.lightColor.xyz;
    float3 Kd = lighting.lightColor.xyz;
    float3 Ks = lighting.lightColor.xyz;
    float Shininess = 128.0;
    float3 n = fast::normalize(v_normal.xyz);
    float3 s = fast::normalize(LightPosition - FragPos);
    float3 v = fast::normalize(-FragPos);
    float3 r = reflect(-s, n);
    return modelColor * (((Ka * mKa) + ((Kd * mKd) * fast::max(dot(s, n), 0.0))) + ((Ks * mKs) * powr(fast::max(dot(r, v), 0.0), Shininess)));
}

static inline __attribute__((always_inline))
void mainFS(thread float3& LightPos, thread float4& v_normal, thread float3& FragPos, thread float2& fragTexCoord, thread float4& ShadowCoord, thread float4& fragColor, constant LightingUBO& lighting, texture2d<float> texWoodFloor, sampler texWoodFloorSampler, depth2d<float> shadowMapTexture, sampler shadowMapSampler)
{
    float4 color1 = texWoodFloor.sample(texWoodFloorSampler, fragTexCoord);
    float4 _198 = color1;
    float3 _200 = powr(_198.xyz, float3(2.2000000476837158203125));
    color1.x = _200.x;
    color1.y = _200.y;
    color1.z = _200.z;
    float3 projCoords = ShadowCoord.xyz / float3(ShadowCoord.w);
    bool _215 = projCoords.x < 0.0;
    bool _222;
    if (!_215)
    {
        _222 = projCoords.x > 1.0;
    }
    else
    {
        _222 = _215;
    }
    bool _229;
    if (!_222)
    {
        _229 = projCoords.y < 0.0;
    }
    else
    {
        _229 = _222;
    }
    bool _236;
    if (!_229)
    {
        _236 = projCoords.y > 1.0;
    }
    else
    {
        _236 = _229;
    }
    bool _243;
    if (!_236)
    {
        _243 = projCoords.z < 0.0;
    }
    else
    {
        _243 = _236;
    }
    bool _250;
    if (!_243)
    {
        _250 = projCoords.z > 1.0;
    }
    else
    {
        _250 = _243;
    }
    if (_250)
    {
        float3 param = color1.xyz;
        fragColor = float4(phong(param, LightPos, v_normal, FragPos, lighting), 1.0);
        float4 _260 = fragColor;
        float3 _262 = powr(_260.xyz, float3(0.4545454680919647216796875));
        fragColor.x = _262.x;
        fragColor.y = _262.y;
        fragColor.z = _262.z;
        fragColor.w = 1.0;
        return;
    }
    float3 normal = fast::normalize(v_normal.xyz);
    float3 lightDir = fast::normalize(LightPos - FragPos);
    float cosTheta = fast::clamp(dot(normal, lightDir), 0.0, 1.0);
    float bias0 = fast::max(9.9999997473787516355514526367188e-05 * (1.0 - cosTheta), 9.9999997473787516355514526367188e-05);
    float3 _296 = float3(projCoords.xy, projCoords.z - bias0);
    float visibility = shadowMapTexture.sample_compare(shadowMapSampler, _296.xy, _296.z);
    visibility = fast::clamp(visibility, 0.1500000059604644775390625, 1.0);
    float3 param_1 = color1.xyz;
    fragColor = float4(phong(param_1, LightPos, v_normal, FragPos, lighting), 1.0) * visibility;
    float4 _310 = fragColor;
    float3 _312 = powr(_310.xyz, float3(0.4545454680919647216796875));
    fragColor.x = _312.x;
    fragColor.y = _312.y;
    fragColor.z = _312.z;
    fragColor.w = 1.0;
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], constant LightingUBO& lighting [[buffer(7)]], texture2d<float> texWoodFloor [[texture(3)]], depth2d<float> shadowMapTexture [[texture(5)]], sampler texWoodFloorSampler [[sampler(4)]], sampler shadowMapSampler [[sampler(6)]])
{
    mainFS_out out = {};
    mainFS(in.LightPos, in.v_normal, in.FragPos, in.fragTexCoord, in.ShadowCoord, out.fragColor, lighting, texWoodFloor, texWoodFloorSampler, shadowMapTexture, shadowMapSampler);
    return out;
}

