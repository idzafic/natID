//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ContextMenus.h>

class CtxRemoveViews : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    CtxRemoveViews()
    : _cmi(100, "cmRV", 5)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("CloseAllOnLeft"), 10); //move shape to front
        items[1].initAsActionItem(tr("CloseAllOnRight"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("CloseAllButThis"), 30); //delete shape
        items[4].initAsActionItem(tr("CloseAll"), 40); //delete shape
        setItem(&_cmi);
    }
};


class CtxHandleInView : public gui::ContextMenu
{
private:
    gui::SubMenu _cmi;
protected:

public:
    CtxHandleInView()
    : _cmi(101, "cmSE", 2)
    {
        auto& items = _cmi.getItems();
        items[0].initAsActionItem(tr("Option1"), 10); //move shape to front
        items[1].initAsActionItem(tr("Option2"), 20); //move shape to background
        setItem(&_cmi);
    }
};

class ContextMenus : public gui::ContextMenus
{
protected:
    CtxRemoveViews _cmRemoveTBViews;
    CtxHandleInView _cmHandleInView;
public:
    ContextMenus()
    : gui::ContextMenus(2) //two menuw
    {
        setItem(0, &_cmRemoveTBViews);
        setItem(1, &_cmHandleInView);
    }
};
