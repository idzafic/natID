// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MACAddress.h
 * @brief MAC address representation with type and connection-status tracking.
 */
//
//  MACAddress.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>
#include <no/netOpLib.h>

namespace no
{
#define MAX_MAC_ADDRESS_SIZE 6

/// @brief Stores a 48-bit MAC address together with its interface type and connection status.
class NETOP_API MACAddress
{
public:
    /// @brief Network interface type associated with this MAC address.
    enum class Type : td::BYTE { LoopBack = 0, Ethernet, WiFi, Other };

    /// @brief Current connection status of the network interface.
    enum class Status : td::BYTE {Disconnected = 0, Unreachable, Discovering, Authenticating, Connecting, Connected, NotReady, AddHoc, Associating, Operational, Disconnecting, Other};

protected:
    /// @brief Internal storage type that splits the 6-byte MAC into two fields.
    class macAddr
    {
    public:
        td::UINT4 part1; ///< First 4 bytes of the MAC address.
        td::UINT2 part2; ///< Last 2 bytes of the MAC address.

        /// @brief Default constructor; zeroes all fields.
        macAddr()
        : part1(0)
        , part2(0)
        { }

        /// @brief Constructs a macAddr from two integer parts.
        /// @param p1 First 4-byte part.
        /// @param p2 Last 2-byte part.
        macAddr(td::UINT4 p1, td::UINT2 p2)
        : part1(p1)
        , part2(p2)
        { }

        /// @brief Copy constructor.
        /// @param addr Source macAddr to copy.
        macAddr(const macAddr& addr)
        : part1(addr.part1)
        , part2(addr.part2)
        { }

        /// @brief Assignment operator.
        /// @param addr Source macAddr to assign from.
        /// @return Reference to this object.
        const macAddr& operator = (const macAddr& addr)
        {
            part1 = addr.part1;
            part2 = addr.part2;
            return *this;
        }
    };

    union
    {
        td::BYTE _address[MAX_MAC_ADDRESS_SIZE]; ///< Raw MAC address bytes.
        macAddr _macAddress;                     ///< MAC address as split integer parts.
    };
    Type _type;     ///< Interface type (Ethernet, WiFi, etc.).
    Status _status; ///< Current connection status.
public:
    /// @brief Default constructor; zeroes the address and sets type to LoopBack.
    MACAddress();

    /// @brief Copy constructor.
    /// @param address Source address to copy.
    MACAddress(const MACAddress& address);

    /// @brief Assignment operator.
    /// @param address Source address to assign.
    void operator = (const MACAddress& address);

    /// @brief Formats the MAC address as a colon-separated hex string.
    /// @param buffer Destination buffer for the null-terminated string.
    /// @param buffLen Size of the destination buffer in bytes.
    /// @return Pointer to @p buffer on success, or nullptr on failure.
    const char* buildString(char* buffer, size_t buffLen) const;

    /// @brief Formats the MAC address into a fixed-size character array.
    /// @tparam BUFFLEN Size of the destination array (deduced automatically).
    /// @param buffer Destination array for the null-terminated string.
    /// @return Pointer to @p buffer on success, or nullptr on failure.
    template <size_t BUFFLEN>
    inline const char* buildString(char(&buffer)[BUFFLEN]) const
    {
        return buildString(buffer, BUFFLEN);
    }

    /// @brief Checks whether the MAC address is all-zeros (empty).
    /// @return true if the address has never been set, false otherwise.
    bool isEMpty() const;

    /// @brief Sets the interface type for this MAC address.
    /// @param macType The interface type to assign.
    void setType(no::MACAddress::Type macType);

    /// @brief Returns the interface type associated with this MAC address.
    /// @return Current interface type.
    no::MACAddress::Type getType() const;

    /// @brief Sets the MAC address bytes and interface type.
    /// @param mac Pointer to the raw MAC bytes.
    /// @param macLen Number of bytes to copy (should be 6).
    /// @param sysType Interface type reported by the operating system.
    void setAddress(const td::BYTE* mac, td::BYTE macLen, no::MACAddress::Type sysType);

    /// @brief Updates the connection status.
    /// @param status New status value to store.
    void setStatus(no::MACAddress::Status status);

    /// @brief Returns the current connection status.
    /// @return Current Status enumeration value.
    no::MACAddress::Status getStatus() const;

    /// @brief Resets the MAC address and status to their default (empty) state.
    void clear();

    /// @brief Returns the connection status as a human-readable string.
    /// @return Null-terminated C string describing the current status.
    const char* getStatusAsString() const;
};


} //namespace no
