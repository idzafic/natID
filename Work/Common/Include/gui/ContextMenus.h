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
#include <gui/ContextMenu.h>

namespace gui
{

class Frame;
class IMessageConsumer;
class ContextMenusHelper;

class NATGUI_API ContextMenus
{
    friend class ContextMenusHelper;
private:
    cnt::SafeFullVector<ContextMenu*> _menus;
private:
    ContextMenus();
    void prepare();
protected:
    
 public:
    ContextMenus(size_t nContextMenus);
    ~ContextMenus();
    MenuItem* getMenu(td::BYTE menuID);
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    bool open(td::BYTE menuID, gui::Frame* pFrame, const gui::Point& pointOnFrame, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 contextMenuGroup = 0);
    void setItem(size_t pos, ContextMenu* mi);
};

} //namespace gui
