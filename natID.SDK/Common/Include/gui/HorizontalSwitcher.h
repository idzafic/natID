// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HorizontalSwitcher.h
    @brief Template view that provides a row of toggle buttons to switch between views. */
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

/// @brief Template view that shows NITEMS flat toggle buttons arranged horizontally.
///        Each button switches the associated ViewSwitcher to the corresponding view.
/// @tparam NITEMS Number of buttons/views to manage.
template <size_t NITEMS>
class HorizontalSwitcher : public View
{
protected:
    ViewSwitcher* _pViewSwitcher = nullptr; ///< Pointer to the associated ViewSwitcher being controlled.
    ISwitch* _pTurnOn = nullptr; //optional switcher to turn on hidden views ///< Optional switch to restore hidden views when a button is clicked.
    HorizontalLayout _layout; ///< Horizontal layout containing the toggle buttons.
    Button _buttons[NITEMS]; ///< Array of toggle buttons, one per view.
    td::UINT2 _selected = 0; ///< Index of the currently selected button/view.

    HorizontalSwitcher() = delete;
    HorizontalSwitcher(const HorizontalSwitcher&) = delete;
    HorizontalSwitcher& operator =(const HorizontalSwitcher&) = delete;
public:
    /// @brief Constructs the switcher with labels for each button.
    /// @param buttonLabels Initializer list of strings, one label per button. Must match NITEMS.
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

    /// @brief Sets tooltip strings for each button.
    /// @param toolTips Initializer list of tooltip strings. Must match NITEMS.
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

    /// @brief Associates a ViewSwitcher that this control will switch between.
    /// @param pViewSwitcher Pointer to the ViewSwitcher to control.
    void setViewSwitcher(ViewSwitcher* pViewSwitcher)
    {
        _pViewSwitcher = pViewSwitcher;
    }

    /// @brief Programmatically selects a view by index, updating button states.
    /// @param viewPos Zero-based index of the view to show.
    /// @param setFocus Whether to set focus on the shown view.
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

    /// @brief Sets an optional ISwitch to turn on when any button is clicked.
    /// @param pTurnOn Pointer to an ISwitch that will be set to On state on button click.
    void setViewRestorer(ISwitch* pTurnOn)
    {
        _pTurnOn = pTurnOn;
    }
};

} //namespace gui
