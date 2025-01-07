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
#include <cnt/SafeFullVector.h>
#include <cnt/PushBackVector.h>
#include <td/String.h>
#include <vector>

namespace gui
{
	class NATGUI_API ComboBox : public BaseComboBox
	{
    private:
        std::function<void()> _onChangedSelection;
    public:
        ComboBox();
        ComboBox(const td::String& toolTip);
       
        gui::ObjType getObjType() const override { return ObjType::ComboBox;}
        void addItem(const td::String& item);
        void addItem(const char* item);
        void addItems(const td::String* pItems, size_t nItems);
        void addItems(const cnt::SafeFullVector<td::String>& strings);
        void addItems(const cnt::PushBackVector<td::String>& strings);
        void addItems(const std::vector<td::String>& strings);
        void removeItem(int pos);

        bool setValue(const td::Variant& val, bool sendMessage=true) override;
        bool getValue(td::Variant& val, bool checkType = false) const override;
        void clean();
        void toZero(); //same as clean

        const std::function<void()>& getChangedSelectionHandler() const;
        void onChangedSelection(const std::function<void()>& fnToCall);
	};
}
