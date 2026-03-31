#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UBO
{
    float4x4 projection;
};

struct mainFS_out
{
    float4 outColor [[color(0)]];
};

struct mainFS_in
{
    float4 vColor [[user(locn0)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float4& outColor, thread float4& vColor)
{
    outColor = vColor;
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]])
{
    mainFS_out out = {};
    mainFS(out.outColor, in.vColor);
    return out;
}

