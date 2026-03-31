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
#include "NetworkLineGraph.h"
#include <gui/ViewScroller.h>
#include <gui/SplitterLayout.h>
#include <gui/Slider.h>
#include <gui/ColorPicker.h>
#include <gui/LinePatternPicker.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

class NetGridView : public gui::View
{
public:
    enum class Connection : td::BYTE {Disconnected=0, InProcess, Connected};
protected:
    gui::Button btnConnect;
    gui::Button btnSettings;
    gui::TextEdit messages;
    gui::GridLayout gl;
    WebClientWithChat webClient;

    gui::Label labelSlid;
    gui::Label labelx;
    gui::NumericEdit numx;
    gui::NumericEdit numy;
    gui::Label labely;
    gui::Label labelThickness;
    gui::Label labelColor;
    gui::Label labelPattern;
    gui::Button refresh;

    gui::HorizontalLayout layoutH;
    gui::VerticalLayout controlls;
    gui::HorizontalLayout red3;
    gui::HorizontalLayout red4;
    gui::HorizontalLayout red5;
    gui::HorizontalLayout red6;
    gui::GridLayout xAndY;
    gui::GridLayout net;
    gui::HorizontalLayout dugmad;

    gui::Slider xscale;
    gui::Slider thickness;
    gui::ColorPicker color;
    gui::LinePatternPicker pattern;

    td::String userName;
    td::String serverAddress;
    td::UINT2 serverPort = 2504;
    td::UINT2 connectionID = 0;
    Connection connection = Connection::Disconnected;
    td::BYTE running = 0;

    std::function<void(double)> callChangeScale;
    std::function<void(double)> callChangeThickness;
    std::function<void(td::ColorID)> callChangeColor;
    std::function<void(td::LinePattern)> callChangePattern;

