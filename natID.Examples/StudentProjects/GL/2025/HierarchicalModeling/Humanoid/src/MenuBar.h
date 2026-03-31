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
    
    void populateFrameMenu()
    {
        auto& items = subEdit.getItems();
        items[0].initAsActionItem(tr("Export"), 10, "e");
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subApp(10, "testApp", 3) //allocate items for the Application subMenu
    , subEdit(20, "Frame", 1) //allocate items for the Edit subMenu
    {
        populateSubAppMenu();
        populateFrameMenu();
        _menus[0] = &subApp;
        _menus[1] = &subEdit;
    }
};
