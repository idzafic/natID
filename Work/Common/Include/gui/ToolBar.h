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
#include "Consumer.h"
#include <cnt/PushBackVector.h>
#include <gui/ToolBarItem.h>
#include <mu/IAppProperties.h>

namespace gui
{
class ToolBarHelper;
class ToolBarView;

class NATGUI_API ToolBar : public Consumer
{
    friend class SymbolPopoverView;
    friend class ToolBarHelper;
public:
    enum class IconSize : td::BYTE {Small=0, Regular, SystemDefault};
private:
    cnt::PushBackVector<gui::ToolBarItem> _allowedItems;
    cnt::PushBackVector<td::WORD> _defaultItems;
protected:
    ToolBarView* _view = nullptr; //for systems without native toolbar support
    td::String _strID; //toolbar ID (for properties)
    
    IconSize _iconSize = IconSize::Small;
    mu::IAppProperties::ToolBarType _propTBType = mu::IAppProperties::ToolBarType::Main;
    td::BYTE _customizable = 0;
    td::BYTE _showLabels = 1;
    td::BYTE _leadingSpace = 0;
protected:
    ToolBar();
    void reserve(td::WORD nAllowedItems, td::WORD nDefaultItems = 0);
    
    td::WORD addItem(const td::String& lbl, Image* image, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    td::WORD addItem(const td::String& lbl, Symbol* symbol, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
//    void initItem(td::WORD itemPos, gui::PopoverView* pPopoverView, td::UINT2 ctrlID);
    td::WORD addItem(gui::PopoverView* pPopoverView, td::UINT2 ctrlID, const td::String& lbl = "", const td::String& toolTip = "");
    td::WORD addItem(const td::String& lbl, const td::String& toolTip, gui::DataCtrl* pCtrl, td::UINT2 ctrlID);
    
    td::WORD addItem(const td::String& lbl, const td::String& toolTip, gui::View* pView);

    td::WORD addSpaceItem();
    
    const ToolBarItem& getItemByPosition(td::WORD pos) const;
    ToolBarItem* getItem(td::WORD pos);
public:
    ToolBar(const char* strID, td::WORD nAllowedItems, td::WORD nDefaultItems = 0, bool customizable = false);
    ~ToolBar();
    const td::String& getID() const;
    bool isCustomizable() const;
    //void setAsMain() const;
    gui::ObjType getObjType() const override { return ObjType::ToolBar; }
    cnt::PushBackVector<gui::ToolBarItem>& getAllowedItems();
    cnt::PushBackVector<td::WORD>& getDefaultItems();
    void setIconSize(IconSize iconSize);
    ToolBar::IconSize getIconSize() const;
    int getIconSizeInPixels() const;
    
    ToolBarView* getView();
    const ToolBarView* getView() const;
    
    ToolBarItem* getItem(const char* strID, size_t nCh);
    ToolBarItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    ToolBarItem* getItem(td::UINT4 gid);
    gui::DataCtrl* getDataCtrl(td::UINT4 ctrlID);
    gui::PopoverView* getPopoverView(td::UINT2 popoverID);
    const gui::PopoverView* getPopoverView(td::UINT2 popoverID) const;
    void enableCtrl(td::UINT4 ctrlID, bool bEnable);
    void enablePopover(td::UINT2 popoverID, bool bEnable);
    
    td::WORD getNoOfAllowedItems() const;
    td::WORD getNoOfDefaultItems() const;
    td::WORD getIndexOfDefaultItem(td::WORD pos) const;
    bool isVisible() const;
    void show(bool bShow);
    void hide(bool bHide);
    void showLabels(bool bShowLabels);
    bool areLabelsVisible() const;
    
    void setPropertyType(mu::IAppProperties::ToolBarType propType);
    mu::IAppProperties::ToolBarType getPropertyType() const;
    
    //Windows only (with GTK)
    void setLeadingSpace(td::BYTE leadingSpace);
    td::BYTE getLeadingSpace() const;
};

} //namespace gui
