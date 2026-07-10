// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ActionItemDescriptor.h
    @brief Descriptor structure carrying full identification and context of a triggered action item. */
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

/// @brief Describes a triggered action item, including its source, menu hierarchy IDs, and associated context data.
class NATGUI_API ActionItemDescriptor
{
    friend class Consumer;
    ActionItemDescriptor(const ActionItemDescriptor&) = delete;
    ActionItemDescriptor& operator =(const ActionItemDescriptor&) = delete;
public:
    /// @brief Indicates the origin of the action item trigger.
    enum class Source : td::BYTE {MenuOrToolbar=0, ///< Action triggered from a menu or toolbar.
                                   Report,           ///< Action triggered from a report.
                                   Other};           ///< Action triggered from another source.
    gui::ActionItem* _pActionItem;          ///< Pointer to the associated ActionItem.
    const gui::ContextData* _pContextData;  ///< Pointer to the context data at the time of the action.
    td::String _menuItemFileName;           ///< File name associated with the menu item.
    td::String _menuItemShortName;          ///< Short name of the menu item.
    td::BYTE _menuID;                       ///< ID of the menu that owns this action item.
    td::BYTE _firstSubmenuID;               ///< ID of the first submenu in the hierarchy.
    td::BYTE _lastSubMenuID;                ///< ID of the last submenu in the hierarchy.
    td::BYTE _actionItemID;                 ///< ID of the action item itself.
    Source _source;                         ///< Source from which the action was triggered.

protected:
    /// @brief Default protected constructor.
    ActionItemDescriptor();
public:
    /// @brief Constructs an ActionItemDescriptor with menu hierarchy information and an action item pointer.
    /// @param menuID ID of the owning menu.
    /// @param firstSubmenuID ID of the first submenu.
    /// @param lastSubMenuID ID of the last submenu.
    /// @param actionItemID ID of the action item.
    /// @param pActionItem Pointer to the ActionItem.
    ActionItemDescriptor(td::BYTE menuID, td::BYTE firstSubmenuID, td::BYTE lastSubMenuID, td::BYTE actionItemID, gui::ActionItem* pActionItem);
    /// @brief Constructs an ActionItemDescriptor with source, context data, menu hierarchy information, and an action item pointer.
    /// @param source The source of the action trigger.
    /// @param pContextData Pointer to the associated context data.
    /// @param menuID ID of the owning menu.
    /// @param firstSubmenuID ID of the first submenu.
    /// @param lastSubMenuID ID of the last submenu.
    /// @param actionItemID ID of the action item.
    /// @param pActionItem Pointer to the ActionItem.
    ActionItemDescriptor(Source source, const gui::ContextData* pContextData, td::BYTE menuID, td::BYTE firstSubmenuID, td::BYTE lastSubMenuID, td::BYTE actionItemID, gui::ActionItem* pActionItem);
    /// @brief Returns a tuple of all four ID components (menuID, firstSubmenuID, lastSubMenuID, actionItemID).
    /// @return Tuple containing (menuID, firstSubmenuID, lastSubMenuID, actionItemID).
    std::tuple<td::BYTE, td::BYTE, td::BYTE, td::BYTE> getIDs() const;
    /// @brief Returns a pointer to the associated ActionItem.
    /// @return Pointer to the ActionItem.
    gui::ActionItem* getActionItem();
    /// @brief Returns the file name associated with the menu item.
    /// @return Const reference to the menu item file name string.
    const td::String& getMenuItemFileName() const;
    /// @brief Returns the short name of the menu item.
    /// @return Const reference to the menu item short name string.
    const td::String& getMenuItemShortName() const;
    /// @brief Returns the source from which this action was triggered.
    /// @return The Source enum value.
    Source getSource() const;
    /// @brief Returns a pointer to the associated context data.
    /// @return Const pointer to the ContextData.
    const gui::ContextData* getContextData() const;
};

}
