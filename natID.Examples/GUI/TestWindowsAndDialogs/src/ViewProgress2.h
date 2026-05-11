//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ProgressIndicator.h>
#include <gui/VerticalLayout.h>
#include <gui/Dialog.h>
#include <gui/Thread.h>
#include <thread>
#include <thread/Thread.h>
#include <mutex>
#include <mu/IProgressor.h>

class ViewProgress2 : public gui::View, public gui::Dialog::CloseChecker
{
private:
protected:
    gui::Label _lblStatus;
    gui::ProgressIndicator _progInd;
    gui::VerticalLayout _vl;
    std::thread _worker; //this thread should do something for you
    std::mutex _mtx;
    bool _bCanceled = false;
    
protected:
    //from gui::Dialog::CloseChecker
    bool shouldTriggerClose(gui::Dialog::Button::ID, gui::Button*) override final
    {
        //when we press Cancel button dialog just send stop to the solver,
        //the dialog will be closed after solver completes the process
        return false;
    }
    
    void updateStatus(double val)
    {
        _progInd.setValue(val);
        td::String str;
        str.format("%.1f/10.0", val*10);
        _lblStatus = str;
    }
    
    void closeMe()
    {
        if (_worker.joinable())
            _worker.join();
        gui::Dialog* parent = dynamic_cast<gui::Dialog*>(getParentWindow());
        assert(parent);
        parent->close();
    }
    
    void onInitialAppearance() override final
    {
        int k=5;
        ++k;
    }
    
    void workerMethod()
    {
        try
        {
            for (int i = 1; i <= 1000; ++i)
            {
                const double val = static_cast<double>(i) / 1000.0;
                
                {
                    std::lock_guard<std::mutex> lock(_mtx);
                    if (_bCanceled)
                        break;
                }

                gui::thread::asyncExecInMainThread([this, val]()
                {
                    updateStatus(val);
                });
                

                thread::sleepMilliSeconds(10);
            }
        }
        catch (...)
        {
        }
        
        gui::thread::asyncExecInMainThread([this]()
        {
            closeMe();
        });
        return;
    }
    
public:
    ViewProgress2()
    : _lblStatus("0/100")
    , _vl(2)
    {
        _lblStatus.setResizable(48);
        _vl.setMargins(8, 0);
        _lblStatus.setAlignment(td::HAlignment::Center);
        _vl << _lblStatus << _progInd;
        
        setLayout(&_vl);
    }
    
   ~ViewProgress2()
    {
       mu::dbgLog("ViewProgress2 destructor!");
   }
    
    void start()
    {
        _worker = std::thread(&ViewProgress2::workerMethod, this);
    }
    
    void stop()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _bCanceled = true;
    }
    
    //concepts (C++ 20)
    //Panel checks if this method exists at compile time
    static td::HAlignment getDialogButtonAlignment()
    {
        return td::HAlignment::Center;
    }
};
