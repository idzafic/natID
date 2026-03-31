#pragma clang diagnostic ignored "-Wmissing-prototypes"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct OutputBuffer
{
    float data[1];
};

struct InputBuffer
{
    float data[1];
};

struct ComputeParams
{
    float multiplier;
    float padding1;
    float padding2;
    float padding3;
};

constant uint3 gl_WorkGroupSize [[maybe_unused]] = uint3(32u, 1u, 1u);

static inline __attribute__((always_inline))
void mainComp(thread uint3& gl_GlobalInvocationID, device OutputBuffer& outputBuffer, device InputBuffer& inputBuffer, constant ComputeParams& params)
{
    uint idx = gl_GlobalInvocationID.x;
    outputBuffer.data[idx] = inputBuffer.data[idx] * params.multiplier;
}

kernel void mainComp(device InputBuffer& inputBuffer [[buffer(0)]], device OutputBuffer& outputBuffer [[buffer(1)]], constant ComputeParams& params [[buffer(2)]], uint3 gl_GlobalInvocationID [[thread_position_in_grid]])
{
    mainComp(gl_GlobalInvocationID, outputBuffer, inputBuffer, params);
}

