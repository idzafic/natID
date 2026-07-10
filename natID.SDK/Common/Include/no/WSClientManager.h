// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WSClientManager.h
 * @brief WebSocket client connection manager with optional authentication support.
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
#include <crpt/SHA256.h>
#include <no/Command.h>

namespace no
{

class WebClient;

/// @brief Manages a pool of outgoing WebSocket connections, including upgrade handshake
///        and optional two-stage authentication.
class NETOP_API WSClientManager : protected IProsumer
{
    friend class WebClient;

protected:
    /// @brief Server response to a WebSocket upgrade request.
    enum class UpgradeResponse : unsigned char { Reject=0, AcceptAfterAuthentication, AcceptWithoutAuthentication};

    /// @brief Holds all state associated with a single outgoing WebSocket connection.
    class Entry
    {
    public:
        no::SockDesc sockDesc;                                           ///< Socket descriptor for this connection.
        td::String hostName;                                             ///< Remote host name.
        td::String link;                                                 ///< HTTP request path used during the upgrade.
        td::String userName;                                             ///< Username for authenticated connections.
        td::DateTime lastPingOrActivity;                                 ///< Timestamp of the last ping or data activity.
        char loginSalt[80];                                              ///< Salt value used during login stage 1.
        char serverKey[33];                                              ///< Server-provided encryption key.
        char serverIV[17];                                               ///< Server-provided initialisation vector.
        char clientPassPhrase[33];                                       ///< Client passphrase for encryption.
        char clientKey[33];                                              ///< Client-side encryption key.
        char passwordHash[SHA256_DIGEST_SIZE];                           ///< SHA-256 hash of the user password.
        TSockPos userPos = 0;                                            ///< Application-level user position (0 = anonymous).
        td::UINT2 portNo = 0;                                            ///< Remote port number.
        HTTP::WebSocket::State connectionState = HTTP::WebSocket::State::NotConnected; ///< Current handshake/connection state.
        bool requiresLogin = false;                                      ///< True if the server requires authentication.
        //td::BYTE loggedIn = 0;
    };

    no::ICommunicator* _pCommunicator = nullptr;    ///< Pointer to the underlying communicator.
    WebClient* _pWebClient = nullptr;               ///< Owning WebClient instance.
    cnt::SafeFullVector<Entry> _entries;            ///< Pool of connection entries.
    TSockPos _nActiveConnections = 0;               ///< Number of currently active connections.
    td::UINT2 _timeOut = 2000;                      ///< Connection/operation timeout in milliseconds.
    td::BYTE _prosumerPos = 0;                      //1 based, 0-means not used ///< Position in the prosumer array (1-based; 0 = unused).
    //td::BYTE _sendHashOnInit : 1;
private:
    /// @brief Handles the initial TCP connect and sets up WebSocket upgrade.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param userData Variant carrying optional user data.
    /// @return true if handling succeeded, false otherwise.
    bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) override;

    /// @brief Processes the server's WebSocket upgrade response.
    /// @param sockDesc Descriptor of the connection being upgraded.
    /// @param pWebSocketKey Server-provided WebSocket key.
    /// @param userData Variant carrying optional user data.
    /// @return UpgradeResponse indicating acceptance or rejection.
    UpgradeResponse handleUpgradeResponse(const no::SockDesc& sockDesc, const char* pWebSocketKey, const td::Variant& userData);

    /// @brief Sends the first authentication stage challenge to the server.
    /// @param sockDescIn Descriptor of the authenticated connection.
    /// @param userData Variant carrying optional user data.
    void handleAuthStage1(const no::SockDesc& sockDescIn, const td::Variant& userData) const;
//    void handleAuthenticationSalt(const no::SockDesc& sockDesc, const mem::Buffer& buffer, size_t posInBuffer, const td::Variant& userData);

    /// @brief Processes the server's login stage-2 response.
    /// @param sockDesc Descriptor of the connection.
    /// @param buffer Buffer containing the stage-2 server message.
    /// @param userData Variant carrying optional user data.
    void handleLoginStage2Response(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData);
//    bool initConnection(no::SockDesc& sockDesc, mem::Buffer& buffer, const td::String& request, const td::Variant*params, size_t nParams);

