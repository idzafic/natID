// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  IP6Address.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/GUID.h>
#include <no/netOpLib.h>

namespace no
{

class NETOP_API IP6Address
{
    union
    {
        td::BYTE _address[16];
        td::GUID _guid;
    };
public:
    IP6Address();

    IP6Address(const IP6Address& address);

    void operator = (const IP6Address& address);
    
    bool fromString(const char *pAddressStr);
    
    const char* buildString(char* buffer, size_t buffLen) const;
    
    template <size_t BUFFLEN>
    const char* buildString(char(&buffer)[BUFFLEN]) const
    {
        return buildString(buffer, BUFFLEN);
    }
};

} //namespace no
