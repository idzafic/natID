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
void mainFS(thread uint& mixingMode, thread float4& fragColor, thread float2& fragTexCoord, texture2d<float> texture1, sampler sampler1, texture2d<float> texture2, sampler sampler2)
{
    if (mixingMode == 0u)
    {
        fragColor = texture1.sample(sampler1, fragTexCoord);
    }
    else
    {
        if (mixingMode == 1u)
        {
            fragColor = texture2.sample(sampler2, fragTexCoord);
        }
        else
        {
            if (mixingMode == 2u)
            {
                fragColor = mix(texture1.sample(sampler1, fragTexCoord), texture2.sample(sampler2, fragTexCoord), float4(0.5));
            }
            else
            {
                discard_fragment();
            }
        }
    }
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]], texture2d<float> texture1 [[texture(2)]], texture2d<float> texture2 [[texture(3)]], sampler sampler1 [[sampler(4)]], sampler sampler2 [[sampler(5)]])
{
    mainFS_out out = {};
    mainFS(in.mixingMode, out.fragColor, in.fragTexCoord, texture1, sampler1, texture2, sampler2);
    return out;
}

