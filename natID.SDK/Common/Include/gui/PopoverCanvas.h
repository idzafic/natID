// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PopoverCanvas.h @brief Declares the PopoverCanvas base class for drawing custom popover selection grids. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Canvas.h"
#include <gui/IPopoverUpdater.h>

namespace gui
{

class IPopoverButton;
class PopoverCanvasHelper;
class ToolBarItem;

/// @brief Abstract base class for a canvas that renders a grid of selectable items in a popover.
class NATGUI_API PopoverCanvas : public Canvas
{
    friend class PopoverCanvasHelper;
    friend class ToolBarItem;
public:
    /// @brief Display type of the popover items.
    enum class Type : td::BYTE {SymbolOnly=0, ///< Items show only a symbol/image.
                                SymbolAndText  ///< Items show a symbol and a text label.
                               };
    /// @brief Controls how symbol width is determined.
    enum class SymbolWidth : td::BYTE {UserDefined=0,        ///< Symbol width is set explicitly by the user.
                                       AdjustToMaxWHRatio    ///< Symbol width is adjusted to maintain aspect ratio.
                                      };
protected:
    IPopoverButton* _btnPopover = nullptr;       ///< The button that owns and triggers this popover.
    Handle _popoverFrame = nullptr;              ///< Native handle for the popover window frame.
    IPopoverUpdater* _popoverUpdater = nullptr;  ///< Optional updater that can change item states dynamically.
    // Frame* _frameToFocusAfterClose = nullptr;
    td::UINT2 _nItems;             ///< Total number of items in the popover.
    td::UINT2 _currentSelection;   ///< Index of the currently selected item.
    td::UINT2 _hoverSelection;     ///< Index of the item currently under the mouse cursor.
    td::UINT2 _cellWidth;          ///< Width of each cell in pixels.
    td::UINT2 _cellHeight;         ///< Height of each cell in pixels.
    td::UINT2 _symbolWidth;        ///< Width allocated for the symbol within a cell.
    td::UINT2 _symbolHeight;       ///< Height allocated for the symbol within a cell.
    td::BYTE _numberOfToolTipRows; ///< Number of rows reserved for tooltip text below items.
    td::BYTE _nRows;               ///< Number of item rows in the grid.
    td::BYTE _nCols;               ///< Number of item columns in the grid.
    Type _type = Type::SymbolOnly; ///< Display type of the popover.
    Popover::Location _location = Popover::Location::AboveButton; ///< Where the popover appears relative to its button.
    td::BYTE _toolTipAreaHight = 0;   ///< Pixel height of the tooltip area.
    static td::BYTE _margin;          ///< Global margin around the popover content.
    td::BYTE _spaceBetweenItems;      ///< Spacing in pixels between adjacent items.
    td::BYTE _styleOnParent : 1;           ///< Flag: draw current selection style on parent button.
    td::BYTE _sendClickMessageAlways: 1;   ///< Flag: always send a click message even if selection did not change.
    td::BYTE _highlighCurrentSelection: 1; ///< Flag: visually highlight the current selection.
    td::BYTE _drawOnPOButton : 1;          ///< Flag: draw the current item preview on the popover button.
    td::BYTE _needsSizeCalculation : 1;    ///< Flag: cell sizes need to be recalculated.
    td::BYTE _transferLabelToPOButton : 1; ///< Flag: transfer the selected item label to the button label.
    td::BYTE _onToolBar : 1;               ///< Flag: this popover is hosted inside a toolbar.
    td::BYTE _keepSymboWHRatio : 1;        ///< Flag: maintain the symbol width-to-height ratio.
    td::BYTE _disabled : 1;                ///< Flag: the popover is disabled.
private:
    /// @brief Draws the currently selected item on the associated popover button (by size).
    virtual void drawCurrentItemOnPOButton(const Size& sz) = 0;
    /// @brief Draws the currently selected item on the associated popover button (by rect).
    virtual void drawCurrentItemOnPOButton(const Rect& r);

protected:
    /// @brief Creates the underlying native canvas for the popover.
    void createCanvas();

    /// @brief Handles draw requests for the popover canvas.
    /// @param rect Rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Handles primary mouse button press to select an item.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override final;

    /// @brief Handles cursor movement to update the hover highlight.
    void onCursorMoved(const gui::InputDevice& inputDevice) override final;

    /// @brief Handles cursor exit to clear the hover highlight.
    void onCursorExited(const gui::InputDevice& inputDevice) override final;

    /// @brief Calculates the tool (button) size required to show the current item.
    /// @param sz Output size in pixels.
    void getToolSize(gui::Size& sz);

    /// @brief Calculates the full size needed for the popover window.
    /// @param sz Output size in pixels.
    void getPopoverSize(Size& sz);

    /// @brief Returns the number of items in the popover.
    /// @return Item count.
    td::UINT2 getPopoverNumberOfItems() const;

    /// @brief Determines which item is under the given view-coordinate point.
    /// @param viewPoint Point in view coordinates.
    /// @return Index of the item under the point, or an invalid index if none.
    td::UINT2 getHoverSelection(const gui::Point& viewPoint) const;

    /// @brief Visually highlights or removes highlight from the current selection.
    /// @param bHighlight True to highlight, false to remove highlight.
    void highlightCurrentSelection(bool bHighlight);

    /// @brief Calculates and caches cell sizes. Override to customise.
    virtual void calcCellSize();

