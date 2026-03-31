//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <no/WebClient.h>
#include <no/WebSocket.h>
#include <no/HTTP.h>
#include <no/Command.h>
#include <gui/no/natNet.h>

//chat part is done by overloading no::WSClientManager
class WebChatClientManager : public no::WSClientManager
{
private:
    gui::IMessageConsumer* _pConsumer = nullptr;
    no::WebSocket::Command _cmdSHAU;
    no::WebSocket::Command _cmdMSGA;
public:
    WebChatClientManager(td::UINT2 maxHTTPConnections)
    : no::WSClientManager(maxHTTPConnections)
    , _cmdSHAU("SHAU")
    , _cmdMSGA("MSGA")
    {
    }
    
    void setConsumer(gui::IMessageConsumer* pConsumer)
    {
        _pConsumer = pConsumer;
    }
    
    gui::IMessageConsumer* getConsumer( )
    {
        return _pConsumer;
    }
    
protected:
    void onConnect(const WSClientManager::Entry* pEntry) override
    {
        if (!pEntry->sockDesc.isConnected())
        {
            if (_pConsumer)
            {
                gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::ConnectionFailed);
            }
            
            //std::cout << "ERROR! ConnectionID=" << pEntry->sockDesc.webSockPos << " failed to connect!" << td::endl;
        }
        else
        {
            if (_pConsumer)
            {
                gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::ConnectionSuccess);
            }
            
            //std::cout << "SUCCESS! ConnectionID=" << pEntry->sockDesc.webSockPos << " connected to server successfully!" << td::endl;
        }
    }
    
    bool onMsgRead(const WSClientManager::Entry* pEntry, const no::WebSocket::Message& wsMessage, const td::Variant& varUserData) override
    {
        if (wsMessage.cmd == _cmdSHAU)
        {
            if (_pConsumer)
            {
                const char* pMsg = "List of active users:\n";
                gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::MessageReceived, pMsg);
            }
        }
//        else if (wsMessage.cmd == _cmdMSGA)
//        {
//            const char* pMsg = "Public message ";
//            gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::MessageReceived, pMsg);
//        }
        
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::MessageReceived, wsMessage.pMsg);
        }
        
        return true;
    }
    
    void onDisconnect(const WSClientManager::Entry* pEntry, no::DisconnectReason reason) override
    {
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::Disconnected);
        }
        
        mu::dbgLog("WebChatClientManager closed connection for user=%s!", pEntry->userName.c_str());
    }
    
   void onMsgSent(const Entry* pEntry, const td::Variant& varUserData) override
    {
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, this, gui::no::Notification::ClientMessage::MesssageSent);
        }
   }
    
};

//web chat with computation possibility
class WebClientWithChat : public no::WebClient
{
protected:
    WebChatClientManager _wsManager;
protected:
    void onConnect(no::SockDesc& sockDesc, const td::Variant& userData)
    {
//        gui::IMessageConsumer* pConsumer = _wsManager.getConsumer();
//        if (pConsumer)
//        {
//            gui::no::notifyMainThread(pConsumer, &_wsManager, gui::no::Notification::ClientMessage::ConnectionSuccess);
//        }
    }
    
public:
    WebClientWithChat(td::UINT2 nMaxHTTPConnections = 0, td::UINT4 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut)
    : no::WebClient(nMaxHTTPConnections, timeOutInMilliSeconds)
    , _wsManager(nMaxHTTPConnections)
    {
        setWebSocketManager(&_wsManager);
    }
    
    void setConsumer(gui::IMessageConsumer* pConsumer)
    {
        _wsManager.setConsumer(pConsumer);
    }
    
    void disconnect(td::UINT2 connectionID)
    {
        _wsManager.disconnect(connectionID);
    }
    
};
