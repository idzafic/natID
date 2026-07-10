// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IP4Address.h
 * @brief IPv4 address representation and string conversion utilities.
 */
//
//  IP4Address.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>
#include <no/netOpLib.h>

namespace no
{

/// @brief Stores and manipulates a 32-bit IPv4 address.
class NETOP_API IP4Address
{
    union
    {
        td::BYTE _address[4]; ///< Raw address bytes in network byte order.
        td::UINT4 _nAddress;  ///< Address as a single 32-bit unsigned integer.
    };

public:
    /// @brief Default constructor; initialises the address to 0.0.0.0.
    IP4Address();

    /// @brief Copy constructor.
    /// @param address Source address to copy.
    IP4Address(const IP4Address& address);

    /// @brief Assignment operator.
    /// @param address Source address to assign.
    void operator = (const IP4Address& address);

    /// @brief Parses a dotted-decimal IPv4 string and stores the result.
    /// @param pAddrStr Null-terminated string in "a.b.c.d" notation.
    /// @return true if parsing succeeded, false if the string is invalid.
    bool fromString(const char* pAddrStr);

    /// @brief Formats the address into a caller-supplied character buffer.
    /// @param buffer Destination buffer for the null-terminated dotted-decimal string.
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