    /// @brief Returns the label string for a given item.
    /// @param itemPos Zero-based item index.
    /// @return Const reference to the label string.
    virtual const td::String& getPopoverLabel(td::UINT2 itemPos) const = 0;

    /// @brief Returns the tooltip string for a given item.
    /// @param itemPos Zero-based item index.
    /// @return Const reference to the tooltip string.
    virtual const td::String& getPopoverToolTip(td::UINT2 itemPos) const = 0;

    /// @brief Draws a single item inside the provided rectangle.
    /// @param itemPos Zero-based item index.
    /// @param r Rectangle in which to draw the item.
    virtual void drawItem(td::UINT2 itemPos, const gui::Rect& r) = 0;

    /// @brief Draws the tooltip text for a given item inside the provided rectangle.
    /// @param itemPos Zero-based item index.
    /// @param r Rectangle in which to draw the tooltip text.
    virtual void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r);

    /// @brief Sets how many rows are reserved for tooltip text display.
    /// @param nToolTipRows Number of tooltip text rows.
    void setNumberOfToolTipRows(td::BYTE nToolTipRows);

    /// @brief Indicates whether this popover canvas can be resized.
    /// @return True if resizable.
    virtual bool isResizible() const;

    /// @brief Returns the minimum width required for the popover button.
    /// @return Required button width in pixels.
    virtual td::UINT2 getRequiredButtonWidth() const;

protected:
    /// @brief Default constructor for derived classes.
    PopoverCanvas() {};
    PopoverCanvas(const PopoverCanvas&) = delete;
    PopoverCanvas& operator =(const PopoverCanvas&) = delete;
public:
    /// @brief Constructs a PopoverCanvas with the specified grid layout and display parameters.
    /// @param type Display type (symbol only or symbol with text).
    /// @param nItems Total number of items.
    /// @param nCols Number of columns in the grid.
    /// @param cellWidth Width of each cell in pixels.
    /// @param cellHeight Height of each cell in pixels.
    /// @param numberOfToolTipRows Number of rows for tooltip text.
    /// @param spaceBetweenItems Pixel spacing between items.
    PopoverCanvas(Type type, td::UINT2 nItems, td::BYTE nCols, td::UINT2 cellWidth, td::UINT2 cellHeight, td::BYTE numberOfToolTipRows, td::BYTE spaceBetweenItems = 2);

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::PopoverCanvas.
    gui::ObjType getObjType() const override { return ObjType::PopoverCanvas;}

    /// @brief Controls whether the popover draws its style on the parent button.
    /// @param bStyleOnParent True to draw on parent, false otherwise.
    void styleOnParent(bool bStyleOnParent);

    /// @brief Associates this canvas with a popover button.
    /// @param pIBtn Pointer to the IPopoverButton interface.
    void setPopoverButton(gui::IPopoverButton* pIBtn);

    /// @brief Returns the associated popover button interface.
    /// @return Pointer to IPopoverButton.
    gui::IPopoverButton* getPopoverButton();

    /// @brief Programmatically sets the current selection.
    /// @param currentSelection Zero-based index of the item to select.
    void setCurrentSelection(td::UINT2 currentSelection);

    /// @brief Returns the index of the currently selected item.
    /// @return Zero-based index of the current selection.
    td::UINT2 getCurrentSelection() const;

    /// @brief Sets the location where the popover appears relative to its button.
    /// @param location Desired popover location.
    void setLocation(Popover::Location location);

    /// @brief Returns the current popover location setting.
    /// @return Current Popover::Location value.
    Popover::Location getLocation() const;

    /// @brief Controls whether the selected item's label is transferred to the popover button.
    /// @param bTransferLabelToPOButton True to transfer the label.
    void transferLabelToButton(bool bTransferLabelToPOButton);

    /// @brief Controls whether the symbol's width-to-height ratio is preserved.
    /// @param bKeepWHRatio True to maintain the aspect ratio.
    void keepSymboWHRatio(bool bKeepWHRatio);

    /// @brief Attaches an IPopoverUpdater that can dynamically modify item states.
    /// @param pUpdater Pointer to the updater.
    void setUpdater(IPopoverUpdater* pUpdater);

    /// @brief Returns the associated IPopoverUpdater.
    /// @return Pointer to the updater, or nullptr if none.
    IPopoverUpdater* getUpdater();

    // void setFrameToFocusAfterClose(gui::Frame* pFrameToFocus)
    // {
    //     _frameToFocusAfterClose = pFrameToFocus;
    // }

    /// @brief Disables or enables the entire popover.
    /// @param bDisable True to disable, false to enable.
    void disable(bool bDisable);

    /// @brief Returns whether the popover is currently enabled.
    /// @return True if enabled.
    bool isEnabled() const;

    /// @brief Returns whether the popover is currently disabled.
    /// @return True if disabled.
    bool isDisabled() const;

    //IPopoverUpdater should call these methods
    /// @brief Marks an item as checked or unchecked.
    /// @param iPos Zero-based item index.
    /// @param bChecked True to check the item.
    virtual void setChecked(size_t iPos, bool bChecked);

    /// @brief Marks an item as disabled or enabled.
    /// @param iPos Zero-based item index.
    /// @param bDisabled True to disable the item.
    virtual void setDisabled(size_t iPos, bool bDisabled);
};

} //namespace gui
