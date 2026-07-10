// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BlitCommandEncoder.h
    @brief Provides the BlitCommandEncoder class for GPU blit and copy operations. */
#pragma once
#include "natGLX.h"
#include "Buffer.h"
#include "Texture.h"
#include "Region.h"
#include "Device.h"
#include <td/String.h>

namespace MTL
{
class BlitCommandEncoder;
} //namespace MTL

namespace glx
{

class CommandBuffer;

/// @brief Encodes blit (memory copy and fill) commands for submission to the GPU.
class NATGLX_API BlitCommandEncoder
{
    friend class CommandBuffer;
private:
    MTL::BlitCommandEncoder* _pBlitCommandEncoder = nullptr; ///< Pointer to the underlying Metal blit command encoder.
    /// @brief Constructs a BlitCommandEncoder wrapping an existing Metal encoder.
    /// @param pBlitCommandEncoder Pointer to the native Metal blit command encoder.
    BlitCommandEncoder(MTL::BlitCommandEncoder* pBlitCommandEncoder);

public:
    /// @brief Default constructor. Creates an empty BlitCommandEncoder.
    BlitCommandEncoder();
    /// @brief Copy-assigns another BlitCommandEncoder to this one.
    /// @param blitCommandEncoder The source BlitCommandEncoder to copy from.
    /// @return Reference to this BlitCommandEncoder.
    BlitCommandEncoder& operator = (const BlitCommandEncoder& blitCommandEncoder);

    // Buffer-to-buffer copy operations
    /// @brief Copies data from one buffer to another.
    /// @param sourceBuffer The source buffer to copy from.
    /// @param sourceOffset Byte offset into the source buffer.
    /// @param destinationBuffer The destination buffer to copy into.
    /// @param destinationOffset Byte offset into the destination buffer.
    /// @param size Number of bytes to copy.
    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset,
                       const Buffer& destinationBuffer, size_t destinationOffset, size_t size);

    // Buffer fill operations
    /// @brief Fills a range of a buffer with a constant byte value.
    /// @param buffer The buffer to fill.
    /// @param range The range within the buffer to fill.
    /// @param value The byte value to fill the range with.
    void fillBuffer(const Buffer& buffer, const Buffer::Range& range, uint8_t value);

    // Buffer-to-texture copy operations (essential for texture uploads)
    /// @brief Copies pixel data from a buffer into a texture region.
    /// @param sourceBuffer The source buffer containing pixel data.
    /// @param sourceOffset Byte offset into the source buffer.
    /// @param sourceBytesPerRow Number of bytes per row in the source buffer.
    /// @param destinationTexture The destination texture.
    /// @param destinationRegion The region in the texture to write into.
    /// @param destinationLevel Mipmap level of the destination texture (default 0).
    /// @param destinationSlice Array slice of the destination texture (default 0).
    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset, size_t sourceBytesPerRow,
                       const Texture& destinationTexture, const Region& destinationRegion,
                       uint32_t destinationLevel = 0, uint32_t destinationSlice = 0);

    /// @brief Copies pixel data from a buffer into a texture region with explicit image stride.
    /// @param sourceBuffer The source buffer containing pixel data.
    /// @param sourceOffset Byte offset into the source buffer.
    /// @param sourceBytesPerRow Number of bytes per row in the source buffer.
    /// @param sourceBytesPerImage Number of bytes per image slice in the source buffer.
    /// @param destinationTexture The destination texture.
    /// @param destinationRegion The region in the texture to write into.
    /// @param destinationLevel Mipmap level of the destination texture (default 0).
    /// @param destinationSlice Array slice of the destination texture (default 0).
    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset, size_t sourceBytesPerRow, size_t sourceBytesPerImage,
                       const Texture& destinationTexture, const Region& destinationRegion,
                       uint32_t destinationLevel = 0, uint32_t destinationSlice = 0);

    // Texture-to-buffer copy operations (for readbacks)
    /// @brief Copies pixel data from a texture region into a buffer.
    /// @param sourceTexture The source texture to read from.
    /// @param sourceRegion The region within the source texture to copy.
    /// @param sourceLevel The mipmap level of the source texture.
    /// @param destinationBuffer The destination buffer to write pixel data into.
    /// @param destinationOffset Byte offset into the destination buffer.
    /// @param destinationBytesPerRow Number of bytes per row in the destination buffer.
    void copyFromTexture(const Texture& sourceTexture, const Region& sourceRegion, uint32_t sourceLevel,
                        const Buffer& destinationBuffer, size_t destinationOffset, size_t destinationBytesPerRow);

    /// @brief Copies pixel data from a texture region and slice into a buffer with explicit image stride.
    /// @param sourceTexture The source texture to read from.
    /// @param sourceRegion The region within the source texture to copy.
    /// @param sourceLevel The mipmap level of the source texture.
    /// @param sourceSlice The array slice of the source texture.
    /// @param destinationBuffer The destination buffer to write pixel data into.
    /// @param destinationOffset Byte offset into the destination buffer.
    /// @param destinationBytesPerRow Number of bytes per row in the destination buffer.
    /// @param destinationBytesPerImage Number of bytes per image slice in the destination buffer.
    void copyFromTexture(const Texture& sourceTexture, const Region& sourceRegion, uint32_t sourceLevel, uint32_t sourceSlice,
                        const Buffer& destinationBuffer, size_t destinationOffset, size_t destinationBytesPerRow, size_t destinationBytesPerImage);


    /// @brief Generates all mipmap levels for the given texture.
    /// @param texture The texture for which to generate mipmaps.
    void generateMipmaps(const Texture& texture);

    // Resource synchronization
    /// @brief Synchronizes a buffer resource between CPU and GPU.
    /// @param resource Pointer to the buffer to synchronize.
    void synchronizeResource(Buffer* resource);
    /// @brief Synchronizes a texture resource between CPU and GPU.
    /// @param resource Pointer to the texture to synchronize.
    void synchronizeResource(Texture* resource);

    // Command encoder interface
    /// @brief Returns the device associated with this encoder.
    /// @return The Device used to create this encoder.
    Device device() const;
    /// @brief Returns the debug label of this encoder.
    /// @return The label string.
    td::String label() const;
    /// @brief Sets the debug label of this encoder.
    /// @param label The label string to assign.
    void setLabel(const td::String& label);
    /// @brief Finalizes the encoding of blit commands.
    void endEncoding();
    /// @brief Inserts a debug signpost marker into the command stream.
    /// @param string The signpost label text.
    void insertDebugSignpost(const char* string);
    /// @brief Pushes a debug group onto the encoder's debug group stack.
    /// @param string The name of the debug group.
    void pushDebugGroup(const char* string);
    /// @brief Pops the current debug group from the encoder's debug group stack.
    void popDebugGroup();
    /// @brief Releases the underlying native resource.
    void release();

private:
    /// @brief Returns a mutable pointer to the underlying Metal encoder.
    /// @return Pointer to the native MTL::BlitCommandEncoder.
    MTL::BlitCommandEncoder* getNatPtr()
    {
        return _pBlitCommandEncoder;
    }

    /// @brief Returns a const pointer to the underlying Metal encoder.
    /// @return Const pointer to the native MTL::BlitCommandEncoder.
    const MTL::BlitCommandEncoder* getNatPtr() const
    {
        return _pBlitCommandEncoder;
    }
};


} //namespace glx
