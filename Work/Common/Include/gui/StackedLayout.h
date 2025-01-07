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
#include <cnt/PushBackVector.h>

namespace gui
{
	class NATGUI_API StackedLayout : public Layout
	{
    protected:
        cnt::PushBackVector<Cell> _cells;
        td::BYTE _spaceBetweenCells;
        td::BYTE _marginX;
        td::BYTE _marginY;
    protected:
        size_t getNoOfVisibleCells() const;
        void initialMeasure(CellInfo&) override;
        void measure(CellInfo& cell) override;
        void reMeasure(CellInfo& cell) override;
        virtual void updateCell(CellInfo& cell, const CellInfo& ci, int iPos) const;
        
        td::BYTE getXMargin() const;
        td::BYTE getYMargin() const;        
        //virtual Size getMinSize(const Cell& cell);
    public:
        StackedLayout(size_t nElems);
        void setMargins(td::BYTE marginX, td::BYTE marginY);
        void setSpaceBetweenCells(td::BYTE spaceBetweenCells);
        void append(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Center, td::VAlignment vAlign = td::VAlignment::Center);
        StackedLayout& operator << (Control& ctrl);
        void appendLayout(Layout& layout);
        StackedLayout& operator << (Layout& layout);
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;
        const Control* getControl(size_t cellPosition) const;
        size_t getNumberOfCells() const;
	};
}
