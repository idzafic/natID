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
#include "Layout.h"
#include <mtx/SimpleDense.h>
#include <gui/_aux/GridCell.h>
//#include <tuple>
//#include <cnt/Array.h>
//#include "GridHeadInfo.h"

namespace gui
{

class ToolBarView;
typedef mtx::SimpleDense<GridCell, td::BYTE> Grid;

class NATGUI_API GridLayout : public Layout
{
    friend class ToolBarView;
protected:
    Grid _grid;
    td::BYTE _spaceBetweenRows;
    td::BYTE _spaceBetweenCols;
    td::BYTE _marginX;
    td::BYTE _marginY;
private:
    void updateSpan(GridCell& cell, td::BYTE iRow, td::BYTE iCol, td::BYTE rowSpan, td::BYTE colSpan,  const char* redistribution);
protected:
    void initialize(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::BYTE rowSpan, td::BYTE colSpan,  td::HAlignment hAlign, td::VAlignment vAlign);
    void initialMeasure(CellInfo&) override;
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    void setGeometry(const Geometry& geometry, const Cell& cell) override;
    bool freeze() override;
    void unFreeze() override;
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    
    GridLayout();
public:
    GridLayout(td::BYTE nRows, td::BYTE nCols);
    
    virtual gui::ObjType getObjType() const override { return ObjType::GridLayout;}
    
    void setMargins(td::BYTE marginX, td::BYTE marginY);
    
    td::BYTE getXMargin() const;
    td::BYTE getYMargin() const;
    
    void insert(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);
    
//    void putSpanCtrlWithRedistribution(td::BYTE iRow, td::BYTE iCol, Control* pCtrl, td::BYTE rowSpan, td::BYTE colSpan,  const char* redistribution="*", HAlignment hAlign = HAlignment::Left, VAlignment vAlign = VAlignment::Center);
    
    void insert(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);
    
    void insertSpace(td::BYTE iRow, td::BYTE iCol, td::UINT2 horizontalSpace, td::UINT2 verticalSpace);
    
    void insertSpacer(td::BYTE iRow, td::BYTE iCol, td::UINT2 horizontalSpace, td::BYTE horMultiplier, td::UINT2 verticalSpace = 0, td::BYTE vertialMultiplier = 0);
    
    void populateView(View* pView, const Cell& cell, const Size& szMargins) override;
    
    void serialize(std::ofstream& f) const;
    
    void setSpaceBetweenCells(td::BYTE spaceBetweenRows, td::BYTE spaceBetweenColumns);
    td::BYTE getSpaceBetweenRows() const;
    td::BYTE getSpaceBetweenColumns() const;
};

} //namespace gui
