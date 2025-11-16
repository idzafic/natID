//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
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
        items[0].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateSubFileMenu()
    {
        auto& items = subFile.getItems();
        items[0].initAsActionItem(tr("open"), 10, "o");
        items[1].initAsActionItem(tr("save"), 20, "s");
        items[2].initAsActionItem(tr("rect"), 30);
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subApp(10, "App", 1) //allocate items for the Application subMenu
    , subFile(20, "Model", 3) //allocate items for the Model subMenu
    {
        populateSubAppMenu();
        populateSubFileMenu();
        setMenu(0, &subApp);
        setMenu(1, &subFile);
    }
    
    ~MenuBar()
    {
    }
};
