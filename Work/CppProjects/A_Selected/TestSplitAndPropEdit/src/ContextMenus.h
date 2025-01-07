//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ContextMenus.h>

class ContextMenu : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    ContextMenu()
    : _cmi(100, "cm1", 4)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("SetFront"), 10); //move shape to front
        items[1].initAsActionItem(tr("SetBack"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("Delete"), 30); //delete shape
        setItem(&_cmi);
    }
};

class ContextMenus : public gui::ContextMenus
{
protected:
    ContextMenu _cm1;
public:
    ContextMenus()
    : gui::ContextMenus(1) //one menu
    {
        setItem(0, &_cm1);
    }
};
