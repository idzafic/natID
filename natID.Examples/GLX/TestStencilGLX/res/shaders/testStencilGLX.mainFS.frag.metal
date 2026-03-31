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
    float3 v_tangent [[user(locn4)]];
    float3 v_bitangent [[user(locn5)]];
};

static inline __attribute__((always_inline))
float3 getNormalFromMap(thread float2& fragTexCoord, thread float4& v_normal, thread float3& v_tangent, thread float3& v_bitangent, texture2d<float> texWoodFloorNorm, sampler texWoodFloorNormSampler)
{
    float3 normalMap = texWoodFloorNorm.sample(texWoodFloorNormSampler, fragTexCoord).xyz;
    normalMap = (normalMap * 2.0) - float3(1.0);
    float3 N = fast::normalize(v_normal.xyz);
    float3 T = fast::normalize(v_tangent);
    float3 B = fast::normalize(v_bitangent);
    T = fast::normalize(T - (N * dot(T, N)));
    B = cross(N, T);
    float3x3 TBN = float3x3(float3(T), float3(B), float3(N));
    return fast::normalize(TBN * normalMap);
}

static inline __attribute__((always_inline))
float3 phong(thread const float3& modelColor, thread const float3& normal, thread float3& LightPos, thread float3& FragPos, constant LightingUBO& lighting)
{
    float3 LightPosition = LightPos;
    float mKa = 0.100000001490116119384765625;
    float mKd = 1.0;
    float mKs = 0.4000000059604644775390625;
    float3 Ka = lighting.lightColor;
    float3 Kd = lighting.lightColor;
    float3 Ks = lighting.lightColor;
    float Shininess = 128.0;
    float3 n = fast::normalize(normal);
    float3 s = fast::normalize(LightPosition - FragPos);
    float3 v = fast::normalize(-FragPos);
    float3 r = reflect(-s, n);
    return modelColor * (((Ka * mKa) + ((Kd * mKd) * fast::max(dot(s, n), 0.0))) + ((Ks * mKs) * powr(fast::max(dot(r, v), 0.0), Shininess)));
}

static inline __attribute__((always_inline))
void mainFS(thread float3& LightPos, thread float3& FragPos, thread float2& fragTexCoord, thread float4& v_normal, thread float3& v_tangent, thread float3& v_bitangent, thread float4& fragColor, constant LightingUBO& lighting, texture2d<float> texWoodFloorNorm, sampler texWoodFloorNormSampler, texture2d<float> texWoodFloor, sampler texWoodFloorSampler)
{
    float4 color1 = texWoodFloor.sample(texWoodFloorSampler, fragTexCoord);
    float4 _245 = color1;
    float3 _247 = powr(_245.xyz, float3(2.2000000476837158203125));
    color1.x = _247.x;
    color1.y = _247.y;
    color1.z = _247.z;
    float3 finalNormal = getNormalFromMap(fragTexCoord, v_normal, v_tangent, v_bitangent, texWoodFloorNorm, texWoodFloorNormSampler);
    float3 param = color1.xyz;
    float3 param_1 = finalNormal;
    fragColor = float4(phong(param, param_1, LightPos, FragPos, lighting), color1.w);
    float4 _265 = fragColor;
    float3 _267 = powr(_265.xyz, float3(0.4545454680919647216796875));
    fragColor.x = _267.x;
    fragColor.y = _267.y;
    fragColor.z = _267.z;
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], constant LightingUBO& lighting [[buffer(2)]], texture2d<float> texWoodFloor [[texture(3)]], texture2d<float> texWoodFloorNorm [[texture(5)]], sampler texWoodFloorSampler [[sampler(4)]], sampler texWoodFloorNormSampler [[sampler(6)]])
{
    mainFS_out out = {};
    mainFS(in.LightPos, in.FragPos, in.fragTexCoord, in.v_normal, in.v_tangent, in.v_bitangent, out.fragColor, lighting, texWoodFloorNorm, texWoodFloorNormSampler, texWoodFloor, texWoodFloorSampler);
    return out;
}

