//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SplitterLayout.h>
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/Timer.h>
#include <gui/DrawableString.h>
#include <gui/Transformation.h>
#include <gui/Font.h>
#include "ViewETF.h"
#include "ViewPropreties.h"
#include <cmath>



class MainView : public gui::View
{
protected:
    std::function<bool(void)> _fcnIsRunning;
    ViewETF _animation;
    std::function<void(td::ColorID, td::ColorID, float, Direction, double)> _fcnChangeSettings;
    gui::SplitterLayout _splitter;
    ViewProperties _properties;
    gui::Timer _timer;

protected:
    
    bool onTimer(gui::Timer* pTimer) override
    {
        float tNow = _timer.getInterval();
        _animation.reDrawScene( tNow );
        return true;
    }
    
public:
    MainView()
        : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)
        , _timer(this, dT, false)
        //, _fcnIsTimerRunning(std::bind(&MainView::startStop, this))
        , _fcnChangeSettings(std::bind(&ViewETF::changeSettings, &_animation, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5))
        , _fcnIsRunning(std::bind(&MainView::isRunning, this))
        , _animation(_fcnIsRunning)
        , _properties(_fcnChangeSettings)
    {
        setMargins(0, 0, 0, 0);
        _splitter.setContent(_animation, _properties);
        setLayout(&_splitter);
    }
    
    void startStopAnimation()
    {
        if (_timer.isRunning())
        {
            _timer.stop();
        }
        else
        {
            _timer.start();
        }
    }

    bool isRunning()
    {
        return _timer.isRunning();
    }
    
    
    ViewETF& getAnimation()
    {
        return _animation;
    }
    
};
