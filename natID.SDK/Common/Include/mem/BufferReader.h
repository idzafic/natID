// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BufferReader.h
    @brief Concrete IBufferReader implementation that reads data chunk-by-chunk from a mem::Buffer. */
//
//  Buffer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <mem/IBufferReader.h>
#include <mem/Buffer.h>

namespace mem
{
/// @brief Wraps a mem::Buffer and exposes it through the IBufferReader interface for sequential chunk-based reading.
class BufferReader : public IBufferReader
{
    mem::Buffer* _pBuffer;               ///< Pointer to the underlying buffer being read.
    mem::Buffer::Reader _buffIterator;   ///< Iterator maintaining the current read position within the buffer.
public:
    /// @brief Constructs a BufferReader positioned at the start of the given buffer.
    /// @param pBuffer Pointer to the buffer to read from; must remain valid for the lifetime of this reader.
    BufferReader(mem::Buffer* pBuffer)
    : _pBuffer(pBuffer)
    , _buffIterator(pBuffer)
    {
    }

    /// @brief Returns the next available data chunk and advances the internal position.
    /// @return Tuple of (pointer to chunk data, number of bytes in the chunk),
    ///         or (nullptr, 0) when all chunks have been consumed.
    virtual std::tuple<char*, td::UINT4> getDataChunk()
    {
        if (_buffIterator.isOK())
        {
            auto [pChunk, size] = _buffIterator.getDataChunk();
            ++_buffIterator;
            return {pChunk, size};
        }
        return {nullptr, 0};
    }
};

} //namespace no
