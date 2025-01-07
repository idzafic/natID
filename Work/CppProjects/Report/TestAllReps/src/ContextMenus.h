//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ContextMenus.h>

class ContextMenu1 : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    ContextMenu1()
    : _cmi(10, "cm1", 4)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("Update"), 10); //move shape to front
        items[1].initAsActionItem(tr("AddParams"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("Properies"), 30); //delete shape
        setItem(&_cmi);
    }
};

class ContextMenu2 : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    ContextMenu2()
    : _cmi(20, "cm2", 4)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("Ingradient"), 10); //move shape to front
        items[1].initAsActionItem(tr("Nutrition"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("Details"), 30); //delete shape
        setItem(&_cmi);
    }
};

class ContextMenu3 : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    ContextMenu3()
    : _cmi(30, "cm3", 4)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("Ingradient2"), 10); //move shape to front
        items[1].initAsActionItem(tr("Nutrition2"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("Details2"), 30); //delete shape
        setItem(&_cmi);
    }
};

class ContextMenus : public gui::ContextMenus
{
protected:
    ContextMenu1 _cm1;
    ContextMenu2 _cm2;
    ContextMenu3 _cm3;
public:
    ContextMenus()
    : gui::ContextMenus(3) //one menu
    {
        setItem(0, &_cm1);
        setItem(1, &_cm2);
        setItem(2, &_cm3);
    }
};
