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
#include <mem/IBufferReader.h>
#include <mem/Buffer.h>

namespace mem
{
class BufferReader : public IBufferReader
{
    mem::Buffer* _pBuffer;
    mem::Buffer::Reader _buffIterator;
public:
    BufferReader(mem::Buffer* pBuffer)
    : _pBuffer(pBuffer)
    , _buffIterator(pBuffer)
    {
    }
    
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
