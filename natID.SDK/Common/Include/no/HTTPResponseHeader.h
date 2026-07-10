// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HTTPResponseHeader.h
 * @brief HTTP response header parser and accessor for the no networking library.
 */
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

/// @brief Parses and provides access to the fields of an HTTP response header.
class NETOP_API ResponseHeader
{
public:

    /// @brief Name/value pair representing a single HTTP header parameter.
    typedef struct _CP
    {
        const char* name;  ///< Parameter name string.
        const char* value; ///< Parameter value string.
    } Parameter;

protected:
    //char _buff[2048];
    char* _pBuff;                            ///< Pointer to the raw header buffer.
    cnt::Array<Parameter, 32> _params;       //maximum 32 params ///< Array of parsed header parameters (up to 32).
    const char* _serverName;                 ///< Server name extracted from the header.
    const char* _webSocketKey;               ///< WebSocket handshake key extracted from the header.
    td::UINT4 _contentLength;                ///< Content-Length value from the header.
    td::UINT2 _responseVal;                  ///< HTTP response status code.
    td::UINT4 _buffLen;                      ///< Length of the raw header buffer.
    td::UINT2 _processedBytes;               ///< Number of bytes consumed during decoding.
    td::UINT2 _nParams;                      ///< Number of parsed parameters.
    HTTP::Version _version = no::HTTP::Version::v1_1; ///< HTTP protocol version.
    td::BYTE _encoding;                      ///< Content encoding type.
    td::BYTE _keepAlive : 1;                 ///< Flag indicating whether keep-alive is requested.
    td::BYTE _webSocketConnUpgrade : 1;      ///< Flag indicating a WebSocket connection upgrade.
public:
    /// @brief Default constructor; initialises all fields to their default state.
    ResponseHeader();

    /// @brief Decodes an HTTP response header from a raw buffer.
    /// @param buffer Source memory buffer containing the raw HTTP data.
    /// @param pAuxBuff Auxiliary working buffer used during parsing.
    /// @param buffLen Length of the auxiliary buffer in bytes.
    /// @return true if the header was decoded successfully, false otherwise.
    bool decode(mem::Buffer& buffer, char* pAuxBuff, td::UINT4 buffLen);

    /// @brief Returns whether the connection should be kept alive.
    /// @return true if the keep-alive flag is set, false otherwise.
    bool keepAlive() const;

    /// @brief Returns the number of parsed header parameters.
    /// @return Count of parameters present in the response header.
    td::UINT2 getNoOfParams() const;

    /// @brief Returns the content length declared in the header.
    /// @return Content-Length value in bytes.
    td::UINT4 getContentLength() const;

    /// @brief Returns the WebSocket handshake key from the header.
    /// @return Pointer to the null-terminated WebSocket key string, or nullptr if absent.
    const char* getWebSocketKey() const;

    /// @brief Checks whether this is a WebSocket handshake response.
    /// @return true if the response indicates a WebSocket upgrade, false otherwise.
    bool isWebSocketHandshakingResponse() const;

    /// @brief Returns the number of bytes consumed when decoding the header.
    /// @return Byte count processed during the last decode call.
    td::UINT2 getProcessedBytes() const;

    /// @brief Returns a reference to the array of parsed header parameters.
    /// @return Reference to the internal parameter array.
    cnt::Array<Parameter, 32>& getParams() const;

    /// @brief Validates internal consistency of the decoded header.
    /// @return true if the header passes all correctness checks, false otherwise.
    bool checkCorrectness() const;

    /// @brief Returns the HTTP status code from the response line.
    /// @return Numeric HTTP response status code (e.g. 200, 404).
    td::UINT2 getResponseVal() const;
};

} //namespace HTTP

} //namespace no
