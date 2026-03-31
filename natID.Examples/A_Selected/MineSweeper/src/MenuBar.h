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
        size_t actionID = 0;

        items[actionID++].initAsActionItem(tr("Easy"), 20, "<Alt>e"); //Option+E on macOS, Alt+E on other systems

        items[actionID++].initAsActionItem(tr("Medium"), 21, "<Alt>m"); //Option+M; on macOS, Alt+M on other systems

        items[actionID++].initAsActionItem(tr("Hard"), 22, "<Ctrl><Alt>d"); //Ctrl+Option+D on macOS, Ctrl+Alt+D on other systems (D-difficult(

        items[actionID++].initAsSeparator();

        items[actionID++].initAsActionItem(tr("Restart"), 23, "r"); //equal to Cmd+R on macos, Ctrl+R on other systems
    }
public:
    MenuBar()
    : gui::MenuBar(2) //two menus
    , subFirst(10, tr("App"), 3) //allocate items for the Application subMenu
    , subThread(20, tr("game"), 5) //allocate items for the Model subMenu
    {
        populateFirstMenu();
        populateThreadMenu();
        _menus[0] = &subFirst;
        _menus[1] = &subThread;
    }
    
    ~MenuBar()
    {
    }
};
