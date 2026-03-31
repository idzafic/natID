#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/Button.h>
#include <gui/DBComboBox.h>
#include <gui/TextEdit.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Panel.h>
#include "WebChatClient.h"
#include "ViewSettings.h"
#include "ViewLogin.h"
#include <gui/no/natNet.h>
#include <functional>
#include <gui/Application.h>

class MainView : public gui::View
{
public:
    enum class Connection : td::BYTE {Disconnected=0, InProcess, Connected};
protected:
    std::function<void(void)> _fnUpdateTB;
    gui::Label _lblMessage;
    gui::LineEdit _editMsg;
    gui::Button _btnSend;
    gui::TextEdit _messages;
    gui::GridLayout _gl;
    WebClientWithChat _webClient;
    td::String _userName;
    td::String _serverAddress;
    td::UINT2 _serverPort = 2504;
    td::UINT2 _connectionID = 0;
    Connection _connection = Connection::Disconnected;
    td::BYTE _running = 0;
protected:
    bool onClientSocket(const gui::no::Notification& notification) override
    {
        auto clientMessage = notification.getClientMessage();
        switch (clientMessage)
        {
            case gui::no::Notification::ClientMessage::ConnectionSuccess:
            {
                _messages.appendString("ConnectionSuccess\n");
                _connection = Connection::Connected;
                _btnSend.disable(false);
                _fnUpdateTB();
            }
                break;
            case gui::no::Notification::ClientMessage::ConnectionFailed:
                _messages.appendString("ConnectionFailed\n");
                _btnSend.disable();
                _connection = Connection::Disconnected;
                _fnUpdateTB();
                break;
            case gui::no::Notification::ClientMessage::UserLeft:
                _messages.appendString("UserLeft\n");
                break;
            case gui::no::Notification::ClientMessage::MesssageSent:
                _messages.appendString("MesssageSent\n");
                break;
            case gui::no::Notification::ClientMessage::MessageReceived:
            {
//                _messages.appendString("MessageReceived\n");
                auto payloadType = notification.getPayload();
                if (payloadType == gui::no::Notification::Payload::String)
                {
                    const td::String& strMsg = notification.getStringPayload();
                    _messages.appendString(strMsg);
                    _messages.appendString(" \n");
                }
            }
                break;
            case gui::no::Notification::ClientMessage::Disconnected:
                _messages.appendString("Disconnected\n");
                _connection = Connection::Disconnected;
                _btnSend.disable();
                _fnUpdateTB();
                break;
            default:
                assert(false);
        }
        return true;
    }
public:
    MainView(bool comboBoxFirst=false)
    : _lblMessage(tr("Msg:"))
    , _btnSend(tr("Send"))
    , _gl(2,3)
    , _webClient(1)
    {
        
        if (_webClient.run())
            _running = 1;
        
        auto prop = getAppProperties();
        _userName = prop->getValue("un", "Emir");
        _serverAddress = prop->getValue("server/Address", "localhost");
        _serverPort = prop->getValue("server/Port", _serverPort);
        
        _btnSend.disable();
        
        _btnSend.setType(gui::Button::Type::Default);
        _messages.setAsReadOnly();
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblMessage) << _editMsg << _btnSend;
        gc.appendRow(_messages, 0);
        setLayout(&_gl);
        _webClient.setConsumer(this);
        
        _btnSend.onClick([this]()
        {
            if (_connection == Connection::Connected)
            {
                td::String strMsg = _editMsg.getText();
                _messages.appendString("Sending:");
                _messages.appendString(strMsg);
                _messages.appendString("\n");
                
                no::WSClientManager* pWSManager = _webClient.getWSClientManager();
                no::WebSocket::Command cmd("MSGA");
                if (!pWSManager->asyncSend(_connectionID, cmd, strMsg))
                {
                    _messages.appendString("ERROR! Cannot execute asyncSend(MSGA)!\n");
                }
            }
        });
    }
    
    ~MainView()
    {
        _webClient.stop();
    }
    
    Connection getStatus() const
    {
        return _connection;
    }
    
    void setTBUpdater(const std::function<void(void)>& fn)
    {
        _fnUpdateTB = fn;
    }
    
    void connectDisconnect()
    {
        if (_connection == Connection::Connected)
        {
            _webClient.disconnect(_connectionID);
            _connection = Connection::Disconnected;
            _btnSend.disable();
        }
        else if (_connection == Connection::Disconnected)
        {
            if (_running == 0)
            {
                _messages.appendString("ERROR! Client manager was not started!\n");
                return;
            }
            else
            {
                _connection = Connection::InProcess;
                _fnUpdateTB();
                td::UINT4 loginDlgID = 100;
                gui::Panel::show<ViewLogin>(this, tr("Login"), loginDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewLogin* pViewLogin)
                {
                    if (clickedButtonID == gui::Dialog::Button::ID::OK)
                    {
                        td::String userName = pViewLogin->getUserName();
                        td::String pwd = pViewLogin->getPassword();

                        no::WSClientManager* pWSManager = _webClient.getWSClientManager();

                        td::String strCmd("/natIDchat"); //make permanent connection
                        _connectionID = pWSManager->asyncConnect(_serverAddress, _serverPort, strCmd, userName, pwd);
                        if (_connectionID == 0)
                        {
                            _messages.appendString("ERROR! Cannot start connection to server!\n");
                            return;
                        }
                    }
                    else
                    {
                        _connection = Connection::Disconnected;
                        _fnUpdateTB();
                    }
                }, _userName);
            }
        }
    }
    
    void adjustSettings()
    {
        td::UINT4 settingsDlgID = 110;
        gui::Panel::show<ViewSettings>( this, tr("Settings"), settingsDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewSettings* pDlg)
        {
            if (clickedButtonID == gui::Dialog::Button::ID::OK)
            {
                _userName = pDlg->getUserName();
                _serverAddress = pDlg->getServerAddress();
                _serverPort= pDlg->getServerPort();
                
                auto prop = getAppProperties();
                prop->setValue("un", _userName);
                prop->setValue("server/Address", _serverAddress);
                prop->setValue("server/Port", _serverPort);
            }
        }, _userName, _serverAddress, _serverPort);
    }
    
};