    /// @brief Dispatches a received WebSocket message to the appropriate handler.
    /// @param sockDesc Descriptor of the connection.
    /// @param wsMessage Received WebSocket message.
    /// @param varUserData User data passed with the read operation.
    /// @return true if the message was handled successfully, false otherwise.
    bool handleMsgRead(const no::SockDesc& sockDesc, const WebSocket::Message& wsMessage, const td::Variant& varUserData);

    /// @brief Handles completion of an outgoing WebSocket message send.
    /// @param sockDesc Descriptor of the connection.
    /// @param varUserData User data passed with the write operation.
    void handleMsgSent(const no::SockDesc& sockDesc, const td::Variant& varUserData);

    /// @brief Handles a disconnection event for an entry.
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
    /// @param reason Reason for closing the connection.
    void closeEntry(Entry* pEntry, no::DisconnectReason reason);

    /// @brief Sends a ping to all active connections using the provided buffer.
    /// @param buffer Buffer containing the ping payload.
    void sendPingToAll(mem::Buffer& buffer);

protected:
    //IProsumer
    //virtual void handleConnect(no::SockDesc& sockDesc, td::Variant& userData);

    /// @brief Called after a connection has been fully established.
    /// @param sockDesc Descriptor of the connected socket.
    /// @param userData User-defined data associated with this connection.
    void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) override;

    /// @brief Queries whether the connection should be closed.
    /// @param sockDesc Descriptor under consideration.
    /// @param reason Reason triggering the disconnect check.
    /// @return true to disconnect, false to keep alive.
    bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when a connection has been fully disconnected.
    /// @param sockDesc Descriptor of the closed socket.
    /// @param reason Reason for the disconnection.
    void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) override;

    /// @brief Called when an exact-size buffer read has completed.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param varUserData User data passed with the read.
    void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Called when a partial buffer read has completed.
    /// @param sockDesc Descriptor of the socket.
    /// @param buffer Buffer with received data.
    /// @param varUserData User data passed with the read.
    void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) override;

    /// @brief Called when a buffer has been fully sent.
    /// @param sockDesc Descriptor of the socket.
    /// @param varUserData User data passed with the write.
    void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) override;

    /// @brief Called periodically when the event loop is idle.
    void onIdle() override;
    //end IProsumer

    /// @brief Override point: called with the server's upgrade response for custom handling.
    /// @param sockDesc Descriptor of the connection being upgraded.
    /// @return UpgradeResponse decision for this connection.
    virtual UpgradeResponse onUpgradeResponse(const no::SockDesc& sockDesc);

    /// @brief Finds the Entry for the given socket descriptor.
    /// @param sockDesc Descriptor to search for.
    /// @return Pointer to the matching Entry, or nullptr.
    Entry* findEntry(const no::SockDesc& sockDesc);

    /// @brief Finds the Entry for the given socket descriptor (const version).
    /// @param sockDesc Descriptor to search for.
    /// @return Const pointer to the matching Entry, or nullptr.
    const Entry* findEntry(const no::SockDesc& sockDesc) const;

    /// @brief Finds a connection entry by application-level connection ID.
    /// @param connID Connection ID to search for.
    /// @return Const pointer to the matching Entry, or nullptr.
    const Entry* findConnection(td::UINT2 connID) const;

    /// @brief Override point: called after a WebSocket connection is fully established.
    /// @param pEntry Pointer to the connected Entry.
    virtual void onConnect(const Entry* pEntry);

    /// @brief Override point: called when a WebSocket message has been received.
    /// @param pEntry Pointer to the Entry that received the message.
    /// @param wsMessage The received WebSocket message.
    /// @param varUserData User data associated with the read.
    /// @return true if the message was processed, false otherwise.
    virtual bool onMsgRead(const Entry* pEntry, const WebSocket::Message& wsMessage, const td::Variant& varUserData);

    /// @brief Override point: called when a WebSocket message has been sent.
    /// @param pEntry Pointer to the Entry on which the message was sent.
    /// @param varUserData User data associated with the write.
    virtual void onMsgSent(const Entry* pEntry, const td::Variant& varUserData);

    /// @brief Override point: called when a WebSocket connection is disconnected.
    /// @param pEntry Pointer to the disconnected Entry.
    /// @param reason Reason for the disconnection.
    virtual void onDisconnect(const Entry* pEntry, no::DisconnectReason reason);

    /// @brief Override point: resolves a username to an application user position.
    /// @param pUserName Pointer to the username string.
    /// @param nameLen Length of the username string.
    /// @param pwd Output string to receive the expected password for the user.
    /// @return Application-level user position, or 0 if the user was not found.
    virtual TSockPos findUser(const char* pUserName, size_t nameLen, td::String& pwd);

    /// @brief Parses and dispatches a complete WebSocket frame from the buffer.
    /// @param pAuxBuff Auxiliary working buffer.
    /// @param auxBuffLen Size of the auxiliary buffer.
    /// @param sockDesc Descriptor of the connection.
    /// @param buffer Buffer containing raw WebSocket data.
    /// @param userData User data associated with the operation.
    void processWebSocket(char* pAuxBuff, td::UINT4 auxBuffLen, const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& userData);
