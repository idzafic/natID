// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <no/netOpLib.h>
#include <no/ICommunicator.h>
#include <no/netOp.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <no/WSClientManager.h>
#include <no/HTTP.h>
#include <no/HTTPResponseHeader.h>
#include <cnt/SafeFullVector.h>

namespace no
{

class NETOP_API WebClient : protected no::IProsumer
{
//    friend class WSClientManager;
    
    class Entry
    {
    public:
        no::SockDesc sockDesc;
        td::String host;
        td::UINT2 port;
    };
    
protected:
    no::ICommunicator* _pCommunicator = nullptr;
    no::WSClientManager* _pWebSocketManager = nullptr;
    cnt::SafeFullVector<Entry> _httpConnections;
    td::UINT2 _timeOutInMilliSeconds;
    td::BYTE _prosumerPos = 0; //1 based, 0-means not used
    
protected:
    
    void onConnect(no::SockDesc& sockDesc, td::UINT2 entryPos);
    
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    Entry* getEmptyEntry();
    
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData);
    
    //IProsumer
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;
    
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    void onIdle() override;
    
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData) override;
    
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& userData) override;
    
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData) override;
    
    void shutDown();
    
    void setWebSocketManager(no::WSClientManager* pWebSocketManager);
    
public:
    WebClient(td::UINT2 nMaxHTTPConnections = 0, td::UINT2 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut);
    
    ~WebClient();
    
    bool run();
    
    bool asyncConnect(const char* serverAddress, td::UINT2 serverPort);
    
    void stop();
    
    bool isRunning() const;
    
    bool sendRequest(const no::SockDesc& sockDesc, no::HTTP::Request request, const char* httpRequestLink, const td::Variant* params = nullptr, size_t nParams = 0, const char* pContent = nullptr, size_t contentLen = 0, const char* contentType = "text;charset=utf-8", const td::Variant& varUserData=td::Variant());
    
    no::WSClientManager* getWSClientManager();
    
    mem::Buffer getEmtypBuffer();
};

} //namespace no
