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

class NETOP_API RequestHeader
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
    cnt::Array<Parameter, 32> _params; //maximum 16 params
    cnt::Array<td::Variant, 64> _requestParams; //maximum 32 request params
    const char* _hostNameAndPort;
    const char* _webSocketKey;
    const char* _request;
    td::UINT4 _contentLength;
    td::UINT4 _buffLen;
    td::UINT2 _processedBytes;
    td::UINT2 _nParams;
    td::UINT2 _nRequestParams;
    HTTP::Request _requestType;
    HTTP::Version _version = no::HTTP::Version::v1_1;
    td::BYTE _keepAlive : 1;
    td::BYTE _webSocketConnUpgrade : 1;
    
    td::DataType getDataType(const char* pBuff) const;
    bool decodeRequest(const char* pBuff);
    void getValue(const char* pBuff, td::Variant& val);
    bool checkCorrectness() const;
public:
    RequestHeader();
    bool decode(mem::Buffer& buffer, char* pAuxBuff, td::UINT4 buffLen);
    HTTP::Request getRequestType() const;
    const char* getRequest() const;
    bool keepAlive() const;
    td::UINT2 getNoOfParams() const;
    td::UINT2 getNoOfRequestParams() const;
    const cnt::Array<td::Variant, 64>& getRequestParams() const;
    td::UINT4 getContentLength() const;
    const char* getWebSocketKey() const;
    bool isWebSocketInitHandshaking() const;
    td::UINT2 getProcessedBytes() const;
};

} //namespace HTTP

} //namespace no

