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
#include <gui/GridLayout.h>
#include <cnt/SafeFullVector.h>
//#include <cnt/PushBackVector.h>
#include <gui/Label.h>

namespace gui
{

class ToolBarView;
class ToolBarItem;

class NATGUI_API ToolBarLayout : public GridLayout
{
    friend class ToolBarView;
    
public:
    class Pair
    {
    public:
        Control* pControl = nullptr;
        Label label = "";
        bool dynamicControl = false;
    };
    
    typedef struct _BH
    {
        td::INT2 pos;
        td::WORD width;
        td::BYTE hidden;
    } Buttons;
protected:
    cnt::SafeFullVector<Pair> _pairs;
    cnt::SafeFullVector<Buttons> _buttonPositions;
    ToolBarView* _pToolBarView;
    td::BYTE _menuIncluded = 0;
protected:
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    void initialMeasure(CellInfo& cell) override;
    int getMinWidth();
    td::WORD getColWidth(td::BYTE colPos) const;
//    Grid& getGrid();
    ToolBarLayout();
public:
    ToolBarLayout(ToolBarView*);
    ~ToolBarLayout();
    ToolBarLayout(td::BYTE nCols);
//    void reserve(td::BYTE nCols);
//    void populatePairs();
//    void insert(td::BYTE iCol, Pair& pair);
    void insert(td::BYTE iCol, Control* pCtrl, const td::String& label, bool showLabel);
    void insertSpace(td::BYTE iCol, td::UINT2 horSpace = 10);
    bool showLabels(bool bShow);
    bool updateLabel(const ToolBarItem* pTBI, const td::String& strLbl);
    void updateToolTip(const ToolBarItem* pTBI, const td::String& strToolTip);
    void setMenuIncluded();
    bool isMenuIncluded() const;
};

} //namespace gui
