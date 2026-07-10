// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ToolBar.h
    @brief Application toolbar holding a collection of action and popover items. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Consumer.h"
#include <cnt/PushBackVector.h>
#include <gui/ToolBarItem.h>
#include <mu/IAppProperties.h>

namespace gui
{
class ToolBarHelper;
class ToolBarView;

/// @brief Manages an application toolbar with a list of allowed and default items.
class NATGUI_API ToolBar : public Consumer
{
    friend class SymbolPopoverView;
    friend class ToolBarHelper;
public:
    /// @brief Icon size policy for toolbar items.
    enum class IconSize : td::BYTE {
        Small=0,      ///< Small icons (typically 16 px).
        Regular,      ///< Regular-sized icons (typically 24 px).
        SystemDefault ///< Icon size follows the system toolbar preference.
    };
private:
    cnt::PushBackVector<gui::ToolBarItem> _allowedItems; ///< Full set of items that can appear in the toolbar.
    cnt::PushBackVector<td::WORD> _defaultItems;         ///< Indices into _allowedItems defining the default item order.
protected:
    ToolBarView* _view = nullptr; ///< Non-native toolbar view used on platforms without native toolbar support.
    td::String _strID;            ///< String identifier used for toolbar persistence (preferences).

    IconSize _iconSize = IconSize::Small;                                      ///< Current icon size setting.
    mu::IAppProperties::ToolBarType _propTBType = mu::IAppProperties::ToolBarType::Main; ///< Property storage type for this toolbar.
    td::BYTE _customizable = 0;  ///< Non-zero if the toolbar can be customised by the user.
    td::BYTE _showLabels = 1;    ///< Non-zero if item labels are shown beneath the icons.
    td::BYTE _leadingSpace = 0;  ///< Leading space in pixels before the first item (Windows/GTK).

    ToolBar(const ToolBar&) = delete;
    ToolBar& operator =(const ToolBar&) = delete;
protected:
    /// @brief Protected default constructor for use by the derived toolbar classes.
    ToolBar();

    /// @brief Reserves storage for the allowed and default item lists.
    /// @param nAllowedItems Capacity for the allowed-items list.
    /// @param nDefaultItems Capacity for the default-items index list.
    void reserve(td::WORD nAllowedItems, td::WORD nDefaultItems = 0);

