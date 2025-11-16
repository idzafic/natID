// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <td/Types.h>
#include <td/String.h>

namespace MTL
{
class Resource;
}

namespace glx
{

class Device;

class NATGLX_API Resource
{
public:
    enum class Options : uint32_t
    {
        // CPU cache modes
        CPUCacheModeDefaultCache = 0x0,
        CPUCacheModeWriteCombined = 0x1,
        OptionCPUCacheModeDefault = CPUCacheModeDefaultCache,
        OptionCPUCacheModeWriteCombined = CPUCacheModeWriteCombined,

        // Storage modes (<< 4)
        StorageModeShared = (0 << 4),
        StorageModeManaged = (1 << 4),
        StorageModePrivate = (2 << 4),
        StorageModeMemoryless = (3 << 4),

        // Hazard tracking modes (<< 8)
        HazardTrackingModeDefault = (0 << 8),
        HazardTrackingModeUntracked = (1 << 8),
        HazardTrackingModeTracked = (2 << 8),
    };

    enum class CPUCacheMode : td::BYTE { DefaultCache = 0, WriteCombined = 1 };
    enum class StorageMode : td::BYTE { Shared = 0, Managed = 1, Private = 2, Memoryless = 3 };
    enum class HazardTrackingMode : td::BYTE {Default = 0, Untracked = 1, Tracked = 2};
    enum class PurgeableState : td::BYTE { KeepCurrent = 1, NonVolatile = 2, Volatile = 3, Empty = 4};
private:
    MTL::Resource* _pResource = nullptr;
public:
    td::String label() const;
    void setLabel(const td::String& label);

    Device* device() const;

    CPUCacheMode cpuCacheMode() const;

    StorageMode storageMode() const;

    HazardTrackingMode hazardTrackingMode() const;

    Options resourceOptions() const;

    PurgeableState setPurgeableState(PurgeableState state);

//    class Heap*             heap() const;

    size_t heapOffset() const;

    size_t allocatedSize() const;

    void makeAliasable();

    bool isAliasable();
};

// Bitwise operators for Options flags
inline constexpr Resource::Options operator|(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
inline constexpr Resource::Options operator&(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
inline constexpr Resource::Options operator^(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
}
inline constexpr Resource::Options operator~(Resource::Options a) noexcept
{
    return static_cast<Resource::Options>(~static_cast<uint32_t>(a));
}
inline Resource::Options& operator|=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a | b; return a;
}
inline Resource::Options& operator&=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a & b; return a;
}
inline Resource::Options& operator^=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a ^ b; return a;
}

} //namespace glx

