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
#include "PopoverView.h"
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <cnt/PushBackVector.h>
#include <gui/DrawableString.h>

namespace gui
{

class SymbolPopoverButton;

class NATGUI_API SymbolPopoverView : public PopoverView
{
    friend class SymbolPopoverButton;

protected:
    class Item
    {
    public:
        enum class Type : td::BYTE {Symbol=0, Image};
        union
        {
            gui::Symbol* symbol = nullptr;
            gui::Image* image;
        };
        
        union
        {
            gui::Symbol* attachedSymbol = nullptr;
            float hMargin;
        };
        
        td::String label;
        DrawableString drawableLabel;
        
        td::String toolTip;
        td::Variant userData;
        //gui::NatString natLabel = nullptr;
        //gui::NatString natToolTip = nullptr;
        union
        {
            float rotationClockwiseDeg = 0;
            float vMargin;
        };
        
        td::BYTE useSymbolStyling = 0;
        Type type = Type::Symbol;

        Item() {}
        
        Item(const Item& item)
        : symbol(item.symbol)
        , attachedSymbol(item.attachedSymbol)
        , label(item.label)
        , toolTip(item.toolTip)
        , userData(item.userData)
        , rotationClockwiseDeg(item.rotationClockwiseDeg)
        , useSymbolStyling(item.useSymbolStyling)
        , type(item.type)
        {
        }
        
        const Item& operator = (const Item& item)
        {
            symbol = item.symbol;
            attachedSymbol = item.attachedSymbol;
            label = item.label;
            toolTip = item.toolTip;
            userData = item.userData;
            rotationClockwiseDeg = item.rotationClockwiseDeg;
            useSymbolStyling = item.useSymbolStyling;
            type = item.type;
            return *this;
        }
    };
    
    cnt::PushBackVector<SymbolPopoverView::Item> _items;
    union
    {
        gui::Symbol* _pPOSymbol = nullptr;
        gui::Image* _pPOImage;
    };
    float _maxHWRatio = 1.0f;
    Item::Type _typePOButton = Item::Type::Image;
private:
    void drawCurrentItemOnPOButton(const Size& sz) override;
protected:
    //void prepareImage();
    void drawSymbol(td::UINT2 simbPos, const gui::Rect& rec, td::BYTE doStyling, bool forParrentButton);
    
    SymbolPopoverView::Item& getItem(size_t iPos);
    
    void calcCellSize() override;
    
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;
    
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;
    void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r) override;
private:
    SymbolPopoverView(){}
public:
    
    SymbolPopoverView(Type type, size_t nItems, td::BYTE nCols, td::BYTE cellWidth=48, td::BYTE cellHeight = 48, td::BYTE numberOfToolTipTextLines = 1);
    SymbolPopoverView(Type type, size_t nItems, PopoverView::SymbolWidth symbolWidth= PopoverView::SymbolWidth::UserDefined); //more suitable with SystemAndText type, nRows = nItems, nCols = 1, symbolHeight = textHeight
    
    ~SymbolPopoverView();
    
    //allocate capacity
    void reserve(size_t nItems);
    //remove existing items while keeping allocated capacity
    void reset();
    
    void setButtonImage(Image* pFixedPOImage);
    void setButtonSymbol(Symbol* pFixedPOSymbol);
    
    void addItem(SymbolPopoverView::Item& item);
    void addItem(gui::Symbol* symbol, gui::Symbol* attachedSymbol, const td::String& label, const td::String& toolTip, bool styleWithColors, td::Variant userData = (td::UINT4)0, float rotation = 0, char deltaX = 0, char deltaY = 0);
    void addItem(gui::Image* image, const td::String& label, const td::String& toolTip, float hMargin, float vMargin, td::Variant userData = (td::UINT4)0);
};

} //namespace gui
