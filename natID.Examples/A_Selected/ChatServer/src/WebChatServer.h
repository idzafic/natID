//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <no/WebServer.h>
#include <no/WebSocket.h>
#include <no/HTTP.h>
#include <gui/no/natNet.h>

class DynamicReqResponder
{
protected:
    gui::IMessageConsumer* _pConsumer = nullptr;
    void* _pServer = nullptr;
public:
    DynamicReqResponder(void* pServer)
    : _pServer(pServer)
    {
    }
    
    bool mult(const td::Variant* pParams, size_t nParams, mem::Buffer& buffer, td::UINT2 timeoutInSec)
    {
        assert(nParams == 2);
        double res = pParams[0].r8Val() * pParams[1].r8Val();
        char tmpRes[32];
        auto nContentLen = SNPRINTF(tmpRes, 32, _TRUNCATE, "%.4f", res);
        tmpRes[31] = 0;
        no::HTTP::prepareResponseBuffer(&tmpRes[0], nContentLen, buffer, timeoutInSec);
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::DynRequest);
        }
        
        return true;
    }
    
    void setConsumer(gui::IMessageConsumer* pConsumer)
    {
        _pConsumer = pConsumer;
    }
};

//chat part is donw by overloading no::WebSocketManager
class ChatServerManager : public no::WSServerManager
{
protected:
    gui::IMessageConsumer* _pConsumer = nullptr;
    void* _pServer = nullptr;
    
private:
    class User
    {
    public:
        td::String name;
        td::String password;
        size_t nActiveConnections = 0;
//        td::INT1 loginStage = 0;
    };
    cnt::SafeFullVector<User> _users;
    no::TSockPos _nActiveUsers = 0;
    //available commands
    no::WebSocket::Command _cmdShowActiveUsers;
    no::WebSocket::Command _cmdMessageToAll;

private:
    
    void initTestUsers()
    {
        //should be loaded from DB, pwd cannot be saved on server side
        _users.reserve(4);
        _users[0].name = "Amir";
        _users[0].password = "AmirA";

        _users[1].name = "Damir";
        _users[1].password = "DamirD";
        
        _users[2].name = "Emir";
        _users[2].password = "EmirE";
        
        _users[3].name = "Samir";
        _users[3].password = "SamirS";
        
        _nActiveUsers = 4;
    }
    
protected:
    
    WSServerManager::UpgradeRequest onUpgradeRequest(const char* request, const td::Variant*params, size_t nParams) override
    {
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::WebSocketUpgradeRequest);
        }
        return UpgradeRequest::AcceptAfterAuthentication;
    }
    
    no::TSockPos findUser(const char* pUserName, size_t nameLen, td::String& pwd) override
    {
        no::TSockPos userPos = 0;
        for (auto& u : _users)
        {
            if (u.name.length() == nameLen)
            {
                if (u.name.cCompareNoCase(pUserName) == 0)
                {
                    pwd = u.password;
                    return userPos + 1;
                }
            }
            ++userPos;
        }
        return 0;
    }
    
    virtual no::TSockPos findUser(const char* pUserName, size_t nameLen)
    {
        no::TSockPos userPos = 0;
        for (auto& u : _users)
        {
            if (u.name.length() == nameLen)
            {
                if (u.name.cCompareNoCase(pUserName) == 0)
                {
                    return userPos + 1;
                }
            }
            ++userPos;
        }
        return 0;
    }
    
    User* findUser(const WSServerManager::Entry* pEntry)
    {
        if (!pEntry)
            return nullptr;
        auto userPos = pEntry->userPos;
        if (userPos == 0)
            return nullptr;
        if (userPos > _nActiveUsers)
            return nullptr;
        return &_users[userPos - 1];
    }
    
    const td::String& getUserName(const WSServerManager::Entry* pEntry) const
    {
        if (!pEntry)
            return mu::__emptyString;
        if (pEntry->userPos == 0)
            return mu::__emptyString;
        if (pEntry->userPos > _nActiveUsers)
            return mu::__emptyString;
        return _users[pEntry->userPos - 1].name;
    }
    
    bool onConnect(const WSServerManager::Entry* pEntry) override
    {
        User* pUser = findUser(pEntry);
        if (!pUser)
            return false;
        
        auto& userName = pUser->name;
        mu::dbgLog("WebSocket: Connected user='%s' on socketID=%d", userName.c_str(), pEntry->sockDesc.socketID);
        ++pUser->nActiveConnections;
        
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::NewConnection);
        }
        return true;
    }
    
    void onDisconnect(const WSServerManager::Entry* pEntry, no::DisconnectReason reason) override
    {
        User* pUser = findUser(pEntry);
        if (!pUser)
            return;
        --pUser->nActiveConnections;
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::ClientDisconnected);
        }
    }
    
    bool onMsgRead(const WSServerManager::Entry* pEntry, const no::WebSocket::Message& wsMessage, const td::Variant& varUserData) override
    {
        if (_pConsumer)
        {
            gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::MsgReceived);
        }
        
        no::WebSocket::Command cmd = wsMessage.getCommand();
        if (cmd == _cmdMessageToAll) //"MSGA"
        {
            const td::String& userName = getUserName(pEntry);
            mu::dbgLog("Received MSGA command user:'%s' on socketID=%d, msgLen=%d", userName.c_str(), pEntry->sockDesc.socketID, wsMessage.len);
            //char buff[4096];
            //auto newMsgLen = SPRINTF(buff, "%s=%s", userName.c_str(), pMsg);
            //sendMessageToAll(pEntry->sockDesc, &buff[0], newMsgLen);
            cnt::Array<no::WebSocket::Message, 3> msgParts;
            
            msgParts[0].pMsg = userName.c_str();
            msgParts[0].len = userName.length();
            msgParts[0].cmd = cmd;
            msgParts[0].type = wsMessage.type;
            
            msgParts[1].pMsg = "-->";
            msgParts[1].len = 3;
            
            msgParts[2].pMsg = wsMessage.pMsg;
            msgParts[2].len = wsMessage.len;
            sendMessageToAll(pEntry->sockDesc, msgParts);
            if (_pConsumer)
            {
                gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::DeliverToAll);
            }
            return true;
        }
        else if (cmd == _cmdShowActiveUsers) //"SHAU"
        {
            cnt::StringBuilderSmall strBuilder;
            
//            strBuilder.appendCString("List of active connected users:\r\n");
            for (const auto& user : _users)
            {
                if (user.nActiveConnections > 0)
                {
                    td::String userName = user.name;
                    strBuilder.appendString(userName);
                    strBuilder.appendCString(" [");
                    char tmp[32];
                    auto len =SNPRINTF(tmp, 32, _TRUNCATE, "%zd", user.nActiveConnections);
                    strBuilder.appendString(tmp, len);
                    strBuilder.appendCString("]\r\n");
                }
            }
            
            td::String strMsg = strBuilder.toString();
            
            no::WebSocket::Message retMsg;
            retMsg.pMsg = strMsg.c_str();
            retMsg.len = strMsg.length();
            retMsg.cmd = cmd;
            
            sendMessage(pEntry->sockDesc, retMsg);
            if (_pConsumer)
            {
                gui::no::notifyMainThread(_pConsumer, _pServer, gui::no::Notification::ServerMessage::ShowUsers);
            }
            return true;
        }
        else
        {
            assert(false);
            
//            no::WebSocket::Message retMsg;
//            retMsg.pMsg = "Unknown command\r\n";
//            retMsg.len = (td::UINT4) strlen(retMsg.pMsg);
//            sendMessage(pEntry->sockDesc, retMsg);
            return true;
        }
        return false;
    }
    
