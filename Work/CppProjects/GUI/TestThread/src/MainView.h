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
#include <thread/Thread.h>


class MainView : public gui::View
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
    std::thread _thread;
    gui::AsyncFn _asyncUpdatePB;
    gui::AsyncFn* _pAsyncOnThreadStart;
    gui::AsyncFn* _pAsyncOnThreadStop;
    double _currProgress = 0;
    bool _running = false;
    bool _bStop = false;
    
protected:
    
    void updateProgress()
    {
        _progIndicator.setValue(_currProgress / 100.); //from 0 to 1
    }
    
    //thread method
    void doHeavyTask()
    {
        asyncCall(_pAsyncOnThreadStart); //invoke in main thread
        _running = true;
        _currProgress = 0; //reset indicator
        asyncCall(&_asyncUpdatePB); //invoke in main thread
        
        while (_currProgress < 100)
        {
            if (_bStop)
            {
                asyncCall(_pAsyncOnThreadStop); //invoke in main thread
                return;
            }
            thread::sleepMilliSeconds(500); //simulate some work
            _currProgress += 5;
            asyncCall(&_asyncUpdatePB); //invoke in main thread
        }
        _currProgress = 100;
        asyncCall(&_asyncUpdatePB); //invoke in main thread
        asyncCall(_pAsyncOnThreadStop); //invoke in main thread
    }
    
    bool onClick(gui::Button* pBtn) override
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
    MainView(gui::AsyncFn* pOnStart, gui::AsyncFn* pOnStop)
    : _lblUN(tr("lblUN"))
    , _lblPwd(tr("lblPwd"))
    , _hl(4)
    , _btnStartStop(tr("start"))
    , _btnClearText(tr("delete"))
    , _showUNandPwd(tr("shPWD"))
    , _gl(4,4)
    , _asyncUpdatePB(std::bind(&MainView::updateProgress, this))
    , _pAsyncOnThreadStart(pOnStart)
    , _pAsyncOnThreadStop(pOnStop)
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
        return _running;
    }
    
    void startStop()
    {
        if (_running)
        {
            _bStop = true;
        }
        else
        {
            _bStop = false;
            _thread = std::thread(&MainView::doHeavyTask, this);
            _btnStartStop.setTitle(tr("stop"));
            _btnStartStop.setToolTip(tr("stopTT"));
        }
    }
    
    void handleStop()
    {
        _btnStartStop.setTitle(tr("start"));
        _btnStartStop.setToolTip(tr("startTT"));
        _thread.join();
        _running = false;
        _bStop = false;
    }
    
};
