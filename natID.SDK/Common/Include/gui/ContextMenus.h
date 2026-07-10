// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ContextMenus.h
    @brief Declares the ContextMenus container that manages a collection of context menus for a frame. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ContextMenu.h>

namespace gui
{

class Frame;
class IMessageConsumer;
class ContextMenusHelper;

/// @brief Manages an ordered collection of ContextMenu objects and provides methods to open them.
///
/// Typically owned by a Frame or View to hold all context menus that can be shown
/// from that frame. Menus are identified by a zero-based byte index.
class NATGUI_API ContextMenus
{
    friend class ContextMenusHelper;
private:
    cnt::SafeFullVector<ContextMenu*> _menus; ///< Storage for the registered context menu pointers.
private:
    /// @brief Default constructor used internally; creates an empty, unprepared collection.
    ContextMenus();

    /// @brief Prepares all menus for display (called by the framework).
    void prepare();
protected:

    ContextMenus(const ContextMenus&) = delete;
    ContextMenus& operator =(const ContextMenus&) = delete;
 public:
    /// @brief Constructs the collection and pre-allocates space for the given number of menus.
    /// @param nContextMenus  Number of context menus to accommodate.
    ContextMenus(size_t nContextMenus);

    /// @brief Destructor; releases all owned ContextMenu objects.
    ~ContextMenus();

    /// @brief Returns the root MenuItem of the menu at the specified index.
    /// @param menuID  Zero-based index of the context menu.
    /// @return Pointer to the root menu item, or nullptr if the index is out of range.
    MenuItem* getMenu(td::BYTE menuID);

    /// @brief Searches for a specific action item within a sub-menu range.
    /// @param menuID        Zero-based index of the context menu.
    /// @param firstSubMenuID  First sub-menu identifier in the search range.
    /// @param lastSubMenuID   Last sub-menu identifier in the search range.
    /// @param actionID      Action identifier to locate.
    /// @return Pointer to the matching MenuItem, or nullptr if not found.
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    /// @brief Opens the specified context menu at the given frame-relative position.
    /// @param menuID           Zero-based index of the context menu to open.
    /// @param pFrame           Frame relative to which the position is expressed.
    /// @param pointOnFrame     Position in frame coordinates where the menu should appear.
    /// @param pConsumer        Optional message consumer to receive action notifications.
    /// @param pContextData     Optional context data to associate with the menu.
    /// @param contextMenuGroup Optional group identifier for the menu.
    /// @return true if the menu was successfully opened.
    bool open(td::BYTE menuID, gui::Frame* pFrame, const gui::Point& pointOnFrame, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 contextMenuGroup = 0);

    /// @brief Assigns a ContextMenu pointer to a specific position in the collection.
    /// @param pos  Zero-based index at which to place the menu.
    /// @param mi   Pointer to the ContextMenu to store.
    void setItem(size_t pos, ContextMenu* mi);
};

} //namespace gui
