#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct CubeUniforms
{
    float4x4 mvp;
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
void mainFS(thread float4& fragColor, thread float2& fragTexCoord, texture2d<float> textureImage, sampler textureSampler)
{
    fragColor = textureImage.sample(textureSampler, fragTexCoord);
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], texture2d<float> textureImage [[texture(3)]], sampler textureSampler [[sampler(4)]])
{
    mainFS_out out = {};
    mainFS(out.fragColor, in.fragTexCoord, textureImage, textureSampler);
    return out;
}

