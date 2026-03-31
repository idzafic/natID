#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct LightingUBO
{
    float3 lightPos;
    float3 lightColor;
};

struct MatrixUBO
{
    float4x4 v;
    float4x4 mv;
    float4x4 n;
    float4x4 mvp;
};

struct OutlineMatrixUBO
{
    float4x4 m;
    float4x4 v;
    float4x4 p;
};

struct mainFS_outline_out
{
    float4 FragColor [[color(0)]];
};

static inline __attribute__((always_inline))
void mainFS_outline(thread float4& FragColor)
{
    FragColor = float4(1.0, 0.0, 0.0, 1.0);
}

fragment mainFS_outline_out mainFS_outline()
{
    mainFS_outline_out out = {};
    mainFS_outline(out.FragColor);
    return out;
}

