//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Slider.h>
#include <gui/CheckBox.h>
#include <gui/View.h>
#include <gui/HorizontalLayout.h>
#include <functional>

class TBSlider : public gui::Slider
{
protected:
    
    void getMinSize(gui::Size& minSize) const override
    {
        minSize.height = 22;
        minSize.width = 150;
    }
    
public:
    TBSlider()
    {
        setRange(0.01, 0.1);
        setValue(0.01);
    }
    
//    void getRealSize(gui::Size& sz)
//    {
//        getSize(sz);
//    }
};

class TBCheckBox : public gui::CheckBox
{
protected:
    
    void getMinSize(gui::Size& minSize) const override
    {
        minSize.height = 24;
        minSize.width = 150;
    }
    
public:
    TBCheckBox()
    : gui::CheckBox("Switch textures")
    {
    }
};

class TBSliderView : public gui::View
{
protected:
    TBSlider _sliderSpeed;
    TBCheckBox _cb;
    TBSlider _sliderScale;
    gui::HorizontalLayout _hl;
    std::function< void(float)> _updateFont3Scale;
protected:
public:
    TBSliderView()
    : _hl(4)
    {
        setMargins(0, 0, 0, 0);
        _hl.setMargins(0,0);
        _hl.setSpaceBetweenCells(8);
        _sliderScale.setRange(0.2, 1);
        _sliderScale.setValue(0.6);
        _hl.appendSpace(5);
        _hl << _sliderSpeed << _cb << _sliderScale;
        setLayout(&_hl);
        
        _sliderScale.onChangedValue([this]()
        {
            float scale = (float) _sliderScale.getValue();
            _updateFont3Scale(scale);
        });
    }
    
    void setFontScaleUpdater(const std::function< void(float)>& updateFont3Scale)
    {
        _updateFont3Scale = updateFont3Scale;
    }
    
    TBSlider* getSliderSpeed()
    {
        return &_sliderSpeed;
    }
    
    TBCheckBox* getCheckBox()
    {
        return &_cb;
    }
    
    TBSlider* getSliderScale()
    {
        return &_sliderSpeed;
    }
};
