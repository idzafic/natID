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
	class NATGUI_API CheckBox : public DataCtrl
	{
    private:
        std::function<void()> _onClick;
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
    public:
        CheckBox(const char* lbl);
        CheckBox(const td::String& lbl);
        ~CheckBox();
        gui::ObjType getObjType() const override { return ObjType::CheckBox;}
        void setTitle(const td::String& title)  override;
        bool setValue(const td::Variant& val, bool sendMessage=true) override;
        bool getValue(td::Variant& val, bool checkType = false) const override;
        bool getValue() const;
        void setValue(bool bVal);
        bool isChecked() const;
        void setChecked(bool bChecked, bool sendMessage = true);
        
        const std::function<void()>& getClickHandler() const;
        void onClick(const std::function<void()>& fnToCall);
	};
}
