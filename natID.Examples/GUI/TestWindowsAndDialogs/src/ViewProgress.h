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

class ViewProgress : public gui::View
{
private:
protected:
    gui::Label _lblStatus;
    gui::ProgressIndicator _progInd;
    gui::VerticalLayout _vl;
protected:
    void updateStatus(double val)
    {
        //double val = valIn.r8Val();
        _progInd.setValue(val);
        td::String str;
        str.format("%.1f/10", val*10);
        _lblStatus = str;
    }
    
    void closeMe()
    {
        gui::Dialog* parent = dynamic_cast<gui::Dialog*>(getParentWindow());
        assert(parent);
        parent->close();
    }
    
public:
    ViewProgress()
    : _lblStatus("0/100")
    , _vl(2)
    {
        _lblStatus.setResizable(32);
        _vl.setMargins(8, 0);
        _lblStatus.setAlignment(td::HAlignment::Center);
        _vl << _lblStatus << _progInd;
        
        setLayout(&_vl);
    }
    
    
    void setValue(double val, bool bClose)
    {
        assert(val >= 0.0 && val <= 1.0);

        // invoke updateStatus in the main thread
        auto fnUpdateProgInd = [this](td::Variant v)
            {
                double val = v.r8Val();
                updateStatus(val);
            };
        td::Variant varVal(val);
        gui::thread::asyncExecInMainThread(fnUpdateProgInd, varVal);

        if (bClose)
        {
            // invoke closeMe in the main thread
            auto fnCloseDlg = [this]()
                {
                    closeMe();
                };
            gui::thread::asyncExecInMainThread(fnCloseDlg);
        }
    }
};
