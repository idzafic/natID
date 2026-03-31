#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 mvp;
    float4 params;
};

struct mainVS_out
{
    float2 fragTexCoord [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float2 position [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread float2& position, thread float2& fragTexCoord, thread float2& texCoord, constant UniformBufferObject& ubo)
{
    gl_Position = ubo.mvp * float4(position, 0.0, 1.0);
    fragTexCoord = texCoord * ubo.params.x;
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant UniformBufferObject& ubo [[buffer(3)]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, in.position, out.fragTexCoord, in.texCoord, ubo);
    return out;
}

