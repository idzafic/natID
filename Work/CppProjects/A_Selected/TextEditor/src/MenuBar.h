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
protected:
    void populateFirstMenu()
    {
        auto& items = subFirst.getItems();
		items[0].initAsActionItem(tr("OpenHTML"), 10);
		items[1].initAsActionItem(tr("SaveHTML"), 20);
		items[2].initAsSeparator();
        items[3].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
public:
    MenuBar()
    : gui::MenuBar(1) //two menus
    , subFirst(10, tr("App"), 4) //allocate items for the Application subMenu
    {
        populateFirstMenu();
        _menus[0] = &subFirst;
    }
    
    ~MenuBar()
    {
    }
};
