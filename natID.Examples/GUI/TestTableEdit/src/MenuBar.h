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
    gui::SubMenu subFile;
protected:
    void populateSubAppMenu()
    {
        auto& items = subApp.getItems();
        //items[0].initAsActionItem(tr("Properties"), 10, "p");
        //tems[1] is separator
        items[0].initAsActionItem(tr("Update"), 10, "u"); //id 0 is used to terminate app
        items[1].initAsQuitAppActionItem(tr("Quit"), "q"); //id 0 is used to terminate app
    }
    
    void populateSubFileMenu()
    {
        auto& items = subFile.getItems();
        items[0].initAsActionItem(tr("Open"), 10, "o");
        items[1].initAsSubMenu(50, "Podmeni", 2);
        //submenu items
        {
            auto& itemsSubMenu = items[1].getItems();
            itemsSubMenu[0].initAsActionItem(tr("Sub1"), 10);
            itemsSubMenu[1].initAsActionItem(tr("Sub2"), 20);
        }
        
        items[2].initAsActionItem(tr("Save"), 20, "s");
    }
    
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subApp(10, "App", 2) //allocate items for the Application subMenu
    , subFile(20, "Model", 3) //allocate items for the Edit subMenu
    {
        populateSubAppMenu();
        populateSubFileMenu();
        _menus[0] = &subApp;
        _menus[1] = &subFile;
//        prepare();
    }
    
    ~MenuBar()
    {
    }
};

