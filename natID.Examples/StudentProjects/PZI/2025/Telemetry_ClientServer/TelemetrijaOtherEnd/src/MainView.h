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
#include <mu/Timer.h>
#include <chrono>
#include <thread>


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

    std::vector<double> y = {
        // block of 50 values
        -1345.2, 0, -500, 987.4,  -412.8,  1765.9,  -89.3,
        1540.6, -1987.1,  645.2,   312.7, -1650.4,
        428.9, -743.5,  1899.8,  -256.1,  1023.6,
        -1784.9,   95.4,  1678.3, -1198.6,   560.1,
        -342.7,  1432.8, -188.9, -1567.2,   734.5,
        198.6, -923.4,  1820.7, -134.2,  1299.1,
        -167.5,   876.2, -1945.8,  158.4,  1120.6,
        -684.9,  1405.7, -321.8,  1903.4, -1012.5,
        499.2, -1728.6,  908.1,   -45.7,  1612.9,

        // repeat block (2)
        -1345.2,  987.4,  -412.8,  1765.9,  -89.3,
        1540.6, -1987.1,  645.2,   312.7, -1650.4,
        428.9, -743.5,  1899.8,  -256.1,  1023.6,
        -1784.9,   95.4,  1678.3, -1198.6,   560.1,
        -342.7,  1432.8, -188.9, -1567.2,   734.5,
        198.6, -923.4,  1820.7, -134.2,  1299.1,
        -167.5,   876.2, -1945.8,  158.4,  1120.6,
        -684.9,  1405.7, -321.8,  1903.4, -1012.5,
        499.2, -1728.6,  908.1,   -45.7,  1612.9,

        // repeat block (3)
        -1345.2,  987.4,  -412.8,  1765.9,  -89.3,
        1540.6, -1987.1,  645.2,   312.7, -1650.4,
        428.9, -743.5,  1899.8,  -256.1,  1023.6,
        -1784.9,   95.4,  1678.3, -1198.6,   560.1,
        -342.7,  1432.8, -188.9, -1567.2,   734.5,
        198.6, -923.4,  1820.7, -134.2,  1299.1,
        -167.5,   876.2, -1945.8,  158.4,  1120.6,
        -684.9,  1405.7, -321.8,  1903.4, -1012.5,
        499.2, -1728.6,  908.1,   -45.7,  1612.9,

        // repeat block (4)
        -1345.2,  987.4,  -412.8,  1765.9,  -89.3,
        1540.6, -1987.1,  645.2,   312.7, -1650.4,
        428.9, -743.5,  1899.8,  -256.1,  1023.6,
        -1784.9,   95.4,  1678.3, -1198.6,   560.1,
        -342.7,  1432.8, -188.9, -1567.2,   734.5,
        198.6, -923.4,  1820.7, -134.2,  1299.1,
        -167.5,   876.2, -1945.8,  158.4,  1120.6,
        -684.9,  1405.7, -321.8,  1903.4, -1012.5,
        499.2, -1728.6,  908.1,   -45.7,  1612.9,

        // final element (201st)
        512.0
    };


    std::vector<double> x = {
        0, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110,
        120, 130, 140, 150, 160, 170, 180, 190, 200, 210,
        220, 230, 240, 250, 260, 270, 280, 290, 300, 310,
        320, 330, 340, 350, 360, 370, 380, 390, 400, 410,
        420, 430, 440, 450, 460, 470, 480, 490, 500, 510,
        520, 530, 540, 550, 560, 570, 580, 590, 600, 610,
        620, 630, 640, 650, 660, 670, 680, 690, 700, 710,
        720, 730, 740, 750, 760, 770, 780, 790, 800, 810,
        820, 830, 840, 850, 860, 870, 880, 890, 900, 910,
        920, 930, 940, 950, 960, 970, 980, 990, 1000, 1010,
        1020, 1030, 1040, 1050, 1060, 1070, 1080, 1090, 1100, 1110,
        1120, 1130, 1140, 1150, 1160, 1170, 1180, 1190, 1200, 1210,
        1220, 1230, 1240, 1250, 1260, 1270, 1280, 1290, 1300, 1310,
        1320, 1330, 1340, 1350, 1360, 1370, 1380, 1390, 1400, 1410,
        1420, 1430, 1440, 1450, 1460, 1470, 1480, 1490, 1500, 1510,
        1520, 1530, 1540, 1550, 1560, 1570, 1580, 1590, 1600, 1610,
        1620, 1630, 1640, 1650, 1660, 1670, 1680, 1690, 1700, 1710,
        1720, 1730, 1740, 1750, 1760, 1770, 1780, 1790, 1800, 1810,
        1820, 1830, 1840, 1850, 1860, 1870, 1880, 1890, 1900, 1910,
        1920, 1930, 1940, 1950, 1960, 1970, 1980, 1990, 2000, 3000
    };

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
                td::String strMsg;
                for(int i=0; i<y.size(); i++){

                    strMsg = "x: "+std::to_string(x[i])+" y: "+std::to_string(y[i]);

                    _messages.appendString("Sending:");
                    _messages.appendString(strMsg);
                    _messages.appendString("\n");
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    no::WSClientManager* pWSManager = _webClient.getWSClientManager();
                    no::WebSocket::Command cmd("MSGA");
                    if (!pWSManager->asyncSend(_connectionID, cmd, strMsg))
                    {
                        _messages.appendString("ERROR! Cannot execute asyncSend(MSGA)!\n");
                    }
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
