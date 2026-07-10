// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ToolBarItem.h
    @brief Descriptor for a single item in a toolbar, combining an action with a visual. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "gui/ActionItem.h"
#include <cnt/SafeFullVector.h>
#include <td/String.h>
#include <gui/IPopoverButton.h>

namespace cnt
{
    template <typename T, bool EXTERN_ALLOCATOR>
    class SafeFullVector;
}

namespace gui
{

class Image;
class Symbol;
class DataCtrl;
//class BaseView;
class ToolBar;
class PopoverCanvas;
class PopoverButton;
class PopupView;
class ToolBarItemHelper;
class View;

/// @brief Represents a single entry in a toolbar, which may be an image, symbol, control, popover, or space.
class NATGUI_API ToolBarItem : public ActionItem, public IPopoverButton
{
    friend class ToolBar;
    friend class PopoverView;
    friend class ToolBarItemHelper;

    template <typename T, bool EXTERN_ALLOCATOR>
    friend class cnt::SafeFullVector;
public:
    /// @brief Classifies the kind of visual or interactive element this item holds.
    enum class Type : unsigned char {
        Image,         ///< Item is represented by a raster image.
        Symbol,        ///< Item is represented by a vector symbol.
        Control,       ///< Item hosts an embedded data control (e.g. combo box).
        PopoverCanvas, ///< Item opens a popover canvas on click.
        View,          ///< Item embeds an arbitrary view.
        PopupView,     ///< Item opens a popup view on click.
        Space          ///< Flexible space separator between items.
    };
protected:
    ToolBar* _pToolBar = nullptr; ///< Back-pointer to the toolbar that owns this item.
    td::String _strID;            ///< String identifier used for persistence and lookup.
    td::String _lbl;              ///< Display label shown beneath the icon.
    td::String _toolTip;          ///< Tooltip text displayed on hover.
    union
    {
        Image* _image = nullptr;       ///< Raster image (when type == Image).
        Symbol* _symbol;               ///< Vector symbol (when type == Symbol).
        DataCtrl* _control;            ///< Embedded data control (when type == Control).
        PopoverCanvas* _popoverCanvas; ///< Popover canvas (when type == PopoverCanvas).
        View* _view;                   ///< Embedded view (when type == View).
        PopupView* _popupView;         ///< Popup view (when type == PopupView).
    };

    td::UINT4 _gid = 0;           ///< Global action identifier combining menu and action IDs.
    Type _type = Type::Space;     ///< Current item type.
protected:
    //for popover
    /// @brief Sets the current popover selection and optionally closes the popover.
    /// @param pos Zero-based index of the selected popover item.
    /// @param closePopover True to close the popover after the selection.
    void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) override;

    /// @brief Sends the popover selection as a message to the owning consumer.
    void sendPopoverMessage() override;

    /// @brief Closes the popover without sending a message.
    void closePopover() override;

    /// @brief Enables or disables the popover interaction.
    /// @param bEnable True to enable.
    void enablePopover(bool bEnable) override;

    /// @brief Sets the native OS handle for this item.
    /// @param handle Native handle to assign.
    void setHandle(gui::Handle handle);

    /// @brief Returns the native OS handle.
    /// @return Native handle.
    gui::Handle getHandle();

    /// @brief Returns a const native OS handle.
    /// @return Const native handle.
    const gui::Handle getHandle() const;

