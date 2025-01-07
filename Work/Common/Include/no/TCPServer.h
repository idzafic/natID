// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  TCPServer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/Socket.h>
#include <td/String.h>

namespace no
{
	
class NETOP_API TCPServer : public Socket
{
protected:
    TCPServer(const TCPServer& server);
    void operator = (const TCPServer& server);
public:
    TCPServer();
    ~TCPServer();

    bool start(const char* localAddress, td::UINT2 localPort, int queueLen = 5, bool nonBlocking = false);
    bool start(const td::String& localAddress, td::UINT2 localPort, int queueLen = 5, bool nonBlocking = false);
    int accept();
    void stop();
};

} //namespace no
