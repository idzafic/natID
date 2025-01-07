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
#include <cnt/Array.h>
#include <td/StringUtils.h>
#include <td/Variant.h>
#include <no/Definitions.h>

namespace no
{

namespace HTTP
{

class NETOP_API ResponseHeader
{
public:

    typedef struct _CP
    {
        const char* name;
        const char* value;
    } Parameter;

protected:
    //char _buff[2048];
    char* _pBuff;
    cnt::Array<Parameter, 32> _params; //maximum 32 params
    const char* _serverName;
    const char* _webSocketKey;
    td::UINT4 _contentLength;
    td::UINT2 _responseVal;
    td::UINT4 _buffLen;
    td::UINT2 _processedBytes;
    td::UINT2 _nParams;
    HTTP::Version _version = no::HTTP::Version::v1_1;
    td::BYTE _encoding;
    td::BYTE _keepAlive : 1;
    td::BYTE _webSocketConnUpgrade : 1;
public:
    ResponseHeader();
    bool decode(mem::Buffer& buffer, char* pAuxBuff, td::UINT4 buffLen);
    bool keepAlive() const;
    td::UINT2 getNoOfParams() const;
    td::UINT4 getContentLength() const;
    const char* getWebSocketKey() const;
    bool isWebSocketHandshakingResponse() const;
    td::UINT2 getProcessedBytes() const;
    cnt::Array<Parameter, 32>& getParams() const;
    bool checkCorrectness() const;
    td::UINT2 getResponseVal() const;
};

} //namespace HTTP

} //namespace no

