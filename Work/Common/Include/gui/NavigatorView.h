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
#include "ViewScroller.h"
#include "Canvas.h"
#include "Image.h"
#include <cnt/SafeFullVector.h>
//#include <gui/GraphicsShapeFactory.h>
#include <gui/DrawableString.h>
#include <gui/Symbol.h>

namespace gui
{

class Navigator;
class HorizontalNavigator;
class VerticalNavigator;

class NATGUI_API NavigatorView : protected Canvas
{
    friend class Navigator;
    friend class HorizontalNavigator;
    friend class VerticalNavigator;
    
    class Item
    {
    public:
        enum class Type:unsigned char {NA=0, Image, Symbol};
        union
        {
            Image* pImage;
            Symbol* pSymbol;
        };
        DrawableString text;
        float rotateRad = 0;
        Type type;
    };
    
    Navigator* _pNavigator;
    cnt::SafeFullVector<NavigatorView::Item> _items;
    td::UINT2 _imageWidth;
    td::UINT2 _imageHeight;
    td::UINT2 _width;
    td::UINT2 _height;
    td::UINT2 _startPos = 0;
    td::ColorID symbolColor = td::ColorID::Transparent;
    td::UINT2 symbolLineWidth = 0;
    td::UINT2 _currentSelection=0;
    td::BYTE _vertical = 1;
    td::BYTE _alignment = 0;
    
    NavigatorView();
    void getTxtRect(td::UINT2 itemPos, Rect& r) const;
    void getImgRect(td::UINT2 itemPos, Rect& r) const;
    void getBoundingRect(td::UINT2 itemPos, Rect& r) const;
    void getContentSize(gui::Size& sz) const;
    bool getModelSize(gui::Size& modelSize) const override;
    void sendSelChangeMessage();
    gui::CoordType getLengthForNItems(size_t nItems) const;
    gui::Orientation getOrientation() const;
    td::BYTE getAlignment() const;
    void setAlignment(td::BYTE alignment);
protected:
    void onResize(const gui::Size& newSize) override;
    void onDraw(const gui::Rect& rDraw) override;
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
public:
    NavigatorView(Navigator* pNavigator, td::UINT2 nItems, gui::Orientation orientation, td::UINT2 imageHeight, float widthMultiplier);
    ~NavigatorView();
};

} //namespace gui
