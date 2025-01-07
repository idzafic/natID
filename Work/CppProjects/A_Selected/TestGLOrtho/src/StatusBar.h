//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StatusBar.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/Font.h>

class StatusBar : public gui::StatusBar
{
protected:
    gui::Label _leMsg;
    gui::Label _lblZoom;
    gui::Label _leZoom;
    gui::Label _lblOrigin;
    gui::Label _leOrigin;
    gui::Label _lblCurrPos;
    gui::Label _leCurrPos;
public:
    StatusBar()
    :  gui::StatusBar(10)
    , _leMsg(tr("UsageHints"))
    , _lblZoom(tr("Zoom:"))
    , _leZoom("000.00%%") //string patterns are taken for initial size detection
    , _lblOrigin(tr("Origin:"))
    , _leOrigin("-0000.000,-0000.000") //string patterns are taken for initial size detection
    , _lblCurrPos(tr("CurrPos:"))
    , _leCurrPos("-0000.000,-0000.000") //string patterns are taken for initial size detection
    {
        _leMsg.setFont(gui::Font::ID::SystemBoldItalic);
        _leMsg.setResizable();
        
        _leZoom.disableRemeasuring();
        _leOrigin.disableRemeasuring();
        _leCurrPos.disableRemeasuring();
        
        setMargins(4, 4, 4, 4);
        
        _layout.setSpaceBetweenCells(1);
        _layout << _leMsg;
        _layout.appendSpace(8);
        _layout << _lblZoom << _leZoom;
        _layout.appendSpace(4);
        _layout << _lblOrigin << _leOrigin;
        _layout.appendSpace(4);
        _layout << _lblCurrPos << _leCurrPos;
        setLayout(&_layout);
    }
    
    void setZoom(float zoom)
    {
        td::String str;
        str.format("%.2f%%", zoom);
        _leZoom.setTitle(str);
    }
    
    void setOrigin(const gui::Point& pt)
    {
        td::String str;
        str.format("%.3f,%.3f", pt.x, pt.y);
        _leOrigin.setTitle(str);
    }
    
    void setCurrentPos(const gui::Point& pt)
    {
        td::String str;
        str.format("%.3f,%.3f", pt.x, pt.y);
        _leCurrPos.setTitle(str);
    }
    
    void setMessage(const td::String& msg)
    {
        _leMsg.setTitle(msg);
    }
    
    void resetValues()
    {
        setZoom(100.0f);
        gui::Point zero;
        setOrigin(zero);
        setCurrentPos(zero);
    }
};
