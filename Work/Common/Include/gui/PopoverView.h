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
#include "Canvas.h"

namespace gui
{

class IPopoverButton;
class PopoverHelper;
class ToolBarItem;

class NATGUI_API PopoverView : public Canvas
{
    friend class PopoverHelper;
    friend class ToolBarItem;
public:
    enum class Type : td::BYTE {SymbolOnly=0, SymbolAndText};
    enum class Location : td::BYTE {AboveButton=0, BelowButon, LeftToButton, RightToButton};
    enum class SymbolWidth : td::BYTE {UserDefined=0, AdjustToMaxWHRatio};
protected:
    IPopoverButton* _btnPopover;
    Handle _popoverFrame = nullptr;
    td::UINT2 _nItems;
    td::UINT2 _currentSelection;
    td::UINT2 _hoverSelection;
    td::UINT2 _cellWidth;
    td::UINT2 _cellHeight;
    td::UINT2 _symbolWidth;
    td::UINT2 _symbolHeight;
    td::BYTE _numberOfToolTipRows;
    td::BYTE _nRows;
    td::BYTE _nCols;
    Type _type = Type::SymbolOnly;
    Location _location = Location::AboveButton;
    td::BYTE _toolTipAreaHight = 0;
    static td::BYTE _margin;
    td::BYTE _spaceBetweenItems;
    td::BYTE _styleOnParent : 1;
    td::BYTE _sendClickMessageAlways: 1;
    td::BYTE _highlighCurrentSelection: 1;
    td::BYTE _drawOnPOButton : 1;
    td::BYTE _needsSizeCalculation : 1;
    td::BYTE _transferLabelToPOButton : 1;
    td::BYTE _onToolBar : 1;
    td::BYTE _keepSymboWHRatio : 1;
    td::BYTE _disabled : 1;
private:
    virtual void drawCurrentItemOnPOButton(const Size& sz) = 0; //to draw on parent button
    virtual void drawCurrentItemOnPOButton(const Rect& r); //to draw on parent button
    
protected:
    void createCanvas();
    void onDraw(const gui::Rect& rect) override;
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override final;
    void onCursorMoved(const gui::InputDevice& inputDevice) override final;
    void onCursorExited(const gui::InputDevice& inputDevice) override final;
    
    //virtual BaseView::PopoverType getPopoverType() const;
    void getToolSize(gui::Size& sz);
    void getPopoverSize(Size& sz);
    
    td::UINT2 getPopoverNumberOfItems() const;
    td::UINT2 getHoverSelection(const gui::Point& viewPoint) const;
//    td::UINT2 getPopoverCurrentSelection() const;    
    void highlightCurrentSelection(bool bHighlight);
    
    virtual void calcCellSize();
    virtual const td::String& getPopoverLabel(td::UINT2 itemPos) const = 0;
    virtual const td::String& getPopoverToolTip(td::UINT2 itemPos) const = 0;
    
    //virtual std::tuple<const td::String&, const td::String&> getPopoverLabelAndToolTip(td::UINT2 itemPos) const = 0;
    virtual void drawItem(td::UINT2 itemPos, const gui::Rect& r) = 0;
    virtual void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r);
    void setNumberOfToolTipRows(td::BYTE nToolTipRows);
    
    virtual bool isResizible() const;
    virtual td::UINT2 getRequiredButtonWidth() const;
    
    PopoverView(){}
    
public:
    PopoverView(Type type, td::UINT2 nItems, td::BYTE nCols, td::UINT2 cellWidth, td::UINT2 cellHeight, td::BYTE numberOfToolTipRows, td::BYTE spaceBetweenItems = 2);

    gui::ObjType getObjType() const override { return ObjType::PopoverView;}
    void styleOnParent(bool bStyleOnParent);
    void setPopoverButton(gui::IPopoverButton* pIBtn);
    void setCurrentSelection(td::UINT2 currentSelection);
    td::UINT2 getCurrentSelection() const;
    void setLocation(Location location);
    PopoverView::Location getLocation() const;
    void disable(bool bDisable);
    void transferLabelToButton(bool bTransferLabelToPOButton);
    void keepSymboWHRatio(bool bKeepWHRatio);
    bool isEnabled() const;
    bool isDisabled() const;
};

} //namespace gui
