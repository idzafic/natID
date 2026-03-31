#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct SimpleUniforms
{
    float zoomFactor;
    float pad0;
    float pad1;
    float pad2;
};

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

struct mainFS_in
{
    float4 vertexColor [[user(locn0)]];
};

static inline __attribute__((always_inline))
void mainFS(thread float4& fragColor, thread float4& vertexColor)
{
    fragColor = vertexColor;
}

fragment mainFS_out mainFS(mainFS_in in [[stage_in]])
{
    mainFS_out out = {};
    mainFS(out.fragColor, in.vertexColor);
    return out;
}

