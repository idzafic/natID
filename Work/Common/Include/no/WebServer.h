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
//#include <fo/FileOperations.h>
#include <map>
#include <no/Socket.h>
#include <no/HTTP.h>
#include <no/WSServerManager.h>
#include <no/HTTPRequestHeader.h>
#include <functional>

namespace no
{

#define MAX_NO_SERVER_CONNECTIONS 64
#define MAX_NO_WEBCHATSERER_CONNECTIONS 64

class NETOP_API WebServer : protected no::IProsumer
{
    class Resource
    {
    public:
        enum class Type {HTML=0, PNG, JPG, SVG};
        mem::Buffer buffer;
        Type type;
    };
protected:
    no::ICommunicator* _pCommunicator = nullptr;
    WSServerManager* _pWebSocketManager = nullptr;
    fo::fs::path _homePath;
    cnt::SafeFullVector<Resource> _resources;
    std::map<td::String, mem::Buffer*> _mapResources;
    std::map<td::String, std::function<bool (const td::Variant*, size_t, mem::Buffer&)> > _mapDynamicRequests;
    td::String _rootPage;
    td::String _strErr400BadRequest;
    td::String _strErr400MaxBuff;
    td::String _strErr404NotFound;
    td::String _strErr408RequestTimeOut;
    
    //fo::OutFile _outFile;
    td::UINT4 _nRes = 0;
    td::UINT2 _timeOutInMilliSeconds;
    
protected:
    
    void formatErrStrings();
    
    void onIdle() override;
    
    bool createWebSocketInitialHandshakeResponse(const no::HTTP::RequestHeader& header, mem::Buffer& buffer);
    
    template <typename TMEMBFN, typename TOBJ>
    bool addDynamicRequest(const char* reqName, const td::DataType* pParamTypes, size_t nParams, TMEMBFN memFn, TOBJ* pObj)
    {
        assert(nParams > 0 && nParams <= 64);
        char tmp[256];
        char chNoParams = '0' + nParams;
        auto nLen = SNPRINTF(tmp, 256, _TRUNCATE, "/%s\t%c", reqName, chNoParams);
        tmp[255] = 0;
        for (size_t i=0; i<nParams; ++i)
        {
            char chDataType = ' ' + (char) pParamTypes[i];
            tmp[nLen++] = chDataType;
        }
        tmp[nLen] = 0;
        td::String strReq = &tmp[0];
        auto it = _mapDynamicRequests.find(strReq);
        if (it != _mapDynamicRequests.end())
            return false;
        using namespace std::placeholders;
        td::UINT2 timeOutInSec = _timeOutInMilliSeconds / 1000;
        auto fn = std::bind(memFn, pObj, _1, _2, _3, timeOutInSec);
        _mapDynamicRequests[strReq] = fn;
        return true;
    }
    
//    void initDynamicRequests()
//    {
//        const td::DataType pParamTypes[] = {td::real8, td::real8};
//        addDynamicRequest("calculate", &pParamTypes[0], 2, &WebServer::drCalculate, this);
//    }
  
    //IProsumer
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;
    
    void onConnect(no::SockDesc& sockDesc, const td::Variant&) override;
    
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;
    
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    
    void upgradeToWebSocket(const no::SockDesc& sockDesc, mem::Buffer& buffer, const no::HTTP::RequestHeader& header);
    
    //getting data from header
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;
    
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;
    
    bool loadResources(const td::String& configPath, const char* relativeResPath);
    
    virtual bool onLoadResources();
    
public:
    
    WebServer(td::UINT2 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut);
    
    void setWebSocketManager(WSServerManager* pWSManager);
    
    void onConfigEntry(const char* pKey, size_t keyLen, const char* pVal, size_t valLen, const char* relativeResPath, int compressionLevel);
    
    bool run(const char* localAddress, td::UINT2 serverPort, td::UINT4 nMaxConnections = MAX_NO_SERVER_CONNECTIONS);
    
    bool isRunning() const;
    
    void stop();
};

}
