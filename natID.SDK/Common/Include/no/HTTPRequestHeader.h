// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HTTPRequestHeader.h
    @brief Declares the HTTP RequestHeader class for parsing and inspecting incoming HTTP request headers. */
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

/// @brief Parses and provides access to the fields of an incoming HTTP request header.
class NETOP_API RequestHeader
{
public:

    /// @brief Represents a single HTTP header parameter as a name-value pair.
    typedef struct _CP
    {
        const char* name; ///< Parameter name string.
        const char* value; ///< Parameter value string.
    } Parameter;

protected:
    //char _buff[2048];
    char* _pBuff; ///< Pointer to the raw header buffer used for in-place parsing.
    cnt::Array<Parameter, 32> _params; ///< Parsed HTTP header parameters (maximum 32 entries).
    cnt::Array<td::Variant, 64> _requestParams; ///< Parsed URL query parameters (maximum 64 entries).
    const char* _hostNameAndPort; ///< Pointer to the Host header value (host name and optional port).
    const char* _webSocketKey; ///< Pointer to the Sec-WebSocket-Key header value, if present.
    const char* _request; ///< Pointer to the request path string.
    td::UINT4 _contentLength; ///< Value of the Content-Length header field.
    td::UINT4 _buffLen; ///< Total length of the raw header buffer.
    td::UINT2 _processedBytes; ///< Number of bytes consumed during header parsing.
    td::UINT2 _nParams; ///< Number of parsed HTTP header parameters.
    td::UINT2 _nRequestParams; ///< Number of parsed URL query parameters.
    HTTP::Request _requestType; ///< Decoded HTTP request method (GET, POST, etc.).
    HTTP::Version _version = no::HTTP::Version::v1_1; ///< Decoded HTTP protocol version.
    td::BYTE _keepAlive : 1; ///< Bitfield flag indicating whether the keep-alive connection mode was requested.
    td::BYTE _webSocketConnUpgrade : 1; ///< Bitfield flag indicating whether the request contains a WebSocket upgrade.

    /// @brief Determines the data type of a value found in the request URL parameters.
    /// @param pBuff Pointer to the raw parameter value string.
    /// @return The inferred td::DataType for the value.
    td::DataType getDataType(const char* pBuff) const;

    /// @brief Decodes and parses the HTTP request line from the buffer.
    /// @param pBuff Pointer to the raw header buffer.
    /// @return True if the request line was parsed successfully.
    bool decodeRequest(const char* pBuff);

    /// @brief Parses a single parameter value from the buffer into a Variant.
    /// @param pBuff Pointer to the raw parameter value string.
    /// @param val Output Variant that receives the parsed value.
    void getValue(const char* pBuff, td::Variant& val);

    /// @brief Checks whether the parsed header fields are consistent and valid.
    /// @return True if the header is considered correct and complete.
    bool checkCorrectness() const;
public:
    /// @brief Default constructor. Initializes all members to their default state.
    RequestHeader();

    /// @brief Decodes an HTTP request header from a network buffer.
    /// @param buffer The memory buffer containing the received HTTP data.
    /// @param pAuxBuff Pointer to an auxiliary working buffer used during parsing.
    /// @param buffLen Length of the auxiliary buffer in bytes.
    /// @return True if the header was decoded successfully.
    bool decode(mem::Buffer& buffer, char* pAuxBuff, td::UINT4 buffLen);

    /// @brief Returns the HTTP request method type.
    /// @return The decoded HTTP::Request type (e.g., GET or POST).
    HTTP::Request getRequestType() const;

    /// @brief Returns the request path string.
    /// @return Pointer to the null-terminated request path.
    const char* getRequest() const;

    /// @brief Checks whether the HTTP keep-alive flag was set in the request.
    /// @return True if keep-alive was requested.
    bool keepAlive() const;

    /// @brief Returns the number of parsed HTTP header parameters.
    /// @return The count of parsed header name-value parameters.
    td::UINT2 getNoOfParams() const;

    /// @brief Returns the number of parsed URL query parameters.
    /// @return The count of parsed request (URL query) parameters.
    td::UINT2 getNoOfRequestParams() const;

    /// @brief Returns the array of parsed URL query parameter variants.
    /// @return Const reference to the array of request parameter values.
    const cnt::Array<td::Variant, 64>& getRequestParams() const;

    /// @brief Returns the value of the Content-Length header field.
    /// @return The content length in bytes, or 0 if not present.
    td::UINT4 getContentLength() const;

    /// @brief Returns the value of the Sec-WebSocket-Key header field.
    /// @return Pointer to the WebSocket key string, or nullptr if not present.
    const char* getWebSocketKey() const;

    /// @brief Checks whether the request is a WebSocket upgrade handshake.
    /// @return True if the request contains a valid WebSocket upgrade.
    bool isWebSocketInitHandshaking() const;

    /// @brief Returns the number of bytes consumed during header parsing.
    /// @return The count of processed bytes.
    td::UINT2 getProcessedBytes() const;
};

} //namespace HTTP

} //namespace no
