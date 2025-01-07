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
	class NATGUI_API TextCtrl : public DataCtrl
	{
    protected:
        TextCtrl(td::DataType dataType = td::string8);
    public:
        void setAsReadOnly(bool readOnly=true);
        void setText(const char* val);
        void setText(const td::String& val);
        td::String getText() const;
	};
}
