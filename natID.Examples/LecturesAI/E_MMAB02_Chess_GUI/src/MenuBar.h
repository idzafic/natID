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
        items[0].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateThreadMenu()
    {
        auto& items = _subThread.getItems();
        items[0].initAsActionItem(tr("start"), cStartStopActionItem);
        items[0].setAsCheckable();
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , _subFirst(cMenuApp, tr("App"), 1) //allocate items for the Application subMenu
    , _subThread(cMenuGame, tr("Animation"), 1) //allocate items for the Model subMenu
    {
        populateFirstMenu();
        populateThreadMenu();
        _menus[0] = &_subFirst;
        _menus[1] = &_subThread;
    }
};
