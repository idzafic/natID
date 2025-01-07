//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/SymbolPopoverView.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class SymbolsPopover : public gui::SymbolPopoverView
{
    gui::Symbol _pointer;
    gui::Symbol _rect;
    gui::Symbol _roundedRect;
    gui::Symbol _circ;
    //gui::Image _toolbarItemImage;
public:
    SymbolsPopover()
    : SymbolPopoverView(gui::PopoverView::Type::SymbolOnly, 4, 4)
    , _pointer(":pointer")
    , _rect(":rect")
    , _roundedRect(":roundRct")
    , _circ(":circ")
    //, _toolbarItemImage(":poly128")
    {
        gui::Symbol* nullSymbol = nullptr;
        addItem(&_pointer, nullSymbol,  tr("pointer"), tr("pointerTT"), false, 0);
        addItem(&_rect, nullSymbol, tr("rect"), tr("rectTT"), true, 0);
        addItem(&_roundedRect, nullSymbol, tr("rndRct"), tr("rndRctTT"), true, 0);
        addItem(&_circ, nullSymbol, tr("circ"), tr("circTT"), true, 0);
        
        styleOnParent(false); //remove this line if you want to see color symbols on toolbar
    }
    
};
