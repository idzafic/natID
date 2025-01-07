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
#include <gui/GridLayout.h>
#include <gui/Timer.h>
#include <gui/Slider.h>
#include "SimulationParams.h"

class ViewProperties : public gui::View
{
protected:
    gui::Label _lblInitialX;
    gui::Slider _slInitialX;
    gui::Label _lblK;
    gui::Slider _slK;
    gui::Label _lblG;
    gui::Slider _slG;
    gui::Label _lblM;
    gui::Slider _slM;
    gui::CheckBox _cbChangeColor;
    gui::Label _lblJetColor;
    gui::ColorPicker _fillColor;
    gui::Label _lblETFColor;
    gui::ColorPicker _etfColor;
//    gui::Button _btnUpdate;
    gui::VerticalLayout _vl;
//    gui::HorizontalLayout _hl;
    gui::GridLayout _gl;
    gui::Label _lblParams;
    gui::Label _lblL;
    gui::NumericEdit _edL;
    gui::Label _lblValK;
    gui::NumericEdit _edK;
    gui::Label _lblValG;
    gui::NumericEdit _edG;
    gui::Label _lblValM;
    gui::NumericEdit _edM;
    gui::Label _lblInitialVals;
    gui::Label _lblX0;
    gui::NumericEdit _edX0;
    gui::Label _lblY0;
    gui::NumericEdit _edY0;
    SimulationParams* _pSimParams;
    std::function<void()>* _pFnSetVisualEffects = nullptr;
    float _frequency = 5.0f;
protected:
    void populateSimulationData()
    {
        assert(_pSimParams);
        bool bUseCustomColor = _cbChangeColor.getValue();
        td::ColorID fillClrID = _fillColor.getValue();
        td::ColorID fontClrID = _etfColor.getValue();
        double k = _slK.getValue();
        double g = _slG.getValue();
        double m = _slM.getValue();
        double x0 = _slInitialX.getValue();
        double l = _pSimParams->l;
        _pSimParams->x0 = x0;
        _pSimParams->y0 = std::sqrt(l*l-x0*x0);
        _pSimParams->g = g;
        _pSimParams->m = m;
        _pSimParams->k = k;
        _pSimParams->useCustomColors = bUseCustomColor;
        _pSimParams->fillColor = fillClrID;
        _pSimParams->fontColor = fontClrID;
        td::Decimal2 d2 = g;
        _edG.setValue(d2);
        d2 = m;
        _edM.setValue(d2);
        
        td::Decimal4 d4 = k;
        _edK.setValue(d4);
        d4 = x0;
        _edX0.setValue(d4);
        d4 = _pSimParams->y0;
        _edY0.setValue(d4);
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (_pFnSetVisualEffects)
        {
            populateSimulationData();
            (*_pFnSetVisualEffects)();
            return true;
        }
        return false;
    }
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (_pFnSetVisualEffects)
        {
            populateSimulationData();
            (*_pFnSetVisualEffects)();
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::ColorPicker* pCP) override
    {
        if (_pFnSetVisualEffects)
        {
            populateSimulationData();
            (*_pFnSetVisualEffects)();
            return true;
        }
        return false;
    }
    
public:
    ViewProperties()
    : _lblInitialX(tr("lblX"))
    , _lblK(tr("lblK"))
    , _lblG(tr("lblG"))
    , _lblM(tr("lblM"))
    , _lblJetColor(tr("lblColor"))
    , _cbChangeColor(tr("custColor"))
    , _lblETFColor(tr("fontColor"))
    , _vl(15)
    , _gl(8,2)
    , _lblParams(tr("lblParams"))
    , _lblL(tr("lblL"))
    , _edL(td::decimal2)
    , _lblValK(tr("lblValK"))
    , _edK(td::decimal4)
    , _lblValG(tr("lblValG"))
    , _edG(td::decimal2)
    , _lblValM(tr("lblValM"))
    , _edM(td::decimal2)
    , _lblInitialVals(tr("lblInitVals"))
    , _lblX0(tr("lblX0"))
    , _edX0(td::decimal4)
    , _lblY0(tr("lblY0"))
    , _edY0(td::decimal4)
    , _pSimParams(nullptr)
    {
        setMargins(10, 10, 4, 4);
        _lblParams.setBold();
        _lblInitialVals.setBold();
        
        td::Decimal2 dec2Val = 1.0;
        _edL.setValue(dec2Val);
        _edL.setAsReadOnly();
        _edX0.setAsReadOnly();
        _edY0.setAsReadOnly();
        _edK.setAsReadOnly();
        _edG.setAsReadOnly();
        _edM.setAsReadOnly();
        _edM.setValue(dec2Val);
        dec2Val = 9.81;
        _edG.setValue(dec2Val);
        
        _slInitialX.setRange(-0.8, 0.8);
        _slInitialX.setValue(X0);
        _slK.setRange(0, 0.5);
        _slK.setValue(InitialK);
        _slG.setRange(0.5, 20);
        _slG.setValue(9.81);
        _slM.setRange(0.1, 10);
        _slM.setValue(1.0);
        _slM.disable(true);
        
        _frequency = _frequency / 60;

        _fillColor.setValue(td::ColorID::Blue);
        _etfColor.setValue(td::ColorID::Yellow);
        
        _vl << _lblInitialX << _slInitialX << _lblK << _slK << _lblG << _slG << _lblM << _slM << _cbChangeColor << _lblJetColor << _fillColor << _lblETFColor << _etfColor;
        _vl.appendSpacer();
        
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblParams, 0);
        gc.appendRow(_lblL); gc.appendCol(_edL);
        gc.appendRow(_lblValK); gc.appendCol(_edK);
        gc.appendRow(_lblValG); gc.appendCol(_edG);
        gc.appendRow(_lblValM); gc.appendCol(_edM);
        gc.appendRow(_lblInitialVals, 0);
        gc.appendRow(_lblX0); gc.appendCol(_edX0);
        gc.appendRow(_lblY0); gc.appendCol(_edY0);
        _vl.append(_gl);
        
        setLayout(&_vl);
    }

    void setCallback(SimulationParams* pSimParams, std::function<void()>* pFnSetVisualEffects)
    {
        _pSimParams = pSimParams;
        _pFnSetVisualEffects = pFnSetVisualEffects;
    }
    
    void enableControls()
    {
        _slInitialX.disable(false);
        _slK.disable(false);
//        _slM.disable(false);
        _slG.disable(false);
    }
    
    void disableControls()
    {
        _slInitialX.disable(true);
        _slK.disable(true);
//        _slM.disable(true);
        _slG.disable(true);
    }
    
};
