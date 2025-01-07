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
#include "StackedLayout.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	class NATGUI_API VerticalLayout : public StackedLayout
	{
    protected:
        void updateCell(CellInfo& cell, const CellInfo& ci, int iPos) const override;
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    public:
        VerticalLayout(size_t nElems);
        gui::ObjType getObjType() const override { return ObjType::VerticalLayout;}
        VerticalLayout& appendSpace(td::BYTE spaceSize);
        VerticalLayout& appendSpacer(td::BYTE weight=1);
	};
}
