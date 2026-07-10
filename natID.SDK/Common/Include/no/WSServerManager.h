// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WSServerManager.h
 * @brief WebSocket server connection manager with optional authentication and broadcast support.
 */
//
//  WebSocketManager.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/SockDesc.h>
#include <td/String.h>
#include <td/Variant.h>
#include <mem/Buffer.h>
#include <cnt/SafeFullVector.h>
#include <no/ICommunicator.h>
#include <no/WebSocket.h>
//#include <cnt/Array.h>

namespace no
{
class WebServer;
class WebClient;

/// @brief Manages a pool of incoming WebSocket connections on the server side, including
///        upgrade handshake and optional authentication.
class NETOP_API WSServerManager : protected no::IProsumer
{
    friend class WebServer;
    //friend class WebClient;

protected:
    /// @brief Server-side decision in response to a client's WebSocket upgrade request.
    enum class UpgradeRequest : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication};

    //enum class UpgradeResponse : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication}; //client

    /// @brief Holds all state associated with a single server-side WebSocket connection.
    class Entry
    {
    public:
        no::SockDesc sockDesc;                                           ///< Socket descriptor for this connection.
        td::DateTime lastPingOrActivity;                                 ///< Timestamp of the last ping or data activity.
        unsigned char loginSalt[80];                                     ///< Salt used for authentication.
        TSockPos userPos = 0;                                            ///< Application-level user position (0 = anonymous).
        HTTP::WebSocket::State connectionState = HTTP::WebSocket::State::NotConnected; ///< Current handshake/connection state.
        //td::BYTE loggedIn = 0;
        td::BYTE customClient = 0;                                       ///< Non-zero if this is a custom (non-standard) client.
    };

    no::ICommunicator* _pCommunicator = nullptr;  ///< Pointer to the underlying communicator.
    cnt::SafeFullVector<Entry> _entries;           ///< Pool of connection entries.
    TSockPos _nMaxConnections = 0;                 ///< Maximum number of simultaneous connections.
    TSockPos _nActiveConnections = 0;              ///< Number of currently active connections.
    td::UINT2 _timeOut = 20000;                    ///< Idle/ping timeout in milliseconds.
    td::BYTE _consumerPos = 1;                     ///< IProsumer position index for this manager.
    //td::BYTE _sendHashOnInit : 1;
private:
    //WebSocket::Frame decode(const no::SockDesc& sockDesc, mem::Buffer& buffer, char* pBuffer, size_t buffLen, td::UINT4& startingPos, td::UINT4& stillToProcess, td::UINT4& msgLen);
    //bool encode(mem::Buffer& buffer, const char* pMsg, size_t msgLen, bool appendToBuff = false) const;
    //bool encode(mem::Buffer& buffer, const WebSocket::Message* pMsgParts, size_t nParts, bool appendToBuff = false) const;

    //td::UINT2 getWebSocketPos(const no::SockDesc& sockDesc) const;

    /// @brief Processes the initial WebSocket upgrade HTTP request.
    /// @param sockDesc Descriptor of the upgrading socket.
    /// @param buffer Buffer containing the HTTP request data.
    /// @param request Null-terminated request path/resource string.
    /// @param params Optional variant parameters accompanying the request.
    /// @param nParams Number of parameters.
    /// @return true if the upgrade was initialised successfully, false otherwise.
    bool initConnection(no::SockDesc& sockDesc, mem::Buffer& buffer, const char* request, const td::Variant*params, size_t nParams);

    /// @brief Dispatches a received WebSocket message and returns the owning entry.
    /// @param sockDesc Descriptor of the connection.
    /// @param wsMessage Received message.
    /// @param varUserData User data from the read operation.
    /// @param pEntry Output pointer to the Entry that owns the connection.
    /// @return true if the message was handled, false otherwise.
    bool handleMsgRead(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage, const td::Variant& varUserData, Entry*& pEntry);

    /// @brief Handles completion of a sent WebSocket message.
    /// @param sockDesc Descriptor of the connection.
    /// @param varUserData User data from the write operation.
    void handleMsgSent(const no::SockDesc& sockDesc, const td::Variant& varUserData);

