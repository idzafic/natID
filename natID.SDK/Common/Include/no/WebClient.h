// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WebClient.h
 * @brief HTTP and WebSocket client with asynchronous I/O support.
 */
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
#include <no/WSClientManager.h>
#include <no/HTTP.h>
#include <no/HTTPResponseHeader.h>
#include <cnt/SafeFullVector.h>

namespace no
{

/// @brief Asynchronous HTTP and WebSocket client that manages a pool of HTTP connections
///        and delegates WebSocket traffic to an optional WSClientManager.
class NETOP_API WebClient : protected no::IProsumer
{
//    friend class WSClientManager;

    /// @brief Holds state for a single HTTP connection entry.
    class Entry
    {
    public:
        no::SockDesc sockDesc; ///< Socket descriptor for this HTTP connection.
        td::String host;       ///< Remote host name.
        td::UINT2 port;        ///< Remote port number.
    };

protected:
    no::ICommunicator* _pCommunicator = nullptr;          ///< Underlying asynchronous communicator.
    no::WSClientManager* _pWebSocketManager = nullptr;    ///< Optional WebSocket manager for WS connections.
    cnt::SafeFullVector<Entry> _httpConnections;          ///< Pool of HTTP connection entries.
    td::UINT2 _timeOutInMilliSeconds;                     ///< Request/connection timeout in milliseconds.
    td::BYTE _prosumerPos = 0;                            //1 based, 0-means not used ///< IProsumer position index (1-based; 0 = unused).

protected:

    /// @brief Called when an HTTP socket connects; initialises the entry.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param entryPos Index of the corresponding entry slot.
    void onConnect(no::SockDesc& sockDesc, td::UINT2 entryPos);

    /// @brief Called when an HTTP connection is disconnected.
    /// @param sockDesc Descriptor of the closed socket.
    /// @param reason Reason for the disconnection.
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Returns a pointer to the first unused HTTP entry slot, or nullptr if full.
    /// @return Pointer to an empty Entry, or nullptr.
    Entry* getEmptyEntry();

    /// @brief Parses and dispatches an incoming WebSocket frame.
    /// @param pAuxBuff Auxiliary working buffer.
    /// @param auxBuffLen Size of the auxiliary buffer.
    /// @param sockDesc Descriptor of the connection.
    /// @param buffer Buffer containing raw WebSocket data.
    /// @param varUserData User data from the read operation.
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData);

    //IProsumer
    /// @brief Handles the initial TCP connect callback.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param userData Variant carrying optional user data.
    /// @return true to accept the connection, false to reject it.
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;

    /// @brief Queries whether the connection should be closed.
    /// @param sockDesc Descriptor under consideration.
    /// @param reason Reason triggering the check.
    /// @return true to disconnect, false to keep alive.
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called periodically when the event loop is idle.
    void onIdle() override;

    /// @brief Called when an exact-size buffer read completes on an HTTP socket.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param userData User data from the read.
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData) override;

    /// @brief Called when a write operation completes on an HTTP socket.
    /// @param sockDesc Descriptor of the socket.
    /// @param userData User data from the write.
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& userData) override;

    /// @brief Called when a partial buffer read completes on an HTTP socket.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param userData User data from the read.
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData) override;

    /// @brief Requests a graceful shutdown of the underlying communicator.
    void shutDown();

    /// @brief Associates a WSClientManager for handling WebSocket upgrade connections.
    /// @param pWebSocketManager Pointer to the WSClientManager to use.
    void setWebSocketManager(no::WSClientManager* pWebSocketManager);

public:
    /// @brief Constructs a WebClient with the given HTTP-connection pool size and timeout.
    /// @param nMaxHTTPConnections Maximum number of simultaneous HTTP connections (0 = no HTTP).
    /// @param timeOutInMilliSeconds Keep-alive / request timeout in milliseconds.
    WebClient(td::UINT2 nMaxHTTPConnections = 0, td::UINT2 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut);

    /// @brief Destructor; stops the communicator and releases resources.
    ~WebClient();

    /// @brief Starts the client's internal event loop in the calling thread.
    /// @return true if the event loop started successfully, false otherwise.
    bool run();

    /// @brief Initiates an asynchronous TCP connection to the given server.
    /// @param serverAddress Null-terminated server hostname or IP address.
    /// @param serverPort Remote port number.
    /// @return true if the connection attempt was queued, false otherwise.
    bool asyncConnect(const char* serverAddress, td::UINT2 serverPort);

    /// @brief Requests the client to stop its event loop.
    void stop();

    /// @brief Returns whether the client's event loop is currently active.
    /// @return true if running, false otherwise.
    bool isRunning() const;

    /// @brief Sends an HTTP request over an established connection.
    /// @param sockDesc Descriptor of the connection to use.
    /// @param request HTTP method/request type.
    /// @param httpRequestLink Null-terminated request path/resource string.
    /// @param params Optional array of variant parameters appended to the request.
    /// @param nParams Number of parameters in @p params.
    /// @param pContent Optional pointer to the request body data.
    /// @param contentLen Length of the request body in bytes.
    /// @param contentType MIME type string for the request body.
    /// @param varUserData Optional user data forwarded to the completion callback.
    /// @return true if the request was queued successfully, false otherwise.
    bool sendRequest(const no::SockDesc& sockDesc, no::HTTP::Request request, const char* httpRequestLink, const td::Variant* params = nullptr, size_t nParams = 0, const char* pContent = nullptr, size_t contentLen = 0, const char* contentType = "text;charset=utf-8", const td::Variant& varUserData=td::Variant());

    /// @brief Returns a pointer to the associated WSClientManager, if any.
    /// @return Pointer to the WSClientManager, or nullptr if none is set.
    no::WSClientManager* getWSClientManager();

    /// @brief Returns an empty buffer obtained from the underlying communicator.
    /// @return An empty mem::Buffer ready for use.
    mem::Buffer getEmtypBuffer();
};

} //namespace no
