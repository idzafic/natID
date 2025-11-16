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
    gui::SubMenu subFirst;
    gui::SubMenu subModel;
    gui::SubMenu subThread;
protected:
    void populateFirstMenu()
    {
        auto& items = subFirst.getItems();
        items[0].initAsActionItem(tr("settings"), 10); //prevedeno u natGUI
        items[1].initAsSeparator();
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); //prevedeno u natGUI
    }
    
    void populateModelMenu()
    {
        auto& items = subModel.getItems();
        items[0].initAsActionItem(tr("Simple04"), 10); //prevedeno u natGUI
        items[1].initAsActionItem(tr("Simple10"), 20); //prevedeno u natGUI
        items[2].initAsSeparator();
        items[3].initAsActionItem(tr("BosnianMap"), 30); //prevedeno u natGUI
    }
    
    void populateThreadMenu()
    {
        auto& items = subThread.getItems();
        items[0].initAsActionItem(tr("start"), 10);
        items[0].setAsCheckable();
    }
public:
    MenuBar()
    : gui::MenuBar(3) //two menus
    , subFirst(cMenuApp, tr("App"), 3) //allocate items for the Application subMenu
    , subModel(cMenuModel, tr("Model"), 4) //allocate items for the Application subMenu
    , subThread(cMenuAnimation, tr("Animation"), 1) //allocate items for the Model subMenu
    {
        populateFirstMenu();
        populateModelMenu();
        populateThreadMenu();
        _menus[0] = &subFirst;
        _menus[1] = &subModel;
        _menus[2] = &subThread;
//        prepare();
    }
    
    ~MenuBar()
    {
    }
};
