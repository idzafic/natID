// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  IProsumer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <mem/Buffer.h>
#include <td/Variant.h>

namespace no
{

class ICommunicator;
class IProsumer
{
public:
    enum class ConnStatus {NotConnected=0, Connected};
    virtual bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) = 0;
    virtual void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) = 0;
    virtual bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;
    virtual void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;
    
    virtual void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) = 0;
    virtual void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) = 0;
    virtual void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) = 0;
    virtual void onIdle() = 0;
};

} //namespace no
