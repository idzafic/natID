// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "View.h"
#include "HorizontalLayout.h"
#include "ViewSwitcher.h"
#include "Button.h"
#include "ISwitch.h"
#include <initializer_list>

namespace gui
{

template <size_t NITEMS>
class HorizontalSwitcher : public View
{
protected:
    ViewSwitcher* _pViewSwitcher = nullptr;
    ISwitch* _pTurnOn = nullptr; //optional switcher to turn on hidden views
    HorizontalLayout _layout;
    Button _buttons[NITEMS];
    td::UINT2 _selected = 0;
    HorizontalSwitcher() = delete;
public:
    HorizontalSwitcher(const std::initializer_list<td::String>& buttonLabels)
    : _layout(NITEMS)
    {
        assert(NITEMS == buttonLabels.size());
        setMargins(0,0,0,0);
        _layout.setMargins(0,0);
        td::UINT4 i = 0;
        for (const td::String& str : buttonLabels)
        {
            gui::Button& button = _buttons[i];
            button.setTitle(str);
            button.setFlat();
            button.setToMinSize();
            if (i == 0)
                button.setChecked(true);
            
            _layout << button;
            button.onClick([this, i]
            {
                //main thing
                if (_pViewSwitcher)
                {
                    if (_selected != i)
                    {
                        _buttons[_selected].setChecked(false);
                        _selected = i;
                        _buttons[_selected].setChecked(true);
                    }
                    _pViewSwitcher->showView((int)i, true);
                }
                //optional : unhide some views
                if (_pTurnOn)
                    _pTurnOn->setState(ISwitch::State::On);
            });
            ++i;
        }
        assert(i == NITEMS);
        setLayout(&_layout);
    }
    
    void setToolTips(const std::initializer_list<td::String>& toolTips)
    {
        assert(NITEMS == toolTips.size());
        td::UINT4 i = 0;
        for (const td::String& str : toolTips)
        {
            gui::Button& button = _buttons[i];
            button.setToolTip(str);
            ++i;
        }
    }

    void setViewSwitcher(ViewSwitcher* pViewSwitcher)
    {
        _pViewSwitcher = pViewSwitcher;
    }

    void show(size_t viewPos, bool setFocus = true)
    {
        if (_pViewSwitcher)
        {
            if (_selected != viewPos)
            {
                _buttons[_selected].setChecked(false);
                _selected = viewPos;
                _buttons[_selected].setChecked(true);
            }
            
            _pViewSwitcher->showView(viewPos, setFocus);
        }
    }
    
    void setViewRestorer(ISwitch* pTurnOn)
    {
        _pTurnOn = pTurnOn;
    }
};

} //namespace gui
