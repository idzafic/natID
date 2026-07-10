// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MenuItem.h
    @brief Menu item and sub-menu classes for building application menus. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ActionItem.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <gui/Image.h>

namespace cnt
{
    template <typename T, bool EXTERN_ALLOCATOR>
    class SafeFullVector;
}

namespace gui
{

class MenuBar;
class Consumer;

class MenuItemHelper;

/// @brief Represents a single entry in a menu; can be a separator, sub-menu, action, or checkable action.
class NATGUI_API MenuItem : public ActionItem
{
    template <typename T, bool EXTERN_ALLOCATOR>
    friend class cnt::SafeFullVector;

    friend class MenuItemHelper;
public:
    /// @brief Classifies the role of a MenuItem within its parent menu.
    enum class Type : unsigned char
    {
        Separator,   ///< A visual separator line with no associated action.
        SubMenu,     ///< A nested sub-menu that reveals additional items on hover.
        Action,      ///< A regular command item that triggers an action when selected.
        CheckAction  ///< A toggleable action item that displays a checkmark when active.
    };
protected:
    cnt::SafeFullVector<MenuItem> _items;         ///< Child items when this item is a sub-menu.
    td::String _lbl;                              ///< Display label shown in the menu.
    td::String _shortCut;                         ///< Keyboard shortcut string set during initialisation.
    gui::MenuBar* _pMenuBar = nullptr;            ///< Pointer to the owning menu bar, set after menu creation.
    td::UINT4 _gid = 0;                           ///< Global action identifier for this item.
    Type _type = Type::Separator;                 ///< The type of this menu item.
    td::BYTE _id = 0;                             ///< Local identifier within its parent menu.
    td::BYTE _posMenuFileNames = 0;               ///< Index into the menu bar's file-name collections for recent-file items.
    td::BYTE _checked : 1;                        ///< Checkmark state bit (1 = checked) for CheckAction items.
    td::BYTE _initialized : 1;                    ///< Initialisation flag bit (1 = fully initialised).
private:
    /// @brief Returns whether this item has been fully initialised.
    /// @return True if the item is initialised.
    bool isInitialized() const;

    /// @brief Associates this item with its owning menu bar.
    /// @param pMenuBar Pointer to the parent MenuBar.
    void setMenuBar(gui::MenuBar* pMenuBar);

    /// @brief Reads the checked state from the underlying platform object.
    /// @return True if the item is currently checked.
    bool getCheckedState() const;

    /// @brief Writes the checked state to the underlying platform object.
    /// @param checked The new checked state to apply.
    void setCheckedState(bool checked);

    /// @brief Stores the position index for recent-file menu entries.
    /// @param pos The index into the menu bar file-name collections.
    void setPosMenuFileNames(td::BYTE pos);

protected:
    MenuItem(const MenuItem&) = delete;
    MenuItem& operator =(const MenuItem&) = delete;

    /// @brief Default constructor; creates an uninitialised MenuItem.
    MenuItem();

    /// @brief Destroys the MenuItem and its child items.
    ~MenuItem();

    /// @brief Sets the native platform handle for this item.
    /// @param handle The platform-specific handle to assign.
    void setHandle(gui::Handle handle);

    /// @brief Assigns the global action identifier.
    /// @param gid The global identifier to assign.
    void setActionID(td::UINT4 gid);

    /// @brief Returns the keyboard shortcut string for this item.
    /// @return Const reference to the shortcut string.
    const td::String& getShortCut() const;

public:
    /// @brief Initialises this item as a sub-menu with child items.
    /// @param subMenuID Identifier for this sub-menu.
    /// @param lbl Display label for the sub-menu.
    /// @param nSubItems Number of child items to reserve.
    void initAsSubMenu(td::BYTE subMenuID, const td::String& lbl, size_t nSubItems);

    /// @brief Initialises this item as a sub-menu loaded from an external menu resource file.
    /// @param pParent Pointer to the owning MenuBar.
    /// @param subMenuID Identifier for this sub-menu.
    /// @param lbl Display label for the sub-menu.
    /// @param menuResFileName Path to the XML menu resource file.
    /// @return 0 on success, or a non-zero error code on failure.
    int initAsSubMenuFromResMenuFile(MenuBar* pParent, td::BYTE subMenuID, const td::String& lbl, const char* menuResFileName);

    /// @brief Initialises this item as a standard command action.
    /// @param lbl Display label for the action.
    /// @param itemID Local identifier for this action within its parent menu.
    /// @param shortCut Optional keyboard shortcut string (may be nullptr).
    void initAsActionItem(const td::String& lbl, unsigned char itemID, const char* shortCut = nullptr);

    /// @brief Initialises this item as a visual separator with no action.
    void initAsSeparator();

    /// @brief Initialises this item as the application quit action.
    /// @param lbl Display label for the quit action.
    /// @param shortCut Optional keyboard shortcut string (may be nullptr).
    void initAsQuitAppActionItem(const td::String& lbl, const char* shortCut = nullptr);

    /// @brief Makes this action item toggleable with an optional initial check state.
    /// @param bInitalyChecked If true, the item starts in the checked state.
    void setAsCheckable(bool bInitalyChecked = false); //must be initialized first with initAsActionItem

    //void addMenuItem(const MenuItem& subMenu);

    /// @brief Returns the type classification of this menu item.
    /// @return The MenuItem::Type value for this item.
    MenuItem::Type getType() const;

    /// @brief Returns the local identifier of this item within its parent menu.
    /// @return The byte-sized item ID.
    td::BYTE getID() const;

    /// @brief Returns the global action identifier associated with this item.
    /// @return The UINT4 global action ID.
    td::UINT4 getActionID() const;

    /// @brief Returns the display label of this item.
    /// @return Const reference to the label string.
    const td::String& getLbl() const;

    /// @brief Returns a pointer to the owning menu bar.
    /// @return Pointer to the parent MenuBar, or nullptr if not yet associated.
    gui::MenuBar* getMenuBar();

    /// @brief Returns the position index into the menu bar file-name collections.
    /// @return The file-names slot position.
    td::BYTE getPosMenuFileNames() const;

    /// @brief Provides mutable access to the child item collection.
    /// @return Reference to the SafeFullVector of child MenuItem objects.
    cnt::SafeFullVector<MenuItem>& getItems();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::MenuItem.
    gui::ObjType getObjType() const override { return ObjType::MenuItem; }

    /// @brief Searches for a nested action item using menu and action identifiers.
    /// @param menuID Identifier of the enclosing top-level menu.
    /// @param firstSubMenuID First sub-menu ID in the search range.
    /// @param lastSubMenuID Last sub-menu ID in the search range.
    /// @param actionID Identifier of the specific action item.
    /// @return Pointer to the matching MenuItem, or nullptr if not found.
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
};

/// @brief Convenience class that constructs and initialises a MenuItem as a sub-menu in a single step.
class NATGUI_API SubMenu : public MenuItem
{
public:
    /// @brief Constructs and initialises a sub-menu with the given identifier, label, and capacity.
    /// @param subMenuID Identifier for this sub-menu.
    /// @param lbl Display label for the sub-menu.
    /// @param nSubItems Number of child items to reserve.
    SubMenu(td::BYTE subMenuID, const td::String& lbl, size_t nSubItems);
};

} //namespace gui
