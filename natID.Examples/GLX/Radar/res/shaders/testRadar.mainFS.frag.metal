#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

// Implementation of the GLSL mod() function, which is slightly different than Metal fmod()
template<typename Tx, typename Ty>
inline Tx mod(Tx x, Ty y)
{
    return x - y * floor(x / y);
}

struct UniformBuffer
{
    float2 leftClick;
    float2 rightClick;
    float2 viewportSize;
    float animStep;
};

struct Positions
{
    float4 positions[1];
};

struct mainFS_out
{
    float4 fragColor [[color(0)]];
};

static inline __attribute__((always_inline))
float radar_trail(thread const float2& uv, thread const float2& center, thread const float& radius, constant UniformBuffer& _35)
{
    float angle = 0.5 * _35.animStep;
    float angle_rad = (angle * 3.1415927410125732421875) / 180.0;
    float2 h = uv - center;
    float r = length(h);
    if (r > radius)
    {
        return 0.0;
    }
    h = fast::normalize(h);
    float theta = mod(precise::atan2(h.y, h.x) + angle_rad, 6.283185482025146484375);
    float gradient = fast::clamp(1.0 - (theta * 0.4000000059604644775390625), 0.0, 0.5);
    return gradient;
}

static inline __attribute__((always_inline))
float enemy(thread const float2& uv, thread const float2& center, thread const float& size, constant UniformBuffer& _35)
{
    float r = length(uv - center);
    float maxCircleRadius = 0.5 * fast::min(_35.viewportSize.x, _35.viewportSize.y);
    float2 centerOfViewport = _35.viewportSize / float2(2.0);
    float2 centerToRadarCoord = center - centerOfViewport;
    if (length(centerToRadarCoord) > (maxCircleRadius - 36.0))
    {
        return 0.0;
    }
    return (0.5 + (0.5 * cos(0.0599999986588954925537109375 * _35.animStep))) * smoothstep(size + 2.0, size - 2.0, r);
}

static inline __attribute__((always_inline))
float circle(thread const float2& uv, thread const float2& center, thread const float& radius, thread const float& width)
{
    float r = length(uv - center);
    return smoothstep(radius + 1.0, radius - 1.0, r - (width / 2.0)) - smoothstep(radius + 1.0, radius - 1.0, r + (width / 2.0));
}

static inline __attribute__((always_inline))
float radar_line(thread const float2& uv, thread const float2& center, thread const float& radius, thread const float& lineW, constant UniformBuffer& _35)
{
    float angle = 0.5 * _35.animStep;
    float angle_rad = (angle * 3.1415927410125732421875) / 180.0;
    float2 h = uv - center;
    float r = length(h);
    if (r > radius)
    {
        return 0.0;
    }
    float2 p = float2(cos(angle_rad), -sin(angle_rad)) * radius;
    float l = length(h - (p * fast::clamp(dot(h, p) / dot(p, p), 0.0, 1.0)));
    return smoothstep(lineW, 0.5 * lineW, l);
}

static inline __attribute__((always_inline))
float point(thread const float2& uv, thread const float2& center, thread const float& size)
{
    float r = length(uv - center);
    return smoothstep(size + 1.0, size - 1.0, r);
}

static inline __attribute__((always_inline))
float4 primjer_Radar3(thread float4& gl_FragCoord, constant UniformBuffer& _35)
{
    float2 uv = gl_FragCoord.xy;
    float2 center = _35.viewportSize / float2(2.0);
    float3 color = float3(0.0);
    float maxCircleRadius = 0.5 * fast::min(_35.viewportSize.x, _35.viewportSize.y);
    float2 param = uv;
    float2 param_1 = center;
    float param_2 = 0.89999997615814208984375 * maxCircleRadius;
    float radar_trail_1 = radar_trail(param, param_1, param_2, _35);
    float2 param_3 = uv;
    float2 param_4 = _35.leftClick;
    float param_5 = 10.0;
    color += ((float3(1.0, 0.3499999940395355224609375, 0.3499999940395355224609375) * enemy(param_3, param_4, param_5, _35)) * radar_trail_1);
    float2 param_6 = uv;
    float2 param_7 = _35.rightClick;
    float param_8 = 10.0;
    color += ((float3(1.0, 1.0, 0.3499999940395355224609375) * enemy(param_6, param_7, param_8, _35)) * radar_trail_1);
    float2 param_9 = uv;
    float2 param_10 = center;
    float param_11 = 0.300000011920928955078125 * maxCircleRadius;
    float param_12 = 1.0;
    color += (float3(0.699999988079071044921875, 0.89999997615814208984375, 1.0) * circle(param_9, param_10, param_11, param_12));
    float2 param_13 = uv;
    float2 param_14 = center;
    float param_15 = 0.5 * maxCircleRadius;
    float param_16 = 2.0;
    color += (float3(0.699999988079071044921875, 0.89999997615814208984375, 1.0) * circle(param_13, param_14, param_15, param_16));
    float2 param_17 = uv;
    float2 param_18 = center;
    float param_19 = 0.699999988079071044921875 * maxCircleRadius;
    float param_20 = 3.0;
    color += (float3(0.699999988079071044921875, 0.89999997615814208984375, 1.0) * circle(param_17, param_18, param_19, param_20));
    float2 param_21 = uv;
    float2 param_22 = center;
    float param_23 = 0.89999997615814208984375 * maxCircleRadius;
    float param_24 = 4.0;
    color += (float3(0.699999988079071044921875, 0.89999997615814208984375, 1.0) * circle(param_21, param_22, param_23, param_24));
    float2 param_25 = uv;
    float2 param_26 = center;
    float param_27 = 0.89999997615814208984375 * maxCircleRadius;
    float param_28 = 4.0;
    color += fast::max(float3(0.300000011920928955078125, 0.699999988079071044921875, 0.89999997615814208984375) * radar_trail_1, float3(0.300000011920928955078125, 0.89999997615814208984375, 0.60000002384185791015625) * radar_line(param_25, param_26, param_27, param_28, _35));
    float2 param_29 = uv;
    float2 param_30 = center;
    float param_31 = 7.0;
    color += (float3(0.300000011920928955078125, 0.89999997615814208984375, 0.60000002384185791015625) * point(param_29, param_30, param_31));
    return float4(color, 1.0);
}

static inline __attribute__((always_inline))
void mainFS(thread float4& gl_FragCoord, thread float4& fragColor, constant UniformBuffer& _35)
{
    fragColor = primjer_Radar3(gl_FragCoord, _35);
}

fragment mainFS_out mainFS(constant UniformBuffer& _35 [[buffer(1)]], float4 gl_FragCoord [[position]])
{
    mainFS_out out = {};
    mainFS(gl_FragCoord, out.fragColor, _35);
    return out;
}

