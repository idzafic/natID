// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IPAddress.h
 * @brief Lightweight IPv4 and IPv6 address types without external library dependency.
 */
//
//  IP4Address.cpp
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>

namespace no
{
	/// @brief Stores and manipulates a 32-bit IPv4 address (header-only version).
	class IP4Address
	{
		union
		{
			char address[4]; ///< Raw address bytes in network byte order.
			td::UINT4 nAddress; ///< Address as a single 32-bit unsigned integer.
		};

	public:
		/// @brief Default constructor; initialises the address to 0.0.0.0.
		IP4Address()
			: nAddress(0)
		{
		}

		/// @brief Copy constructor.
		/// @param address Source address to copy.
		IP4Address(const IP4Address& address)
			: nAddress(address.nAddress)
		{
		}

		/// @brief Assignment operator.
		/// @param address Source address to assign.
		void operator = (const IP4Address& address)
		{
			nAddress = address.nAddress;
		}
	};

	/// @brief Stores and manipulates a 128-bit IPv6 address (header-only version).
	class IP6Address
	{
		union
		{
			char address[16]; ///< Raw address bytes in network byte order.
			td::GUID guid;    ///< Address overlaid as a GUID for convenient comparison.
		};
	public:
		/// @brief Default constructor; initialises the address to the all-zeros address.
		IP6Address()
			: guid(0,0)
		{
		}

		/// @brief Copy constructor.
		/// @param address Source address to copy.
		IP6Address(const IP6Address& address)
			: guid(address.guid)
		{
		}

		/// @brief Assignment operator.
		/// @param address Source address to assign.
		void operator = (const IP6Address& address)
		{
			guid = address.guid;
		}
	};
}
