//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StatusBar.h>
#include <gui/Label.h>
//#include <gui/LineEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/Font.h>

class StatusBar : public gui::StatusBar
{
protected:
    gui::Label _lblMessage;
    gui::Label _lblFlags;
    gui::Label _lblBombs;
    gui::Label _lblSize;
    
    gui::Label _noOfFlags;
    gui::Label _noOfBombs;
    gui::Label _fieldInfo;
public:
    StatusBar()
    :  gui::StatusBar(10)
    , _lblMessage("")
    , _lblFlags(tr("Flags:"))
    , _lblBombs(tr("Bombs:"))
    , _lblSize(tr("Size:"))
    , _noOfFlags("00")
    , _noOfBombs("00")
    , _fieldInfo("00x00")
    {
        _lblMessage.setFont(gui::Font::ID::SystemBoldItalic);
        _lblMessage.setResizable();
        
        _noOfFlags.disableRemeasuring();
        _noOfBombs.disableRemeasuring();
        _fieldInfo.disableRemeasuring();
        
        setMargins(4, 0, 4, 4);
        
        _layout.setSpaceBetweenCells(0);
        _layout << _lblMessage;
        _layout.appendSpace(4);
        _layout << _lblFlags << _noOfFlags;
        _layout.appendSpace(4);
        _layout << _lblBombs << _noOfBombs;
        _layout.appendSpace(4);
        _layout << _lblSize << _fieldInfo;
        setLayout(&_layout);
    }
    
    void setNoOfBombs(int nBombs)
    {
        td::String str;
        str.format("%d", nBombs);
        _noOfBombs.setTitle(str);
    }
    
    void setNoOfFlags(int nFlags)
    {
        td::String str;
        str.format("%d", nFlags);
        _noOfFlags.setTitle(str);
    }
    
    void setMessage(const td::String& msg)
    {
        _lblMessage.setTitle(msg);
    }
    
    void setFieldInfo(int w, int h)
    {
        td::String str;
        str.format("%dx%d", w, h);
        _fieldInfo.setTitle(str);
    }
};
