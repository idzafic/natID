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
#include <crpt/SHA256.h>
#include <no/Command.h>

namespace no
{

class WebClient;

class NETOP_API WSClientManager : protected IProsumer
{
    friend class WebClient;
    
protected:
    enum class UpgradeResponse : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication};
    class Entry
    {
    public:
        no::SockDesc sockDesc;
        td::String hostName;
        td::String link;
        td::String userName;
        td::DateTime lastPingOrActivity;
        char loginSalt[80];
        char serverKey[33];
        char serverIV[17];
        char clientPassPhrase[33];
        char clientKey[33];
        char passwordHash[SHA256_DIGEST_SIZE];
        TSockPos userPos = 0;
        td::UINT2 portNo = 0;
        HTTP::WebSocket::State connectionState = HTTP::WebSocket::State::NotConnected;
        bool requiresLogin = false;
        //td::BYTE loggedIn = 0;
    };
    
    no::ICommunicator* _pCommunicator = nullptr;
    WebClient* _pWebClient = nullptr;
    cnt::SafeFullVector<Entry> _entries;
    TSockPos _nActiveConnections = 0;
    td::UINT2 _timeOut = 2000;
    td::BYTE _prosumerPos = 0; //1 based, 0-means not used
    //td::BYTE _sendHashOnInit : 1;
private:
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;
    UpgradeResponse handleUpgradeResponse(const no::SockDesc& sockDesc, const char* pWebSocketKey, const td::Variant& userData);
    void handleAuthStage1(const no::SockDesc& sockDescIn, const td::Variant& userData) const;
//    void handleAuthenticationSalt(const no::SockDesc& sockDesc, const mem::Buffer& buffer, size_t posInBuffer, const td::Variant& userData);
    void handleLoginStage2Response(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData);
//    bool initConnection(no::SockDesc& sockDesc, mem::Buffer& buffer, const td::String& request, const td::Variant*params, size_t nParams);
    bool handleMsgRead(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage, const td::Variant& varUserData);
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
    
    void sendPingToAll(mem::Buffer& buffer);
protected:
    //IProsumer
    //virtual void handleConnect(no::SockDesc& sockDesc, td::Variant& userData);
    void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) override;
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;
    void onIdle() override;
    //end IProsumer
    virtual UpgradeResponse onUpgradeResponse(const no::SockDesc& sockDesc);
    Entry* findEntry(const no::SockDesc& sockDesc);
    const Entry* findEntry(const no::SockDesc& sockDesc) const;
    const Entry* findConnection(td::UINT2 connID) const;
    virtual void onConnect(const Entry* pEntry);
    virtual bool onMsgRead(const Entry* pEntry, const WebSocket::Message& wsMessage, const td::Variant& varUserData);
    virtual void onMsgSent(const Entry* pEntry, const td::Variant& varUserData);
    virtual void onDisconnect(const Entry* pEntry, no::DisconnectReason reason);
    virtual TSockPos findUser(const char* pUserName, size_t nameLen, td::String& pwd);
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData);
public:
    WSClientManager(TSockPos maxConnections);
    bool isConnected(td::UINT2 connID) const;
    bool asyncSend(td::UINT2 connID, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());
    bool asyncSend(td::UINT2 connID, const td::String& strMsg, const td::Variant& varUserData = td::Variant());
    bool asyncSend(td::UINT2 connID, no::WebSocket::Command cmd, const td::Variant& varUserData = td::Variant());
    bool asyncSend(td::UINT2 connID, no::WebSocket::Command cmd, const td::String& commndData, const td::Variant& varUserData = td::Variant());
    bool asyncSend(td::UINT2 connID, mem::Buffer& buff, const td::Variant& varUserData = td::Variant());
    td::UINT2 asyncConnect(const td::String& hostName, td::UINT2 portNo,  const td::String& httpRequestLink, const td::String& userName = td::String(), const td::String& pwd = td::String(), const td::Variant& varUserData=td::Variant());
    void disconnect(td::UINT2 connID);
};

} //namespace no
