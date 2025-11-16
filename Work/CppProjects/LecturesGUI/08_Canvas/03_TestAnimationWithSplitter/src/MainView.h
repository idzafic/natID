//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SplitterLayout.h>
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ColorPicker.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Timer.h>
#include "ViewFighter.h"
#include "ViewProperties.h"


const float FPS = 50.0f; //desired frame per second
const float dT = 1/FPS;

class MainView : public gui::View
{
protected:
    gui::SplitterLayout _splitter;
    ViewFighter _animation;
    ViewProperties _properties;
    gui::Timer _timer;
    std::function<void()>* _pUpdateMenuAndTB;
    std::function<void(float, bool, td::ColorID, td::ColorID)> _fnSetVisualEffects;
protected:
    
public:
    MainView(std::function<void()>* pUpdateMenuAndTB)
    : _splitter(gui::SplitterLayout::Orientation::Horizontal, gui::SplitterLayout::AuxiliaryCell::Second)
    , _timer(this, dT, false)
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    , _fnSetVisualEffects(std::bind(&ViewFighter::setVisualEffects, &_animation, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4))
    {
        setMargins(0, 0, 0, 0);
        _splitter.setContent(_animation, _properties);
        _properties.setCallback(&_fnSetVisualEffects);
        setLayout(&_splitter);
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
            (*_pUpdateMenuAndTB)();
        }
        else
        {
            _timer.start();
            _animation.initAngle(math::FLT_PI);
            (*_pUpdateMenuAndTB)();
        }
    }

    bool onTimer(gui::Timer* pTimer) override
    {
        float f = 5.0f / 60;
        float f2 = _animation.getFrequency();
        if (f2 != 0)
            f = f2;
        float dTheta = 2 * math::FLT_PI * f * dT;
        _animation.incrementAngle(dTheta);
        return true;
    }
    
};