    /// @brief Adds an image-backed action item to the toolbar.
    /// @param lbl Display label.
    /// @param image Pointer to the item icon image.
    /// @param toolTip Tooltip text.
    /// @param menuID Menu identifier for the associated action.
    /// @param firstSubMenuID First sub-menu identifier.
    /// @param lastSubMenuID Last sub-menu identifier.
    /// @param actionID Action identifier within the menu.
    /// @return Position of the newly added item.
    td::WORD addItem(const td::String& lbl, Image* image, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    /// @brief Adds a symbol-backed action item to the toolbar.
    /// @param lbl Display label.
    /// @param symbol Pointer to the item vector symbol.
    /// @param toolTip Tooltip text.
    /// @param menuID Menu identifier.
    /// @param firstSubMenuID First sub-menu identifier.
    /// @param lastSubMenuID Last sub-menu identifier.
    /// @param actionID Action identifier.
    /// @return Position of the newly added item.
    td::WORD addItem(const td::String& lbl, Symbol* symbol, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

//    void initItem(td::WORD itemPos, gui::PopoverView* pPopoverView, td::UINT2 ctrlID);

    /// @brief Adds a popover canvas item to the toolbar.
    /// @param pPopoverCanvas Pointer to the popover canvas.
    /// @param ctrlID Control identifier.
    /// @param lbl Optional display label.
    /// @param toolTip Optional tooltip text.
    /// @return Position of the newly added item.
    td::WORD addItem(gui::PopoverCanvas* pPopoverCanvas, td::UINT2 ctrlID, const td::String& lbl = "", const td::String& toolTip = "");

    /// @brief Adds a popup view item to the toolbar.
    /// @param pPopupView Pointer to the popup view.
    /// @param ctrlID Control identifier.
    /// @param lbl Display label.
    /// @param toolTip Optional tooltip text.
    /// @return Position of the newly added item.
    td::WORD addItem(gui::PopupView* pPopupView, td::UINT2 ctrlID, const td::String& lbl, const td::String& toolTip = "");

    /// @brief Adds an embedded data control item (e.g. combo box) to the toolbar.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    /// @param pCtrl Pointer to the data control.
    /// @param ctrlID Control identifier.
    /// @return Position of the newly added item.
    td::WORD addItem(const td::String& lbl, const td::String& toolTip, gui::DataCtrl* pCtrl, td::UINT2 ctrlID);

    /// @brief Adds an embedded view item to the toolbar.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    /// @param pView Pointer to the view to embed.
    /// @return Position of the newly added item.
    td::WORD addItem(const td::String& lbl, const td::String& toolTip, gui::View* pView);

    /// @brief Adds a flexible space item to the toolbar.
    /// @return Position of the space item.
    td::WORD addSpaceItem();

    /// @brief Returns a const reference to the toolbar item at the given position.
    /// @param pos Zero-based position.
    /// @return Const reference to the item.
    const ToolBarItem& getItemByPosition(td::WORD pos) const;

    /// @brief Returns a pointer to the toolbar item at the given position.
    /// @param pos Zero-based position.
    /// @return Pointer to the item, or nullptr if out of range.
    ToolBarItem* getItem(td::WORD pos);
public:
    /// @brief Constructs a toolbar with a string identifier and pre-allocated item lists.
    /// @param strID String identifier for preferences persistence.
    /// @param nAllowedItems Capacity for the allowed-items list.
    /// @param nDefaultItems Capacity for the default-items list.
    /// @param customizable True if the toolbar layout can be customised by the user.
    ToolBar(const char* strID, td::WORD nAllowedItems, td::WORD nDefaultItems = 0, bool customizable = false);

    /// @brief Destructor.
    ~ToolBar();

    /// @brief Returns the string identifier of the toolbar.
    /// @return Const reference to the identifier string.
    const td::String& getID() const;

    /// @brief Returns whether the toolbar is customisable by the user.
    /// @return True if customisable.
    bool isCustomizable() const;

    //void setAsMain() const;
    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::ToolBar.
    gui::ObjType getObjType() const override { return ObjType::ToolBar; }

    /// @brief Returns a reference to the vector of all allowed toolbar items.
    /// @return Reference to the allowed-items vector.
    cnt::PushBackVector<gui::ToolBarItem>& getAllowedItems();

    /// @brief Returns a reference to the vector of default item indices.
    /// @return Reference to the default-items index vector.
    cnt::PushBackVector<td::WORD>& getDefaultItems();

    /// @brief Sets the icon size for all toolbar items.
    /// @param iconSize Desired icon size.
    void setIconSize(IconSize iconSize);

    /// @brief Returns the current icon size setting.
    /// @return Icon size enum value.
    ToolBar::IconSize getIconSize() const;

    /// @brief Returns the icon size in device pixels.
    /// @return Icon size in pixels.
    int getIconSizeInPixels() const;

    /// @brief Returns a pointer to the non-native toolbar view (platforms without native toolbar).
    /// @return Pointer to the ToolBarView, or nullptr if native.
    ToolBarView* getView();

    /// @brief Returns a const pointer to the non-native toolbar view.
    /// @return Const pointer to the ToolBarView.
    const ToolBarView* getView() const;

    /// @brief Looks up a toolbar item by its string identifier.
    /// @param strID Pointer to the identifier string.
    /// @param nCh Length of the identifier string.
    /// @return Pointer to the matching item, or nullptr if not found.
    ToolBarItem* getItem(const char* strID, size_t nCh);

    /// @brief Looks up a toolbar item by its menu and action identifiers.
    /// @param menuID Menu identifier.
    /// @param firstSubMenuID First sub-menu identifier.
    /// @param lastSubMenuID Last sub-menu identifier.
    /// @param actionID Action identifier.
    /// @return Pointer to the matching item, or nullptr if not found.
    ToolBarItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    /// @brief Looks up a toolbar item by its global action identifier.
    /// @param gid Global action identifier.
    /// @return Pointer to the matching item, or nullptr if not found.
    ToolBarItem* getItem(td::UINT4 gid);

    /// @brief Returns the data control embedded in the item with the given control ID.
    /// @param ctrlID Control identifier.
    /// @return Pointer to the DataCtrl, or nullptr if not found.
    gui::DataCtrl* getDataCtrl(td::UINT4 ctrlID);

    /// @brief Returns the popover canvas item with the given control ID.
    /// @param popoverID Popover control identifier.
    /// @return Pointer to the PopoverCanvas, or nullptr if not found.
    gui::PopoverCanvas* getPopoverCanvas(td::UINT2 popoverID);

    /// @brief Returns a const pointer to the popover canvas with the given ID.
    /// @param popoverID Popover control identifier.
    /// @return Const pointer to the PopoverCanvas.
    const gui::PopoverCanvas* getPopoverCanvas(td::UINT2 popoverID) const;

    /// @brief Enables or disables the embedded control with the given ID.
    /// @param ctrlID Control identifier.
    /// @param bEnable True to enable; false to disable.
    void enableCtrl(td::UINT4 ctrlID, bool bEnable);

    /// @brief Enables or disables the popover with the given ID.
    /// @param popoverID Popover identifier.
    /// @param bEnable True to enable; false to disable.
    void enablePopover(td::UINT2 popoverID, bool bEnable);

    /// @brief Returns the number of items in the allowed-items list.
    /// @return Count of allowed items.
    td::WORD getNoOfAllowedItems() const;

    /// @brief Returns the number of items in the default-items list.
    /// @return Count of default items.
    td::WORD getNoOfDefaultItems() const;

    /// @brief Returns the allowed-items position of the default item at the given index.
    /// @param pos Zero-based index in the default-items list.
    /// @return Corresponding index in the allowed-items list.
    td::WORD getIndexOfDefaultItem(td::WORD pos) const;

    /// @brief Returns whether the toolbar is currently visible.
    /// @return True if visible.
    bool isVisible() const;

    /// @brief Shows or hides the toolbar.
    /// @param bShow True to show; false to hide.
    void show(bool bShow);

    /// @brief Hides the toolbar.
    /// @param bHide True to hide; false to show.
    void hide(bool bHide);

    /// @brief Shows or hides item labels beneath the icons.
    /// @param bShowLabels True to show labels.
    void showLabels(bool bShowLabels);

    /// @brief Returns whether item labels are currently shown.
    /// @return True if labels are visible.
    bool areLabelsVisible() const;

    //void enableItem(td::WORD itemPos, bool bEnable); //enable/disable toolbar item

    /// @brief Sets the property storage type for this toolbar.
    /// @param propType Property type enum value.
    void setPropertyType(mu::IAppProperties::ToolBarType propType);

    /// @brief Returns the property storage type for this toolbar.
    /// @return Property type enum value.
    mu::IAppProperties::ToolBarType getPropertyType() const;

    //Windows only (with GTK)
    /// @brief Sets the leading space before the first item (Windows/GTK only).
    /// @param leadingSpace Space in pixels.
    void setLeadingSpace(td::BYTE leadingSpace);

    /// @brief Notifies the toolbar that the system colour mode has changed.
    /// @param bDarkMode True if the system is now in dark mode.
    void systemColorModeChanged(bool bDarkMode);

    /// @brief Returns the leading space before the first item.
    /// @return Leading space in pixels.
    td::BYTE getLeadingSpace() const;
};

} //namespace gui
