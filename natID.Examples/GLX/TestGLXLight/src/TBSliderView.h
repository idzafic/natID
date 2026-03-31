#pragma once
#include <gui/Slider.h>
#include <gui/CheckBox.h>
#include <gui/View.h>
#include <gui/HorizontalLayout.h>

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
    TBCheckBox(td::String txt)
    : gui::CheckBox(txt)
    {
    }
};

class TBSliderView : public gui::View
{
protected:
    gui::HorizontalLayout _hl;
public:
    TBSlider _sliderSpeed;
    TBCheckBox _rotateObject;
protected:
public:
    TBSliderView()
    : _hl(2)
    , _rotateObject ("Rotate")
    {
        setMargins(0, 0, 0, 0);
        _hl.setMargins(0,0);
        _hl  << _rotateObject << _sliderSpeed;
        setLayout(&_hl);
    }
   
};
