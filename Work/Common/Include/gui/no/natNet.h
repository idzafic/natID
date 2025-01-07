// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATGUI_API Notification
{
    friend class NotificationHelper;
    
public:
    enum class Notifier : td::BYTE {NA=0, Server, Client};
    enum class Payload : td::BYTE {NA=0, Buffer, String};
    enum class ServerMessage : td::BYTE {NA=0, ResLoadOK, ResLoadFailed, Started, Ended, NewConnection, InitDynRequests, DynRequest, MsgReceived, DeliverTo1, DeliverToAll, ShowUsers, MsgSent, WebSocketUpgradeRequest, ClientDisconnected};
    enum class ClientMessage : td::BYTE {NA=0, ConnectionSuccess, ConnectionFailed, UserLeft, MesssageSent, MessageReceived, Disconnected};
    
private:
    union
    {
        ::no::WSClientManager* _pClientManager = nullptr;
        void* _pServer;
    };
    
    mem::Buffer _buffer;
    ::no::SockDesc _socketDescriptor;
    td::String _message;
    Notifier _notifier = Notifier::NA;
    Payload _payload = Payload::NA;
    ServerMessage _serverMessage = ServerMessage::NA;
    ClientMessage _clientMessage = ClientMessage::NA;
public:
    Notification();
    ::no::WSClientManager* getClientManager();
    void* getServer();
    const ::no::SockDesc& getSocketDescriptor() const;
    mem::Buffer& getBufferPayoad();
    const td::String& getStringPayload() const;
    Notifier getNotifier() const;
    Payload getPayload() const;
    ServerMessage getServerMessage() const;
    ClientMessage getClientMessage() const;
};

//server notifications to main GUI thread
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType);
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType, const char* strMessage);
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, void* server, Notification::ServerMessage msgType, mem::Buffer& buffer);

//client notifications to main GUI thread
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType);
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType, const char* strMessage);
NATGUI_API void notifyMainThread(gui::IMessageConsumer* pConsumer, ::no::WSClientManager* clientManager, Notification::ClientMessage msgType, mem::Buffer& buffer);

} //namespace no
} //namespace gui

