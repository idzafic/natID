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
#include <no/netOpLib.h>
#include <no/Socket.h>
#include <no/NetworkInterfaces.h>
#include <td/String.h>
#include <mem/Buffer.h>

namespace no
{
	
class TCPServer;
class NETOP_API TCPSocket : public Socket
{
    friend class TCPServer;
    
protected:
    TCPSocket();
    
    void setAsNonBlocking();
public:
    TCPSocket(int sockID);
    
    TCPSocket(const TCPSocket& sock);

    void operator = (const TCPSocket& sock);
    
	ssize_t receive(char* buffer, ssize_t nToReceive) const;
    ssize_t receive(mem::Buffer& buffer);
    td::String receive(ssize_t nToReceive) const;
	
    ssize_t send(const char* buffer, ssize_t nToWrite);
    ssize_t send(mem::Buffer& buffer);
    ssize_t send(const td::String& str);
    
    bool isNonBlocking() const;
    
    td::String getMACAddress() const;
    td::String getMACAddress(const no::NetworkInterfaces& nifs) const;
    
    bool getLocalIPAddressAndPortNumber(td::String& ipAddress, td::UINT2& portNo) const;
    td::String getLocalIPAddress() const;
    td::UINT2 getLocalPortNumber() const;
    
    bool getForeignIPAddressAndPortNumber(td::String& ipAddress, td::UINT2& portNo) const;
    td::String getForeignIPAddress() const;
    td::UINT2 getForeignPortNumber() const;
    
};

} //namespace no
