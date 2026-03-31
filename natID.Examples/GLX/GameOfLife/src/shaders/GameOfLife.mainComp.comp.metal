#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wmissing-braces"

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

template<typename T, size_t Num>
struct spvUnsafeArray
{
    T elements[Num ? Num : 1];
    
    thread T& operator [] (size_t pos) thread
    {
        return elements[pos];
    }
    constexpr const thread T& operator [] (size_t pos) const thread
    {
        return elements[pos];
    }
    
    device T& operator [] (size_t pos) device
    {
        return elements[pos];
    }
    constexpr const device T& operator [] (size_t pos) const device
    {
        return elements[pos];
    }
    
    constexpr const constant T& operator [] (size_t pos) const constant
    {
        return elements[pos];
    }
    
    threadgroup T& operator [] (size_t pos) threadgroup
    {
        return elements[pos];
    }
    constexpr const threadgroup T& operator [] (size_t pos) const threadgroup
    {
        return elements[pos];
    }
};

struct GameBoardData
{
    uint width;
    uint height;
    uint data[1];
};

constant uint3 gl_WorkGroupSize [[maybe_unused]] = uint3(32u, 32u, 1u);

struct GameBoardData_1
{
    uint width;
    uint height;
    uint data[1];
};

struct ViewportUBO
{
    float4 extentPx;
};

constant spvUnsafeArray<float2, 4> _99 = spvUnsafeArray<float2, 4>({ float2(-1.0), float2(1.0, -1.0), float2(-1.0, 1.0), float2(1.0) });

static inline __attribute__((always_inline))
uint countNeighbors(thread const uint& x, thread const uint& y, device GameBoardData& boardRead)
{
    uint count = 0u;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if ((dx == 0) && (dy == 0))
            {
                continue;
            }
            int nx = int(x) + dx;
            int ny = int(y) + dy;
            bool _141 = nx >= 0;
            bool _149;
            if (_141)
            {
                _149 = nx < int(boardRead.width);
            }
            else
            {
                _149 = _141;
            }
            bool _152 = _149 && (ny >= 0);
            bool _160;
            if (_152)
            {
                _160 = ny < int(boardRead.height);
            }
            else
            {
                _160 = _152;
            }
            if (_160)
            {
                uint neighborIdx = (uint(ny) * boardRead.width) + uint(nx);
                if (boardRead.data[neighborIdx] != 0u)
                {
                    count++;
                }
            }
        }
    }
    return count;
}

static inline __attribute__((always_inline))
void mainComp(thread uint3& gl_GlobalInvocationID, device GameBoardData& boardRead, device GameBoardData& boardWrite)
{
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;
    bool _192 = x < boardRead.width;
    bool _199;
    if (_192)
    {
        _199 = y < boardRead.height;
    }
    else
    {
        _199 = _192;
    }
    if (_199)
    {
        uint idx = (y * boardRead.width) + x;
        uint param = x;
        uint param_1 = y;
        uint neighbors = countNeighbors(param, param_1, boardRead);
        uint currentCell = boardRead.data[idx];
        uint nextState = 0u;
        if (currentCell != 0u)
        {
            if ((neighbors == 2u) || (neighbors == 3u))
            {
                nextState = 1u;
            }
        }
        else
        {
            if (neighbors == 3u)
            {
                nextState = 1u;
            }
        }
        boardWrite.data[idx] = nextState;
    }
}

kernel void mainComp(device GameBoardData& boardRead [[buffer(3)]], device GameBoardData& boardWrite [[buffer(4)]], uint3 gl_GlobalInvocationID [[thread_position_in_grid]])
{
    mainComp(gl_GlobalInvocationID, boardRead, boardWrite);
}

