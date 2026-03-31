#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 mvp;
};

struct mainFSflat_out
{
    float4 frag_color [[color(0)]];
};

struct mainFSflat_in
{
    float3 color [[user(locn0), flat]];
};

static inline __attribute__((always_inline))
void mainFSflat(thread float4& frag_color, thread float3& color)
{
    frag_color = float4(color, 1.0);
}

fragment mainFSflat_out mainFSflat(mainFSflat_in in [[stage_in]])
{
    mainFSflat_out out = {};
    mainFSflat(out.frag_color, in.color);
    return out;
}

