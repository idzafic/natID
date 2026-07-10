// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WebServer.h
 * @brief Asynchronous HTTP and WebSocket server with static resource and dynamic request support.
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
//#include <fo/FileOperations.h>
#include <map>
#include <no/Socket.h>
#include <no/HTTP.h>
#include <no/WSServerManager.h>
#include <no/HTTPRequestHeader.h>
#include <functional>

namespace no
{

#define MAX_NO_SERVER_CONNECTIONS 64       ///< Default maximum number of simultaneous server connections.
#define MAX_NO_WEBCHATSERER_CONNECTIONS 64 ///< Default maximum connections for the WebChat server variant.

/// @brief Asynchronous HTTP server that serves static resources, handles dynamic requests,
///        and optionally upgrades connections to WebSocket via a WSServerManager.
class NETOP_API WebServer : protected no::IProsumer
{
    /// @brief Holds a static resource buffer along with its MIME type.
    class Resource
    {
    public:
        /// @brief MIME type of a static resource.
        enum class Type {HTML=0, PNG, JPG, SVG};
        mem::Buffer buffer; ///< Buffer containing the resource data.
        Type type;          ///< MIME type of the resource.
    };
protected:
    no::ICommunicator* _pCommunicator = nullptr;  ///< Underlying asynchronous communicator.
    WSServerManager* _pWebSocketManager = nullptr; ///< Optional WebSocket manager for upgrade connections.
    //fo::fs::path _resRelativePath;
    cnt::SafeFullVector<Resource> _resources;      ///< Vector of loaded static resources.
    std::map<td::String, mem::Buffer*> _mapResources; ///< Map from URL path to a static resource buffer.
    std::map<td::String, std::function<bool (const td::Variant*, size_t, mem::Buffer&)> > _mapDynamicRequests; ///< Map from request key to dynamic request handler.
    td::String _rootPage;                          ///< Content of the root (index) page.
    td::String _strErr400BadRequest;               ///< Pre-formatted 400 Bad Request error response.
    td::String _strErr400MaxBuff;                  ///< Pre-formatted 400 error response for exceeded buffer size.
    td::String _strErr404NotFound;                 ///< Pre-formatted 404 Not Found error response.
    td::String _strErr408RequestTimeOut;           ///< Pre-formatted 408 Request Timeout error response.

    //fo::OutFile _outFile;
    td::UINT4 _nRes = 0;                           ///< Number of loaded static resources.
    td::UINT2 _timeOutInMilliSeconds;              ///< Keep-alive / request timeout in milliseconds.

protected:

    /// @brief Builds all pre-formatted HTTP error response strings.
    void formatErrStrings();

    /// @brief Called periodically when the event loop is idle.
    void onIdle() override;

    /// @brief Builds the HTTP 101 Switching Protocols response for a WebSocket upgrade.
    /// @param header Parsed HTTP request header containing the upgrade keys.
    /// @param buffer Output buffer to receive the response.
    /// @return true if the response was built successfully, false otherwise.
    bool createWebSocketInitialHandshakeResponse(const no::HTTP::RequestHeader& header, mem::Buffer& buffer);

    /// @brief Registers a dynamic request handler for a named resource path.
    /// @tparam TMEMBFN Member function pointer type of the handler.
    /// @tparam TOBJ Object type that owns the handler member function.
    /// @param reqName Null-terminated resource name (without leading '/').
    /// @param pParamTypes Array of expected parameter data types.
    /// @param nParams Number of parameters (1..64).
    /// @param memFn Member function pointer to the handler.
    /// @param pObj Pointer to the object on which @p memFn will be invoked.
    /// @return true if the handler was registered, false if the name already exists.
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
    /// @brief Handles the initial TCP connect callback.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param userData Variant carrying optional user data.
    /// @return true to accept the connection, false to reject it.
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;

    /// @brief Called after a TCP connection has been accepted.
    /// @param sockDesc Descriptor of the connected socket.
    void onConnect(no::SockDesc& sockDesc, const td::Variant&) override;

    /// @brief Queries whether the connection should be closed.
    /// @param sockDesc Descriptor under consideration.
    /// @param reason Reason triggering the check.
    /// @return true to disconnect, false to keep alive.
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when a connection has been fully disconnected.
    /// @param sockDesc Descriptor of the closed socket.
    /// @param reason Reason for the disconnection.
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when an exact-size buffer read completes (full HTTP request received).
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with the received HTTP data.
    /// @param varUserData User data from the read.
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Upgrades an HTTP connection to WebSocket using the provided request header.
    /// @param sockDesc Descriptor of the connection to upgrade.
    /// @param buffer Buffer to populate with the upgrade response.
    /// @param header Parsed HTTP request header.
    void upgradeToWebSocket(const no::SockDesc& sockDesc, mem::Buffer& buffer, const no::HTTP::RequestHeader& header);

    //getting data from header
    /// @brief Called when a partial buffer read completes (initial header bytes available).
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with the received data.
    /// @param varUserData User data from the read.
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Called when a write operation completes.
    /// @param sockDesc Descriptor of the socket.
    /// @param varUserData User data from the write.
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;

    /// @brief Loads static resources from disk relative to the given config path.
    /// @param configPath Base path from which relative resource paths are resolved.
    /// @param relativeResPath Relative sub-path to the resource directory.
    /// @return true if all resources were loaded successfully, false otherwise.
    bool loadResources(const td::String& configPath, const char* relativeResPath);

    /// @brief Override point: called once to allow subclasses to load custom resources.
    /// @return true on success, false on failure.
    virtual bool onLoadResources();

public:

    /// @brief Constructs a WebServer with the given keep-alive timeout.
    /// @param timeOutInMilliSeconds Keep-alive / request timeout in milliseconds.
    WebServer(td::UINT2 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut);

    /// @brief Associates a WSServerManager for handling WebSocket connections.
    /// @param pWSManager Pointer to the WSServerManager to use.
    void setWebSocketManager(WSServerManager* pWSManager);

    /// @brief Processes a single configuration key/value entry (e.g. from a config file).
    /// @param pKey Null-terminated configuration key string.
    /// @param keyLen Length of the key string.
    /// @param pVal Null-terminated configuration value string.
    /// @param valLen Length of the value string.
    /// @param relativeResPath Relative path used to resolve resource references in the value.
    /// @param compressionLevel Compression level to apply when loading resources.
    /// @return true if the entry was handled, false otherwise.
    bool onConfigEntry(const char* pKey, size_t keyLen, const char* pVal, size_t valLen, const char* relativeResPath, int compressionLevel);

    /// @brief Starts listening on the given address and port.
    /// @param localAddress Null-terminated local IP address to bind (nullptr for INADDR_ANY).
    /// @param serverPort Local port number to listen on.
    /// @param nMaxConnections Maximum number of simultaneous client connections.
    /// @return true if the server started successfully, false otherwise.
    bool run(const char* localAddress, td::UINT2 serverPort, td::UINT4 nMaxConnections = MAX_NO_SERVER_CONNECTIONS);

    /// @brief Returns whether the server's event loop is currently active.
    /// @return true if running, false otherwise.
    bool isRunning() const;

    /// @brief Stops the server and closes the listening socket.
    void stop();
};

}
