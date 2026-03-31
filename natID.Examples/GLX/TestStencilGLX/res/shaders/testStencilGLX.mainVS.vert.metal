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

struct mainVS_out
{
    float2 fragTexCoord [[user(locn0)]];
    float4 v_normal [[user(locn1)]];
    float3 FragPos [[user(locn2)]];
    float3 LightPos [[user(locn3)]];
    float3 v_tangent [[user(locn4)]];
    float3 v_bitangent [[user(locn5)]];
    float4 gl_Position [[position]];
};

struct mainVS_in
{
    float3 vertexPosition [[attribute(0)]];
    float2 texCoord [[attribute(1)]];
    float3 vertexNormal [[attribute(2)]];
};

static inline __attribute__((always_inline))
void mainVS(thread float3& LightPos, thread float4& v_normal, thread float3& vertexNormal, thread float3& FragPos, thread float3& vertexPosition, thread float2& fragTexCoord, thread float2& texCoord, thread float3& v_tangent, thread float3& v_bitangent, thread float4& gl_Position, constant MatrixUBO& matrices, constant LightingUBO& lighting)
{
    LightPos = float3((matrices.v * float4(lighting.lightPos, 1.0)).xyz);
    v_normal = fast::normalize(matrices.n * float4(vertexNormal, 1.0));
    FragPos = float3((matrices.mv * float4(vertexPosition, 1.0)).xyz);
    fragTexCoord = texCoord;
    float3 normal = fast::normalize(vertexNormal);
    float3 tangent;
    float3 bitangent;
    if (abs(normal.x) > 0.89999997615814208984375)
    {
        tangent = float3(0.0, 0.0, sign(normal.x));
        bitangent = float3(0.0, 1.0, 0.0);
    }
    else
    {
        if (abs(normal.y) > 0.89999997615814208984375)
        {
            tangent = float3(1.0, 0.0, 0.0);
            bitangent = float3(0.0, 0.0, -sign(normal.y));
        }
        else
        {
            tangent = float3(-sign(normal.z), 0.0, 0.0);
            bitangent = float3(0.0, 1.0, 0.0);
        }
    }
    v_tangent = float3((matrices.n * float4(tangent, 0.0)).xyz);
    v_bitangent = float3((matrices.n * float4(bitangent, 0.0)).xyz);
    gl_Position = matrices.mvp * float4(vertexPosition, 1.0);
}

vertex mainVS_out mainVS(mainVS_in in [[stage_in]], constant MatrixUBO& matrices [[buffer(1)]], constant LightingUBO& lighting [[buffer(2)]])
{
    mainVS_out out = {};
    mainVS(out.LightPos, out.v_normal, in.vertexNormal, out.FragPos, in.vertexPosition, out.fragTexCoord, in.texCoord, out.v_tangent, out.v_bitangent, out.gl_Position, matrices, lighting);
    return out;
}

