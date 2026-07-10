// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IBufferReader.h
    @brief Abstract interface for sequential chunk-based buffer reading. */
//
//  Buffer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/Types.h>
#include <tuple>

namespace mem
{
/// @brief Abstract base class for objects that expose buffer data as a sequence of raw memory chunks.
class IBufferReader
{
public:
    /// @brief Returns the next available chunk of data.
    /// @return Tuple of (pointer to the chunk data, number of valid bytes in the chunk).
    ///         Returns (nullptr, 0) when no more data is available.
    virtual std::tuple<char*, td::UINT4> getDataChunk() = 0;
};

} //namespace no