    std::function<void(double, double)> appendPoint;

protected:
    bool onClientSocket(const gui::no::Notification& notification) override //handles comunication with the client socket
    {
        auto clientMessage = notification.getClientMessage();
        switch (clientMessage)
        {
        case gui::no::Notification::ClientMessage::ConnectionSuccess:
        {
            messages.appendString("ConnectionSuccess\n");
            connection = Connection::Connected;
        }
        break;
        case gui::no::Notification::ClientMessage::ConnectionFailed:
            messages.appendString("ConnectionFailed\n");
            connection = Connection::Disconnected;
            break;
        case gui::no::Notification::ClientMessage::UserLeft:
            messages.appendString("UserLeft\n");
            break;
        case gui::no::Notification::ClientMessage::MesssageSent:
            messages.appendString("MesssageSent\n");
            break;
        case gui::no::Notification::ClientMessage::MessageReceived:
        {
            auto payloadType = notification.getPayload();
            if (payloadType == gui::no::Notification::Payload::String)  //if a message is recieved it is parsed for the x and y values
            {                                                           //it carries and those are sent to be added to the graph
                const td::String& strMsg = notification.getStringPayload();
                std::stringstream ss(strMsg.c_str());
                std::string temp;
                double x, y;
                ss>>temp>>x>>temp>>y;
                appendPoint(x,y);
                messages.appendString(strMsg);
                messages.appendString(" \n");
            }
        }
        break;
        case gui::no::Notification::ClientMessage::Disconnected:
            messages.appendString("Disconnected\n");
            connection = Connection::Disconnected;
            break;
        default:
            assert(false);
        }
        return true;
    }
public:
    NetGridView(std::function<void(double)> CallChangeScale, std::function<void(double,double)> AppendPoint,
                std::function<void(double)> CallChangeThickness, std::function<void(td::ColorID)> CallChangeColor,
                std::function<void(td::LinePattern)> CallChangePattern, bool comboBoxFirst=false)
        : btnConnect("Connect/Disconnect")
        , btnSettings("Settings")
        , gl(2,1)
        , webClient(1)
        , appendPoint(AppendPoint)
        , layoutH(3)
        , controlls(4), red3(2), red4(2), red5(4), red6(1), xAndY(2,2)
        , labelx("x:")
        , numx(td::decimal3, gui::LineEdit::Messages::Send)
        , labely("y:")
        , numy(td::decimal3, gui::LineEdit::Messages::Send)
        , labelThickness("Line Thickness")
        , labelColor("Line Color")
        , labelPattern("Line Pattern")
        , labelSlid("X Scale: ")
        , callChangeScale(CallChangeScale)
        , callChangeThickness(CallChangeThickness)
        , callChangeColor(CallChangeColor)
        , callChangePattern(CallChangePattern)
        , refresh("Refresh")
        , net(2,2), dugmad(2)
    {
        
        if (webClient.run())
            running = 1;
        
        auto prop = getAppProperties();
        userName = prop->getValue("un", "Test");
        serverAddress = prop->getValue("server/Address", "localhost");
        serverPort = prop->getValue("server/Port", serverPort);
        
        xscale.setRange(-100,100);
        xscale.setValue(1);

        thickness.setRange(1, 5);
        thickness.setValue(2);

        numx.setAsReadOnly();
        numy.setAsReadOnly();
        color.setValue(td::ColorID::Yellow);

        messages.setAsReadOnly();

        red3.append(labelSlid);
        red3.append(xscale);
        red4.append(labelThickness);
        red4.append(thickness);
        red5.append(labelColor);
        red5.append(color);
        red5.append(labelPattern);
        red5.append(pattern);
        red6.append(refresh);


        controlls.append(red3);controlls.append(red4);controlls.append(red5);controlls.append(red6);
        gui::GridComposer gc(xAndY);
        gc.appendRow(labelx)<<numx;
        gc.appendRow(labely)<<numy;

        gui::GridComposer gcp(net);
        gcp.appendRow(messages);
        dugmad.append(btnConnect);
        dugmad.append(btnSettings);
        gcp.appendRow(dugmad);

        layoutH.append(controlls);
        layoutH.append(xAndY);
        layoutH.append(net);

        setLayout(&layoutH);
        webClient.setConsumer(this);

        btnConnect.onClick([this](){connectDisconnect();});

        btnSettings.onClick([this](){adjustSettings();});
        btnConnect.setAsDefault();
    }

    
    ~NetGridView()
    {
        webClient.stop();
    }
    
    Connection getStatus() const
    {
        return connection;
    }
    
