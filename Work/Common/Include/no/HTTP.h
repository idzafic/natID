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
#include <mem/Buffer.h>
#include <no/Definitions.h>
#include <td/String.h>
#include <cnt/SafeFullVector.h>

namespace no
{

namespace HTTP
{
enum class Cmd : unsigned char {Get=0, Post};

typedef struct _Attrib
{
    const char* name;
    const char* value;
} Attrib;

class IConsumer
{
public:
    virtual bool consume(const char* buff, size_t len) = 0;
};

//standard unsecured (http) operations
NETOP_API bool sendPost(const td::String& hostName, const td::String& cmd, const td::String& strData, const td::String& strPort, td::String& retData);
NETOP_API bool sendPost(const td::String& hostName, const td::String& cmd, const td::String& strData, td::String& retData, int port = 80);

//standard secured (https)
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, td::UINT4& retCode);

NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, td::String& retData, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, HTTP::IConsumer* pConsumer, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String & url, const td::String& payLoadToSend, td::String& retData, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String & url, const td::String& payLoadToSend, HTTP::IConsumer* pConsumer, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

//custom web client-server communnication

//enum class SocketAction : td::INT2 {Normal = 0, ReadingTimeOut, Close};
enum class Action : td::INT2 {Normal = 0, WSLoginStage1, WSLoginStage2, WSLoginStage3, WSLoggedIn, ReadingTimeOut, WSClientUpgrade, WSPingSent, Close};

NETOP_API void prepareResponseBuffer(const char* pResult, size_t nResultLen, mem::Buffer& buffer, td::UINT2 timeOutInSec, const char* contentType = "text;charset=utf-8");

NETOP_API bool prepareRequestBuffer(mem::Buffer& buffer, no::HTTP::Request request, const char* httpRequestLink, const td::Variant* params = nullptr, size_t nParams = 0, const char* pContent = nullptr, size_t contentLen = 0, const char* contentType = "text;charset=utf-8");

NETOP_API void prepareWebSocketUpgradeRequestBuffer(mem::Buffer& buffer, const char* hostName, const char* httpRequestLink, const char wsKey[16]/*, const td::Variant* params = nullptr, size_t nParams = 0*/);

} ///namespace HTTP
} //namespace no

