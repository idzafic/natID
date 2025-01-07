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
#include "ActionItem.h"
#include "ContextData.h"
#include <tuple>

namespace rpt
{
class ContextData;
}

namespace gui
{
class Consumer;

class NATGUI_API ActionItemDescriptor
{
    friend class Consumer;
    
public:
    enum class Source : td::BYTE {MenuOrToolbar=0, Report, Other};
    gui::ActionItem* _pActionItem;
    const gui::ContextData* _pContextData;
    td::BYTE _menuID;
    td::BYTE _firstSubmenuID;
    td::BYTE _lastSubMenuID;
    td::BYTE _actionItemID;
    Source _source;
    
protected:
    ActionItemDescriptor();
public:
    ActionItemDescriptor(td::BYTE menuID, td::BYTE firstSubmenuID, td::BYTE lastSubMenuID, td::BYTE actionItemID, gui::ActionItem* pActionItem);
    ActionItemDescriptor(Source source, const gui::ContextData* pContextData, td::BYTE menuID, td::BYTE firstSubmenuID, td::BYTE lastSubMenuID, td::BYTE actionItemID, gui::ActionItem* pActionItem);
    std::tuple<td::BYTE, td::BYTE, td::BYTE, td::BYTE> getIDs() const;
    gui::ActionItem* getActionItem();
    Source getSource() const;
    const gui::ContextData* getContextData() const;
};

}
