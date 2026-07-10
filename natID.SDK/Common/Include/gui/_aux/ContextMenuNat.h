// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ContextMenuNat.h
 * @brief Native context menu implementation backed by a platform object.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/NatObject.h>

namespace gui
{
class NatObjHelper;

/// @brief Wraps a native context menu and tracks its identifier and separator count.
class NATGUI_API ContextMenuNat : public NatObject
{
    friend class NatObjHelper;
private:
    td::BYTE _menuID;           ///< Unique identifier assigned to this context menu.
    td::BYTE _nSeparators = 0;  ///< Number of separator items currently in the menu.
    ContextMenuNat(const ContextMenuNat&) = delete;
    ContextMenuNat& operator = (const ContextMenuNat&) = delete;
 public:
    /// @brief Default constructor. Creates an empty native context menu.
    ContextMenuNat();
    /// @brief Move constructor. Transfers ownership of native menu resources.
    /// @param other The ContextMenuNat instance to move from.
    ContextMenuNat(ContextMenuNat&& other);
    /// @brief Move assignment operator. Transfers ownership of native menu resources.
    /// @param other The ContextMenuNat instance to move from.
    /// @return Reference to this instance.
    ContextMenuNat& operator = (ContextMenuNat&& other);
    /// @brief Destructor. Releases native menu resources.
    ~ContextMenuNat();
    /// @brief Returns the GUI object type identifier for this context menu.
    /// @return ObjType::ContextMenuNat.
    gui::ObjType getObjType() const override { return ObjType::ContextMenuNat; }
    /// @brief Returns the unique menu identifier.
    /// @return The byte identifier of this context menu.
    td::BYTE getMenuID() const;
    /// @brief Returns the number of separator items in the menu.
    /// @return Count of separators.
    td::BYTE getNoOfSeparators() const;
};

} //namespace gui
