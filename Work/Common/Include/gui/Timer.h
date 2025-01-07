// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NatObject.h"
#include "IMessageConsumer.h"

namespace gui
{
class TimerHelper;

class NATGUI_API Timer : public NatObject
{
    friend class TimerHelper;
private:
    std::function<void()> _onTimer;
protected:
    gui::IMessageConsumer* _pConsumer = nullptr;
    float _interval = 1.0f;
    td::UINT4 _id = 0;
//    td::BYTE _running = 0;
protected:
    Timer();
public:
    Timer(IMessageConsumer* pConsumer, float interval, bool bStart = true);
    ~Timer();
    gui::ObjType getObjType() const override { return ObjType::Timer;}
    void start();
    void stop();
    void setInterval(float interval);
    float getInterval() const;
    bool isRunning() const;

    const std::function<void()>& getTimerHandler() const;
    void onTimer(const std::function<void()>& fnToCall);
};

} //namespace gui

