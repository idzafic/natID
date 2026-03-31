// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <td/String.h>

namespace MTL
{
class Buffer;
} //namespace MTL

#ifdef MU_MACOS
namespace NS
{
    class Range;
}
#endif

namespace glx
{

class Device;
class RenderPassDescriptor;
class RenderCommandEncoder;
class BlitCommandEncoder;
class ComputeCommandEncoder;
//class Pipeline;
//class Triang2D;

class NATGLX_API Buffer
{
    friend class Device;
    friend class RenderPassDescriptor;
	friend class RenderCommandEncoder;
	friend class BlitCommandEncoder;
	friend class ComputeCommandEncoder;

public:
    struct Range
    {
        size_t position = 0;
        size_t len = 0;

        Range(size_t pos, size_t lenIn)
        : position(pos)
        , len (lenIn)
        {
        }
        
#ifdef MU_MACOS
        operator NS::Range() const;
#endif
        

    };
    
private:
    MTL::Buffer* _pBuffer;

protected:
    Buffer(MTL::Buffer* pBuffer);

public:
    Buffer();
    Buffer(const Buffer& buffer);
    void operator = (const Buffer& buffer);
    ~Buffer();
    
    size_t length() const;

    void* contents();

    void  didModifyRange(const Range& range);

//    class Texture* newTexture(const class TextureDescriptor* descriptor, NS::UInteger offset, NS::UInteger bytesPerRow);

    void addDebugMarker(const char* marker, const Range& range);

    void removeAllDebugMarkers();

//    Buffer* remoteStorageBuffer() const;
//
//    Buffer* newRemoteBufferViewForDevice(const Device& device);
    
    
//    //add and initialize buffer layout, space for tmp buffers, and space for commands of the layout
//    bool init(size_t nRowsToReserve, const std::initializer_list<DataType>& vboLayout);
//    
//    //append data to current layoutID
//    void append(const glx::VertexType* pVertexData, size_t nRows);

//    //transfer data to GPU
//    bool commit();
//    
//    //release from GPU and reset internal pointers
//    void reset();
//    //release all memory allocated by the Buffer
    
    bool isOk() const;
    void release();
    
    static size_t minStorageBufferOffsetAlignment();
    static size_t optimalBufferCopyRowPitchAlignment();

private:

    MTL::Buffer* getNatPtr()
    {
        return _pBuffer;
    }
    
    const MTL::Buffer* getNatPtr() const
    {
        return _pBuffer;
    }
};


} //namespace glx


