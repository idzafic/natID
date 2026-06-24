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
        items[0].initAsQuitAppActionItem(tr("Quit"), "q");
    }
public:
    MenuBar()
    : gui::MenuBar(1)
    , subApp(10, "App", 1)
    {
        populateSubAppMenu();
        _menus[0] = &subApp;
    }
};
