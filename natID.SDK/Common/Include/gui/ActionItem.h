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
#include "NatObject.h"

namespace gui
{
	class NATGUI_API ActionItem : public NatObject
	{
    protected:
        ActionItem(const ActionItem&) = delete;
        ActionItem& operator =(const ActionItem&) = delete;
        ActionItem();
        //void setID(td::UINT4 id);
        //td::UINT4 getID() const;
    public:
        ActionItem(ActionItem&&);
        ActionItem& operator =(ActionItem&&);

        void enable(bool bEnable = true);
        void disable();
        bool isEnabled() const;
        void setChecked(bool bChecked);
        bool isChecked() const;
	};
}