//    virtual void onDisconnect(const WSServerManager::Entry* pEntry) 
//    {
//    }
public:
    ChatServerManager(void* pServer)
    : no::WSServerManager(MAX_NO_WEBCHATSERER_CONNECTIONS)
    , _pServer(pServer)
    , _cmdShowActiveUsers("SHAU")
    , _cmdMessageToAll("MSGA")
    {
        initTestUsers(); //load from DB
    }
    
    void setConsumer(gui::IMessageConsumer* pConsumer)
    {
        _pConsumer = pConsumer;
    }
    
    gui::IMessageConsumer* getConsumer( )
    {
        return _pConsumer;
    }
};

//web chat with computation possibility
class WebServerWithChat : public no::WebServer
{
protected:
    DynamicReqResponder _dynResponder;
    ChatServerManager _wsManager;
    td::String _strConfigPath;
    
    void initDynamicRequests()
    {
        const td::DataType pParamTypes[] = {td::real8, td::real8};
        addDynamicRequest("calculate", &pParamTypes[0], 2, &DynamicReqResponder::mult, &_dynResponder);
        auto pConsumer = _wsManager.getConsumer();
        if (pConsumer)
        {
            gui::no::notifyMainThread(pConsumer, this, gui::no::Notification::ServerMessage::InitDynRequests);
        }
    }
    
    bool onLoadResources() override
    {
        auto pConsumer = _wsManager.getConsumer();
        if (!loadResources(_strConfigPath, nullptr))
        {
            if (pConsumer)
            {
                gui::no::notifyMainThread(pConsumer, this, gui::no::Notification::ServerMessage::ResLoadFailed);
            }
            return false;
        }
            
        if (pConsumer)
        {
            gui::no::notifyMainThread(pConsumer, this, gui::no::Notification::ServerMessage::ResLoadOK);
        }
        return true;
    }
    
public:
    WebServerWithChat(td::UINT4 timeOutInMilliSeconds = no::HTTP::DefaultKeepAliveTimeOut)
    : no::WebServer(timeOutInMilliSeconds)
    , _dynResponder(this)
    , _wsManager(this)
    {
        initDynamicRequests();
        setWebSocketManager(&_wsManager);
    }
    
    void setConsumer(gui::IMessageConsumer* pConsumer)
    {
        _dynResponder.setConsumer(pConsumer);
        _wsManager.setConsumer(pConsumer);
    }
    
    void setConfigFileName(const td::String& configPath)
    {
        _strConfigPath = configPath;
    }
};
