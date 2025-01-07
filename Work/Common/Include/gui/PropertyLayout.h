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
//#include <cnt/PBVector.h>
#include <cnt/PushBackVector.h>
#include <gui/Control.h>
#include <gui/Label.h>
#include <gui/Properties.h>
#include <gui/_aux/PropertyGroupCell.h>

namespace gui
{
	class NATGUI_API PropertyLayout : public Layout
	{
        enum class Type : td::BYTE {Group, Entry};
        struct VisualPropery
        {
            Cell labelCell;
            Cell ctrlCell;
            Type type;
            //td::BYTE collapsed;
        };
        
    protected:
        //cnt::PBVector<VisualPropery> _visualProperties;
        cnt::PushBackVector<VisualPropery> _visualProperties;
        td::UINT2 _maxGroupWidth;
        td::UINT2 _maxLabelWidth;
        td::UINT2 _maxCtrlWidth;
        
        td::UINT2 _currMinWidth;
        td::UINT2 _currMinHeight;
        
        td::BYTE _spaceBetweenRows;
        td::BYTE _spaceBeweenCols;
        td::HAlignment _labelHAlignment;
        //td::BYTE _marginX;
        //td::BYTE _marginY;
//        td::BYTE _nResVert;
//        td::BYTE _nResHor;
    protected:
        void initialMeasure(CellInfo&) override;
        void measure(CellInfo& cell) override;
        void reMeasure(CellInfo& cell) override;
        
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
        void deleteVisualControls();
    public:
        //PropertyLayout(const Properties* properties);
        PropertyLayout(size_t nProps, td::HAlignment labelAlignment = td::HAlignment::Left);
        PropertyLayout(td::HAlignment labelAlignment = td::HAlignment::Left);
        ~PropertyLayout();        
        
        virtual gui::ObjType getObjType() const override { return ObjType::PropertyLayout;}
        void reserve(size_t nProps);
        void addGroup(gui::PropertyGroupCell* pPropertyGroupCell);
        void addProperty(gui::Label* pLabel, gui::Control* pControl);
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;
        void setLabelAlignment(td::HAlignment hAlign);
        td::UINT2 getCurrMinHeight() const;
        int getControlPosition(const Control* pControl) const;
        Control* getControlAtPosition(int pos);
	};
}
