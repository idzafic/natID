//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Application.h>

class ViewSettings : public gui::View
{
private:
protected:
    gui::Label _lblUn;
    gui::LineEdit _leUN;
    gui::Label _lblServerAddress;
    gui::LineEdit _leServerAddress;
    gui::Label _lblServerPort;
    gui::NumericEdit _neServerPort;
    gui::GridLayout _gl;
public:
    ViewSettings(const td::String& un, const td::String& serverAddr, td::UINT2 serverPort)
    : _lblUn("User name:")
    , _lblServerAddress("Server Address:")
    , _lblServerPort("Server Port:")
    , _neServerPort(td::word, gui::LineEdit::Messages::DoNotSend, false)
    , _gl(3, 2)
    {
        _leUN.setText(un);
        _leServerAddress.setText(serverAddr);
//        _neServerPort.showThSep(false);
        _neServerPort.setValue(serverPort);
        _neServerPort.setHAlignment(td::HAlignment::Left);
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblUn) << _leUN;
        gc.appendRow(_lblServerAddress) << _leServerAddress;
        gc.appendRow(_lblServerPort) << _neServerPort;
        setLayout(&_gl);
    }

    td::String getUserName() const
    {
        return _leUN.getText();
    }

    td::String getServerAddress() const
    {
        return _leServerAddress.getText();
    }
    
    td::UINT2 getServerPort() const
    {
        return _neServerPort.getValue().u2Val();
    }

};
