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
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/Timer.h>

class ViewProperties : public gui::View
{
protected:
    gui::Label _lblFreq;
    gui::NumericEdit _neFrequency;
    gui::CheckBox _cbChangeColor;
    gui::Label _lblJetColor;
    gui::ColorPicker _jetColor;
    gui::Label _lblETFColor;
    gui::ColorPicker _etfColor;
    gui::Button _btnUpdate;
    gui::VerticalLayout _vl;
    gui::HorizontalLayout _hl;
    std::function<void(float, bool, td::ColorID, td::ColorID)>* _pFnSetVisualEffects = nullptr;
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
            if (_pFnSetVisualEffects)
                (*_pFnSetVisualEffects)(_frequency, bUseCustomColor, jetClrID, etfClrID);
//            _animation.setVisualEffects(bUseCustomColor, jetClrID, etfClrID);
            return true;
        }
        return false;
    }
    
public:
    ViewProperties()
    : _lblFreq(tr("lblFreq"))
    , _neFrequency(td::real4, gui::LineEdit::Messages::Send)
    , _lblJetColor(tr("lblColor"))
    , _cbChangeColor(tr("custColor"))
    , _lblETFColor(tr("etfColor"))
    , _btnUpdate(tr("Update"))
    , _vl(10)
    , _hl(3)
    {
        _neFrequency.setNoDec(2);
        _neFrequency.setValue(_frequency);
        _neFrequency.setMaxValue(60);
        _neFrequency.setMinValue(0.01);
        _frequency = _frequency / 60;

        _jetColor.setValue(td::ColorID::Blue);
        _etfColor.setValue(td::ColorID::Yellow);
        _btnUpdate.setAsDefault();
        
        _hl.appendSpacer();
        _hl.append(_btnUpdate);
        _hl.appendSpacer();
        
        _vl << _lblFreq << _neFrequency << _cbChangeColor << _lblJetColor << _jetColor << _lblETFColor << _etfColor << _hl;
        _vl.appendSpacer();
        
        setLayout(&_vl);
    }

    void setCallback(std::function<void(float, bool, td::ColorID, td::ColorID)>* pFnSetVisualEffects)
    {
        _pFnSetVisualEffects = pFnSetVisualEffects;
    }
    
};
