#pragma once

#include <glx/Device.h>
#include <glx/Buffer.h>
#include <glx/RenderCommandEncoder.h>
#include <cstring>
#include <vector>
#include <mu/DebugConsoleLog.h>

template<typename VertexType, typename InstanceDataType>
struct InstancedObject 
{
    static constexpr size_t INITIAL_BUFFER_SIZE = 100000;
    
    static glx::Device* s_device;
    static glx::Buffer s_buffer;
    static size_t s_capacity;
    static uint32_t s_offset;
    static bool s_buffersInitialized;
    
    // Helper to align sizes to 4-byte boundary
    static inline size_t align4(size_t size) {
        return (size + 3) & ~3;
    }
    
    uint32_t _vertexOffset;
    uint32_t _vertexCount;
    uint32_t _indexOffset;
    uint32_t _indexCount;
    uint32_t _instanceDataOffset;
    uint32_t _maxInstances;
    uint32_t _activeInstances;
    
    static void initializeBuffers(glx::Device& device);
    static void releaseBuffers();

    InstancedObject(const VertexType* verts, size_t vertCount, const uint16_t* inds, size_t indCount, uint32_t maxInstances = 50);
    InstancedObject(const std::vector<VertexType> &verts, const std::vector<uint16_t> &inds, uint32_t maxInstances = 50);

    void addInstance(const InstanceDataType& instance);
    void updateInstance(uint32_t instanceIndex, const InstanceDataType& instance);
    void draw(glx::RenderCommandEncoder& cmdEnc, uint32_t vertexBufferIndex, uint32_t instanceBufferIndex, glx::PrimitiveType primitiveType = glx::PrimitiveType::Triangle);
};

// Static member definitions
template<typename VertexType, typename InstanceDataType>
glx::Device* InstancedObject<VertexType, InstanceDataType>::s_device = nullptr;

template<typename VertexType, typename InstanceDataType>
glx::Buffer InstancedObject<VertexType, InstanceDataType>::s_buffer;

template<typename VertexType, typename InstanceDataType>
size_t InstancedObject<VertexType, InstanceDataType>::s_capacity = 0;

template<typename VertexType, typename InstanceDataType>
uint32_t InstancedObject<VertexType, InstanceDataType>::s_offset = 0;

template<typename VertexType, typename InstanceDataType>
bool InstancedObject<VertexType, InstanceDataType>::s_buffersInitialized = false;

// Implementation
template<typename VertexType, typename InstanceDataType>
void InstancedObject<VertexType, InstanceDataType>::initializeBuffers(glx::Device& device)
{
    if (s_buffersInitialized)
        return;

    s_device = &device;
    s_buffer = device.newBuffer(INITIAL_BUFFER_SIZE, glx::Resource::Options::StorageModeManaged);
    s_capacity = INITIAL_BUFFER_SIZE;
    s_offset = 0;
    s_buffersInitialized = true;
    mu::DebugConsoleLog::info() << "InstancedObject unified buffer initialized (" << s_capacity << " bytes)";
}

template<typename VertexType, typename InstanceDataType>
void InstancedObject<VertexType, InstanceDataType>::releaseBuffers()
{
    if (s_buffersInitialized)
    {
        s_buffer.release();
        s_device = nullptr;
        s_buffersInitialized = false;
    }
}

