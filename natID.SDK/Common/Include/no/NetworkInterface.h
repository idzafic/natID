// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file NetworkInterface.h
 * @brief Represents a single network interface with its IP and MAC address information.
 */
//
//  NetworkInterface.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/String.h>
#include <cnt/PushBackVector.h>
#include <cnt/List.h>
#include <no/netOpLib.h>
#include <no/IP4Address.h>
#include <no/IP6Address.h>
#include <no/MACAddress.h>
//#include <compiler/DLLInterface.h>

namespace no
{

/// @brief Describes a single network interface including its name, addresses and status.
class NETOP_API NetworkInterface
{
public:
    typedef cnt::List<no::IP4Address, true>  TIP4List; ///< List type for IPv4 addresses.
    typedef cnt::List<no::IP6Address, true>  TIP6List; ///< List type for IPv6 addresses.
//#ifdef MU_VS_COMPILER
    //class NETOP_API TIP4List;
    //class NETOP_API TIP6List;
//#endif
protected:
    td::String _name;             ///< Human-readable name of the network interface.
    td::String _driverName;       ///< Operating-system driver name for the interface.
    td::String _wifiSID;          ///< Wi-Fi SSID, if the interface is a wireless adapter.
    TIP4List _ip4Addresses;       ///< List of IPv4 addresses assigned to this interface.
    TIP6List _ip6Addresses;       ///< List of IPv6 addresses assigned to this interface.
    no::MACAddress _macAddress;   ///< MAC address (hardware address) of this interface.
public:
    /// @brief Default constructor; creates an interface with empty name and addresses.
    NetworkInterface();

    /// @brief Constructs an interface with the given name.
    /// @param pNameNIF Null-terminated string containing the interface name.
    NetworkInterface(const char* pNameNIF);

    /// @brief Returns the interface type as a human-readable string.
    /// @return Null-terminated C string describing the MAC address type.
    const char* getTypeAsString() const;

    /// @brief Sets the MAC address from raw bytes and assigns the interface type.
    /// @param mac Pointer to the raw MAC bytes.
    /// @param macLen Number of bytes to copy (typically 6).
    /// @param sysType Interface type reported by the operating system.
    void setMacAddress(const td::BYTE* mac, td::BYTE macLen, no::MACAddress::Type sysType);

    /// @brief Clears the MAC address to its empty (all-zeros) state.
    void clearMAC();

    /// @brief Sets the driver name from a C string.
    /// @param drName Null-terminated driver name string.
    void setDriverName(const char* drName);

    /// @brief Sets the driver name from a td::String.
    /// @param drName Driver name string.
    void setDriverName(const td::String& drName);

    /// @brief Returns whether the interface is currently active (connected and operational).
    /// @return true if the interface is active, false otherwise.
    bool isActive() const;

    /// @brief Returns a mutable reference to the list of IPv4 addresses.
    /// @return Reference to the internal TIP4List.
    cnt::List<no::IP4Address, true>& getIP4Addresses();

    /// @brief Returns a mutable reference to the list of IPv6 addresses.
    /// @return Reference to the internal TIP6List.
    cnt::List<no::IP6Address, true>& getIP6Addresses();

    /// @brief Checks whether any assigned address matches the given IP string.
    /// @param ipAddress IP address string to search for.
    /// @return true if a matching address was found, false otherwise.
    bool hasIPAddress(const td::String& ipAddress) const;

    /// @brief Formats the MAC address into a fixed-size character array.
    /// @tparam BUFFLEN Size of the destination array (deduced automatically).
    /// @param buffer Destination array for the null-terminated MAC string.
    /// @return Pointer to @p buffer on success, or nullptr on failure.
    template <size_t BUFFLEN>
    inline const char* buildMACString(char(&buffer)[BUFFLEN]) const
    {
        return _macAddress.buildString(buffer);
    }

    /// @brief Returns whether the MAC address has never been set.
    /// @return true if the MAC address is all-zeros, false otherwise.
    bool isMACEMpty() const;

    /// @brief Returns the name of the network interface.
    /// @return Const reference to the interface name string.
    const td::String& getName() const;

    /// @brief Sets the interface name from a td::String.
    /// @param name New name for the interface.
    void setName(const td::String& name);

    /// @brief Sets the interface name from a C string.
    /// @param pName Null-terminated new name for the interface.
    void setName(const char* pName);

    /// @brief Sets the Wi-Fi SSID for this interface.
    /// @param ssidName SSID string to associate with this interface.
    void setWiFiSSID(const td::String& ssidName);

    /// @brief Sets the MAC address type (interface category).
    /// @param macType Interface type to assign.
    void setType(no::MACAddress::Type macType);

    /// @brief Updates the connection status of this interface.
    /// @param sysStatus New status value.
    void setStatus(no::MACAddress::Status sysStatus);

    /// @brief Returns the current connection status.
    /// @return Current MACAddress::Status value.
    no::MACAddress::Status getStatus() const;

    /// @brief Writes a formatted summary of this interface to the given output stream.
    /// @tparam TOSTR Type of the output stream (must support << operator).
    /// @param o Output stream to write to.
    /// @return Reference to @p o for chaining.
    template <class TOSTR>
    TOSTR& show(TOSTR& o) const
    {
        char tmp[1024];
        buildMACString(tmp);

        o << _name;
        o << "(" << _macAddress.getStatusAsString() << ") - " << getTypeAsString();
        if (_wifiSID.length() > 0)
            o << " (" << _wifiSID << ")";

        o <<  " - " << tmp <<  " - " << _driverName << td::endl;
        if (!_ip4Addresses.isEmpty())
        {

            int i=0;
            o << "    IP4: ";
            for (auto& ip4Address : _ip4Addresses)
            {
                if (i++>0)
                    o << "; ";
                const char* pAddress = ip4Address.buildString(tmp); //no::buildIPAddressString(tmp, &ip4Address);
                o << pAddress;;
            }
            o << td::endl;
        }

        if (!_ip6Addresses.isEmpty())
        {
            int i=0;
            o << "    IP6: ";
            for (auto& ip6Address : _ip6Addresses)
            {
                if (i++ > 0)
                    o << "; ";
                const char* pAddress = ip6Address.buildString(tmp); //no::buildIPAddressString(tmp, &ip6Address);
                o << pAddress;;
            }
            o << td::endl;
        }
        return o;
    }
};
} // namespace no
