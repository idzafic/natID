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
#include <gui/ActionItem.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <gui/Image.h>

namespace cnt
{
    template <typename T, bool EXTERN_ALLOCATOR>
    class SafeFullVector;
}

namespace gui
{

class MenuBar;
class Consumer;

class MenuItemHelper;

class NATGUI_API MenuItem : public ActionItem
{
    template <typename T, bool EXTERN_ALLOCATOR>
    friend class cnt::SafeFullVector;
    
    friend class MenuItemHelper;
//    using CallBack = struct CB
//    {
//        Consumer* pConsumer;
//        std::function<void()> callBack;
//    };
public:
    enum class Type : unsigned char {Separator, SubMenu, Action, CheckAction};
protected:
    cnt::SafeFullVector<MenuItem> _items;
//    cnt::SafeFullVector<CallBack> _callBacks; //need to check which one is active
    td::String _lbl;
    td::String _shortCut;  //on init
    gui::MenuBar* _pMenuBar = nullptr; //after menu creation (activation)
    td::UINT4 _gid = 0;
    Type _type = Type::Separator;
    td::BYTE _id = 0;
    td::BYTE _posMenuFileNames = 0;
    td::BYTE _checked : 1;
    td::BYTE _initialized : 1;
private:
    bool isInitialized() const;
    void setMenuBar(gui::MenuBar* pMenuBar);
    bool getCheckedState() const;
    void setCheckedState(bool checked);
    void setPosMenuFileNames(td::BYTE pos);
protected:
    MenuItem();
    ~MenuItem();
    void setHandle(gui::Handle handle);
    void setActionID(td::UINT4 gid);
    const td::String& getShortCut() const;
    
public:
    void initAsSubMenu(td::BYTE subMenuID, const td::String& lbl, size_t nSubItems);
    int initAsSubMenuFromResMenuFile(MenuBar* pParent, td::BYTE subMenuID, const td::String& lbl, const char* menuResFileName);
    void initAsActionItem(const td::String& lbl, unsigned char itemID, const char* shortCut = nullptr);
    void initAsSeparator();
    void initAsQuitAppActionItem(const td::String& lbl, const char* shortCut = nullptr);
    void setAsCheckable(bool bInitalyChecked = false); //must be initialized first with initAsActionItem
    //void addMenuItem(const MenuItem& subMenu);
    MenuItem::Type getType() const;
    td::BYTE getID() const;
    
    td::UINT4 getActionID() const;
    
    const td::String& getLbl() const;
    
    gui::MenuBar* getMenuBar();
    td::BYTE getPosMenuFileNames() const;
    
    cnt::SafeFullVector<MenuItem>& getItems();
    gui::ObjType getObjType() const override { return ObjType::MenuItem; }
    
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
};

class NATGUI_API SubMenu : public MenuItem
{
public:
    SubMenu(td::BYTE subMenuID, const td::String& lbl, size_t nSubItems);
};

} //namespace gui
