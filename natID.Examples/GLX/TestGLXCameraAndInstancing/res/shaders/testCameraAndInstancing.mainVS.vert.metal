#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

// Implementation of the GLSL mod() function, which is slightly different than Metal fmod()
template<typename Tx, typename Ty>
inline Tx mod(Tx x, Ty y)
{
    return x - y * floor(x / y);
}

struct UniformBufferObject
{
    float accAngle;
    float4x4 cameraWithProjection;
};

struct mainVS_out
{
    float2 fragTexCoord [[user(locn0)]];
    uint mixingMode [[user(locn1)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 position [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
    float3 initialAngle [[attribute(2)]];
    float3 translation [[attribute(3)]];
    float3 rotationDirection [[attribute(4)]];
};

static inline __attribute__((always_inline))
float4x4 rotateZ(thread const float& angle)
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return float4x4(float4(cosTheta, -sinTheta, 0.0, 0.0), float4(sinTheta, cosTheta, 0.0, 0.0), float4(0.0, 0.0, 1.0, 0.0), float4(0.0, 0.0, 0.0, 1.0));
}

static inline __attribute__((always_inline))
float4x4 rotateY(thread const float& angle)
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return float4x4(float4(cosTheta, 0.0, sinTheta, 0.0), float4(0.0, 1.0, 0.0, 0.0), float4(-sinTheta, 0.0, cosTheta, 0.0), float4(0.0, 0.0, 0.0, 1.0));
}

static inline __attribute__((always_inline))
float4x4 rotateX(thread const float& angle)
{
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);
    return float4x4(float4(1.0, 0.0, 0.0, 0.0), float4(0.0, cosTheta, -sinTheta, 0.0), float4(0.0, sinTheta, cosTheta, 0.0), float4(0.0, 0.0, 0.0, 1.0));
}

static inline __attribute__((always_inline))
void mainVS(thread float2& fragTexCoord, thread float2& texCoord, thread uint& mixingMode, thread uint& gl_InstanceIndex, thread float4& gl_Position, thread float3& initialAngle, thread float3& rotationDirection, thread float3& position, thread float3& translation, constant UniformBufferObject& ubo)
{
    fragTexCoord = texCoord;
    int divisor = 3;
    mixingMode = uint(mod(float(int(gl_InstanceIndex)), float(divisor)));
    divisor = 7;
    float toRemove = mod(float(int(gl_InstanceIndex)), float(divisor));
    if (toRemove == 0.0)
    {
        gl_Position = float4(0.0);
        mixingMode = 5u;
        return;
    }
    float3 angle = initialAngle + (rotationDirection * ubo.accAngle);
    float param = angle.x;
    float param_1 = angle.y;
    float param_2 = angle.z;
    float4x4 rotation = (rotateZ(param) * rotateY(param_1)) * rotateX(param_2);
    float4 rotatedAndTranslatedPosition = (rotation * float4(position, 1.0)) + float4(translation, 0.0);
    gl_Position = ubo.cameraWithProjection * rotatedAndTranslatedPosition;
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant UniformBufferObject& ubo [[buffer(6)]], uint gl_InstanceIndex [[instance_id]])
{
    mainVS_out out = {};
    mainVS(out.fragTexCoord, in.texCoord, out.mixingMode, gl_InstanceIndex, out.gl_Position, in.initialAngle, in.rotationDirection, in.position, in.translation, ubo);
    return out;
}

