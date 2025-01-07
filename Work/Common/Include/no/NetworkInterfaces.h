// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  NetworkInterfaces.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/NetworkInterface.h>
#include <cnt/SafeFullVector.h>
#include <mem/CntFixedSizeMemoryPool.h>
#include <no/netOpLib.h>

namespace no
{

class NETOP_API NetworkInterfaces
{
public:
    typedef mem::CntFixedSizeMemoryPool<NetworkInterface::TIP4List> TMemPool4;
    //NETOP_TEMPLATE class NETOP_API TMemPool4;
    typedef mem::CntFixedSizeMemoryPool<NetworkInterface::TIP6List> TMemPool6;
    //NETOP_TEMPLATE class NETOP_API TMemPool6;

private:
	cnt::SafeFullVector<NetworkInterface> _nifs;
    //mem::CntFixedSizeMemoryPool< cnt::List<no::IP4Address, true> > _memPoolIP4;
    //mem::CntFixedSizeMemoryPool< cnt::List<no::IP6Address, true> > _memPoolIP6;
    TMemPool4 _memPoolIP4;
    TMemPool6 _memPoolIP6;

public:
    
    NetworkInterfaces();
    
    NetworkInterface* findNIF(const char* nifName);

    NetworkInterface* findNIF(const td::String& nifName);
    
    const NetworkInterface* findNIFWithIPAddress(const td::String& ipAddress) const;

    auto cbegin() const
    {
        return _nifs.cbegin();
    }
        
    auto begin()
    {
        return _nifs.begin();
    }
        
    auto cend() const
    {
        return _nifs.cend();
    }
        
    auto end()
    {
        return _nifs.end();
    }
        
    void addAddress(const no::IP4Address* ip4Address, no::NetworkInterface* toNIF);
        
    void addAddress(const no::IP6Address* ip6Address, no::NetworkInterface* toNIF);
    
    const cnt::SafeFullVector<NetworkInterface>& getNIFs();
        
    NetworkInterface* setNIFName(size_t iPos, const char* nifName);
        
    size_t size() const;
        
    void reserve(size_t nNIFs, size_t nIP4, size_t nIP6);
    
	template <class TOSTR>
	TOSTR& show(TOSTR& o) const
	{
	    for (auto& nif : _nifs)
	    {
	        nif.show(o);
	    }
	    return o;
	}
};

} //namespace no

