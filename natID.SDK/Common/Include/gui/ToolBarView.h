// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ToolBarView.h
    @brief View that hosts and renders a toolbar's controls using a ToolBarLayout. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBarItem.h>
#include <gui/ToolBarLayout.h>
#include <gui/View.h>

namespace gui
{

class ToolBar;
class MenuButton;
class ToolBarViewHelper;

/// @brief View responsible for displaying and managing toolbar controls with optional labels.
class NATGUI_API ToolBarView : public View
{
    friend class ToolBarViewHelper;
protected:
    ToolBarLayout _layout;            ///< Grid layout that positions toolbar control/label pairs.
    ToolBar* _pToolBar;               ///< Pointer to the logical ToolBar this view represents.
    MenuButton* _pMenuButton = nullptr; ///< Optional overflow menu button shown when controls do not fit.
    td::UINT2 _fullWidth = 0;         ///< Total pixel width required to display all toolbar items.
    td::BYTE _buttonSize = 24;        ///< Size in pixels used for toolbar button icons.
    td::BYTE _labelSize = 0;          ///< Height in pixels of the label row; 0 when labels are hidden.
    td::BYTE _fullHeight;             ///< Total pixel height of the toolbar view including labels.
protected:
    /// @brief Adjusts the layout when the view is resized.
    /// @param newSize New size of the view.
    void adjustLayout(const Size& newSize) override; //called on every resize
    /// @brief Performs the initial size measurement for the toolbar view.
    /// @return True if measurement succeeded.
    bool initialMeasure() override;
    /// @brief Populates the layout with controls from the associated ToolBar.
    void composeContent();

    ToolBarView() = delete;
    ToolBarView(const ToolBarView&) = delete;
    ToolBarView& operator =(const ToolBarView&) = delete;
public:
    /// @brief Constructs a ToolBarView for the given ToolBar.
    /// @param pToolBar Pointer to the ToolBar whose items this view will display.
    /// @param nCols Number of columns to allocate in the layout.
    /// @param menuID Resource ID of the overflow menu, or -1 if no overflow menu is needed.
    /// @param menuButtonTT Tooltip text for the overflow menu button.
    ToolBarView(ToolBar* pToolBar, td::BYTE nCols, int menuID = -1, const td::String& menuButtonTT = "");
    /// @brief Destructor.
    ~ToolBarView();
    /// @brief Returns the GUI object type identifier for this view.
    /// @return ObjType::ToolBarView.
    gui::ObjType getObjType() const override { return ObjType::ToolBarView;}
//    void reserve(td::BYTE nCols);
//    void setPair(td::BYTE iCol, ToolBarLayout::Pair& tbPair);
    /// @brief Inserts a toolbar item's control into the specified column.
    /// @param iCol Zero-based column index at which to place the item.
    /// @param item The toolbar item descriptor containing the control and metadata.
    /// @param showLabels True to display the item's text label beneath the control.
    /// @return Pointer to the inserted Control, or nullptr on failure.
    Control* insertItem(td::BYTE iCol, const gui::ToolBarItem& item, bool showLabels);

    /// @brief Returns whether the toolbar view has been measured.
    /// @return True if initial measurement has been completed.
    bool isMeasured() const;

    /// @brief Returns the total pixel width required for all toolbar items.
    /// @return Full width in pixels.
    int getFullWidth() const;

    /// @brief Returns the current pixel height of the toolbar view.
    /// @return Current height in pixels.
    int getCurrentHeight() const;

    /// @brief Sets the spacing between toolbar cells.
    /// @param spaceBetweeRows Pixel spacing between rows.
    /// @param spaceBetweenCols Pixel spacing between columns.
    void setSpaceBetweenCells(td::BYTE spaceBetweeRows, td::BYTE spaceBetweenCols);
    /// @brief Shows or hides text labels for all toolbar items.
    /// @param bShow True to show labels, false to hide them.
    void showLabels(bool bShow);
    /// @brief Updates the visible text label for a specific toolbar item.
    /// @param pTBI Pointer to the toolbar item whose label should be updated.
    /// @param strTxt New label text.
    void updateLabel(const gui::ToolBarItem* pTBI, const td::String& strTxt);
    /// @brief Updates the tooltip text for a specific toolbar item.
    /// @param pTBI Pointer to the toolbar item whose tooltip should be updated.
    /// @param strTxt New tooltip text.
    void updateToolTip(const gui::ToolBarItem* pTBI, const td::String& strTxt);
};

} //namespace gui
