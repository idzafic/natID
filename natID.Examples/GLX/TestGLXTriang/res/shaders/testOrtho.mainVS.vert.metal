#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct UBO
{
    float4x4 projection;
};

struct mainVS_out
{
    float4 vColor [[user(locn0)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float2 inPosition [[attribute(0)]];
    float4 inColor [[attribute(1)]];
    float2 inTranslation [[attribute(2)]];
    float inScale [[attribute(3)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float2& inPosition, thread float& inScale, thread float2& inTranslation, thread float4& gl_Position, thread float4& vColor, thread float4& inColor, constant UBO& _17)
{
    float2 worldPos = (inPosition * inScale) + inTranslation;
    gl_Position = _17.projection * float4(worldPos, 0.0, 1.0);
    vColor = inColor;
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant UBO& _17 [[buffer(6)]])
{
    mainVS_out out = {};
    mainVS(in.inPosition, in.inScale, in.inTranslation, out.gl_Position, out.vColor, in.inColor, _17);
    return out;
}

