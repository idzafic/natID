#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct CubeUniforms
{
    float4x4 mvp;
};

struct mainVS_out
{
    float2 fragTexCoord [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 position [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float4& gl_Position, thread float3& position, thread float2& fragTexCoord, thread float2& texCoord, constant CubeUniforms& _16)
{
    gl_Position = _16.mvp * float4(position, 1.0);
    fragTexCoord = texCoord;
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant CubeUniforms& _16 [[buffer(1)]])
{
    mainVS_out out = {};
    mainVS(out.gl_Position, in.position, out.fragTexCoord, in.texCoord, _16);
    return out;
}

