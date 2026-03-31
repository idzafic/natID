#pragma once
#include <gui/CheckBox.h>
#include <gui/View.h>
#include <gui/HorizontalLayout.h>

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
    : gui::CheckBox("Flat shading")
    {
        setValue(true); // Start with flat shading enabled
    }
};

class TBSliderView : public gui::View
{
protected:
    TBCheckBox _cb;
    gui::HorizontalLayout _hl;
public:
    TBSliderView()
    : _hl(1)  // Only one control now
    {
        setMargins(0, 0, 0, 0);
        _hl.setMargins(0,0);
        _hl << _cb;  // Only add checkbox, remove slider
        setLayout(&_hl);
    }
    
    TBCheckBox* getCheckBox()
    {
        return &_cb;
    }
};