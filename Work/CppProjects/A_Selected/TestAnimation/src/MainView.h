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

#include "ViewFighter.h"

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
    ViewFighter _animation;
    gui::GridLayout _gl;
    std::function<void()>* _pUpdateMenuAndTB;
    float _frequency = 5.0f;
    bool _bRestartFronInitialPosition = false; //if false it will continue from the previous position
protected:
    
    bool onKeyPressed(const gui::Key& key) override
    {        
        if (key.getType() == gui::Key::Type::ASCII)
        {
            char ch = key.getChar();
            if (ch == ' ')
            {
                startStop();
                return true;
            }
        }
        return false;
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnUpdate)
        {
            _frequency = _neFrequency.getValue().r4Val();
            _frequency = _frequency / 60;
            
            bool bUseCustomColor = _cbChangeColor.getValue();
            td::ColorID jetClrID = _jetColor.getValue();
            td::ColorID etfClrID = _etfColor.getValue();
            _animation.setVisualEffects(_frequency, bUseCustomColor, jetClrID, etfClrID);
            _animation.setFocus();
            return true;
        }
        else if (pBtn == &_btnBomb)
        {
            _animation.bomb();
            _animation.setFocus();            
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
    , _gl(2,13)
    , _pUpdateMenuAndTB(pUpdateMenuAndTB)
    {
        //setMargins(0,0,0,0);
        _neFrequency.setNumberOfDigitsAfterDecimalPoint(2);
        _neFrequency.setValue(_frequency);
        _neFrequency.setMaxValue(60);
        _neFrequency.setMinValue(0.01);
        _frequency = _frequency / 60;
        _jetColor.setValue(td::ColorID::Blue);
        _etfColor.setValue(td::ColorID::Yellow);
        _btnUpdate.setAsDefault();
        
        auto appProperties = getAppProperties();
        _bRestartFronInitialPosition = appProperties->getValue("restartFromInitialPosition", false);
        
        gui::GridComposer gc(_gl);
        
        gc.startNewRowWithSpace(5,0) << _lblFreq << _neFrequency << _btnBomb << _cbChangeColor << _lblJetColor << _jetColor << _lblETFColor << _etfColor;
        gc.appendSpace(20, 0) << _btnUpdate; gc.appendSpace(5,0);
        gc.appendRow(_animation, -1);
        
        setLayout(&_gl);
    }
    
    bool isRunning() const
    {
        return _animation.isRunning();
    }
    
    void startStop()
    {
        if (_animation.isRunning())
        {
            _animation.stop();
            (*_pUpdateMenuAndTB)();
        }
        else
        {
            _animation.start(_bRestartFronInitialPosition);
            (*_pUpdateMenuAndTB)();
        }
    }

    void setFocusToAnimation()
    {
        _animation.setFocus(); //for keyboard events
    }
    
    void setRestartFromInitialPosition(bool bRestartFromInitialPosition)
    {
        _bRestartFronInitialPosition = bRestartFromInitialPosition;
    }
    
};
