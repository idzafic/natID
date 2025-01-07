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
#include <gui/NatObject.h>

namespace gui
{
class NatObjHelper;

class NATGUI_API ContextMenuNat : public NatObject
{
    friend class NatObjHelper;
private:
    td::BYTE _menuID;
    td::BYTE _nSeparators = 0;
    
 public:
    ContextMenuNat();
    ~ContextMenuNat();
    gui::ObjType getObjType() const override { return ObjType::ContextMenuNat; }
    td::BYTE getMenuID() const;
    td::BYTE getNoOfSeparators() const;
};

} //namespace gui
