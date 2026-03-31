#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float accAngle;
    float4x4 cameraWithProjection;
};

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

struct mainFS_in
{
    float2 fragTexCoord [[user(locn0)]];
    uint mixingMode [[user(locn1)]];
};

static inline __attribute__((always_inline))
void mainFS(thread uint& mixingMode, thread float4& fragColor, thread float2& fragTexCoord, texture2d<float> texture1, sampler textureSampler, texture2d<float> texture2)
{
    if (mixingMode == 0u)
    {
        fragColor = texture1.sample(textureSampler, fragTexCoord);
    }
    else
    {
        if (mixingMode == 1u)
        {
            fragColor = texture2.sample(textureSampler, fragTexCoord);
        }
        else
        {
            if (mixingMode == 2u)
            {
                fragColor = mix(texture1.sample(textureSampler, fragTexCoord), texture2.sample(textureSampler, fragTexCoord), float4(0.5));
            }
            else
            {
                discard_fragment();
            }
        }
    }
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], texture2d<float> texture1 [[texture(2)]], texture2d<float> texture2 [[texture(3)]], sampler textureSampler [[sampler(4)]])
{
    mainFS_out out = {};
    mainFS(in.mixingMode, out.fragColor, in.fragTexCoord, texture1, textureSampler, texture2);
    return out;
}

