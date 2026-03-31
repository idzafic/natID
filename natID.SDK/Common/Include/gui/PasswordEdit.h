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
#include "LineEdit.h"

namespace gui
{
	class NATGUI_API PasswordEdit : public LineEdit
	{
        PasswordEdit(const PasswordEdit&) = delete;
        PasswordEdit& operator =(const PasswordEdit&) = delete;
    public:
        PasswordEdit(Messages sendMsg = Messages::DoNotSend);
        PasswordEdit(const td::String& toolTip, Messages sendMsg = Messages::DoNotSend);
        gui::ObjType getObjType() const override { return ObjType::PasswordEdit;}
//        void toggleVisibility();
	};
}
