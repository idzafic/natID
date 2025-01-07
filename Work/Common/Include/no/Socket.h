// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Socket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <td/String.h>

namespace no
{
	
enum class Protocol : td::BYTE {IPv4=0, IPv6};

class NETOP_API Socket
{
public:
    enum class Type : td::BYTE {TCP=0, UDP};
protected:
	int _socketID;
    //td::BYTE _autoClose : 1;
    
protected:
    
public:
    Socket();
    Socket(int sockID);
    Socket(const Socket& sock);
    
    ~Socket();
    
    void close();
    
    bool create(Protocol ipProtocol, Type type);
    
    void operator = (const Socket& sock);
    int getID() const;

    //void setAutoClose(bool autoClose);
    //bool isAutoClose() const;
    
    bool isOK() const;
    
    bool makeNonBlocking();
};

} //namespace no
