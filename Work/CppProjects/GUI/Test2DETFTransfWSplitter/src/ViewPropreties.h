//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright � 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/ColorPicker.h>
#include <gui/ComboBox.h>
#include <gui/Button.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/Timer.h>
#include <gui/Slider.h>
#include "common.h"


class ViewProperties : public gui::View
{
protected:
    gui::Label _lblFreq;
    gui::NumericEdit _neFrequency;

    gui::Label _lblDirection;
    gui::ComboBox _direction;

    gui::Label _lblColor;
    gui::ColorPicker _color;

    gui::Label _lblLineColor;
    gui::ColorPicker _lineColor;

    gui::Button _btnUpdate;
    gui::HorizontalLayout _hl1;

    gui::Label _lblRad;
    gui::Slider _rotRad;

    gui::VerticalLayout _vl;
    std::function<void(td::ColorID, td::ColorID, float, Direction, double)> _callBackChangeSettings;
    float _frequency = 20.0f;
protected:
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnUpdate)
        {
            refresh();
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == &_rotRad)
        {
            refresh();
            return true;
        }
        return false;
    }
    
    void refresh()
    {
        td::Variant valFreq, valDir;
        _neFrequency.getValue(valFreq);
//        if (_direction.getSelectedText() == tr("clkWise")) //nikad preko stringa jer je sporo
        int currSelection = _direction.getSelectedIndex();
        if (currSelection == 0)
            _callBackChangeSettings(_color.getValue(), _lineColor.getValue(), valFreq.r4Val(), Direction::CLOCKWISE, _rotRad.getValue());
        else
            _callBackChangeSettings(_color.getValue(), _lineColor.getValue(), valFreq.r4Val(), Direction::COUNTRER_CLOCKWISE, _rotRad.getValue());
    }
    
public:
    ViewProperties(std::function<void(td::ColorID, td::ColorID, float, Direction, double)>& callBackChangeSettings) // setMargine(10, 5, 10, 5).
        : _lblFreq(tr("lblFreq"))
        , _neFrequency(td::real4, gui::LineEdit::Messages::Send)
        , _lblDirection(tr("lblDir"))
        , _lblColor(tr("lblColor"))
        , _lblLineColor(tr("lblLineColor"))
        , _btnUpdate(tr("Update"))
        , _color(tr("custColor"))
        , _lblRad(tr("lblRotRad"))
        , _callBackChangeSettings(callBackChangeSettings)
        , _vl(13)
        , _hl1(3)
    {
        setMargins(10, 5, 10, 5);
        _neFrequency.setNoDec(2);
        _neFrequency.setValue(_frequency);
        _neFrequency.setMaxValue(60);
        _neFrequency.setMinValue(0.1);

        _btnUpdate.setAsDefault();

        _hl1.appendSpacer();
        _hl1.append(_btnUpdate);
        _hl1.appendSpacer();

        
        _color.setValue(td::ColorID::Blue);
        _lineColor.setValue(td::ColorID::White);

        _rotRad.setValue(0.5);
        _vl << _lblFreq << _neFrequency << _lblDirection << _direction << _lblColor << _color << _lblLineColor << _lineColor << _lblRad << _rotRad;
        _vl.appendSpace(1);
        _vl.append(_hl1);
        _vl.appendSpacer();

        // populate combo
        _direction.addItem(tr("clkWise"));
        _direction.addItem(tr("cntrClckWise"));
        _direction.selectIndex(0);
        setLayout(&_vl);
    }
    
};
