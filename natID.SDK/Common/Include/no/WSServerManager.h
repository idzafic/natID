// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  WebSocketManager.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/SockDesc.h>
#include <td/String.h>
#include <td/Variant.h>
#include <mem/Buffer.h>
#include <cnt/SafeFullVector.h>
#include <no/ICommunicator.h>
#include <no/WebSocket.h>
//#include <cnt/Array.h>

namespace no
{
class WebServer;
class WebClient;

class NETOP_API WSServerManager : protected no::IProsumer
{
    friend class WebServer;
    //friend class WebClient;
    
protected:
    enum class UpgradeRequest : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication};
    
    //enum class UpgradeResponse : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication}; //client
    class Entry
    {
    public:
        no::SockDesc sockDesc;
        td::DateTime lastPingOrActivity;
        unsigned char loginSalt[80];
        TSockPos userPos = 0;
        HTTP::WebSocket::State connectionState = HTTP::WebSocket::State::NotConnected;
        //td::BYTE loggedIn = 0;
        td::BYTE customClient = 0;
    };
    
    no::ICommunicator* _pCommunicator = nullptr;
    cnt::SafeFullVector<Entry> _entries;
    TSockPos _nMaxConnections = 0;
    TSockPos _nActiveConnections = 0;
    td::UINT2 _timeOut = 20000;
    td::BYTE _consumerPos = 1;
    //td::BYTE _sendHashOnInit : 1;
private:
    //WebSocket::Frame decode(const no::SockDesc& sockDesc, mem::Buffer& buffer, char* pBuffer, size_t buffLen, td::UINT4& startingPos, td::UINT4& stillToProcess, td::UINT4& msgLen);
    //bool encode(mem::Buffer& buffer, const char* pMsg, size_t msgLen, bool appendToBuff = false) const;
    //bool encode(mem::Buffer& buffer, const WebSocket::Message* pMsgParts, size_t nParts, bool appendToBuff = false) const;
    
    //td::UINT2 getWebSocketPos(const no::SockDesc& sockDesc) const;
    bool initConnection(no::SockDesc& sockDesc, mem::Buffer& buffer, const char* request, const td::Variant*params, size_t nParams);
    bool handleMsgRead(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage, const td::Variant& varUserData, Entry*& pEntry);
    void handleMsgSent(const no::SockDesc& sockDesc, const td::Variant& varUserData);
    void handleDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason);
    void handleIdle();
    void cleanEntries();
    Entry* findEmptyEntry();
    void cleanEntry(Entry* pEntry);
    void sendToAll(mem::Buffer& buffer);
    void sendPing();
    void sendPong(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage);
    bool checkPong(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage);
    void closeEntry(Entry* pEntry, no::DisconnectReason reason);
    td::BYTE isCustomClient(const no::SockDesc& sockDesc) const;
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData);
    //UpgradeResponse handleUpgradeResponse(const no::SockDesc& sockDesc);
protected:
    virtual UpgradeRequest onUpgradeRequest(const char* request, const td::Variant*params, size_t nParams);
    //virtual UpgradeResponse onUpgradeResponse(const no::SockDesc& sockDesc);
    Entry* findEntry(const no::SockDesc& sockDesc);
    const Entry* findEntry(const no::SockDesc& sockDesc) const;
    virtual bool onConnect(const WSServerManager::Entry* pEntry);
    virtual bool onMsgRead(const WSServerManager::Entry* pEntry, const WebSocket::Message& wsMessage, const td::Variant& varUserData);
    virtual void onMsgSent(const WSServerManager::Entry* pEntry, const td::Variant& varUserData);
    virtual void onDisconnect(const WSServerManager::Entry* pEntry, no::DisconnectReason reason);
    virtual TSockPos findUser(const char* pUserName, size_t nameLen, td::String& pwd);
    //virtual TSockPos findUser(const char* pUserName, size_t nameLen);
    
    //IProsumer
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;
    void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) override;
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;
    void onIdle() override;
    //end IProsumer
    
public:
    WSServerManager(TSockPos maxConnections);
    void sendMessage(const no::SockDesc& sockDescDest, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());
    void sendMessageToAll(const no::SockDesc& sockDescSource, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());
    void sendMessageToAll(const no::SockDesc& sockDescSource, const WebSocket::Message* pMsgParts, size_t nParts, const td::Variant& varUserData = td::Variant());
    
    template <size_t NMSG>
    inline void sendMessageToAll(const no::SockDesc& sockDescSource, const cnt::Array<WebSocket::Message, NMSG>& msgParts, const td::Variant& varUserData = td::Variant())
    {
        sendMessageToAll(sockDescSource, msgParts.begin(), msgParts.size(), varUserData);
    }
    
    TSockPos getMaxConnections() const;
};

} //namespace no
