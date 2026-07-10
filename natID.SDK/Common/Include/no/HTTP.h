// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HTTP.h
    @brief Declares HTTP utility types, enumerations, and functions for sending HTTP/HTTPS requests and preparing WebSocket upgrade buffers. */
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
/// @brief Enumerates the supported HTTP command types for client requests.
enum class Cmd : unsigned char {Get=0, ///< HTTP GET command.
    Post ///< HTTP POST command.
};

/// @brief Represents an HTTP header attribute as a name-value pair.
typedef struct _Attrib
{
    const char* name; ///< Attribute name string.
    const char* value; ///< Attribute value string.
} Attrib;

/// @brief Interface for consuming raw HTTP response data in chunks.
class IConsumer
{
public:
    /// @brief Called with a chunk of received HTTP response data.
    /// @param buff Pointer to the data buffer.
    /// @param len Number of bytes in the buffer.
    /// @return True to continue receiving data; false to stop.
    virtual bool consume(const char* buff, size_t len) = 0;
};

//standard unsecured (http) operations
/// @brief Sends an HTTP POST request over an unsecured connection using a host name, port string, and command.
/// @param hostName The target host name.
/// @param cmd The POST command path.
/// @param strData The POST body data.
/// @param strPort The port number as a string.
/// @param retData Output string that receives the response body.
/// @return True if the request was sent and a response was received successfully.
NETOP_API bool sendPost(const td::String& hostName, const td::String& cmd, const td::String& strData, const td::String& strPort, td::String& retData);

/// @brief Sends an HTTP POST request over an unsecured connection using a host name and integer port.
/// @param hostName The target host name.
/// @param cmd The POST command path.
/// @param strData The POST body data.
/// @param retData Output string that receives the response body.
/// @param port The port number (default is 80).
/// @return True if the request was sent and a response was received successfully.
NETOP_API bool sendPost(const td::String& hostName, const td::String& cmd, const td::String& strData, td::String& retData, int port = 80);

//standard secured (https)
/// @brief Sends an HTTPS request and retrieves only the HTTP status code.
/// @param cmd The HTTP command (GET or POST).
/// @param url The fully qualified HTTPS URL.
/// @param retCode Output parameter set to the HTTP response status code.
/// @return True if the request was completed successfully.
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, td::UINT4& retCode);

/// @brief Sends an HTTPS request and retrieves the response body as a string.
/// @param cmd The HTTP command (GET or POST).
/// @param url The fully qualified HTTPS URL.
/// @param retData Output string that receives the response body.
/// @param retCode Output parameter set to the HTTP response status code.
/// @param pHeaderAttribs Optional pointer to additional HTTP header attributes to include.
/// @return True if the request was completed successfully.
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, td::String& retData, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

/// @brief Sends an HTTPS request and delivers the response body to a consumer callback.
/// @param cmd The HTTP command (GET or POST).
/// @param url The fully qualified HTTPS URL.
/// @param pConsumer Pointer to the IConsumer implementation that receives response chunks.
/// @param retCode Output parameter set to the HTTP response status code.
/// @param pHeaderAttribs Optional pointer to additional HTTP header attributes to include.
/// @return True if the request was completed successfully.
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String& url, HTTP::IConsumer* pConsumer, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

/// @brief Sends an HTTPS request with a payload and retrieves the response body as a string.
/// @param cmd The HTTP command (GET or POST).
/// @param url The fully qualified HTTPS URL.
/// @param payLoadToSend The request body payload to send.
/// @param retData Output string that receives the response body.
/// @param retCode Output parameter set to the HTTP response status code.
/// @param pHeaderAttribs Optional pointer to additional HTTP header attributes to include.
/// @return True if the request was completed successfully.
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String & url, const td::String& payLoadToSend, td::String& retData, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

/// @brief Sends an HTTPS request with a payload and delivers the response to a consumer callback.
/// @param cmd The HTTP command (GET or POST).
/// @param url The fully qualified HTTPS URL.
/// @param payLoadToSend The request body payload to send.
/// @param pConsumer Pointer to the IConsumer implementation that receives response chunks.
/// @param retCode Output parameter set to the HTTP response status code.
/// @param pHeaderAttribs Optional pointer to additional HTTP header attributes to include.
/// @return True if the request was completed successfully.
NETOP_API bool sendSecured(no::HTTP::Cmd cmd, const td::String & url, const td::String& payLoadToSend, HTTP::IConsumer* pConsumer, td::UINT4& retCode, cnt::SafeFullVector<HTTP::Attrib>* pHeaderAttribs = nullptr);

//custom web client-server communnication

//enum class SocketAction : td::INT2 {Normal = 0, ReadingTimeOut, Close};
/// @brief Enumerates the actions that can be taken on a socket during HTTP/WebSocket communication.
enum class Action : td::INT2 {Normal = 0, ///< Normal operation; continue processing.
    WSLoginStage1, ///< Initiate WebSocket login stage 1.
    WSLoginStage2, ///< Perform WebSocket login stage 2.
    WSLoginStage3, ///< Perform WebSocket login stage 3.
    WSLoggedIn, ///< WebSocket login is complete.
    ReadingTimeOut, ///< A read operation timed out.
    WSClientUpgrade, ///< Upgrade the connection to a WebSocket.
    WSPingSent, ///< A WebSocket ping was sent.
    Close ///< Close the connection.
};

/// @brief Prepares an HTTP response buffer with the given result data and content type.
/// @param pResult Pointer to the response body data.
/// @param nResultLen Length of the response body data in bytes.
/// @param buffer Output buffer that receives the formatted HTTP response.
/// @param timeOutInSec Keep-alive timeout value in seconds to include in the response headers.
/// @param contentType MIME content type string for the response (default: "text;charset=utf-8").
NETOP_API void prepareResponseBuffer(const char* pResult, size_t nResultLen, mem::Buffer& buffer, td::UINT2 timeOutInSec, const char* contentType = "text;charset=utf-8");

/// @brief Prepares an HTTP request buffer for the given request type and parameters.
/// @param buffer Output buffer that receives the formatted HTTP request.
/// @param request The HTTP request method (GET or POST).
/// @param httpRequestLink The request path/link.
/// @param params Optional pointer to an array of request parameter variants.
/// @param nParams Number of parameters in the params array.
/// @param pContent Optional pointer to the request body content.
/// @param contentLen Length of the request body content in bytes.
/// @param contentType MIME content type string for the request body (default: "text;charset=utf-8").
/// @return True if the request buffer was prepared successfully.
NETOP_API bool prepareRequestBuffer(mem::Buffer& buffer, no::HTTP::Request request, const char* httpRequestLink, const td::Variant* params = nullptr, size_t nParams = 0, const char* pContent = nullptr, size_t contentLen = 0, const char* contentType = "text;charset=utf-8");

/// @brief Prepares an HTTP Upgrade request buffer to initiate a WebSocket handshake.
/// @param buffer Output buffer that receives the formatted upgrade request.
/// @param hostName The target host name.
/// @param httpRequestLink The WebSocket endpoint path.
/// @param wsKey A 16-byte WebSocket key used in the handshake.
NETOP_API void prepareWebSocketUpgradeRequestBuffer(mem::Buffer& buffer, const char* hostName, const char* httpRequestLink, const char wsKey[16]/*, const td::Variant* params = nullptr, size_t nParams = 0*/);

} ///namespace HTTP
} //namespace no
