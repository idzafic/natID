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
#include <gui/BaseView.h>
#include <gui/Types.h>
#include <td/String.h>
#include <gui/Canvas.h>
#include <cnt/PushBackVector.h>
#include <gui/Shape.h>
#include <gui/Font.h>

namespace gui
{

class Image;
class Symbol;
class TabView;
class TabHeaderScroller;

class NATGUI_API TabHeader : public Canvas
{
    friend class TabHeaderScroller;
public:
    enum class Type : unsigned char {Fix=0, FitToText, Dynamic};
protected:
    class Item
    {
    public:
        enum class Type: unsigned char {Image=0, Symbol, NA};
        union
        {
            Image* pImage = nullptr;
            Symbol* pSymbol;
        };
        gui::Frame* pCtxFrameHandler;
        gui::CoordType x;
        gui::CoordType width;
        td::String lbl;
        NatString natStr;
        //td::UINT2 posInSwitcher;
        td::UINT2 contextMenuGroup; //system context menu group
        td::ColorID selLineColor;
        Type type;
        td::BYTE contextMenuID;
        td::BYTE nonRemovable = 0;
        td::BYTE modified = 0;
//        td::BYTE contentType = 0;
    };
private:
    TabView* _pTabView;
    cnt::PushBackVector<Item, 32> _items;
    Shape _lineCurrSelection;
    Shape _closeTabSymbol;
    Size _size;
    double _closeTabSymbolScale;
    CoordType _lineLenVisible;
    int _currentSelection;
    int _currentHover;
    td::UINT2 _minWidth;
    td::UINT2 _maxWidth;
    Font::ID _selFontID;
    Font::ID _nonSelFontID;
    Type _type;
    td::BYTE _selLineWidth;
protected:
    void sendSelChangeMessage();
    void calcItemRect(Item& item) const;
    CoordType getImageWidth() const;
    void updateSelectionLine();
    void drawCloseTabSymbol() const;
    void updateGeometryOfItems(int fromItem);
    void calcHoveringItem(const gui::Point& viewPoint);
    void onResize(const gui::Size& newSize) override;
    
    void removeTab(int pos); //from inside
    void removeTabExt(int pos, int newSelection, bool sendMessage, bool bRedraw); //from outside
    
    void showTab(int viewPos, bool bSetFocus = true);
    const Size& getSize() const;
    bool getItemRect(size_t posItem, Rect& r) const;
    
    void setTitle(int pos, const td::String& strTitle);
    td::String getTitle(int pos) const;
    
    void setModified(int pos, bool bModified);
    bool isModified(int Pos) const;
    
//    void setContentTypeID(int pos, td::BYTE cntType);
//    td::BYTE getContentTypeID(int pos) const;
    
    void populate(Item& item, const td::String& lbl, td::ColorID selLineColor);
    
    void append(const Image* pImg, const td::String& lbl, td::ColorID selLineColor);
    void append(const Symbol* pSymb, const td::String& lbl, td::ColorID selLineColor);
    
    void setLineLen(CoordType newLineLen);
    
    //virtual void onResize(const gui::Rect& newBounds);
    
    void onDraw(const gui::Rect& rect) override;
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    //virtual void onMouseDrag(const gui::Point& viewPoint, td::UINT4 keyModifiers);
    
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    
    bool getModelSize(gui::Size& modelSize) const override;
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    void forwardContextMenuEventsTo(int pos, Frame* pFrame);
    void setNonRemovable(int viewPos, bool nonRemovable);
    void systemColorModeChanged(bool bDarkMode) override;
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    TabHeader(TabView* pTabView, Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, Font::ID selFontID, Font::ID fontID);
    ~TabHeader();
};

} //namespace gui