    void connectDisconnect() //handles connecting and disconnecting
    {
        if (connection == Connection::Connected)
        {
            webClient.disconnect(connectionID);
            connection = Connection::Disconnected;
        }
        else if (connection == Connection::Disconnected)
        {
            if (running == 0)
            {
                messages.appendString("ERROR! Client manager was not started!\n");
                return;
            }
            else
            {
                connection = Connection::InProcess;
                td::UINT4 loginDlgID = 100;
                gui::Panel::show<ViewLogin>(this, tr("Login"), loginDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewLogin* pViewLogin)
                                            {
                                                if (clickedButtonID == gui::Dialog::Button::ID::OK)
                                                {
                                                    td::String userName = pViewLogin->getUserName();
                                                    td::String pwd = pViewLogin->getPassword();

                                                    no::WSClientManager* pWSManager = webClient.getWSClientManager();

                                                    td::String strCmd("/natIDchat"); //make permanent connection
                                                    connectionID = pWSManager->asyncConnect(serverAddress, serverPort, strCmd, userName, pwd);
                                                    if (connectionID == 0)
                                                    {
                                                        messages.appendString("ERROR! Cannot start connection to server!\n");
                                                        return;
                                                    }
                                                }
                                                else
                                                {
                                                    connection = Connection::Disconnected;
                                                }
                                            }, userName);
            }
        }
    }
    
    void adjustSettings() //calls on the settings view to change things like the username and ip address
    {
        td::UINT4 settingsDlgID = 110;
        gui::Panel::show<ViewSettings>( this, "Settings", settingsDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewSettings* pDlg)
                                       {
                                           if (clickedButtonID == gui::Dialog::Button::ID::OK)
                                           {
                                               userName = pDlg->getUserName();
                                               serverAddress = pDlg->getServerAddress();
                                               serverPort= pDlg->getServerPort();

                                               auto prop = getAppProperties();
                                               prop->setValue("un", userName);
                                               prop->setValue("server/Address", serverAddress);
                                               prop->setValue("server/Port", serverPort);
                                           }
                                       }, userName, serverAddress, serverPort);
    }

    bool onChangedValue(gui::Slider* pSlider) override  //if the sliders for x axis scaling or line thickness are changed sends those to be displayed
    {
        if (pSlider == &xscale)
        {
            double val = xscale.getValue();
            callChangeScale((val >= 0) ? (1.0 + (val / 100.0) * (100.0 - 1.0)): (1.0 + (val / -100.0) * (0.01 - 1.0)));
            return true;
        }
        else if (pSlider == &thickness){
            callChangeThickness(thickness.getValue());
            return true;
        }
        return false;
    }

    bool onClick(gui::Button* pB) override //refreshes the color and line pattern with the current selection
    {
        if(pB==&refresh){
            callChangePattern(pattern.getValue());
            callChangeColor(color.getValue());
        }
        return true;
    }

    void setXY(td::Decimal3 x, td::Decimal3 y){ //set the values of the two numeric edit boxes to show the value at current mouse position over canvas
        numx.setValue(x);
        numy.setValue(y);
    }

    void setInfo(double xmax, double xmin, double ymax, double ymin){ //sets the text in the text edit to display some of the data in the graph
        messages.clean();
        messages.appendString("X max: "+std::to_string(xmax)+"\nX min: "+std::to_string(xmin));
        messages.appendString("\n");
        messages.appendString("Y max: "+std::to_string(ymax)+"\nY min: "+std::to_string(ymin));
    }
};

class ViewNetCanvas : public gui::Canvas{
protected:
    NetLineGraph grafik;
    gui::DrawableString xOsa;
    gui::DrawableString yOsa;
    gui::Point translate;
    double scaleX = 1.0;
    double scaleY = 1.0;

    std::function<void(td::Decimal3,td::Decimal3)> setXY;
    std::function<void(double, double, double, double)> setInfo;
    double thickness=2.0;
    td::ColorID color=td::ColorID::Yellow;
    td::LinePattern pattern=td::LinePattern::Solid;

protected:

    bool getModelSize(gui::Size& modelSize)const override{ //for scroller to work
        double a=grafik.getXmax()*scaleX;
        modelSize.width=a*1+50;
        modelSize.height=550;
        return true;
    }

    void onDraw(const gui::Rect& rect) override{ //calls on the draw method of the NetworkLineGraph class and the setInfo method

        gui::Size sz;
        getSize(sz);
        grafik.Draw(sz,scaleX, thickness, color, pattern);
        setInfo(grafik.getXmax(), grafik.getXmin(), grafik.getYmax(), grafik.getYmin());
    }

public:
    ViewNetCanvas(std::function<void(td::Decimal3,td::Decimal3)> FsetXY, std::function<void(double, double, double, double)> FsetInfo)
        :gui::Canvas({gui::InputDevice::Event::Zoom, gui::InputDevice::Event::CursorMove})
        ,setXY(FsetXY)
        ,setInfo(FsetInfo){
        enableResizeEvent(true);
        setClipsToBounds();
        registerForScrollEvents();
    }

    void changeScale(double scale){ //changes the scaling on the x axis and redraws the image
        scaleX=scale;
        reDraw();
    }

    void appendPoint(double x, double y){ //adds a point to the end of the line whenever a new message is recieved
        grafik.appendPoint(x,y);
        reDraw();
    }

    NetLineGraph& getGraf(){return grafik;} //returns a reference to the graph in use for use in other classes

