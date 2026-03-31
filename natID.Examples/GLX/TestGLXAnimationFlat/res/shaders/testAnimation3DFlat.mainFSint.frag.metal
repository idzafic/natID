#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UniformBufferObject
{
    float4x4 mvp;
};

struct mainFSint_out
{
    float4 frag_color [[color(0)]];
};

struct mainFSint_in
{
    float3 color [[user(locn0)]];
};

static inline __attribute__((always_inline))
void mainFSint(thread float4& frag_color, thread float3& color)
{
    frag_color = float4(color, 1.0);
}

fragment mainFSint_out mainFSint(mainFSint_in in [[stage_in]])
{
    mainFSint_out out = {};
    mainFSint(out.frag_color, in.color);
    return out;
}

