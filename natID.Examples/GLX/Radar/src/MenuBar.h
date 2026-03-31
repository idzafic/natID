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
        items[0].initAsActionItem(tr("Reverse Radar"), 20, "r");
        items[1].initAsQuitAppActionItem(tr("Quit"), "q"); //id 0 is used to terminate app
    }
public:
    MenuBar()
    : gui::MenuBar(1) //one menu
    , subApp(20, "testApp", 2) //allocate items for the Application subMenu (increased for new button)
    {
        populateSubAppMenu();
        _menus[0] = &subApp;
    }
};