    void changeThickness(double thick){ //changes the thickness of the line and redraws the image
        thickness=thick;
        reDraw();
    }

    void changeColor(td::ColorID c){    //changes the color of the line and redraws the image
        color=c;
        reDraw();
    }

    void changePattern(td::LinePattern p){ //changes the pattern of the line and redraws the image
        pattern=p;
        reDraw();
    }

    void onCursorMoved(const gui::InputDevice& inputDevice)override{ //tracks cursor position and converts it to coordinates to be shown
        gui::Point position= inputDevice.getModelPoint();
        td::Decimal3 y=position.y;
        td::Decimal3 x=(position.x-5)/scaleX;
        gui::Size sz;
        getSize(sz);
        double maxVal = grafik.getYmax();
        double minVal = grafik.getYmin();
        double B = sz.height;

        double range  = maxVal - minVal;
        double outMin = (maxVal <= 0) ? 25.0 : 0.0;
        double outMax = (minVal >= 0) ? (B - 25.0) : B;

        y = maxVal - (y - outMin) * range / (outMax - outMin);
        setXY(x,y);
    }
};

class ViewNetScroll : public gui::ViewScroller //adds a scrollbar that is resizeable based on the size of the image to the canvas
{
private:
protected:
    std::function<void(td::Decimal3,td::Decimal3)> FsetXY;
    std::function<void(double, double, double, double)> FsetInfo;
    ViewNetCanvas canvas;
protected:

public:
    ViewNetScroll(std::function<void(td::Decimal3,td::Decimal3)> fsetxy, std::function<void(double, double, double, double)> fsetInfo)
        : gui::ViewScroller(gui::ViewScroller::Type::ScrollerAlwaysVisible, gui::ViewScroller::Type::NoScroll)
        ,FsetXY(fsetxy)
        ,FsetInfo(fsetInfo)
        ,canvas(FsetXY, FsetInfo)
    {
        setContentView(&canvas);
    }

    ViewNetCanvas& getView()
    {
        return canvas;
    }

    ViewNetCanvas& getCanvas(){return canvas;}

};

class NetView : public gui::View //puts the canvas and grid layout in a spliter layout and also handles some of the passing of data between them
{
    gui::SplitterLayout _splitter;
    std::function<void(td::Decimal3,td::Decimal3)> fcnsetXY;
    std::function<void(double, double, double, double)> fcnsetInfo;
    ViewNetScroll scroll;
    std::function<void(double)> fcnChangeScale;
    std::function<void(double,double)> fcnAppend;
    std::function<void(double)> fcnChangeThickness;
    std::function<void(td::ColorID)> fcnChangeColor;
    std::function<void(td::LinePattern)> fcnChangePattern;
    NetGridView grid;

public:
    NetView():_splitter(gui::SplitterLayout::Orientation::Vertical, gui::SplitterLayout::AuxiliaryCell::Second)
        ,fcnsetXY(std::bind(&NetGridView::setXY, &grid, std::placeholders::_1, std::placeholders::_2))
        ,fcnsetInfo(std::bind(&NetGridView::setInfo, &grid, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))
        ,scroll(fcnsetXY, fcnsetInfo)
        ,fcnChangeScale(std::bind(&ViewNetCanvas::changeScale, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnAppend(std::bind(&ViewNetCanvas::appendPoint, &scroll.getCanvas(), std::placeholders::_1, std::placeholders::_2))
        ,fcnChangeThickness(std::bind(&ViewNetCanvas::changeThickness, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangeColor(std::bind(&ViewNetCanvas::changeColor, &scroll.getCanvas(), std::placeholders::_1))
        ,fcnChangePattern(std::bind(&ViewNetCanvas::changePattern, &scroll.getCanvas(), std::placeholders::_1))
        ,grid(fcnChangeScale, fcnAppend, fcnChangeThickness, fcnChangeColor, fcnChangePattern)
    {
        _splitter.setContent(scroll, grid);
        setLayout(&_splitter);
    }
};
