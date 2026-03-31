#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 mvp;
    float4 params;
};

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

struct mainFS_in
{
    float2 fragTexCoord [[user(locn0)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float4& fragColor, thread float2& fragTexCoord, texture2d<float> textureSampler, sampler samplerState)
{
    fragColor = textureSampler.sample(samplerState, fragTexCoord);
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], texture2d<float> textureSampler [[texture(1)]], sampler samplerState [[sampler(2)]])
{
    mainFS_out out = {};
    mainFS(out.fragColor, in.fragTexCoord, textureSampler, samplerState);
    return out;
}

