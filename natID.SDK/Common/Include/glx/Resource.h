// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Resource.h @brief Base class representing a Metal GPU resource with storage, caching, and hazard-tracking options. */
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

/// @brief Base wrapper for Metal GPU resources (buffers, textures, heaps).
///        Exposes storage-mode, CPU-cache-mode, and hazard-tracking configuration
///        as well as purgeable-state management.
class NATGLX_API Resource
{
public:
    /// @brief Combined bit-field of CPU cache mode, storage mode, and hazard tracking mode flags.
    enum class Options : uint32_t
    {
        // CPU cache modes
        CPUCacheModeDefaultCache     = 0x0,  ///< Default CPU cache mode (write-back on Apple silicon, write-combined on others).
        CPUCacheModeWriteCombined    = 0x1,  ///< Write-combined CPU cache mode; faster writes, slower reads.
        OptionCPUCacheModeDefault    = CPUCacheModeDefaultCache,          ///< Alias for CPUCacheModeDefaultCache.
        OptionCPUCacheModeWriteCombined = CPUCacheModeWriteCombined,      ///< Alias for CPUCacheModeWriteCombined.

        // Storage modes (<< 4)
        StorageModeShared      = (0 << 4), ///< Resource accessible by both CPU and GPU.
        StorageModeManaged     = (1 << 4), ///< Resource with CPU and GPU copies synchronized explicitly.
        StorageModePrivate     = (2 << 4), ///< Resource accessible only by the GPU.
        StorageModeMemoryless  = (3 << 4), ///< Resource exists only in tile memory; no backing store.

        // Hazard tracking modes (<< 8)
        HazardTrackingModeDefault   = (0 << 8), ///< Use the device's default hazard tracking.
        HazardTrackingModeUntracked = (1 << 8), ///< Disable automatic hazard tracking.
        HazardTrackingModeTracked   = (2 << 8), ///< Enable explicit hazard tracking.
    };

    /// @brief CPU-side memory access pattern hint for the resource.
    enum class CPUCacheMode : td::BYTE { DefaultCache = 0, WriteCombined = 1 };

    /// @brief Specifies where and how the resource is stored.
    enum class StorageMode : td::BYTE { Shared = 0, Managed = 1, Private = 2, Memoryless = 3 };

    /// @brief Controls whether Metal tracks hazards automatically for this resource.
    enum class HazardTrackingMode : td::BYTE {Default = 0, Untracked = 1, Tracked = 2};

    /// @brief Controls whether the resource's contents may be discarded under memory pressure.
    enum class PurgeableState : td::BYTE { KeepCurrent = 1, NonVolatile = 2, Volatile = 3, Empty = 4};
private:
    MTL::Resource* _pResource = nullptr; ///< Pointer to the underlying native Metal resource.
public:
    /// @brief Returns the debug label associated with this resource.
    /// @return The label string.
    td::String label() const;

    /// @brief Sets a debug label on this resource for GPU profiling tools.
    /// @param label The label string to assign.
    void setLabel(const td::String& label);

    /// @brief Returns a pointer to the device that created this resource.
    /// @return Pointer to the owning Device.
    Device* device() const;

    /// @brief Returns the CPU cache mode of this resource.
    /// @return The CPUCacheMode enum value.
    CPUCacheMode cpuCacheMode() const;

    /// @brief Returns the storage mode of this resource.
    /// @return The StorageMode enum value.
    StorageMode storageMode() const;

    /// @brief Returns the hazard tracking mode of this resource.
    /// @return The HazardTrackingMode enum value.
    HazardTrackingMode hazardTrackingMode() const;

    /// @brief Returns the combined resource options bit-field used when the resource was created.
    /// @return The Options bit-field.
    Options resourceOptions() const;

    /// @brief Sets the purgeable state of the resource and returns the previous state.
    /// @param state The desired PurgeableState.
    /// @return The previous PurgeableState before the update.
    PurgeableState setPurgeableState(PurgeableState state);

//    class Heap*             heap() const;

    /// @brief Returns the byte offset of this resource within its parent heap.
    /// @return Byte offset within the heap, or 0 if not sub-allocated from a heap.
    size_t heapOffset() const;

    /// @brief Returns the total number of bytes allocated for this resource.
    /// @return Allocated size in bytes.
    size_t allocatedSize() const;

    /// @brief Marks this resource as aliasable so the heap allocator may overlap it with other resources.
    void makeAliasable();

    /// @brief Returns whether this resource has been made aliasable.
    /// @return True if the resource is aliasable.
    bool isAliasable();
};

// Bitwise operators for Options flags
/// @brief Bitwise OR of two Options flags.
inline constexpr Resource::Options operator|(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
/// @brief Bitwise AND of two Options flags.
inline constexpr Resource::Options operator&(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
/// @brief Bitwise XOR of two Options flags.
inline constexpr Resource::Options operator^(Resource::Options a, Resource::Options b) noexcept
{
    return static_cast<Resource::Options>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
}
/// @brief Bitwise NOT of an Options flags value.
inline constexpr Resource::Options operator~(Resource::Options a) noexcept
{
    return static_cast<Resource::Options>(~static_cast<uint32_t>(a));
}
/// @brief Bitwise OR-assignment of two Options flags.
inline Resource::Options& operator|=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a | b; return a;
}
/// @brief Bitwise AND-assignment of two Options flags.
inline Resource::Options& operator&=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a & b; return a;
}
/// @brief Bitwise XOR-assignment of two Options flags.
inline Resource::Options& operator^=(Resource::Options& a, Resource::Options b) noexcept
{
    a = a ^ b; return a;
}

} //namespace glx
