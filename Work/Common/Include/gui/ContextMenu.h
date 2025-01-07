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
#include <gui/MenuBar.h>
#include <gui/ContextData.h>

namespace gui
{

class Frame;
class IMessageConsumer;
class ContextMenusHelper;

class NATGUI_API ContextMenu : public MenuBar
{
protected:
    ContextData _contextData;
    td::UINT2 _groupID = 0;
public:
    enum Location : td::BYTE {Top, Bottom, Right, Left};
protected:
    void setItem(MenuItem* mi);
public:
    ContextMenu();
    void setGroupID(td::UINT2 groupID);
    td::UINT2 getGroupID() const;
    void setContextData(const ContextData* pContextData);
    const ContextData* getContextData() const;
};

} //namespace gui

