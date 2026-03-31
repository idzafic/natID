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
        items[0].initAsActionItem("Line Graph", 10, "l");
        items[1].initAsActionItem("Data Table", 20, "d");
        items[2].initAsActionItem("Column Graph", 30, "b");
        items[3].initAsActionItem("Network Graph", 40, "n");
        items[4].initAsQuitAppActionItem(tr("Quit"), "q");
    }
public:
    MenuBar()
        : gui::MenuBar(1)
        , subApp(10, "App", 5)
    {
        populateSubAppMenu();
        _menus[0] = &subApp;
        //        prepare();
    }
    
    ~MenuBar()
    {
    }
};

