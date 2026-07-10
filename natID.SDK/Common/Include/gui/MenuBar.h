// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MenuBar.h
    @brief Application or window menu bar that owns a collection of top-level menu items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <cnt/SafeFullVector.h>
#include "Consumer.h"
#include <gui/MenuItem.h>

namespace gui
{
class Window;
class MenuBarHelper;

/// @brief Represents the application or window menu bar and manages its top-level menus.
class NATGUI_API MenuBar : public Consumer
{
    friend class MenuBarHelper;
public:
    /// @brief Holds a file-system path together with a short display name for recently-opened files.
    using PathAndShortName = struct _PAndSN
    {
        td::String path;      ///< Full file-system path to the file.
        td::String shortName; ///< Short display name shown in the menu.
    };

protected:
    cnt::PushBackVector< cnt::SafeFullVector<PathAndShortName> > _fileNames; ///< Collections of recently-opened file entries, one collection per designated menu slot.
private:
    td::UINT4 _GID = 0; ///< Global identifier assigned to this menu bar instance.
protected:
    td::BYTE _context = 0; ///< Non-zero when this menu bar is used as a context menu.
//    td::BYTE _takeAppMenuNameFromExecutable = 1;
private:
    /// @brief Default constructor (private; use the public sized constructor instead).
    MenuBar();

    /// @brief Internal preparation step called before the menu bar is displayed.
    /// @param forContextMenu True if the menu bar is being prepared for use as a context menu.
    void prepare(bool forContextMenu);

    MenuBar(const MenuBar&) = delete;
    MenuBar& operator =(const MenuBar&) = delete;
protected:
    cnt::SafeFullVector<MenuItem*> _menus; ///< Ordered collection of top-level menu item pointers.
    //void addMenu(MenuItem* item);

    /// @brief Loads the menu bar structure from an XML resource file.
    /// @param xmlResConfigName Name of the XML resource file that describes the menu layout.
    /// @return True if the resource was loaded successfully, false otherwise.
    bool loadFromRes(const char* xmlResConfigName);

    /// @brief Assigns a menu item to a specific position in the bar.
    /// @param menuPos Zero-based position index in the menu bar.
    /// @param menuItem Pointer to the MenuItem to place at that position.
    void setMenu(td::WORD menuPos, MenuItem* menuItem);

    /// @brief Reserves capacity for file-name collections used by recent-file menus.
    /// @param nMenuFileNames Number of recent-file menu slots to reserve.
    void reserveFileNames(td::BYTE nMenuFileNames);

 public:
    /// @brief Specifies where the menu bar is rendered relative to the application or window.
    enum class Location : td::BYTE
    {
        SystemSpecific = 0, ///< Use the platform-default location (e.g. system menu bar on macOS).
        EmbeddedInWindow    ///< Embed the menu bar inside the window frame.
    };

    /// @brief Attaches this menu bar to a main window and makes it the active menu bar.
    /// @param pMainWnd Pointer to the main window that will own this menu bar.
    /// @param location Specifies where the menu bar should be rendered.
    void setAsMain(gui::Window* pMainWnd, Location location = Location::SystemSpecific);

    /// @brief Constructs a MenuBar with a fixed number of top-level menus.
    /// @param nMenus Number of top-level menu slots to allocate.
    MenuBar(size_t nMenus);

    /// @brief Destroys the MenuBar and releases all owned menu items.
    ~MenuBar();

    /// @brief Returns the global identifier of this menu bar.
    /// @return The UINT4 global ID.
    td::UINT4 getGID() const;

    /// @brief Returns whether this menu bar is configured as a context menu.
    /// @return True if this is a context menu, false otherwise.
    bool isContext() const;

    /// @brief Returns the number of top-level menus in the bar.
    /// @return Count of top-level menu entries.
    size_t getNoOfMenus() const;

    /// @brief Provides mutable access to the internal vector of top-level menu pointers.
    /// @return Reference to the SafeFullVector of MenuItem pointers.
    cnt::SafeFullVector<MenuItem*>& getItems();

    /// @brief Retrieves the top-level menu at the given position.
    /// @param menuPos Zero-based position index.
    /// @return Pointer to the MenuItem at that position, or nullptr if out of range.
    MenuItem* getMenu(size_t menuPos);

    /// @brief Searches for a top-level menu by its numeric ID.
    /// @param menuID The byte-sized identifier of the menu to find.
    /// @return Pointer to the matching MenuItem, or nullptr if not found.
    MenuItem* getMenuByID(td::BYTE menuID);

    /// @brief Returns the GUI object type identifier for this control.
    /// @return ObjType::MenuBar.
    gui::ObjType getObjType() const override { return ObjType::MenuBar; }

    /// @brief Searches for a specific action item nested within the menu hierarchy.
    /// @param menuID Identifier of the top-level menu.
    /// @param firstSubMenuID First sub-menu ID in the range to search.
    /// @param lastSubMenuID Last sub-menu ID in the range to search.
    /// @param actionID Identifier of the action item to locate.
    /// @return Pointer to the matching MenuItem, or nullptr if not found.
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    /// @brief Returns the recently-opened file entries for the given menu slot index.
    /// @param pos Byte-sized index of the file-names collection.
    /// @return Const reference to the SafeFullVector of PathAndShortName entries.
    const cnt::SafeFullVector<MenuBar::PathAndShortName>& getFileNames(td::BYTE pos) const;

    /// @brief Returns the recently-opened file entries using a typed enum or integer position.
    /// @tparam T Type that is implicitly convertible to td::BYTE.
    /// @param pos Typed position index cast to td::BYTE internally.
    /// @return Const reference to the SafeFullVector of PathAndShortName entries.
    template <typename T>
    const cnt::SafeFullVector<MenuBar::PathAndShortName>& getFileNames(T pos) const
    {
        return getFileNames(td::BYTE(pos));
    }
};

} //namespace gui
