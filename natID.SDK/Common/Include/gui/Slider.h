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
	class NATGUI_API Slider : public DataCtrl
	{
    private:
        std::function<void()> _onChangedValue;
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;

        Slider(const Slider&) = delete;
        Slider& operator =(const Slider&) = delete;
    public:
        Slider(DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);
        Slider(const td::String& toolTip, DataCtrl::Orientation orientation = DataCtrl::Orientation::Horizontal, bool bShowValues = false);
        ~Slider();
        gui::ObjType getObjType() const override { return ObjType::Slider;}
        bool setValue(const td::Variant& val, bool sendMessage=true) override;
        bool getValue(td::Variant& val, bool checkType = false) const override;
        void setValue(double val, bool sendMessage=true);
        double getValue() const;
        void setRange(double minVal, double maxVal, int nTicks=-1);

        const std::function<void()>& getChangedValueHandler() const;
        void onChangedValue(const std::function<void()>& fnToCall);
	};
}
