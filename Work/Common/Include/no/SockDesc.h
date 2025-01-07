// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  IP4Address.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/DateTime.h>
#include <no/Definitions.h>

namespace no
{

class SockDesc
{
public:
    int socketID;
    TSockPos sockEntryPos;
    //TSockPos sockPos;
    TSockPos webSockPos;
    td::BYTE consumerPos;
    td::BYTE version;
    
    SockDesc(int sockID = -1)
    : socketID(sockID)
    , sockEntryPos(0)
    //, sockPos(0)
    , webSockPos(0)
    , consumerPos(0)
    , version(0)
    //, webSock(0)
    {
    }
    
    SockDesc(int sockID, TSockPos entryPos, TSockPos webSockPos, td::BYTE consumPos, td::BYTE versionNo)
    //: creationDT(creationDateTime)
    : socketID(sockID)
    , sockEntryPos(entryPos)
    , webSockPos(webSockPos)
    , consumerPos(consumPos)
    , version(versionNo)
    {
    }
    

    //td::BYTE webSock;
    
    bool operator == (const SockDesc& sd)
    {
        return (socketID == sd.socketID && sockEntryPos == sd.sockEntryPos && webSockPos == sd.webSockPos);
    }
    
    const SockDesc& operator = (const SockDesc& sd)
    {
        //creationDT = sd.creationDT;
        socketID = sd.socketID;
        sockEntryPos = sd.sockEntryPos;
        //sockPos = sd.sockPos;
        webSockPos = sd.webSockPos;
        consumerPos = sd.consumerPos;
        version = sd.version;
        //webSock = sd.webSock;
        return *this;
    }
    
    bool isConnected() const
    {
        return (socketID >= 0);
    }
    
    bool isWebSocket() const
    {
        return (webSockPos > 0);
    }
    void clean()
    {
        //creationDT = td::DateTime();
        socketID = -1;
        sockEntryPos = 0;
        //sockPos = 0;
        consumerPos = 0;
        version = 0;
        //webSockPos = 0;
        //webSock = 0;
    }
    
};

} //namespace no
