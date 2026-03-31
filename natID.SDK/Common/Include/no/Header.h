// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Header.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <cassert>
#include <cstring>
#include <td/Types.h>
#include <map>

namespace no
{

enum class CmdType : unsigned char { CMD_REQ=0, CMD_RET_OK=1, CMD_RET_NOK=2, CMD_FN_NO_CNT=3, CMD_NONE=4};
const char * const strCmd[] = {"RQ",       "OK",           "NO",         "FN"};

enum class ContentType : unsigned char {CT_NULL=0, CT_XML=1, CT_BIN=2, CT_BINXML=3, CT_ZIP=4, CT_ZIP_XML=5, CT_ZIP_BIN=6 ,CT_ZIP_BINXML=7, CT_NONE=8} ;
const char * const strContentType[] = {"na",    "xm",     "bn",       "bx",       "zp",       "zx",        "zb",        "zl"};


//HEADER
//header is at least 16 bytes long
//00-01 bytes: CmdType string
//02-03 bytes: Content type string
//04-07 bytes: Content length string binary or text encoded with own 64-base encoding
//08-15 bytes: Extra data in case of return command with number only
//template <bool binFormat>
template <int HEADLEN>
class Header
{
    char* _header;
    bool _binFormat;
    std::map<td::INT2, ContentType> _toContentType;
    std::map<td::INT2, CmdType> _toCmdType;
    
    void populateMaps()
    {
        for (int ct= (int) ContentType::CT_NULL; ct < (int) ContentType::CT_NONE; ++ct)
        {
            const char* const pStr = strContentType[ct];
            td::INT2* pi2 = (td::INT2*) pStr;
            td::INT2 i2Val = *pi2;
            _toContentType[i2Val] = (ContentType) ct;
        }

        for (int cmd= (int) CmdType::CMD_REQ; cmd < (int) CmdType::CMD_NONE; ++cmd)
        {
            const char* const pStr = strCmd[cmd];
            td::INT2* pi2 = (td::INT2*) pStr;
            td::INT2 i2Val = *pi2;
            _toCmdType[i2Val] = (CmdType) cmd;
        }
    }
public:
    Header(bool binFormat = false)
        : _header(0)
        , _binFormat(binFormat)
    {
        populateMaps();
    }

    Header(char* pHeader, bool binFormat = false)
        : _header(pHeader)
        , _binFormat(binFormat)
    {
        populateMaps();
    }

    void clean(char ch)
    {
        memset(_header, ch, HEADLEN);
    }

    void use(char* pHeader)
    {
        _header = pHeader;
    }

    //returns number of bytes read from first socket read, if fails returns -1
    int parse(int len, CmdType& cmd, ContentType& ct, td::String& contDataFromFirstRead)
    {
        if (!decodeCommandAndContent(cmd, ct))
        {
            //close();
            //buildErrStr(retData, "Wrong response and content type!");
            return -1;
        }

        int contentLen = (int)getConentLen();
        if (contentLen > 0)
        {
            if (!contDataFromFirstRead.reserve(contentLen))
            {
                //close();
                //buildErrStr(retData, "Cannot allocate space for response!");
                return -1;
            }
        }
        

        int availableContent = len - HEADLEN;
        if (contentLen >= availableContent)
        {
            char* pStr = contDataFromFirstRead.begin();
            //move content data into retData;
            memcpy(pStr, _header+HEADLEN, availableContent);
        }
        else
        {
            return -1;
        }
        return availableContent;
    }

    inline void encodeCommandAndContent(no::CmdType cmdType, ContentType contentType)
    {
        assert(cmdType >= CmdType::CMD_REQ && cmdType < CmdType::CMD_NONE);
        assert(contentType >= ContentType::CT_NULL && contentType < ContentType::CT_NONE);
        memcpy(_header, strCmd[(int)cmdType], 2);
        memcpy(_header+2, strContentType[(int)contentType], 2);
    }

    inline bool decodeCommandAndContent(no::CmdType& cmdType, ContentType& contentType) const
    {

        {
            td::INT2* piCmd = (td::INT2*) _header;
            td::INT2 iCmd = *piCmd;
            const auto it(_toCmdType.find(iCmd));
            if (it != _toCmdType.end())
                cmdType = it->second;
            else
                return false;
        }
        
        {
            char * pStrContent = _header+2;
            td::INT2* piCnt = (td::INT2*) pStrContent;
            td::INT2 iCnt = *piCnt;
            auto it2(_toContentType.find(iCnt));
            if (it2 != _toContentType.end())
                contentType = it2->second;
            else
                return false;
            return true;
        }
        
    }

    inline void encodeContentLen(td::UINT4 len)
    {
        if (_binFormat)
        {
            memcpy(_header + 4, (void*)&len, 4);
        }
        else
        {
            //only 3 bytes will be stored -> limit 16MB of data content in case of txt or xml transfer
            td::UINT4 mask =0x3F; //6 bits
            char* lenData = _header + 4;
            for (int i=3; i>=0; --i)
            {
                td::UINT4 val = len & mask;
                char ch = (char) val + '0';
                lenData[i] = ch;
                len = len >> 6;
            }
        }
    }

    inline unsigned int size() const
    {
        return HEADLEN;
    }

    
    td::UINT4 getConentLen() const
    {
        td::UINT4 toRet = 0;
        if (_binFormat)
        {
            memcpy((void*)&toRet, _header + 4, 4);
        }
        else
        {
            //only 3 bytes were stored -> limit 16MB of data content in case of txt or xml transfer
            char* lenData = _header + 4;
            for (int i=0; i<4; ++i)
            {
                char ch = lenData[i];
                ch -= 48;
                td::UINT4 uVal = (td::UINT4) ch;
                toRet = toRet << 6;
                toRet |= uVal;
            }
        }
        return toRet;
    }
};

} //namespaceno
