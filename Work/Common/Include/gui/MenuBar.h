// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <cnt/SafeFullVector.h>
#include "Consumer.h"
#include <gui/MenuItem.h>

namespace gui
{
class Window;
class MenuBarHelper;

class NATGUI_API MenuBar : public Consumer
{
    friend class MenuBarHelper;
    td::UINT4 _GID = 0;
protected:
    td::BYTE _context = 0;
//    td::BYTE _takeAppMenuNameFromExecutable = 1;
private:
    MenuBar();
    void prepare(bool forContextMenu);
protected:
    cnt::SafeFullVector<MenuItem*> _menus;
    //void addMenu(MenuItem* item);
    bool loadFromRes(const char* xmlResConfigName);
    void setMenu(td::WORD menuPos, MenuItem* menuItem);
 public:
    enum class Location : td::BYTE {SystemSpecific=0, EmbeddedInWindow};
    
    void setAsMain(gui::Window* pMainWnd, Location location = Location::SystemSpecific);
    MenuBar(size_t nMenus);
    ~MenuBar();
    td::UINT4 getGID() const;
    bool isContext() const;
    size_t getNoOfMenus() const;
    cnt::SafeFullVector<MenuItem*>& getItems();
    MenuItem* getMenu(size_t menuPos);
    MenuItem* getMenuByID(td::BYTE menuID);
    gui::ObjType getObjType() const override { return ObjType::MenuBar; }
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
//    bool isAppMenuNameFromExecutable() const;
//    const td::String* getLeadingName() const;
};

} //namespace gui