    /// @brief Handles a disconnection event for a server-side entry.
    /// @param sockDesc Descriptor of the disconnected socket.
    /// @param reason Reason for the disconnection.
    void handleDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason);

    /// @brief Processes idle time (e.g. sends pings, cleans up stale entries).
    void handleIdle();

    /// @brief Resets all connection entries to their empty state.
    void cleanEntries();

    /// @brief Returns a pointer to the first unused entry slot, or nullptr if full.
    /// @return Pointer to an empty Entry, or nullptr.
    Entry* findEmptyEntry();

    /// @brief Resets a single entry to its default empty state.
    /// @param pEntry Pointer to the entry to clean.
    void cleanEntry(Entry* pEntry);

    /// @brief Broadcasts a buffer to all active connections.
    /// @param buffer Buffer to send to all entries.
    void sendToAll(mem::Buffer& buffer);

    /// @brief Sends a WebSocket ping to all active connections.
    void sendPing();

    /// @brief Sends a WebSocket pong in response to a received ping.
    /// @param sockDesc Descriptor of the connection that sent the ping.
    /// @param wsMessage The ping message to respond to.
    void sendPong(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage);

    /// @brief Validates a received pong against the outstanding ping.
    /// @param sockDesc Descriptor of the connection.
    /// @param wsMessage The pong message received.
    /// @return true if the pong is valid, false otherwise.
    bool checkPong(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage);

    /// @brief Closes a connection entry and notifies of the reason.
    /// @param pEntry Pointer to the entry to close.
    /// @param reason Reason for closing.
    void closeEntry(Entry* pEntry, no::DisconnectReason reason);

    /// @brief Returns the custom-client flag for the given socket.
    /// @param sockDesc Descriptor to query.
    /// @return Non-zero if the connection is from a custom client.
    td::BYTE isCustomClient(const no::SockDesc& sockDesc) const;

    /// @brief Parses and dispatches a complete WebSocket frame from the buffer.
    /// @param pAuxBuff Auxiliary working buffer.
    /// @param auxBuffLen Size of the auxiliary buffer.
    /// @param sockDesc Descriptor of the connection.
    /// @param buffer Buffer containing raw WebSocket data.
    /// @param varUserData User data associated with the operation.
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData);
    //UpgradeResponse handleUpgradeResponse(const no::SockDesc& sockDesc);
