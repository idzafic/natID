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
#include "BaseComboBox.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	class NATGUI_API DBComboBox : public BaseComboBox
	{
        cnt::PushBackVector<td::Variant> _keys;
        std::function<void()> _onChangedSelection;
    protected:
        DBComboBox();
        DBComboBox(const td::String& toolTip);
        int getKeyPos(const td::Variant& key) const;
    public:
        DBComboBox(td::DataType dt);     
        gui::ObjType getObjType() const override{ return ObjType::DBComboBox;}
        bool addItem(const td::String& item, const td::Variant& key);
        bool addItem(const char* item, const td::Variant& key);
        void removeItem(int pos);
        bool setValue(const td::Variant& key, bool sendMessage=true) override;
        bool getValue(td::Variant& key, bool checkType = false) const override;
        void clean();
        void toZero(); //same as clean

        const std::function<void()>& getChangedSelectionHandler() const;
        void onChangedSelection(const std::function<void()>& fnToCall);
	};
}
