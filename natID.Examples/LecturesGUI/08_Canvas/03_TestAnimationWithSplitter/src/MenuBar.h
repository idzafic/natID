//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//

#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu subFirst;
    gui::SubMenu subThread;
protected:
    void populateFirstMenu()
    {
        auto& items = subFirst.getItems();
        items[0].initAsActionItem(tr("settings"), 10); //prevedeno u natGUI
        items[1].initAsSeparator();
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateThreadMenu()
    {
        auto& items = subThread.getItems();
        items[0].initAsActionItem(tr("start"), 10);
        items[0].setAsCheckable();
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subFirst(10, tr("App"), 3) //allocate items for the Application subMenu
    , subThread(20, tr("timer"), 1) //allocate items for the Model subMenu
    {
        populateFirstMenu();
        populateThreadMenu();
        _menus[0] = &subFirst;
        _menus[1] = &subThread;
//        prepare();
    }
    
    ~MenuBar()
    {
    }
};
