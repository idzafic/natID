//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StatusBar.h>
#include <gui/Label.h>
#include <gui/HorizontalLayout.h>
#include <gui/Font.h>
#include <gui/ProgressIndicator.h>

class StatusBar : public gui::StatusBar
{
protected:
    gui::Label _lblMessage;
    gui::Label _lblProgress;
    gui::Label _lblNoOfFaces;
    gui::Label _lblTime;
    gui::Label _noFaces;
    gui::Label _time;
    gui::ProgressIndicator _progressIndicator;
public:
    StatusBar()
    :  gui::StatusBar(11)
    , _lblMessage("")
    , _lblProgress(tr("Progress:"))
    , _lblNoOfFaces(tr("Faces:"))
    , _lblTime(tr("Time:"))
    , _noFaces("00")
    , _time("00.000s")
    {
        _lblMessage.setFont(gui::Font::ID::SystemBoldItalic);
        _lblMessage.setResizable();
        
        setMargins(4, 0, 4, 4);
        
        _layout.setSpaceBetweenCells(0);
        _layout << _lblMessage;
        _layout.appendSpace(4);
        _layout << _lblProgress << _progressIndicator;
        _layout.appendSpace(4);
        _layout << _lblNoOfFaces << _noFaces;
        _layout.appendSpace(4);
        _layout << _lblTime << _time;
        _layout.appendSpace(4);
        setLayout(&_layout);
    }
    
    void setNoOfFaces(int nFaces)
    {
        td::String str;
        str.format("%d", nFaces);
        _noFaces.setTitle(str);
    }
    
    void setMessage(const td::String& msg)
    {
        _lblMessage.setTitle(msg);
    }
    
    void setProgressIndicator(double progress)
    {
        _progressIndicator.setValue(progress);
    }
    
    gui::ProgressIndicator* getProgressIndicator()
    {
        return &_progressIndicator;
    }
    
    void setTime(double timeInSec)
    {
        if (timeInSec == 0)
            _time.setTitle("00.000s");
        else
        {
            td::String str;
            str.format("%.3fs", timeInSec);
            _time.setTitle(str);
        }
    }
};
