// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IP6Address.h
 * @brief IPv6 address representation and string conversion utilities.
 */
//
//  IP6Address.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>
#include <no/netOpLib.h>

namespace no
{

/// @brief Stores and manipulates a 128-bit IPv6 address.
class NETOP_API IP6Address
{
    union
    {
        td::BYTE _address[16]; ///< Raw address bytes in network byte order.
        td::GUID _guid;        ///< Address overlaid as a GUID for convenient comparison.
    };
public:
    /// @brief Default constructor; initialises the address to the all-zeros address.
    IP6Address();

    /// @brief Copy constructor.
    /// @param address Source address to copy.
    IP6Address(const IP6Address& address);

    /// @brief Assignment operator.
    /// @param address Source address to assign.
    void operator = (const IP6Address& address);

    /// @brief Parses a colon-separated IPv6 string and stores the result.
    /// @param pAddressStr Null-terminated IPv6 address string (e.g. "::1").
    /// @return true if parsing succeeded, false if the string is invalid.
    bool fromString(const char *pAddressStr);

    /// @brief Formats the address into a caller-supplied character buffer.
    /// @param buffer Destination buffer for the null-terminated IPv6 string.
    /// @param buffLen Size of the destination buffer in bytes.
    /// @return Pointer to @p buffer on success, or nullptr on failure.
    const char* buildString(char* buffer, size_t buffLen) const;

    /// @brief Formats the address into a fixed-size character array.
    /// @tparam BUFFLEN Size of the destination array (deduced automatically).
    /// @param buffer Destination array for the null-terminated string.
    /// @return Pointer to @p buffer on success, or nullptr on failure.
    template <size_t BUFFLEN>
    const char* buildString(char(&buffer)[BUFFLEN]) const
    {
        return buildString(buffer, BUFFLEN);
    }
};

} //namespace no
