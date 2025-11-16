// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATGLX_API BlitCommandEncoder
{
    friend class CommandBuffer;
private:
    MTL::BlitCommandEncoder* _pBlitCommandEncoder = nullptr;
    BlitCommandEncoder(MTL::BlitCommandEncoder* pBlitCommandEncoder);
public:
    BlitCommandEncoder();
    void operator = (const BlitCommandEncoder& blitCommandEncoder);

    // Buffer-to-buffer copy operations
    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset,
                       const Buffer& destinationBuffer, size_t destinationOffset, size_t size);

    // Buffer fill operations
    void fillBuffer(const Buffer& buffer, const Buffer::Range& range, uint8_t value);

    // Buffer-to-texture copy operations (essential for texture uploads)
    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset, size_t sourceBytesPerRow,
                       const Texture& destinationTexture, const Region& destinationRegion, 
                       uint32_t destinationLevel = 0, uint32_t destinationSlice = 0);

    void copyFromBuffer(const Buffer& sourceBuffer, size_t sourceOffset, size_t sourceBytesPerRow, size_t sourceBytesPerImage,
                       const Texture& destinationTexture, const Region& destinationRegion, 
                       uint32_t destinationLevel = 0, uint32_t destinationSlice = 0);

    // Texture-to-buffer copy operations (for readbacks)
    void copyFromTexture(const Texture& sourceTexture, const Region& sourceRegion, uint32_t sourceLevel,
                        const Buffer& destinationBuffer, size_t destinationOffset, size_t destinationBytesPerRow);

    void copyFromTexture(const Texture& sourceTexture, const Region& sourceRegion, uint32_t sourceLevel, uint32_t sourceSlice,
                        const Buffer& destinationBuffer, size_t destinationOffset, size_t destinationBytesPerRow, size_t destinationBytesPerImage);


    void generateMipmaps(const Texture& texture);

    // Resource synchronization
    void synchronizeResource(Buffer* resource);
    void synchronizeResource(Texture* resource);

    // Command encoder interface
    Device device() const;
    td::String label() const;
    void setLabel(const char* label);
    void endEncoding();
    void insertDebugSignpost(const char* string);
    void pushDebugGroup(const char* string);
    void popDebugGroup();
    void release();

    MTL::BlitCommandEncoder* getNatPtr()
    {
        return _pBlitCommandEncoder;
    }
    
    const MTL::BlitCommandEncoder* getNatPtr() const
    {
        return _pBlitCommandEncoder;
    }
};


} //namespace glx

