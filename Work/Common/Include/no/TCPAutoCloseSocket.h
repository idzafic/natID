// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  TCPSocket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/TCPSocket.h>

namespace no
{
	
class TCPAutoCloseSocket : public TCPSocket
{
    
protected:
    TCPAutoCloseSocket()
    {}
    
public:
    TCPAutoCloseSocket(int sockID)
    : TCPSocket(sockID)
    {}
    
    TCPAutoCloseSocket(const TCPSocket& sock)
    : TCPSocket(sock)
    {}

    void operator = (const TCPSocket& sock)
    {
        TCPSocket::operator = (sock);
    }
    
    ~TCPAutoCloseSocket()
    {
        TCPSocket::close();
    }
    
};

} //namespace no
