#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct TerrainUniforms
{
    float4x4 mvp;
};

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

struct mainFS_in
{
    float3 vColor [[user(locn0)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float4& fragColor, thread float3& vColor)
{
    fragColor = float4(vColor, 1.0);
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]])
{
    mainFS_out out = {};
    mainFS(out.fragColor, in.vColor);
    return out;
}

