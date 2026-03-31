//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ContextMenus.h>

class ContextMenuWindow : public gui::ContextMenu
{
private:
    gui::SubMenu _main;
protected:

public:
    ContextMenuWindow()
        : _main(100, "ctxWnd", 5)
    {
        auto& items = _main.getItems();
        items[0].initAsActionItem(tr("Show special window"), 10); //move shape to front
        items[1].initAsActionItem(tr("Show better one"), 20); //move shape to background
        items[2].initAsSubMenu(10, "Ctx submenu demo", 3);
        //submenu items
        {
            auto& subMenuItems = items[2].getItems();
            subMenuItems[0].initAsActionItem("First in submenu", 10);
            subMenuItems[1].initAsActionItem("First in submenu", 20);
            subMenuItems[2].initAsActionItem("First in submenu", 30);
        }
        items[3].initAsSeparator();
        items[4].initAsActionItem(tr("Properties..."), 30); //delete shape
        setItem(&_main);
    }

};
class ContextMenuGeom : public gui::ContextMenu
{
private:
    gui::SubMenu _main;    
protected:

public:
    ContextMenuGeom()
    : _main(222, "ctxMain", 6)    
    {        
        auto& items = _main.getItems();
        items[0].initAsActionItem(tr("SetFront"), 10); //move shape to front
        items[1].initAsActionItem(tr("SetBack"), 20); //move shape to background
        items[2].initAsSeparator();
        items[3].initAsSubMenu(10, "This is a submenu", 3);
        //submenu items
        {
            auto& subMenuItems = items[3].getItems();
            subMenuItems[0].initAsActionItem("First in submenu", 10);
            subMenuItems[1].initAsActionItem("First in submenu", 20);
            subMenuItems[2].initAsActionItem("First in submenu", 30);
        }                
        items[4].initAsSeparator();
        items[5].initAsActionItem(tr("Delete"), 30); //delete shape
        setItem(&_main);
    }
};

class ContextMenus : public gui::ContextMenus
{
protected:
    ContextMenuWindow _cm1;
    ContextMenuGeom _cm2;
public:
    ContextMenus()
    : gui::ContextMenus(2) //one menu
    {
        setItem(0, &_cm1);
        setItem(1, &_cm2);
    }
};
