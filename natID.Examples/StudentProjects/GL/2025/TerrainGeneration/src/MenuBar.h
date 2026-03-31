#pragma once
#include <gui/MenuBar.h>

class MenuBar : public gui::MenuBar
{
private:
    gui::SubMenu subApp;
    //gui::SubMenu subEdit;

protected:
    // Populates the "App" submenu with actions like "Properties" and "Quit"
    void populateSubAppMenu()
    {
        auto& items = subApp.getItems();
        items[0].initAsActionItem(tr("Properties"), 10, "p");
        items[2].initAsQuitAppActionItem(tr("Quit"), "q"); 
    }

    // Populates the "Frame" (edit) submenu with the "Export" action
    /*void populateFrameMenu()
    {
        auto& items = subEdit.getItems();
        items[0].initAsActionItem(tr("Export"), 10, "e");
    }*/

public:
    // Constructor: creates and registers submenus with items
    MenuBar()
        : gui::MenuBar(1)              
        , subApp(10, "App", 3)        
       // , subEdit(20, "Frame", 1)     
    {
        populateSubAppMenu();
        //populateFrameMenu();
        _menus[0] = &subApp;
        //_menus[1] = &subEdit;
    }
};
