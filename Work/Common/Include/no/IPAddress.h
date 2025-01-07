// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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
	class IP4Address
	{
		union
		{
			char address[4];
			td::UINT4 nAddress;
		};
		
	public:
		IP4Address()
			: nAddress(0)
		{
		}

		IP4Address(const IP4Address& address)
			: nAddress(address.nAddress)
		{
		}

		void operator = (const IP4Address& address)
		{
			nAddress = address.nAddress;
		}
	};

	class IP6Address
	{
		union
		{
			char address[16];
			td::GUID guid;
		};
	public:
		IP6Address()
			: guid(0,0)
		{
		}

		IP6Address(const IP6Address& address)
			: guid(address.guid)
		{
		}

		void operator = (const IP6Address& address)
		{
			guid = address.guid;
		}
	};
}
