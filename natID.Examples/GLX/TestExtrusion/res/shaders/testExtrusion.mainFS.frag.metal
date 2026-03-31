#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float accAngle;
    float _pad0;
    float _pad1;
    float _pad2;
    float4 cameraEye;
    float4x4 cameraWithProjection;
};

struct mainFS_out
{
    float4 outColor [[color(0)]];
};

struct mainFS_in
{
    float3 fragColor [[user(locn0)]];
    float3 fragNormal [[user(locn1)]];
    float3 fragWorldPos [[user(locn2)]];
    float3 fragCameraEye [[user(locn3)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float3& fragColor, thread float3& fragNormal, thread float4& outColor, thread float3& fragCameraEye, thread float3& fragWorldPos)
{
    float3 materialColor = fragColor;
    float normLen = length(fragNormal);
    if (normLen < 0.001000000047497451305389404296875)
    {
        outColor = float4(materialColor * float3(0.20000000298023223876953125), 1.0);
        return;
    }
    float3 norm = fragNormal / float3(normLen);
    float3 ambient = float3(0.20000000298023223876953125) * materialColor;
    float diff = fast::max(dot(norm, float3(0.666666686534881591796875, 0.666666686534881591796875, 0.3333333432674407958984375)), 0.0);
    float3 diffuse = float3(0.699999988079071044921875) * (materialColor * diff);
    float3 viewDir = fast::normalize(fragCameraEye - fragWorldPos);
    float3 reflectDir = reflect(float3(-0.666666686534881591796875, -0.666666686534881591796875, -0.3333333432674407958984375), norm);
    float spec = powr(fast::max(dot(viewDir, reflectDir), 0.0), 16.0);
    float3 specular = float3(1.0) * (float3(0.5) * spec);
    float3 result = (ambient + diffuse) + specular;
    outColor = float4(result, 1.0);
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]])
{
    mainFS_out out = {};
    mainFS(in.fragColor, in.fragNormal, out.outColor, in.fragCameraEye, in.fragWorldPos);
    return out;
}

