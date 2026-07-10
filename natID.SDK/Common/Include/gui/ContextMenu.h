// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ContextMenu.h
    @brief Declares the ContextMenu class, a popup menu associated with a GUI context. */
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

/// @brief A popup context menu that can be associated with a specific GUI context and group.
///
/// Inherits from MenuBar to share menu item management, and adds context data
/// and group identification so that multiple context menus can be distinguished
/// when shown from the same frame.
class NATGUI_API ContextMenu : public MenuBar
{
protected:
    ContextData _contextData;  ///< Context information describing where and why this menu was invoked.
    td::UINT2 _groupID = 0;    ///< Group identifier used to distinguish sets of context menus.
public:
    /// @brief Indicates the screen edge or side from which the menu should appear.
    enum Location : td::BYTE
    {
        Top,    ///< Menu appears above the trigger point.
        Bottom, ///< Menu appears below the trigger point.
        Right,  ///< Menu appears to the right of the trigger point.
        Left    ///< Menu appears to the left of the trigger point.
    };
protected:
    /// @brief Sets a specific menu item within this context menu.
    /// @param mi  Pointer to the menu item to set.
    void setItem(MenuItem* mi);

    ContextMenu(const ContextMenu&) = delete;
    ContextMenu& operator =(const ContextMenu&) = delete;
public:
    /// @brief Default constructor; creates an empty context menu.
    ContextMenu();

    /// @brief Sets the group identifier for this context menu.
    /// @param groupID  Group identifier value.
    void setGroupID(td::UINT2 groupID);

    /// @brief Returns the group identifier of this context menu.
    /// @return The current group identifier.
    td::UINT2 getGroupID() const;

    /// @brief Stores a copy of the supplied context data in this menu.
    /// @param pContextData  Pointer to the context data to associate; may be nullptr.
    void setContextData(const ContextData* pContextData);

    /// @brief Returns a pointer to the context data currently associated with this menu.
    /// @return Pointer to the context data, or nullptr if none is set.
    const ContextData* getContextData() const;
};

} //namespace gui
