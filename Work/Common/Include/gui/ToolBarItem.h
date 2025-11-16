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
#include "gui/ActionItem.h"
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <gui/IPopoverButton.h>

namespace cnt
{
    template <typename T, bool EXTERN_ALLOCATOR>
    class SafeFullVector;
}

namespace gui
{

class Image;
class Symbol;
class DataCtrl;
//class BaseView;
class ToolBar;
class PopoverCanvas;
class PopoverButton;
class PopupView;
class ToolBarItemHelper;
class View;

class NATGUI_API ToolBarItem : public ActionItem, public IPopoverButton
{
    friend class ToolBar;
    friend class PopoverView;
    friend class ToolBarItemHelper;
    
    template <typename T, bool EXTERN_ALLOCATOR>
    friend class cnt::SafeFullVector;
public:
    enum class Type : unsigned char {Image, Symbol, Control, PopoverCanvas, View, PopupView, Space};
protected:
    ToolBar* _pToolBar = nullptr;
    td::String _strID;
    td::String _lbl;
    td::String _toolTip;
    //const NatObject* _pObject = nullptr; //Image, view, EditBox, etc
    //Popover _popover;
    union
    {
        Image* _image = nullptr;
        Symbol* _symbol;
        DataCtrl* _control;
        PopoverCanvas* _popoverCanvas;
        View* _view;
        PopupView* _popupView;
    };
    
    td::UINT4 _gid = 0;
    Type _type = Type::Space;
protected:
    //~ToolBarItem();
    //for popover
    void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) override;
    void sendPopoverMessage() override;
    //virtual void drawPopoverSymbolText(gui::CoordType x, gui::CoordType y, td::UINT2 itemPos) const;
    //const gui::Handle getPOBHandle() const override;
    void closePopover() override;
    
    void enablePopover(bool bEnable) override;

    void setHandle(gui::Handle handle);
    
    gui::Handle getHandle();
    const gui::Handle getHandle() const;
    
    //simple action item (with image or symbol)
    void init(const char* itemID, const td::String& lbl, Image* image, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    void init(const char* itemID, const td::String& lbl, Symbol* symbol, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    //init using control (DataCtrl, like ComboBox)
    void init(const char* itemID, const td::String& lbl, const td::String& toolTip, gui::DataCtrl* pCtrl, td::UINT2 ctrlID);
    
    void init(const char* itemID, const td::String& lbl, const td::String& toolTip, gui::View* pView);
    
    //init using popover view
    void init(const char* itemID, gui::PopoverCanvas* pPopoverCanvas, td::UINT4 ctrlID, const td::String& lbl, const td::String& toolTip);
    
    //init using popup view
    void init(const char* itemID, gui::PopupView* pPopupView, td::UINT4 ctrlID, const td::String& lbl, const td::String& toolTip);
    
public:
    ToolBarItem();
    gui::ObjType getObjType() const override { return ObjType::ToolBarItem;}
    td::UINT4 getActionID() const;
    
    ToolBar* getToolBar();
    const ToolBar* getToolBar() const;
    
    const td::String& getLbl() const;
    const td::String& getStrID() const;
    const td::String& getToolTip() const;
    Image* getImage();
    Symbol* getSymbol();
    void setImage(const Image* pImg);
    void setSymbol(const Symbol* pSymbol);
    
    void setLabel(const td::String& pLbl);
    void setTooltip(const td::String& pToolTip);
    
    //Popover& getPopover();
    DataCtrl* getControl();
    
    PopoverCanvas* getPopoverCanvas();
    const PopoverCanvas* getPopoverCanvas() const;
    View* getView();
    const View* getView() const;
    PopupView* getPopupView();
    const PopupView* getPopupView() const;
    
    Type getType() const;
    
    void initAsSpace();
    
    void systemColorModeChanged(bool bDarkMode);
    
    bool hasStrID(const char *strID, size_t nCh) const;
};

} //namespace gui
