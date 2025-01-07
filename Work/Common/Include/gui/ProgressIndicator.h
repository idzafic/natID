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
#include "DataCtrl.h"

namespace gui
{
	class NATGUI_API ProgressIndicator : public DataCtrl
	{
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
    public:
        ProgressIndicator();
        ProgressIndicator(const td::String& toolTip);
        ~ProgressIndicator();
        gui::ObjType getObjType() const override { return ObjType::ProgressIndicator;}
        bool setValue(const td::Variant& val, bool sendMessage=true) override;
        bool getValue(td::Variant& val, bool checkType = false) const override;
        void setValue(double val);
        double getValue() const;
	};
}
