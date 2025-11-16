//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ColorPicker.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/Timer.h>
#include <gui/Sound.h>
#include "ViewFighter.h"

const float FPS = 50.0f; //desired frame per second
const float dT = 1/FPS;

class MainView : public gui::View
{
private:
protected:
    gui::Label _lblFreq;
    gui::NumericEdit _neFrequency;
    gui::CheckBox _cbChangeColor;
    gui::Label _lblJetColor;
    gui::ColorPicker _jetColor;
    gui::Label _lblETFColor;
    gui::ColorPicker _etfColor;
    gui::Button _btnBomb;
    gui::Button _btnUpdate;
    gui::Sound _flightSound;
    gui::Sound _bomb;
    ViewFighter _animation;
    gui::GridLayout _gl;
    gui::Timer _timer;
    std::function<void()>* _pUpdateMenuAndTB;
    float _frequency = 5.0f;

protected:
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnUpdate)
        {
            _frequency = _neFrequency.getValue().r4Val();
            _frequency = _frequency / 60;
            bool bUseCustomColor = _cbChangeColor.getValue();
            td::ColorID jetClrID = _jetColor.getValue();
            td::ColorID etfClrID = _etfColor.getValue();
            _animation.setVisualEffects(bUseCustomColor, jetClrID, etfClrID);
            return true;
        }
        else if (pBtn == &_btnBomb)
        {
            _bomb.play();
            return true;
        }
        return false;
    }
    
public:
    MainView(std::function<void()>* pUpdateMenuAndTB)
    : _lblFreq(tr("lblFreq"))
    , _neFrequency(td::real4, gui::LineEdit::Messages::Send)
    , _lblJetColor(tr("lblColor"))
    , _cbChangeColor(tr("custColor"))
    , _lblETFColor(tr("etfColor"))
    , _btnBomb(tr("Bomb"))
    , _btnUpdate(tr("Update"))
    , _flightSound(":flight")
    , _bomb(":bomb")
    , _gl(2,10)
    , _timer(this, dT, false)
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    {
        _neFrequency.setNoDec(2);
        _neFrequency.setValue(_frequency);
        _neFrequency.setMaxValue(60);
        _neFrequency.setMinValue(0.01);
        _frequency = _frequency / 60;
        _jetColor.setValue(td::ColorID::Blue);
        _etfColor.setValue(td::ColorID::Yellow);
        _btnUpdate.setAsDefault();
        
        gui::GridComposer gc(_gl);
        
        gc.appendRow(_lblFreq) << _neFrequency << _btnBomb << _cbChangeColor << _lblJetColor << _jetColor << _lblETFColor << _etfColor;
        gc.appendSpace(30, 0) << _btnUpdate;
        gc.appendRow(_animation, -1);
        
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
            _flightSound.stop();
            (*_pUpdateMenuAndTB)();
        }
        else
        {
            _timer.start();
            _flightSound.play(true);
            _animation.initAngle(math::FLT_PI);
            (*_pUpdateMenuAndTB)();
        }
    }

    bool onTimer(gui::Timer* pTimer) override
    {   
        float dTheta = 2 * math::FLT_PI * _frequency * dT;
        _animation.incrementAngle(dTheta);
        return true;
    }
    
};
