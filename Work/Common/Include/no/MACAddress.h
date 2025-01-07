// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  MACAddress.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>
#include <no/netOpLib.h>

namespace no
{
#define MAX_MAC_ADDRESS_SIZE 6
class NETOP_API MACAddress
{
public:
    enum class Type : td::BYTE { LoopBack = 0, Ethernet, WiFi, Other };
    enum class Status : td::BYTE {Disconnected = 0, Unreachable, Discovering, Authenticating, Connecting, Connected, NotReady, AddHoc, Associating, Operational, Disconnecting, Other};
protected:
    class macAddr
    {
    public:
        td::UINT4 part1;
        td::UINT2 part2;
        
        macAddr()
        : part1(0)
        , part2(0)
        { }
        
        macAddr(td::UINT4 p1, td::UINT2 p2)
        : part1(p1)
        , part2(p2)
        { }
        
        macAddr(const macAddr& addr)
        : part1(addr.part1)
        , part2(addr.part2)
        { }
        
        const macAddr& operator = (const macAddr& addr)
        {
            part1 = addr.part1;
            part2 = addr.part2;
            return *this;
        }
    };
    
    union
    {
        td::BYTE _address[MAX_MAC_ADDRESS_SIZE];
        macAddr _macAddress;
    };
    Type _type;
    Status _status;
public:
    MACAddress();

    MACAddress(const MACAddress& address);

    void operator = (const MACAddress& address);
    
    const char* buildString(char* buffer, size_t buffLen) const;
    
    template <size_t BUFFLEN>
    inline const char* buildString(char(&buffer)[BUFFLEN]) const
    {
        return buildString(buffer, BUFFLEN);
    }
    
    bool isEMpty() const;
    
    void setType(no::MACAddress::Type macType);
    
    no::MACAddress::Type getType() const;
    
    void setAddress(const td::BYTE* mac, td::BYTE macLen, no::MACAddress::Type sysType);

    void setStatus(no::MACAddress::Status status);

    no::MACAddress::Status getStatus() const;
    
    void clear();

    const char* getStatusAsString() const;
};


} //namespace no
