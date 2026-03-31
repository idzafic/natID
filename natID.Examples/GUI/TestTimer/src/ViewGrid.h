//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/PasswordEdit.h>
#include <gui/Button.h>

#include <gui/HorizontalLayout.h>
#include <gui/ProgressIndicator.h>

#include <gui/GridLayout.h>
#include <gui/Timer.h>


class ViewGrid : public gui::View
{
private:
protected:
    gui::Label _lblUN;
    gui::LineEdit _un;
    gui::Label _lblPwd;
    gui::PasswordEdit _pwd;
    
    gui::ProgressIndicator _progIndicator;
    //gui::Label _lbl3;
    gui::HorizontalLayout _hl;
    gui::Button _btnStartStop;
    gui::Button _btnClearText;
    gui::Button _showUNandPwd;
    gui::TextEdit _textEdit;
    gui::GridLayout _gl;
    gui::Timer _timer;
    std::function<void()>* _pUpdateMenuAndTB;
    double _currProgress = 0;

protected:
    
    bool onClick(gui::Button* pBtn)
    {
        if (pBtn == &_btnStartStop)
        {
            startStop();
            return true;
        }
        else if (pBtn == &_btnClearText)
        {
            _textEdit.clean();
            return true;
        }
        else if (pBtn == &_showUNandPwd)
        {
            td::String strUN = _un.getText();
            td::String strPwd = _pwd.getText();
            _textEdit.appendString(" UN=");
            _textEdit.appendString(strUN);
            _textEdit.appendString(", PWD=");
            _textEdit.appendString(strPwd);
            return true;
        }
        return false;
    }
    
public:
    ViewGrid(std::function<void()>* pUpdateMenuAndTB)
    : _lblUN(tr("lblUN"))
    , _lblPwd(tr("lblPwd"))
    , _hl(4)
    , _btnStartStop(tr("start"))
    , _btnClearText(tr("delete"))
    , _showUNandPwd(tr("shPWD"))
    , _gl(4,4)
    , _timer(this, 2.0f, false)
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    {
        _btnStartStop.setToolTip(tr("startTT"));
        _showUNandPwd.setAsDefault();
        _gl.insert(0,0, _lblUN);
        _gl.insert(0,1, _un);
        _gl.insert(0,2, _lblPwd);
        _gl.insert(0,3, _pwd);
        _gl.insert(1,0, _progIndicator, -1);
        
        _hl.append(_btnStartStop);
        _hl.appendSpacer();
        _hl.append(_btnClearText);
        _hl.append(_showUNandPwd);
        
        _gl.insert(2,0, _hl, -1);
        _gl.insert(3,0, _textEdit, -1);
        
        setLayout(&_gl);
    }
    
    bool isRunning() const
    {
        return _timer.isRunning();
    }
    
    void startStop()
    {
        if (_timer.isRunning())
        {
            _timer.stop();
            handleStop();
            (*_pUpdateMenuAndTB)();
        }
        else
        {
            _timer.start();
            _currProgress = 0;
            _progIndicator.setValue(_currProgress);
            _btnStartStop.setTitle(tr("stop"));
            _btnStartStop.setToolTip(tr("stopTT"));
            (*_pUpdateMenuAndTB)();
        }
    }
    
    void handleStop()
    {
        _btnStartStop.setTitle(tr("start"));
        _btnStartStop.setToolTip(tr("startTT"));
    }
    
    virtual bool onTimer(gui::Timer* pTimer)
    {
        _textEdit.appendString("--Timer--:");
        _currProgress += 0.05;
        _progIndicator.setValue(_currProgress);
        return true;
    }
    
};