    //simple action item (with image or symbol)
    /// @brief Initialises the item as an image-backed action item.
    /// @param itemID String identifier for the item.
    /// @param lbl Display label.
    /// @param image Pointer to the icon image.
    /// @param toolTip Tooltip text.
    /// @param menuID Menu identifier.
    /// @param firstSubMenuID First sub-menu identifier.
    /// @param lastSubMenuID Last sub-menu identifier.
    /// @param actionID Action identifier.
    void init(const char* itemID, const td::String& lbl, Image* image, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    /// @brief Initialises the item as a symbol-backed action item.
    /// @param itemID String identifier for the item.
    /// @param lbl Display label.
    /// @param symbol Pointer to the vector symbol.
    /// @param toolTip Tooltip text.
    /// @param menuID Menu identifier.
    /// @param firstSubMenuID First sub-menu identifier.
    /// @param lastSubMenuID Last sub-menu identifier.
    /// @param actionID Action identifier.
    void init(const char* itemID, const td::String& lbl, Symbol* symbol, const td::String& toolTip, td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);

    //init using control (DataCtrl, like ComboBox)
    /// @brief Initialises the item to host an embedded data control.
    /// @param itemID String identifier for the item.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    /// @param pCtrl Pointer to the data control.
    /// @param ctrlID Control identifier.
    void init(const char* itemID, const td::String& lbl, const td::String& toolTip, gui::DataCtrl* pCtrl, td::UINT2 ctrlID);

    /// @brief Initialises the item to embed an arbitrary view.
    /// @param itemID String identifier for the item.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    /// @param pView Pointer to the view.
    void init(const char* itemID, const td::String& lbl, const td::String& toolTip, gui::View* pView);

    //init using popover view
    /// @brief Initialises the item to open a popover canvas.
    /// @param itemID String identifier for the item.
    /// @param pPopoverCanvas Pointer to the popover canvas.
    /// @param ctrlID Control identifier.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    void init(const char* itemID, gui::PopoverCanvas* pPopoverCanvas, td::UINT4 ctrlID, const td::String& lbl, const td::String& toolTip);

    //init using popup view
    /// @brief Initialises the item to open a popup view.
    /// @param itemID String identifier for the item.
    /// @param pPopupView Pointer to the popup view.
    /// @param ctrlID Control identifier.
    /// @param lbl Display label.
    /// @param toolTip Tooltip text.
    void init(const char* itemID, gui::PopupView* pPopupView, td::UINT4 ctrlID, const td::String& lbl, const td::String& toolTip);

    ToolBarItem(const ToolBarItem&) = delete;
    ToolBarItem& operator =(const ToolBarItem&) = delete;
public:
    /// @brief Default constructor. Creates a space-type item.
    ToolBarItem();

    /// @brief Move constructor.
    /// @param other Source item to move from.
    ToolBarItem(ToolBarItem&&);

    /// @brief Move assignment operator.
    /// @param other Source item to move from.
    /// @return Reference to this item.
    ToolBarItem& operator =(ToolBarItem&&);

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::ToolBarItem.
    gui::ObjType getObjType() const override { return ObjType::ToolBarItem;}

    /// @brief Returns the global action identifier for this item.
    /// @return Combined action identifier.
    td::UINT4 getActionID() const;

    /// @brief Returns a pointer to the parent toolbar.
    /// @return Pointer to the ToolBar.
    ToolBar* getToolBar();

    /// @brief Returns a const pointer to the parent toolbar.
    /// @return Const pointer to the ToolBar.
    const ToolBar* getToolBar() const;

    /// @brief Returns the display label.
    /// @return Const reference to the label string.
    const td::String& getLbl() const;

    /// @brief Returns the string identifier.
    /// @return Const reference to the identifier string.
    const td::String& getStrID() const;

    /// @brief Returns the tooltip text.
    /// @return Const reference to the tooltip string.
    const td::String& getToolTip() const;

    /// @brief Returns a pointer to the icon image (valid only when type == Image).
    /// @return Pointer to the Image.
    Image* getImage();

    /// @brief Returns a pointer to the vector symbol (valid only when type == Symbol).
    /// @return Pointer to the Symbol.
    Symbol* getSymbol();

    /// @brief Replaces the item's icon image.
    /// @param pImg Pointer to the new image.
    void setImage(const Image* pImg);

    /// @brief Replaces the item's vector symbol.
    /// @param pSymbol Pointer to the new symbol.
    void setSymbol(const Symbol* pSymbol);

    /// @brief Updates the display label.
    /// @param pLbl New label string.
    void setLabel(const td::String& pLbl);

    /// @brief Updates the tooltip text.
    /// @param pToolTip New tooltip string.
    void setTooltip(const td::String& pToolTip);

    //Popover& getPopover();
    /// @brief Returns the embedded data control (valid only when type == Control).
    /// @return Pointer to the DataCtrl.
    DataCtrl* getControl();

    /// @brief Returns the popover canvas (valid only when type == PopoverCanvas).
    /// @return Pointer to the PopoverCanvas.
    PopoverCanvas* getPopoverCanvas();

    /// @brief Returns a const pointer to the popover canvas.
    /// @return Const pointer to the PopoverCanvas.
    const PopoverCanvas* getPopoverCanvas() const;

    /// @brief Returns the embedded view (valid only when type == View).
    /// @return Pointer to the View.
    View* getView();

    /// @brief Returns a const pointer to the embedded view.
    /// @return Const pointer to the View.
    const View* getView() const;

    /// @brief Returns the popup view (valid only when type == PopupView).
    /// @return Pointer to the PopupView.
    PopupView* getPopupView();

    /// @brief Returns a const pointer to the popup view.
    /// @return Const pointer to the PopupView.
    const PopupView* getPopupView() const;

    /// @brief Returns the type of visual or interactive element this item represents.
    /// @return Type enum value.
    Type getType() const;

    /// @brief Re-initialises the item as a flexible space separator.
    void initAsSpace();

    /// @brief Notifies the item that the system colour mode has changed.
    /// @param bDarkMode True if the system is now in dark mode.
    void systemColorModeChanged(bool bDarkMode);

    /// @brief Returns whether the item's string identifier matches the given string.
    /// @param strID Pointer to the identifier string to compare.
    /// @param nCh Length of the identifier string.
    /// @return True if the identifiers match.
    bool hasStrID(const char *strID, size_t nCh) const;
};

} //namespace gui
