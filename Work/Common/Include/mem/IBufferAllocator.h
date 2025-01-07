// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class MAINUTILS_API IBufferAllocator
{
public:
    virtual bool allocate(mem::Buffer::TList& list, mem::Buffer::ListEntry* pPrev, size_t size) = 0;
    virtual bool allocateFromStringArray(mem::Buffer::TList& list, const td::String* pStrings, size_t nStrings) = 0;
    virtual void release(mem::Buffer::TList& list) = 0;
    virtual void releaseFirstEntry(mem::Buffer::TList& list) = 0;
    virtual void releasBeforeEntry(mem::Buffer::TList& list, const mem::Buffer::ListEntry* pEntry) = 0;
    virtual void releasAfterEntry(mem::Buffer::ListEntry* pEntry) = 0;
    virtual td::UINT4 getUnitChunkSize() const = 0;
};

} //namespace mem
