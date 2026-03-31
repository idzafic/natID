// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  ICommunicator.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/SockDesc.h>
#include <mem/Buffer.h>
#include <no/IProsumer.h>
#include <td/Variant.h>
#include <tuple>

namespace no
{

class ICommunicator
{
public:
    //virtual void init(td::UINT2 nMaxEntries) = 0;
    virtual bool start(const char* pAddress, td::UINT2 portNo, const td::Variant& additionalInfo) = 0;
    virtual bool addEntry(no::SockDesc& sockDesc) = 0; //returns reading buffer
    virtual bool getSockDesc(no::SockDesc& sockDesc) = 0;
    virtual void removeEntry(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;
    virtual bool asyncRead(const no::SockDesc& sockDesc, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    //virtual bool asyncRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    virtual bool asyncReadSome(const no::SockDesc& sockDesc, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    //virtual bool asyncReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    virtual bool asyncReadCont(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    
    virtual bool asyncReadSomeCont(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    virtual bool asyncWrite(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    
    //virtual bool shouldClose(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;
    //virtual void releaseBuffer(mem::Buffer* pBuffer) = 0;
    virtual TSockPos getNumberOfEntries() const = 0;
    
    //stop at the next possible moment
    virtual void shutDown() = 0;
    
    //stop immediatelly
    virtual void stop() = 0;
    
    virtual void release() = 0;
    
    virtual mem::Buffer getEmptyBuffer() = 0;
    virtual mem::Buffer makeBuffer(const td::String* strings, int nStrings) = 0;
    
    virtual std::tuple<char*, td::UINT4> getReadWriteAuxilaryBuffer() = 0;
};

} //namespace no
