#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 v;
    float4x4 mv;
    float4x4 n;
    float4x4 mvp;
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
};

static inline __attribute__((always_inline))
float3 ads(thread const float3& modelColor, thread float3& LightPos, thread float4& v_normal, thread float3& FragPos)
{
    float3 LightPosition = LightPos;
    float mKa = 0.100000001490116119384765625;
    float mKd = 1.0;
    float mKs = 0.4000000059604644775390625;
    float3 Ka = float3(1.0);
    float3 Kd = float3(1.0);
    float3 Ks = float3(1.0);
    float Shininess = 128.0;
    float3 n = fast::normalize(v_normal.xyz);
    float3 s = fast::normalize(LightPosition - FragPos);
    float3 v = fast::normalize(-FragPos);
    float3 r = reflect(-s, n);
    return modelColor * (((Ka * mKa) + ((Kd * mKd) * fast::max(dot(s, n), 0.0))) + ((Ks * mKs) * powr(fast::max(dot(r, v), 0.0), Shininess)));
}

static inline __attribute__((always_inline))
void mainFS(thread float3& LightPos, thread float4& v_normal, thread float3& FragPos, thread float2& fragTexCoord, thread float4& fragColor, texture2d<float> texWoodFloor, sampler woodFloorSampler)
{
    float4 color1 = texWoodFloor.sample(woodFloorSampler, fragTexCoord);
    float3 param = color1.xyz;
    fragColor = float4(ads(param, LightPos, v_normal, FragPos), color1.w);
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], texture2d<float> texWoodFloor [[texture(3)]], sampler woodFloorSampler [[sampler(5)]])
{
    mainFS_out out = {};
    mainFS(in.LightPos, in.v_normal, in.FragPos, in.fragTexCoord, out.fragColor, texWoodFloor, woodFloorSampler);
    return out;
}

