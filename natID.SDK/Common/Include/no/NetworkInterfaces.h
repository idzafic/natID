// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file NetworkInterfaces.h
 * @brief Collection of NetworkInterface objects with lookup and address-management helpers.
 */
//
//  NetworkInterfaces.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/NetworkInterface.h>
#include <cnt/SafeFullVector.h>
#include <mem/CntFixedSizeMemoryPool.h>
#include <no/netOpLib.h>

namespace no
{

/// @brief Manages a collection of NetworkInterface objects and associated IP-address memory pools.
class NETOP_API NetworkInterfaces
{
public:
    typedef mem::CntFixedSizeMemoryPool<NetworkInterface::TIP4List> TMemPool4; ///< Memory pool type for IPv4 address lists.
    //NETOP_TEMPLATE class NETOP_API TMemPool4;
    typedef mem::CntFixedSizeMemoryPool<NetworkInterface::TIP6List> TMemPool6; ///< Memory pool type for IPv6 address lists.
    //NETOP_TEMPLATE class NETOP_API TMemPool6;

private:
	cnt::SafeFullVector<NetworkInterface> _nifs; ///< Resizable vector of all discovered network interfaces.
    //mem::CntFixedSizeMemoryPool< cnt::List<no::IP4Address, true> > _memPoolIP4;
    //mem::CntFixedSizeMemoryPool< cnt::List<no::IP6Address, true> > _memPoolIP6;
    TMemPool4 _memPoolIP4; ///< Memory pool backing all IPv4 address lists.
    TMemPool6 _memPoolIP6; ///< Memory pool backing all IPv6 address lists.

public:
    /// @brief Default constructor; creates an empty collection.
    NetworkInterfaces();

    /// @brief Finds a network interface by its C-string name.
    /// @param nifName Null-terminated name of the interface to find.
    /// @return Pointer to the matching NetworkInterface, or nullptr if not found.
    NetworkInterface* findNIF(const char* nifName);

    /// @brief Finds a network interface by its td::String name.
    /// @param nifName Name of the interface to find.
    /// @return Pointer to the matching NetworkInterface, or nullptr if not found.
    NetworkInterface* findNIF(const td::String& nifName);

    /// @brief Finds the interface that owns the given IP address (const version).
    /// @param ipAddress IP address string to search for.
    /// @return Const pointer to the matching NetworkInterface, or nullptr if not found.
    const NetworkInterface* findNIFWithIPAddress(const td::String& ipAddress) const;

    /// @brief Returns a const iterator to the first network interface.
    /// @return Const begin iterator.
    auto cbegin() const
    {
        return _nifs.cbegin();
    }

    /// @brief Returns a mutable iterator to the first network interface.
    /// @return Mutable begin iterator.
    auto begin()
    {
        return _nifs.begin();
    }

    /// @brief Returns a const past-the-end iterator.
    /// @return Const end iterator.
    auto cend() const
    {
        return _nifs.cend();
    }

    /// @brief Returns a mutable past-the-end iterator.
    /// @return Mutable end iterator.
    auto end()
    {
        return _nifs.end();
    }

    /// @brief Appends an IPv4 address to the specified network interface.
    /// @param ip4Address Pointer to the IPv4 address to add.
    /// @param toNIF Pointer to the target NetworkInterface.
    void addAddress(const no::IP4Address* ip4Address, no::NetworkInterface* toNIF);

    /// @brief Appends an IPv6 address to the specified network interface.
    /// @param ip6Address Pointer to the IPv6 address to add.
    /// @param toNIF Pointer to the target NetworkInterface.
    void addAddress(const no::IP6Address* ip6Address, no::NetworkInterface* toNIF);

    /// @brief Returns a const reference to the underlying vector of interfaces.
    /// @return Const reference to the SafeFullVector of NetworkInterface objects.
    const cnt::SafeFullVector<NetworkInterface>& getNIFs();

    /// @brief Sets the name of the interface at the given index and returns a pointer to it.
    /// @param iPos Zero-based index of the interface to rename.
    /// @param nifName Null-terminated new name string.
    /// @return Pointer to the renamed NetworkInterface.
    NetworkInterface* setNIFName(size_t iPos, const char* nifName);

    /// @brief Returns the number of network interfaces in the collection.
    /// @return Count of stored NetworkInterface objects.
    size_t size() const;

    /// @brief Pre-allocates storage for the collection and its address pools.
    /// @param nNIFs Expected number of network interfaces.
    /// @param nIP4 Expected total number of IPv4 addresses.
    /// @param nIP6 Expected total number of IPv6 addresses.
    void reserve(size_t nNIFs, size_t nIP4, size_t nIP6);

    /// @brief Writes a formatted summary of all interfaces to the given output stream.
    /// @tparam TOSTR Type of the output stream (must support << operator).
    /// @param o Output stream to write to.
    /// @return Reference to @p o for chaining.
	template <class TOSTR>
	TOSTR& show(TOSTR& o) const
	{
	    for (auto& nif : _nifs)
	    {
	        nif.show(o);
	    }
	    return o;
	}
};

} //namespace no
