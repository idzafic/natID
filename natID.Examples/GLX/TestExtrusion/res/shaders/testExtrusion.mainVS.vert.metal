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

struct mainVS_out
{
    float3 fragColor [[user(locn0)]];
    float3 fragNormal [[user(locn1)]];
    float3 fragWorldPos [[user(locn2)]];
    float3 fragCameraEye [[user(locn3)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 position [[attribute(0)]];
    float3 normal [[attribute(1)]];
    float3 translation [[attribute(2)]];
    float3 instanceColor [[attribute(3)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float3& fragColor, thread float3& instanceColor, thread float3& fragNormal, thread float3& normal, thread float3& fragCameraEye, thread float3& position, thread float3& translation, thread float3& fragWorldPos, thread float4& gl_Position, constant UniformBufferObject& ubo)
{
    fragColor = instanceColor;
    fragNormal = normal;
    fragCameraEye = ubo.cameraEye.xyz;
    float3 worldPosition = position + translation;
    fragWorldPos = worldPosition;
    gl_Position = ubo.cameraWithProjection * float4(worldPosition, 1.0);
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant UniformBufferObject& ubo [[buffer(6)]])
{
    mainVS_out out = {};
    mainVS(out.fragColor, in.instanceColor, out.fragNormal, in.normal, out.fragCameraEye, in.position, in.translation, out.fragWorldPos, out.gl_Position, ubo);
    return out;
}

