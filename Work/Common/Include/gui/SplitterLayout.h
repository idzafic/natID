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
#include <gui/Layout.h>
#include <gui/_aux/SplitterCell.h>
#include <gui/ISwitch.h>

namespace gui
{    
	class NATGUI_API SplitterLayout : public Layout, public ISwitch
	{
        friend class SplitterCell;
    public:
        enum class Orientation : td::BYTE {Horizontal=0, Vertical};
        enum class AuxiliaryCell : td::BYTE {First=0, Second};
        enum class MinimizedCellPosition : td::BYTE {TopOrLeft, Center, BottomOrDown};
    protected:
        SplitterCell _splitterCell;
        Geometry _lastGeometry;
        Cell _cells[3];
        CoordType _sizeOfAuxiliaryCell;
        CoordType _lastVisibleAuxiliarySize;
        Orientation _orientation;
        AuxiliaryCell _posOfAuxiliaryCell;
        td::BYTE _spaceOfSplitterCell;
    private:
        void unhideAuxliaryView(const Geometry& viewGeometry);
    protected:
        gui::CoordType getMaxSizeForAuxiliaryCell() const;
        void getMinSizeForAuxiliaryCell(Size& sz) const;
        
        size_t getNoOfVisibleCells() const;
        void initialMeasure(CellInfo&) override;
        void measure(CellInfo& cell) override;
        void reMeasure(CellInfo& cell) override;
        
        bool freeze() override;
        void unFreeze() override;
        bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
        
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
        
        td::BYTE getXMargin() const;
        td::BYTE getYMargin() const;        

        Orientation getOrientation() const;
       
        void updateGeometry(const Geometry& viewGeometry);
        void onSplitterCellMove();
        //ISwitch
        ISwitch::State toggle() override;
        void setState(ISwitch::State state) override;
        void setUpdater(ISwitchState* pUpdater) override;
        SplitterLayout() = delete;
    public:
        SplitterLayout(SplitterLayout::Orientation orientation, SplitterLayout::AuxiliaryCell auxCellPos = SplitterLayout::AuxiliaryCell::Second);
        gui::ObjType getObjType() const override { return ObjType::SplitterLayout;}
        
        void setMargins(td::BYTE marginX, td::BYTE marginY);
        void setSpaceBetweenCells(td::BYTE spaceBetweenCells);
        void setContent(Control& ctrl1, Control& ctrl2);
        void setView(size_t pos, Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top);
        void setLayout(size_t pos, Layout& layout);
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;
        void setToolTipForMinimizedAux(const td::String& str);
	};
}
