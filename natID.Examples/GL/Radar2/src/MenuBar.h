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
protected:
    void populateSubAppMenu()
    {
        auto& items = subApp.getItems();
        items[0].initAsActionItem(tr("Properties"), 10, "p");
        //tems[1] is separator
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); //id 0 is used to terminate app
    }
public:
    MenuBar()
    : gui::MenuBar(1) //two menus
    , subApp(10, "testApp", 3) //allocate items for the Application subMenu
    {
        populateSubAppMenu();
        _menus[0] = &subApp;
    }
};
