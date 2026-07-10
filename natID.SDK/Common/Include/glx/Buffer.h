// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Buffer.h
    @brief Provides the Buffer class representing a GPU-accessible memory buffer. */
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

/// @brief Represents a GPU-accessible memory buffer for storing vertex, index, or general-purpose data.
class NATGLX_API Buffer
{
    friend class Device;
    friend class RenderPassDescriptor;
	friend class RenderCommandEncoder;
	friend class BlitCommandEncoder;
	friend class ComputeCommandEncoder;

public:
    /// @brief Describes a contiguous range within a buffer by position and length.
    struct Range
    {
        size_t position = 0; ///< Byte offset of the start of the range.
        size_t len = 0;      ///< Number of bytes in the range.

        /// @brief Constructs a Range with the given position and length.
        /// @param pos Byte offset of the range start.
        /// @param lenIn Number of bytes in the range.
        Range(size_t pos, size_t lenIn)
        : position(pos)
        , len (lenIn)
        {
        }

#ifdef MU_MACOS
        /// @brief Converts this Range to an NS::Range for use with macOS APIs.
        /// @return The equivalent NS::Range value.
        operator NS::Range() const;
#endif


    };

private:
    MTL::Buffer* _pBuffer; ///< Pointer to the underlying Metal buffer.

protected:
    /// @brief Constructs a Buffer wrapping an existing Metal buffer pointer.
    /// @param pBuffer Pointer to the native MTL::Buffer.
    Buffer(MTL::Buffer* pBuffer);

public:
    /// @brief Default constructor. Creates an empty, uninitialized Buffer.
    Buffer();
    /// @brief Copy constructor. Shares the underlying Metal buffer resource.
    /// @param buffer The Buffer to copy from.
    Buffer(const Buffer& buffer);
    /// @brief Copy assignment operator. Shares the underlying Metal buffer resource.
    /// @param buffer The Buffer to assign from.
    void operator = (const Buffer& buffer);
    /// @brief Destructor. Releases the reference to the Metal buffer.
    ~Buffer();

    /// @brief Returns the total size in bytes of this buffer.
    /// @return The buffer length in bytes.
    size_t length() const;

    /// @brief Returns a CPU-accessible pointer to the buffer's contents.
    /// @return Void pointer to the start of the buffer data.
    void* contents();

    /// @brief Notifies the system that the CPU has modified the specified range of the buffer.
    /// @param range The byte range that was modified.
    void  didModifyRange(const Range& range);

//    class Texture* newTexture(const class TextureDescriptor* descriptor, NS::UInteger offset, NS::UInteger bytesPerRow);

    /// @brief Adds a debug marker string associated with a byte range in the buffer.
    /// @param marker The debug marker string to attach.
    /// @param range The byte range in the buffer the marker applies to.
    void addDebugMarker(const char* marker, const Range& range);

    /// @brief Removes all debug markers from this buffer.
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

    /// @brief Checks whether the buffer is valid and successfully allocated.
    /// @return True if the buffer is valid, false otherwise.
    bool isOk() const;
    /// @brief Releases the underlying native buffer resource.
    void release();

    /// @brief Returns the minimum required alignment for storage buffer offsets on this device.
    /// @return Minimum storage buffer offset alignment in bytes.
    static size_t minStorageBufferOffsetAlignment();

    /// @brief Returns the minimum required alignment for uniform buffer offsets on this device.
    /// @return Minimum uniform buffer offset alignment in bytes.
    static size_t minUniformBufferOffsetAlignment();

    /// @brief Returns the optimal alignment for buffer copy row pitches on this device.
    /// @return Optimal buffer copy row pitch alignment in bytes.
    static size_t optimalBufferCopyRowPitchAlignment();

private:

    /// @brief Returns a mutable pointer to the underlying Metal buffer.
    /// @return Pointer to the native MTL::Buffer.
    MTL::Buffer* getNatPtr()
    {
        return _pBuffer;
    }

    /// @brief Returns a const pointer to the underlying Metal buffer.
    /// @return Const pointer to the native MTL::Buffer.
    const MTL::Buffer* getNatPtr() const
    {
        return _pBuffer;
    }
};


} //namespace glx

