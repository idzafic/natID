// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IBufferAllocator.h
    @brief Abstract interface for allocators that manage mem::Buffer chunk lists. */
//
//  Buffer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <mem/Buffer.h>
#include <td/String.h>

namespace mem
{

/// @brief Abstract allocator interface responsible for creating, releasing, and managing the chunk list of a mem::Buffer.
class MAINUTILS_API IBufferAllocator
{
public:
    /// @brief Allocates a new chunk of at least size bytes and appends it to list.
    /// @param list Chunk list to extend.
    /// @param pPrev Previous list entry after which the new chunk is inserted (may be nullptr).
    /// @param size Minimum number of bytes the new chunk must hold.
    /// @return true on success, false on allocation failure.
    virtual bool allocate(mem::Buffer::TList& list, mem::Buffer::ListEntry* pPrev, size_t size) = 0;

    /// @brief Allocates chunks and populates list from an array of strings.
    /// @param list Chunk list to populate.
    /// @param pStrings Pointer to the first element of the source string array.
    /// @param nStrings Number of strings in the array.
    /// @return true on success, false on allocation failure.
    virtual bool allocateFromStringArray(mem::Buffer::TList& list, const td::String* pStrings, size_t nStrings) = 0;

    /// @brief Releases all chunks in the list back to the allocator.
    /// @param list Chunk list whose entries are to be released.
    virtual void release(mem::Buffer::TList& list) = 0;

    /// @brief Releases only the first chunk entry of the list.
    /// @param list Chunk list from which the first entry is released.
    virtual void releaseFirstEntry(mem::Buffer::TList& list) = 0;

    /// @brief Releases all chunks in the list that appear before pEntry.
    /// @param list Chunk list to trim.
    /// @param pEntry Boundary entry; all entries before it are released.
    virtual void releasBeforeEntry(mem::Buffer::TList& list, const mem::Buffer::ListEntry* pEntry) = 0;

    /// @brief Releases all chunks linked after pEntry.
    /// @param pEntry Starting entry; all entries following it are released.
    virtual void releasAfterEntry(mem::Buffer::ListEntry* pEntry) = 0;

    /// @brief Returns the size in bytes of a single allocation unit (chunk).
    /// @return Number of bytes in one chunk.
    virtual td::UINT4 getUnitChunkSize() const = 0;
};

} //namespace mem
