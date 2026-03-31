#pragma once
#include <gui/CheckBox.h>
#include <gui/View.h>
#include <gui/HorizontalLayout.h>

class TBReverseCheckBox : public gui::CheckBox
{
protected:
    
    void getMinSize(gui::Size& minSize) const override
    {
        minSize.height = 24;
        minSize.width = 150;
    }
    
public:
    TBReverseCheckBox()
    : gui::CheckBox("Reverse Radar")
    {
    }
};

class TBReverseView : public gui::View
{
protected:
    TBReverseCheckBox _cb;
    gui::HorizontalLayout _hl;
protected:
public:
    TBReverseView()
    : _hl(1)
    {
        setMargins(0, 0, 0, 0);
        _hl.setMargins(0, 0);
        _hl << _cb;
        setLayout(&_hl);
    }
    
    TBReverseCheckBox* getCheckBox()
    {
        return &_cb;
    }
};


