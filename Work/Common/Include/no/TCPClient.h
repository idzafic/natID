// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  TCPClient.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/TCPSocket.h>

namespace no
{
	class NETOP_API TCPClient : public TCPSocket
	{
	private:

	public:
        TCPClient();
        TCPClient(int);
        ~TCPClient();
        
        bool connect(const char* hostName, td::UINT2 portNo, bool nonBlocking = false);
        
        bool connect(const td::String& hostName, td::UINT2 portNo, bool nonBlocking = false);
	};
}
