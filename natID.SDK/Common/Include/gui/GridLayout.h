// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file GridLayout.h
    @brief Two-dimensional grid layout manager for arranging controls in rows and columns. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Layout.h"
#include <mtx/SimpleDense.h>
#include <gui/_aux/GridCell.h>


namespace gui
{

class ToolBarView;
typedef mtx::SimpleDense<GridCell, td::BYTE> Grid;

/// @brief Layout that positions controls in a fixed grid of rows and columns, supporting cell spanning.
class NATGUI_API GridLayout : public Layout
{
    friend class ToolBarView;
protected:
    Grid _grid; ///< Internal grid data structure holding cell information.
    td::BYTE _spaceBetweenRows; ///< Spacing in pixels between consecutive rows.
    td::BYTE _spaceBetweenCols; ///< Spacing in pixels between consecutive columns.
    td::BYTE _marginX; ///< Horizontal margin around the grid content.
    td::BYTE _marginY; ///< Vertical margin around the grid content.
private:
    /// @brief Updates the span properties of a grid cell.
    /// @param cell The grid cell to update.
    /// @param iRow Row index of the cell.
    /// @param iCol Column index of the cell.
    /// @param rowSpan Number of rows the cell spans.
    /// @param colSpan Number of columns the cell spans.
    /// @param redistribution Redistribution hint string.
    void updateSpan(GridCell& cell, td::BYTE iRow, td::BYTE iCol, td::BYTE rowSpan, td::BYTE colSpan,  const char* redistribution);
protected:
    /// @brief Initializes a grid cell with a control and layout properties.
    /// @param iRow Row index where the control is placed.
    /// @param iCol Column index where the control is placed.
    /// @param ctrl The control to place in the cell.
    /// @param rowSpan Number of rows the control spans.
    /// @param colSpan Number of columns the control spans.
    /// @param hAlign Horizontal alignment within the cell.
    /// @param vAlign Vertical alignment within the cell.
    void initialize(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::BYTE rowSpan, td::BYTE colSpan,  td::HAlignment hAlign, td::VAlignment vAlign);

    /// @brief Performs an initial measurement pass over all cells.
    /// @return True if measurement succeeded.
    bool initialMeasure() override;

    /// @brief Performs initial measurement for a specific cell.
    /// @param cell The cell info to measure.
    void initialMeasure(CellInfo&) override;

    /// @brief Measures the size requirements of a cell.
    /// @param cell The cell info to measure.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the size requirements of a cell after a change.
    /// @param cell The cell info to re-measure.
    void reMeasure(CellInfo& cell) override;

    /// @brief Applies geometry to the grid based on available space.
    /// @param geometry The available geometry for the layout.
    /// @param cell The cell within which the layout resides.
    void setGeometry(const Geometry& geometry, const Cell& cell) override;

    /// @brief Freezes the layout to prevent further modification.
    /// @return True if the layout was successfully frozen.
    bool freeze() override;

    /// @brief Unfreezes the layout to allow modification again.
    void unFreeze() override;

    /// @brief Handles action item events originating from controls in the grid.
    /// @param aiDesc Descriptor of the action item event.
    /// @return True if the event was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;

    GridLayout();
    GridLayout(const GridLayout&) = delete;
    GridLayout& operator =(const GridLayout&) = delete;
public:
    /// @brief Constructs a GridLayout with the specified number of rows and columns.
    /// @param nRows Number of rows in the grid.
    /// @param nCols Number of columns in the grid.
    GridLayout(td::BYTE nRows, td::BYTE nCols);

    /// @brief Returns the object type identifier for this layout.
    /// @return ObjType::GridLayout.
    virtual gui::ObjType getObjType() const override { return ObjType::GridLayout;}

    /// @brief Sets the horizontal and vertical margins around the grid content.
    /// @param marginX Horizontal margin in pixels.
    /// @param marginY Vertical margin in pixels.
    void setMargins(td::BYTE marginX, td::BYTE marginY);

    /// @brief Returns the current horizontal margin.
    /// @return Horizontal margin in pixels.
    td::BYTE getXMargin() const;

    /// @brief Returns the current vertical margin.
    /// @return Vertical margin in pixels.
    td::BYTE getYMargin() const;

    /// @brief Inserts a control at the specified grid position.
    /// @param iRow Row index for the control.
    /// @param iCol Column index for the control.
    /// @param ctrl The control to insert.
    /// @param hAlign Horizontal alignment within the cell.
    /// @param vAlign Vertical alignment within the cell.
    void insert(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);

//    void putSpanCtrlWithRedistribution(td::BYTE iRow, td::BYTE iCol, Control* pCtrl, td::BYTE rowSpan, td::BYTE colSpan,  const char* redistribution="*", HAlignment hAlign = HAlignment::Left, VAlignment vAlign = VAlignment::Center);

    /// @brief Inserts a control at the specified position with optional column and row span.
    /// @param iRow Row index for the control.
    /// @param iCol Column index for the control.
    /// @param ctrl The control to insert.
    /// @param colSpan Number of columns the control spans.
    /// @param hAlign Horizontal alignment within the cell.
    /// @param rowSpan Number of rows the control spans.
    /// @param vAlign Vertical alignment within the cell.
    void insert(td::BYTE iRow, td::BYTE iCol, Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);

    /// @brief Inserts a fixed-size space at the specified grid position.
    /// @param iRow Row index for the space.
    /// @param iCol Column index for the space.
    /// @param horizontalSpace Width of the space in pixels.
    /// @param verticalSpace Height of the space in pixels.
    void insertSpace(td::BYTE iRow, td::BYTE iCol, td::UINT2 horizontalSpace, td::UINT2 verticalSpace);

    /// @brief Inserts a flexible spacer at the specified grid position.
    /// @param iRow Row index for the spacer.
    /// @param iCol Column index for the spacer.
    /// @param horizontalSpace Base horizontal space size.
    /// @param horMultiplier Horizontal multiplier for flexible sizing.
    /// @param verticalSpace Base vertical space size.
    /// @param vertialMultiplier Vertical multiplier for flexible sizing.
    void insertSpacer(td::BYTE iRow, td::BYTE iCol, td::UINT2 horizontalSpace, td::BYTE horMultiplier, td::UINT2 verticalSpace = 0, td::BYTE vertialMultiplier = 0);

    /// @brief Populates a view with the grid's controls using the given cell and margin info.
    /// @param pView Pointer to the view to populate.
    /// @param cell Cell reference for positioning.
    /// @param szMargins Margin sizes to apply.
    void populateView(View* pView, const Cell& cell, const Size& szMargins) override;

    /// @brief Serializes the grid layout to a file stream.
    /// @param f Output file stream to write to.
    void serialize(std::ofstream& f) const;

    /// @brief Sets the spacing between rows and columns.
    /// @param spaceBetweenRows Space in pixels between consecutive rows.
    /// @param spaceBetweenColumns Space in pixels between consecutive columns.
    void setSpaceBetweenCells(td::BYTE spaceBetweenRows, td::BYTE spaceBetweenColumns);

    /// @brief Returns the current spacing between rows.
    /// @return Row spacing in pixels.
    td::BYTE getSpaceBetweenRows() const;

    /// @brief Returns the current spacing between columns.
    /// @return Column spacing in pixels.
    td::BYTE getSpaceBetweenColumns() const;
};

} //namespace gui
