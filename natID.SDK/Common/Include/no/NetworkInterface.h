// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  NetworkInterface.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/String.h>
#include <cnt/PushBackVector.h>
#include <cnt/List.h>
#include <no/netOpLib.h>
#include <no/IP4Address.h>
#include <no/IP6Address.h>
#include <no/MACAddress.h>
//#include <compiler/DLLInterface.h>

namespace no
{

class NETOP_API NetworkInterface
{
public:
    typedef cnt::List<no::IP4Address, true>  TIP4List;
    typedef cnt::List<no::IP6Address, true>  TIP6List;
//#ifdef MU_VS_COMPILER
    //class NETOP_API TIP4List;
    //class NETOP_API TIP6List;
//#endif
protected:
    td::String _name;
    td::String _driverName;
    td::String _wifiSID;
    TIP4List _ip4Addresses;
    TIP6List _ip6Addresses;
    no::MACAddress _macAddress;
public:
    NetworkInterface();

    NetworkInterface(const char* pNameNIF);
    
    const char* getTypeAsString() const;
    
    void setMacAddress(const td::BYTE* mac, td::BYTE macLen, no::MACAddress::Type sysType);

    void clearMAC();

    void setDriverName(const char* drName);

    void setDriverName(const td::String& drName);

    bool isActive() const;
    
    cnt::List<no::IP4Address, true>& getIP4Addresses();
    
    cnt::List<no::IP6Address, true>& getIP6Addresses();
    
    bool hasIPAddress(const td::String& ipAddress) const;
    
    template <size_t BUFFLEN>
    inline const char* buildMACString(char(&buffer)[BUFFLEN]) const
    {
        return _macAddress.buildString(buffer);
    }
    
    bool isMACEMpty() const;

    const td::String& getName() const;
    
    void setName(const td::String& name);
    
    void setName(const char* pName);
    
    void setWiFiSSID(const td::String& ssidName);
    
    void setType(no::MACAddress::Type macType);

    void setStatus(no::MACAddress::Status sysStatus);

    no::MACAddress::Status getStatus() const;
    
    template <class TOSTR>
    TOSTR& show(TOSTR& o) const
    {
        char tmp[1024];
        buildMACString(tmp);
        
        o << _name;
        o << "(" << _macAddress.getStatusAsString() << ") - " << getTypeAsString();
        if (_wifiSID.length() > 0)
            o << " (" << _wifiSID << ")";
        
        o <<  " - " << tmp <<  " - " << _driverName << td::endl;
        if (!_ip4Addresses.isEmpty())
        {
            
            int i=0;
            o << "    IP4: ";
            for (auto& ip4Address : _ip4Addresses)
            {
                if (i++>0)
                    o << "; ";
                const char* pAddress = ip4Address.buildString(tmp); //no::buildIPAddressString(tmp, &ip4Address);
                o << pAddress;;
            }
            o << td::endl;
        }
    
        if (!_ip6Addresses.isEmpty())
        {
            int i=0;
            o << "    IP6: ";
            for (auto& ip6Address : _ip6Addresses)
            {
                if (i++ > 0)
                    o << "; ";
                const char* pAddress = ip6Address.buildString(tmp); //no::buildIPAddressString(tmp, &ip6Address);
                o << pAddress;;
            }
            o << td::endl;
        }
        return o;
    }
};
} // namespace no