template<typename VertexType, typename InstanceDataType>
InstancedObject<VertexType, InstanceDataType>::InstancedObject(const VertexType* verts, size_t vertCount, const uint16_t* inds, size_t indCount, uint32_t maxInstances)
: _vertexOffset(0)
, _vertexCount(0)
, _indexOffset(0)
, _indexCount(0)
, _instanceDataOffset(0)
, _maxInstances(maxInstances)
, _activeInstances(0)
{
    if (!s_buffersInitialized)
    {
        mu::DebugConsoleLog::error() << "InstancedObject buffer not initialized";
        return;
    }
    
    _vertexCount = static_cast<uint32_t>(vertCount);
    _indexCount = static_cast<uint32_t>(indCount);
    
    size_t vertexDataSize = align4(_vertexCount * sizeof(VertexType));
    size_t indexDataSize = align4(_indexCount * sizeof(uint16_t));
    size_t instanceDataSize = align4(_maxInstances * sizeof(InstanceDataType));
    size_t requiredSize = vertexDataSize + indexDataSize + instanceDataSize;
    
    if (s_offset + requiredSize > s_capacity)
    {
        mu::DebugConsoleLog::error() << "Buffer full. Cannot add object requiring " << requiredSize << " bytes (offset: " << s_offset << ", capacity: " << s_capacity << ")";
        return;
    }
    
    _vertexOffset = s_offset;
    
    uint8_t* bufferData = static_cast<uint8_t*>(s_buffer.contents());
    memcpy(bufferData + _vertexOffset, verts, vertexDataSize);
    
    glx::Buffer::Range vertexRange(_vertexOffset, vertexDataSize);
    s_buffer.didModifyRange(vertexRange);
    
    s_offset += vertexDataSize;
    
    _indexOffset = s_offset;
    
    memcpy(bufferData + _indexOffset, inds, indexDataSize);
    
    glx::Buffer::Range indexRange(_indexOffset, indexDataSize);
    s_buffer.didModifyRange(indexRange);
    
    s_offset += indexDataSize;
    
    _instanceDataOffset = s_offset;
    s_offset += instanceDataSize;
    
    mu::DebugConsoleLog::debug() << "InstancedObject created: " << _vertexCount << " vertices at byte offset " << _vertexOffset << ", " << _indexCount << " indices at byte offset " << _indexOffset;
}

template<typename VertexType, typename InstanceDataType>
InstancedObject<VertexType, InstanceDataType>::InstancedObject(const std::vector<VertexType> &verts, const std::vector<uint16_t> &inds, uint32_t maxInstances)
    : InstancedObject(verts.data(), verts.size(), inds.data(), inds.size(), maxInstances)
{
}

template<typename VertexType, typename InstanceDataType>
void InstancedObject<VertexType, InstanceDataType>::addInstance(const InstanceDataType& instance)
{
    if (_activeInstances >= _maxInstances)
    {
        mu::DebugConsoleLog::warning() << "Max instances (" << _maxInstances << ") reached for this object";
        return;
    }
    
    size_t instanceDataSize = sizeof(InstanceDataType);
    uint32_t instanceOffset = _instanceDataOffset + (_activeInstances * instanceDataSize);
    
    uint8_t* bufferData = static_cast<uint8_t*>(s_buffer.contents());
    memcpy(bufferData + instanceOffset, &instance, instanceDataSize);
    
    glx::Buffer::Range instanceRange(instanceOffset, instanceDataSize);
    s_buffer.didModifyRange(instanceRange);
    
    _activeInstances++;
}

template<typename VertexType, typename InstanceDataType>
void InstancedObject<VertexType, InstanceDataType>::updateInstance(uint32_t instanceIndex, const InstanceDataType& instance)
{
    if (instanceIndex >= _activeInstances)
    {
        mu::DebugConsoleLog::warning() << "Instance index " << instanceIndex << " out of range (active instances: " << _activeInstances << ")";
        return;
    }
    
    size_t instanceDataSize = sizeof(InstanceDataType);
    uint32_t instanceOffset = _instanceDataOffset + (instanceIndex * instanceDataSize);
    
    uint8_t* bufferData = static_cast<uint8_t*>(s_buffer.contents());
    memcpy(bufferData + instanceOffset, &instance, instanceDataSize);
    
    glx::Buffer::Range instanceRange(instanceOffset, instanceDataSize);
    s_buffer.didModifyRange(instanceRange);
}

template<typename VertexType, typename InstanceDataType>
void InstancedObject<VertexType, InstanceDataType>::draw(glx::RenderCommandEncoder& cmdEnc, uint32_t vertexBufferIndex, uint32_t instanceBufferIndex, glx::PrimitiveType primitiveType)
{
    if (_activeInstances == 0)
        return;
    
    cmdEnc.setVertexBuffer(s_buffer, _vertexOffset, vertexBufferIndex);
    cmdEnc.setVertexBuffer(s_buffer, _instanceDataOffset, instanceBufferIndex);
    
    cmdEnc.drawIndexedPrimitives(
        primitiveType,
        _indexCount,
        glx::IndexType::UInt16,
        s_buffer,
        _indexOffset,
        _activeInstances
    );
}