protected:
    /// @brief Override point: called when a client requests a WebSocket upgrade.
    /// @param request Null-terminated request resource path.
    /// @param params Optional variant parameters accompanying the request.
    /// @param nParams Number of parameters.
    /// @return UpgradeRequest decision (reject, accept with or without authentication).
    virtual UpgradeRequest onUpgradeRequest(const char* request, const td::Variant*params, size_t nParams);
    //virtual UpgradeResponse onUpgradeResponse(const no::SockDesc& sockDesc);

    /// @brief Finds the Entry for the given socket descriptor.
    /// @param sockDesc Descriptor to search for.
    /// @return Pointer to the matching Entry, or nullptr.
    Entry* findEntry(const no::SockDesc& sockDesc);

    /// @brief Finds the Entry for the given socket descriptor (const version).
    /// @param sockDesc Descriptor to search for.
    /// @return Const pointer to the matching Entry, or nullptr.
    const Entry* findEntry(const no::SockDesc& sockDesc) const;

    /// @brief Override point: called after a client WebSocket connection is fully established.
    /// @param pEntry Pointer to the connected Entry.
    /// @return true to accept the connection, false to close it immediately.
    virtual bool onConnect(const WSServerManager::Entry* pEntry);

    /// @brief Override point: called when a WebSocket message has been received.
    /// @param pEntry Pointer to the Entry that received the message.
    /// @param wsMessage The received message.
    /// @param varUserData User data from the read operation.
    /// @return true if the message was processed successfully, false otherwise.
    virtual bool onMsgRead(const WSServerManager::Entry* pEntry, const WebSocket::Message& wsMessage, const td::Variant& varUserData);

    /// @brief Override point: called when a WebSocket message has been fully sent.
    /// @param pEntry Pointer to the Entry on which the message was sent.
    /// @param varUserData User data from the write operation.
    virtual void onMsgSent(const WSServerManager::Entry* pEntry, const td::Variant& varUserData);

    /// @brief Override point: called when a WebSocket connection is disconnected.
    /// @param pEntry Pointer to the disconnected Entry.
    /// @param reason Reason for the disconnection.
    virtual void onDisconnect(const WSServerManager::Entry* pEntry, no::DisconnectReason reason);

    /// @brief Override point: resolves a username to an application user position and password.
    /// @param pUserName Pointer to the username string.
    /// @param nameLen Length of the username string.
    /// @param pwd Output string to receive the expected password.
    /// @return Application-level user position, or 0 if not found.
    virtual TSockPos findUser(const char* pUserName, size_t nameLen, td::String& pwd);
    //virtual TSockPos findUser(const char* pUserName, size_t nameLen);

    //IProsumer
    /// @brief Handles the initial TCP connect for server-side upgrade.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param userData Variant carrying optional user data.
    /// @return true if handling succeeded, false otherwise.
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;

    /// @brief Called after a socket connection has been accepted.
    /// @param sockDesc Descriptor of the connected socket.
    /// @param userData User-defined data.
    void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) override;

    /// @brief Queries whether the connection should be closed.
    /// @param sockDesc Descriptor under consideration.
    /// @param reason Reason triggering the check.
    /// @return true to disconnect, false to keep alive.
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when a connection has been fully disconnected.
    /// @param sockDesc Descriptor of the closed socket.
    /// @param reason Reason for the disconnection.
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when an exact-size buffer read has completed.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param varUserData User data from the read.
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Called when a partial buffer read has completed.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param varUserData User data from the read.
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Called when a buffer has been fully sent.
    /// @param sockDesc Descriptor of the socket.
    /// @param varUserData User data from the write.
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;

    /// @brief Called periodically when the event loop is idle.
    void onIdle() override;
    //end IProsumer

public:
    /// @brief Constructs a WSServerManager that can hold up to @p maxConnections entries.
    /// @param maxConnections Maximum number of simultaneous WebSocket connections.
    WSServerManager(TSockPos maxConnections);

    /// @brief Sends a WebSocket message to a specific client.
    /// @param sockDescDest Descriptor identifying the destination client.
    /// @param wsMessage Message to send.
    /// @param varUserData Optional user data forwarded to the send-completion callback.
    void sendMessage(const no::SockDesc& sockDescDest, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());

    /// @brief Broadcasts a WebSocket message to all clients except the source.
    /// @param sockDescSource Descriptor of the originating client (excluded from broadcast).
    /// @param wsMessage Message to broadcast.
    /// @param varUserData Optional user data.
    void sendMessageToAll(const no::SockDesc& sockDescSource, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());

    /// @brief Broadcasts a multi-part WebSocket message to all clients except the source.
    /// @param sockDescSource Descriptor of the originating client (excluded from broadcast).
    /// @param pMsgParts Pointer to an array of message parts.
    /// @param nParts Number of message parts.
    /// @param varUserData Optional user data.
    void sendMessageToAll(const no::SockDesc& sockDescSource, const WebSocket::Message* pMsgParts, size_t nParts, const td::Variant& varUserData = td::Variant());

    /// @brief Broadcasts a fixed-size array of message parts to all clients except the source.
    /// @tparam NMSG Number of message parts (deduced from the array).
    /// @param sockDescSource Descriptor of the originating client (excluded).
    /// @param msgParts Fixed-size array of message parts.
    /// @param varUserData Optional user data.
    template <size_t NMSG>
    inline void sendMessageToAll(const no::SockDesc& sockDescSource, const cnt::Array<WebSocket::Message, NMSG>& msgParts, const td::Variant& varUserData = td::Variant())
    {
        sendMessageToAll(sockDescSource, msgParts.begin(), msgParts.size(), varUserData);
    }

    /// @brief Returns the maximum number of connections this manager was configured for.
    /// @return Maximum connection count.
    TSockPos getMaxConnections() const;
};

} //namespace no
