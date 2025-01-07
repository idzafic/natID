//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SymbolPopoverView.h>
#include <gui/Image.h>

class SymbolsPopoverVerticalWithText : public gui::SymbolPopoverView
{
    gui::Image _easy;
    gui::Image _medium;
    gui::Image _hard;
public:
    SymbolsPopoverVerticalWithText()
    : SymbolPopoverView(gui::PopoverView::Type::SymbolAndText, 4, PopoverView::SymbolWidth::AdjustToMaxWHRatio)
    , _easy(":Easy")
    , _medium(":Medium")
    , _hard(":Hard")
    {
        gui::Symbol* nullSymbol = nullptr;
        addItem(&_easy, tr("Easy"), tr("EasyTT"), 2.0f, 2.0f);
        addItem(&_medium, tr("Medium"), tr("MediumTT"), 2.0f, 2.0f);
        addItem(&_hard, tr("Hard"), tr("HardTT"), 2.0f, 2.0f);
        addItem((gui::Image*)nullptr, tr("Restart"), tr("RestartTT"), 2.0f, 2.0f);
//        setItem(itemPos++, &_img, tr("Save"), tr("SaveTT"), 3.0f, 3.0f);
        
    }
    
};
