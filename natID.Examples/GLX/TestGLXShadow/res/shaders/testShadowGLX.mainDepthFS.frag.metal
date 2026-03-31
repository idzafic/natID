#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct DepthPassUBO
{
    float4x4 depthMVP;
};

struct LightingUBO
{
    float4 lightColor;
};

struct SceneUBO
{
    float4x4 v;
    float4x4 mv;
    float4x4 n;
    float4x4 mvp;
    float4x4 DepthBiasMVP;
    float4 lightPos;
};

static inline __attribute__((always_inline))
void mainDepthFS()
{
}

fragment void mainDepthFS()
{
    mainDepthFS();
}

