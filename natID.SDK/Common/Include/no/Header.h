// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Header.h
    @brief Defines the binary/text protocol Header template class and associated command and content type enumerations. */
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

/// @brief Enumerates the types of commands used in the protocol header.
enum class CmdType : unsigned char { CMD_REQ=0, ///< Request command.
    CMD_RET_OK=1, ///< Successful return command.
    CMD_RET_NOK=2, ///< Failed return command.
    CMD_FN_NO_CNT=3, ///< Function command with no content.
    CMD_NONE=4 ///< Sentinel value; not a valid command type.
};
const char * const strCmd[] = {"RQ",       "OK",           "NO",         "FN"}; ///< String representations of CmdType values.

/// @brief Enumerates the content encoding types used in the protocol header.
enum class ContentType : unsigned char {CT_NULL=0, ///< No content.
    CT_XML=1, ///< XML content.
    CT_BIN=2, ///< Binary content.
    CT_BINXML=3, ///< Binary-encoded XML content.
    CT_ZIP=4, ///< ZIP-compressed content.
    CT_ZIP_XML=5, ///< ZIP-compressed XML content.
    CT_ZIP_BIN=6, ///< ZIP-compressed binary content.
    CT_ZIP_BINXML=7, ///< ZIP-compressed binary-encoded XML content.
    CT_NONE=8 ///< Sentinel value; not a valid content type.
} ;
const char * const strContentType[] = {"na",    "xm",     "bn",       "bx",       "zp",       "zx",        "zb",        "zl"}; ///< String representations of ContentType values.


//HEADER
//header is at least 16 bytes long
//00-01 bytes: CmdType string
//02-03 bytes: Content type string
//04-07 bytes: Content length string binary or text encoded with own 64-base encoding
//08-15 bytes: Extra data in case of return command with number only
//template <bool binFormat>
/// @brief Template class for encoding and decoding a fixed-size protocol message header.
/// @tparam HEADLEN The total length of the header in bytes.
template <int HEADLEN>
class Header
{
    char* _header; ///< Pointer to the raw header buffer.
    bool _binFormat; ///< True if the header uses binary encoding; false for text (base-64) encoding.
    std::map<td::INT2, ContentType> _toContentType; ///< Map from 2-byte encoded string to ContentType enum value.
    std::map<td::INT2, CmdType> _toCmdType; ///< Map from 2-byte encoded string to CmdType enum value.

    /// @brief Populates the internal lookup maps for command type and content type decoding.
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
    /// @brief Constructs a Header without an associated buffer.
    /// @param binFormat True to use binary encoding for content length; false to use text (base-64) encoding.
    Header(bool binFormat = false)
        : _header(0)
        , _binFormat(binFormat)
    {
        populateMaps();
    }

    /// @brief Constructs a Header with a pre-allocated buffer.
    /// @param pHeader Pointer to the buffer that holds the raw header bytes.
    /// @param binFormat True to use binary encoding for content length; false to use text (base-64) encoding.
    Header(char* pHeader, bool binFormat = false)
        : _header(pHeader)
        , _binFormat(binFormat)
    {
        populateMaps();
    }

    /// @brief Fills the entire header buffer with a given character.
    /// @param ch The character to fill the buffer with.
    void clean(char ch)
    {
        memset(_header, ch, HEADLEN);
    }

    /// @brief Sets the header buffer pointer to the given memory location.
    /// @param pHeader Pointer to the buffer to use as the header.
    void use(char* pHeader)
    {
        _header = pHeader;
    }

    //returns number of bytes read from first socket read, if fails returns -1
    /// @brief Parses the header and extracts command type, content type, and any content bytes already read.
    /// @param len Total number of bytes read from the socket so far.
    /// @param cmd Output parameter set to the decoded command type.
    /// @param ct Output parameter set to the decoded content type.
    /// @param contDataFromFirstRead Output string that receives the content bytes already in the buffer.
    /// @return Number of content bytes available after the header, or -1 on failure.
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

    /// @brief Encodes the command type and content type into the header buffer.
    /// @param cmdType The command type to encode.
    /// @param contentType The content type to encode.
    inline void encodeCommandAndContent(no::CmdType cmdType, ContentType contentType)
    {
        assert(cmdType >= CmdType::CMD_REQ && cmdType < CmdType::CMD_NONE);
        assert(contentType >= ContentType::CT_NULL && contentType < ContentType::CT_NONE);
        memcpy(_header, strCmd[(int)cmdType], 2);
        memcpy(_header+2, strContentType[(int)contentType], 2);
    }

    /// @brief Decodes the command type and content type from the header buffer.
    /// @param cmdType Output parameter set to the decoded command type.
    /// @param contentType Output parameter set to the decoded content type.
    /// @return True if both command type and content type were decoded successfully; false otherwise.
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

    /// @brief Encodes the content length into the header buffer.
    /// @param len The content length value to encode.
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

    /// @brief Returns the total size of the header in bytes.
    /// @return The header size determined by the HEADLEN template parameter.
    inline unsigned int size() const
    {
        return HEADLEN;
    }

    /// @brief Decodes and returns the content length from the header buffer.
    /// @return The content length encoded in the header.
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
