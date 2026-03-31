//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <gui/MenuBar.h>
#include "Constants.h"

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu _subFirst;
    gui::SubMenu _subThread;
protected:
    void populateFirstMenu()
    {
        auto& items = _subFirst.getItems();
        items[0].initAsActionItem(tr("settings"), 10); //prevedeno u natGUI
        items[1].initAsSeparator();
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateThreadMenu()
    {
        auto& items = _subThread.getItems();
        items[0].initAsActionItem(tr("start"), 10);
        items[0].setAsCheckable();
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , _subFirst(cMenuApp, tr("App"), 3) //allocate items for the Application subMenu
    , _subThread(cMenuAnimation, tr("Animation"), 1) //allocate items for the Model subMenu
    {
        populateFirstMenu();
        populateThreadMenu();
        _menus[0] = &_subFirst;
        _menus[1] = &_subThread;
    }
};
