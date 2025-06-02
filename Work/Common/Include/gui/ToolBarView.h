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
#include <gui/ToolBarItem.h>
#include <gui/ToolBarLayout.h>
#include <gui/View.h>
#include <gui/ToolBarLayout.h>

namespace gui
{

class ToolBar;
class MenuButton;
class ToolBarViewHelper;

class NATGUI_API ToolBarView : public View
{
    friend class ToolBarViewHelper;
protected:
    ToolBarLayout _layout;
    ToolBar* _pToolBar;
    MenuButton* _pMenuButton = nullptr;
    int _fullWidth = 0;
protected:
    void adjustLayout(const Size& newSize) override; //called on every resize
    void initialMeasure();
    void composeContent();
public:
    ToolBarView(ToolBar* pToolBar, td::BYTE nCols, int menuID = -1, const td::String& menuButtonTT = "");
    ~ToolBarView();
    gui::ObjType getObjType() const override { return ObjType::ToolBarView;}
//    void reserve(td::BYTE nCols);
//    void setPair(td::BYTE iCol, ToolBarLayout::Pair& tbPair);
    Control* insertItem(td::BYTE iCol, const gui::ToolBarItem& item, bool showLabels);
    
    bool isMeasured() const;
    
    int getFullWidth() const;
    void setSpaceBetweenCells(td::BYTE spaceBetweeRows, td::BYTE spaceBetweenCols);
    void showLabels(bool bShow);
    void updateLabel(const gui::ToolBarItem* pTBI, const td::String& strTxt);
    void updateToolTip(const gui::ToolBarItem* pTBI, const td::String& strTxt);
};

} //namespace gui
