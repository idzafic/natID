//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu subApp;
    gui::SubMenu subEdit;
protected:
    void populateSubAppMenu()
    {
        auto& items = subApp.getItems();
        items[0].initAsActionItem(tr("Properties"), 10, "p");
        //tems[1] is separator
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); //id 0 is used to terminate app
    }
    
    void populateSubEdtMenu()
    {
        auto& items = subEdit.getItems();
        items[0].initAsActionItem(tr("Find"), 10, "f");
        items[1].initAsActionItem(tr("Reverse"), 20, "r");
        items[2].initAsSubMenu(20, "Submenu_30", 30);
        auto& item2SubMenus = items[2].getItems();
        item2SubMenus[0].initAsActionItem("SubMenu-Item100", 100);
        item2SubMenus[1].initAsActionItem("SubMenu-Item110", 110);
        item2SubMenus[2].initAsSubMenu(120, "SubSubmenu_120", 2);
        auto& item2SubSubMenus = item2SubMenus[2].getItems();
        item2SubSubMenus[0].initAsActionItem("SubSubMenu-120_100", 100);
        item2SubSubMenus[1].initAsActionItem("SubSubMenu-120_110", 110);
        //tems[2] is separator
        items[3].initAsActionItem(tr("ShowScrollViewer"), 20);
        items[4].initAsActionItem(tr("ShowSwitcher"), 40);
        items[5].initAsActionItem(tr("ShowNavig"), 50);
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subApp(10, "App", 3) //allocate items for the Application subMenu
    , subEdit(20, "Edit", 6) //allocate items for the Edit subMenu
    {
        populateSubAppMenu();
        populateSubEdtMenu();
        _menus[0] = &subApp;
        _menus[1] = &subEdit;
        //prepare();
    }
    
    ~MenuBar()
    {
    }
};
