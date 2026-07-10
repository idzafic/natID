// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Navigator.h
    @brief Scrollable navigator control that displays selectable items with images or symbols. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NavigatorView.h"


namespace gui
{

/// @brief A scrollable control presenting a list of icon/label items that the user can select.
class NATGUI_API Navigator : public ViewScroller
{
private:
    std::function<void(gui::Navigator*)> _onChangedSelection; ///< Callback invoked when the selected item changes.
protected:
    NavigatorView _view; ///< The internal canvas view that renders and handles interaction for the items.
protected:
    Navigator() = delete;
    Navigator(const Navigator&) = delete;
    Navigator& operator =(const Navigator&) = delete;
protected:
    /// @brief Performs the initial size measurement for the navigator within a layout cell.
    /// @param cell CellInfo that receives the measured size information.
    void measure(CellInfo&) override;

    /// @brief Performs a re-measurement when the layout changes.
    /// @param cell CellInfo that receives the updated size information.
    void reMeasure(CellInfo&) override;

    /// @brief Returns whether this navigator is currently hidden.
    /// @return True if the navigator is hidden.
    bool isHidden() const override;

    /// @brief Applies the given geometry to position and size the navigator within its cell.
    /// @param cellFrame The geometry of the cell allocated to this navigator.
    /// @param cell The cell descriptor from the parent layout.
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

    /// @brief Constructs a Navigator with the specified number of items and layout parameters.
    /// @param nItems Number of selectable items in the navigator.
    /// @param orientation Layout orientation (Vertical or Horizontal).
    /// @param imageHeight Height in pixels of the image area for each item.
    /// @param widthMultiplier Multiplier applied to the image height to determine item width.
    Navigator(td::UINT2 nItems, gui::Orientation orientation = gui::Orientation::Vertical, td::UINT2 imageHeight = 64, float widthMultiplier=2.5);
public:
    /// @brief Destroys the Navigator and releases associated resources.
    ~Navigator();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::Navigator.
    gui::ObjType getObjType() const override { return ObjType::Navigator;}

    /// @brief Sets the image and label for a given item position.
    /// @param itemPos Zero-based index of the item to configure.
    /// @param pImg Pointer to the Image to display for this item.
    /// @param str Label text to display below the image.
    void setItem(td::UINT2 itemPos, Image* pImg, const td::String& str);

    /// @brief Sets the symbol and label for a given item position.
    /// @param itemPos Zero-based index of the item to configure.
    /// @param pSymb Pointer to the Symbol to display for this item.
    /// @param str Label text to display below the symbol.
    /// @param rotateDeg Optional clockwise rotation angle in degrees for the symbol.
    void setItem(td::UINT2 itemPos, Symbol* pSymb, const td::String& str, float rotateDeg=0);

    /// @brief Returns the index of the currently selected item.
    /// @return Zero-based index of the active selection.
    td::UINT2 getCurrentSelection() const;

    /// @brief Programmatically changes the selected item.
    /// @param selection Zero-based index to select.
    /// @param bSendMsg If true, a selection-changed message is dispatched.
    void setCurrentSelection(td::UINT2 selection, bool bSendMsg = false);

    /// @brief Returns a reference to the registered selection-changed callback.
    /// @return Const reference to the std::function called on selection change.
    const std::function<void(gui::Navigator*)>& getChangedSelectionHandler() const;

    /// @brief Registers a callback to be invoked whenever the selection changes.
    /// @param fnToCall The function to call; receives a pointer to this Navigator.
    void onChangedSelection(std::function<void(gui::Navigator*)> fnToCall);
};

} //namespace gui
