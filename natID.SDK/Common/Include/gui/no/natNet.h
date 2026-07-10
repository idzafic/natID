// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file natNet.h
    @brief WebSocket notification types and helper functions for cross-thread GUI messaging. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <td/Types.h>
#include <no/WSClientManager.h>
#include <gui/IMessageConsumer.h>

namespace gui
{
namespace no
{

class NotificationHelper;

/// @brief Encapsulates a network notification delivered from a WebSocket server or client
///        to the main GUI thread.
class NATGUI_API Notification
{
    friend class NotificationHelper;

public:
    /// @brief Identifies whether the notification originates from a server or a client.
    enum class Notifier : td::BYTE {
        NA=0,    ///< Not applicable / unset.
        Server,  ///< Notification originated from the WebSocket server.
        Client   ///< Notification originated from a WebSocket client.
    };
    /// @brief Describes the type of payload carried by the notification.
    enum class Payload : td::BYTE {
        NA=0,    ///< No payload.
        Buffer,  ///< Payload is a raw memory buffer.
        String   ///< Payload is a text string.
    };
    /// @brief Message types that a WebSocket server can send to the GUI thread.
    enum class ServerMessage : td::BYTE {
        NA=0,                    ///< No message / unset.
        ResLoadOK,               ///< A resource was loaded successfully.
        ResLoadFailed,           ///< A resource failed to load.
        Started,                 ///< The server has started and is listening.
        Ended,                   ///< The server has stopped.
        NewConnection,           ///< A new client has connected.
        InitDynRequests,         ///< Dynamic request handlers are being initialized.
        DynRequest,              ///< A dynamic request was received.
        MsgReceived,             ///< A generic message was received.
        DeliverTo1,              ///< A message should be delivered to one specific client.
        DeliverToAll,            ///< A message should be broadcast to all clients.
        ShowUsers,               ///< The list of connected users should be shown.
        MsgSent,                 ///< A message was sent successfully.
        WebSocketUpgradeRequest, ///< An HTTP upgrade request for WebSocket was received.
        ClientDisconnected       ///< A client has disconnected.
    };
    /// @brief Message types that a WebSocket client can send to the GUI thread.
    enum class ClientMessage : td::BYTE {
        NA=0,              ///< No message / unset.
        ConnectionSuccess, ///< The connection to the server succeeded.
        ConnectionFailed,  ///< The connection to the server failed.
        UserLeft,          ///< A remote user has left the session.
        MesssageSent,      ///< A message was successfully sent to the server.
        MessageReceived,   ///< A message was received from the server.
        Disconnected       ///< The client has disconnected from the server.
    };

private:
    union
    {
        ::no::WSClientManager* _pClientManager = nullptr; ///< Pointer to the client manager (used when Notifier is Client).
        void* _pServer;                                   ///< Opaque pointer to the server instance (used when Notifier is Server).
    };

    mem::Buffer _buffer;                           ///< Raw buffer payload attached to this notification.
    ::no::SockDesc _socketDescriptor;              ///< Socket descriptor identifying the connection source.
    td::String _message;                           ///< String payload attached to this notification.
    Notifier _notifier = Notifier::NA;             ///< Indicates whether the notifier is a server or client.
    Payload _payload = Payload::NA;                ///< Describes the type of payload in this notification.
    ServerMessage _serverMessage = ServerMessage::NA; ///< The server-specific message type (if Notifier is Server).
    ClientMessage _clientMessage = ClientMessage::NA; ///< The client-specific message type (if Notifier is Client).
public:
    /// @brief Default constructor. Initializes the notification to a neutral state.
    Notification();
    /// @brief Returns the client manager associated with this notification.
    /// @return Pointer to the WSClientManager, or nullptr if not a client notification.
    ::no::WSClientManager* getClientManager();
    /// @brief Returns the server pointer associated with this notification.
    /// @return Opaque pointer to the server object, or nullptr if not a server notification.
    void* getServer();
    /// @brief Returns the socket descriptor identifying the originating connection.
    /// @return Const reference to the socket descriptor.
    const ::no::SockDesc& getSocketDescriptor() const;
    /// @brief Returns a reference to the raw buffer payload.
    /// @return Reference to the mem::Buffer holding the binary payload.
    mem::Buffer& getBufferPayoad();
    /// @brief Returns the string payload carried by this notification.
    /// @return Const reference to the string payload.
    const td::String& getStringPayload() const;
    /// @brief Returns the notifier role (Server or Client).
    /// @return The Notifier enum value.
    Notifier getNotifier() const;
    /// @brief Returns the payload type (Buffer, String, or NA).
    /// @return The Payload enum value.
    Payload getPayload() const;
    /// @brief Returns the server-side message type.
    /// @return The ServerMessage enum value.
    ServerMessage getServerMessage() const;
    /// @brief Returns the client-side message type.
    /// @return The ClientMessage enum value.
    ClientMessage getClientMessage() const;
};

//server notifications to main GUI thread
/// @brief Posts a server notification without a payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param server Opaque pointer to the server that generated the event.
/// @param msgType The server message type to deliver.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType);
/// @brief Posts a server notification with a string payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param server Opaque pointer to the server that generated the event.
/// @param msgType The server message type to deliver.
/// @param strMessage Null-terminated string payload to include in the notification.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType, const char* strMessage);
/// @brief Posts a server notification with a buffer payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param server Opaque pointer to the server that generated the event.
/// @param msgType The server message type to deliver.
/// @param buffer Binary buffer payload to include in the notification.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType, mem::Buffer& buffer);

//client notifications to main GUI thread
/// @brief Posts a client notification without a payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param clientManager Pointer to the WSClientManager that generated the event.
/// @param msgType The client message type to deliver.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType);
/// @brief Posts a client notification with a string payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param clientManager Pointer to the WSClientManager that generated the event.
/// @param msgType The client message type to deliver.
/// @param strMessage Null-terminated string payload to include in the notification.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType, const char* strMessage);
/// @brief Posts a client notification with a buffer payload to the main GUI thread.
/// @param pConsumer The GUI message consumer that will receive the notification.
/// @param clientManager Pointer to the WSClientManager that generated the event.
/// @param msgType The client message type to deliver.
/// @param buffer Binary buffer payload to include in the notification.
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType, mem::Buffer& buffer);

} //namespace no
} //namespace gui