public:
    /// @brief Constructs a WSClientManager that can hold up to @p maxConnections entries.
    /// @param maxConnections Maximum number of simultaneous WebSocket connections.
    WSClientManager(TSockPos maxConnections);

    /// @brief Returns whether the connection identified by @p connID is currently open.
    /// @param connID Connection ID to check.
    /// @return true if the connection is active, false otherwise.
    bool isConnected(td::UINT2 connID) const;

    /// @brief Sends a WebSocket message to the specified connection.
    /// @param connID Target connection ID.
    /// @param wsMessage Message to send.
    /// @param varUserData Optional user data forwarded to the send-completion callback.
    /// @return true if the send was queued successfully, false otherwise.
    bool asyncSend(td::UINT2 connID, const WebSocket::Message& wsMessage, const td::Variant& varUserData = td::Variant());

    /// @brief Sends a plain text string as a WebSocket message.
    /// @param connID Target connection ID.
    /// @param strMsg String message to send.
    /// @param varUserData Optional user data.
    /// @return true if queued successfully, false otherwise.
    bool asyncSend(td::UINT2 connID, const td::String& strMsg, const td::Variant& varUserData = td::Variant());

    /// @brief Sends a named WebSocket command with no additional data.
    /// @param connID Target connection ID.
    /// @param cmd Command to send.
    /// @param varUserData Optional user data.
    /// @return true if queued successfully, false otherwise.
    bool asyncSend(td::UINT2 connID, no::WebSocket::Command cmd, const td::Variant& varUserData = td::Variant());

    /// @brief Sends a named WebSocket command with a string payload.
    /// @param connID Target connection ID.
    /// @param cmd Command to send.
    /// @param commndData String payload accompanying the command.
    /// @param varUserData Optional user data.
    /// @return true if queued successfully, false otherwise.
    bool asyncSend(td::UINT2 connID, no::WebSocket::Command cmd, const td::String& commndData, const td::Variant& varUserData = td::Variant());

    /// @brief Sends the contents of a raw buffer as a WebSocket message.
    /// @param connID Target connection ID.
    /// @param buff Buffer to send.
    /// @param varUserData Optional user data.
    /// @return true if queued successfully, false otherwise.
    bool asyncSend(td::UINT2 connID, mem::Buffer& buff, const td::Variant& varUserData = td::Variant());

    /// @brief Initiates an asynchronous WebSocket connection to a remote server.
    /// @param hostName Remote hostname or IP address.
    /// @param portNo Remote port number.
    /// @param httpRequestLink HTTP path for the WebSocket upgrade request.
    /// @param userName Optional username for authenticated connections.
    /// @param pwd Optional password for authenticated connections.
    /// @param varUserData Optional user data.
    /// @return Connection ID assigned to this connection attempt, or 0 on failure.
    td::UINT2 asyncConnect(const td::String& hostName, td::UINT2 portNo,  const td::String& httpRequestLink, const td::String& userName = td::String(), const td::String& pwd = td::String(), const td::Variant& varUserData=td::Variant());

    /// @brief Closes the WebSocket connection identified by @p connID.
    /// @param connID Connection ID to disconnect.
    void disconnect(td::UINT2 connID);
};

} //namespace no
