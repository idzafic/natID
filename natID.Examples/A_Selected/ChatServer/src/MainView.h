#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/DBComboBox.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include "WebChatServer.h"
#include <gui/Panel.h>
#include "ViewSettings.h"

class MainView : public gui::View
{
protected:
    gui::Label _lblMessage;
    gui::TextEdit _txtMessages;
    gui::VerticalLayout _vl;
    WebServerWithChat _webServer;
    td::String _serverAddress;
    td::UINT2 _serverPort = 2504;
    
protected:
    bool onServerSocket(const gui::no::Notification& notification) override
    {
        auto serverMessage = notification.getServerMessage();
        switch (serverMessage)
        {
            case gui::no::Notification::ServerMessage::NewConnection:
                _txtMessages.appendString("New connection\n");
                break;
            case gui::no::Notification::ServerMessage::ClientDisconnected:
                _txtMessages.appendString("Client disconnected\n");
                break;
            case gui::no::Notification::ServerMessage::DynRequest:
                _txtMessages.appendString("Dynamic request\n");
                break;
            case gui::no::Notification::ServerMessage::MsgReceived:
                _txtMessages.appendString("MsgReceived\n");
                break;
            case gui::no::Notification::ServerMessage::InitDynRequests:
                _txtMessages.appendString("InitDynRequests\n");
                break;
            case gui::no::Notification::ServerMessage::ResLoadOK:
            {
                _txtMessages.appendString("ResLoadOK\n------------\n");
                _txtMessages.appendString(tr("info"));
                _txtMessages.appendString("\n------------\n");
            }
                break;
            case gui::no::Notification::ServerMessage::ResLoadFailed:
                _txtMessages.appendString("ResLoadFailed\n");
                break;
            case gui::no::Notification::ServerMessage::WebSocketUpgradeRequest:
                _txtMessages.appendString("WebSocketUpgradeRequest\n");
                break;
            case gui::no::Notification::ServerMessage::DeliverToAll:
                _txtMessages.appendString("DeliverToAll\n");
                break;
            case gui::no::Notification::ServerMessage::DeliverTo1:
                _txtMessages.appendString("DeliverTo1\n");
                break;
            case gui::no::Notification::ServerMessage::ShowUsers:
                _txtMessages.appendString("ShowUsers\n");
                break;
                
                
            default:
                assert(false); //TODO
        }
        return true;
    }
public:
    MainView()
    : _lblMessage(tr("Messages:"))
    , _vl(2)
    {
        auto prop = getAppProperties();
        _serverAddress = prop->getValue("server/Address", "localhost");
        _serverPort = prop->getValue("server/Port", _serverPort);
        
        _txtMessages.setAsReadOnly();
        _vl << _lblMessage << _txtMessages;
       setLayout(&_vl);
        _webServer.setConsumer(this);
        td::String resConfigFN = gui::getResFileName("webConfig");
        _webServer.setConfigFileName(resConfigFN);
    }
    
    bool startStop()
    {
        if (_webServer.isRunning())
        {
            _webServer.stop();
        }
        else
        {
            if (_webServer.run(_serverAddress.c_str(), _serverPort))
            {
                return true;
            }
                
            return false;
        }
        return true;
    }
    
    bool isRunning() const
    {
        return _webServer.isRunning();
    }
    
    void adjustSettings()
    {
        td::UINT4 settingsDlgID = 110;
        gui::Panel::show<ViewSettings>(this, tr("Settings"), settingsDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewSettings* pDlg)
        {
            if (clickedButtonID == gui::Dialog::Button::ID::OK)
            {
                _serverAddress = pDlg->getServerAddress();
                _serverPort= pDlg->getServerPort();
                
                auto prop = getAppProperties();
                prop->setValue("server/Address", _serverAddress);
                prop->setValue("server/Port", _serverPort);
            }
        }, _serverAddress, _serverPort);
    }
};
