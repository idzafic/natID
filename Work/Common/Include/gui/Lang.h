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
#include "natGUI.h"
#include <td/String.h>
#include "Types.h"

namespace gui
{

class NATGUI_API Lang
{
private:
    td::String _extension;
    td::String _description;
public:
    Lang();
    void set(const td::StringExt& ext, const td::String& desc);
    const td::String& getExtension() const;
    const td::String& getDescription() const;
};

} //namespace gui
