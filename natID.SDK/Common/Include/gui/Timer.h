// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Timer.h
    @brief Periodic timer that fires a callback or notifies a message consumer. */
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

/// @brief A repeating timer that delivers periodic events to a consumer or a callback function.
class NATGUI_API Timer : public NatObject
{
    friend class TimerHelper;
private:
    std::function<void()> _onTimer; ///< Callback function invoked on each timer tick.
protected:
    gui::IMessageConsumer* _pConsumer = nullptr; ///< Optional message consumer notified on each tick.
    float _interval = 1.0f; ///< Timer interval in seconds.
    td::UINT4 _id = 0;      ///< System identifier for the running timer.
//    td::BYTE _running = 0;
protected:
    /// @brief Protected default constructor for use by derived classes.
    Timer();
    Timer(const Timer&) = delete;
    Timer& operator =(const Timer&) = delete;
public:
    /// @brief Constructs a timer connected to a message consumer.
    /// @param pConsumer Consumer that receives timer messages on each tick.
    /// @param interval Firing interval in seconds.
    /// @param bStart If true, the timer starts immediately after construction.
    Timer(IMessageConsumer* pConsumer, float interval, bool bStart = true);

    /// @brief Destructor. Stops the timer if running and releases resources.
    ~Timer();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::Timer.
    gui::ObjType getObjType() const override { return ObjType::Timer;}

    /// @brief Starts the timer. Has no effect if the timer is already running.
    void start();

    /// @brief Stops the timer. Has no effect if the timer is not running.
    void stop();

    /// @brief Changes the firing interval.
    /// @param interval New interval in seconds.
    void setInterval(float interval);

    /// @brief Returns the current firing interval.
    /// @return Interval in seconds.
    float getInterval() const;

    /// @brief Returns whether the timer is currently active.
    /// @return True if the timer is running.
    bool isRunning() const;

    /// @brief Returns the currently registered timer callback.
    /// @return Const reference to the callback function.
    const std::function<void()>& getTimerHandler() const;

    /// @brief Registers a callback to be invoked on each timer tick.
    /// @param fnToCall Callback function to register.
    void onTimer(const std::function<void()>& fnToCall);
};

} //namespace gui
