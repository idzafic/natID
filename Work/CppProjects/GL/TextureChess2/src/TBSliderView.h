//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Slider.h>
#include <gui/CheckBox.h>
#include <gui/View.h>
#include <gui/HorizontalLayout.h>

//class TBSlider : public gui::Slider
//{
//protected:
//    
//    void getMinSize(gui::Size& minSize) const override
//    {
//        minSize.height = 24;
//        minSize.width = 150;
//    }
//    
//public:
//    TBSlider()
//    {
//        setRange(0.01, 1.0f);
//        setValue(1.0f);
//    }
//};

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
//    TBSlider _sliderSpeed;
    TBCheckBox _cb;
    gui::HorizontalLayout _hl;
protected:
public:
    TBSliderView()
    : _hl(2)
    {
        setMargins(0, 0, 0, 0);
        _hl.setMargins(0,0);
        _hl << _cb;
        setLayout(&_hl);
    }
    
//    TBSlider* getSlider()
//    {
//        return &_sliderSpeed;
//    }
    
    TBCheckBox* getCheckBox()
    {
        return &_cb;
    }
};
