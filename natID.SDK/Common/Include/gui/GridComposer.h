// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file GridComposer.h
    @brief Helper class for composing controls into a GridLayout sequentially. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "GridLayout.h"

namespace gui
{

/// @brief Utility class that simplifies populating a GridLayout by appending controls row by row or column by column.
class NATGUI_API GridComposer
{
private:
    GridLayout& _grid; ///< Reference to the grid layout being composed.
    int _iRow; ///< Current row index in the grid.
    int _iCol; ///< Current column index in the grid.

    GridComposer(const GridComposer&) = delete;
    GridComposer& operator =(const GridComposer&) = delete;
    GridComposer() = delete;
public:
    /// @brief Constructs a GridComposer for the given GridLayout.
    /// @param grid The grid layout to compose into.
    GridComposer(GridLayout& grid);

    /// @brief Appends a control to the next available row.
    /// @param ctrl The control to append.
    /// @param hAlign Horizontal alignment of the control within its cell.
    /// @param vAlign Vertical alignment of the control within its cell.
    /// @return Reference to this composer for chaining.
    GridComposer& appendRow(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);

    /// @brief Appends a control to the next row with specified column and row span.
    /// @param ctrl The control to append.
    /// @param colSpan Number of columns the control should span.
    /// @param hAlign Horizontal alignment of the control within its cell.
    /// @param rowSpan Number of rows the control should span.
    /// @param vAlign Vertical alignment of the control within its cell.
    /// @return Reference to this composer for chaining.
    GridComposer& appendRow(Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);

    /// @brief Starts a new row with the specified horizontal and vertical spacing.
    /// @param horizontalSpace Horizontal space to insert before the new row.
    /// @param verticalSpace Vertical space to insert before the new row.
    /// @return Reference to this composer for chaining.
    GridComposer& startNewRowWithSpace(td::UINT2 horizontalSpace, td::UINT2 verticalSpace);

    /// @brief Appends a control to the next available column in the current row.
    /// @param ctrl The control to append.
    /// @param hAlign Horizontal alignment of the control within its cell.
    /// @param vAlign Vertical alignment of the control within its cell.
    /// @return Reference to this composer for chaining.
    GridComposer& appendCol(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Center);

    /// @brief Appends a number of empty columns to the current row.
    /// @param nEmptyCols Number of empty columns to append.
    /// @return Reference to this composer for chaining.
    GridComposer& appendEmptyCols(size_t nEmptyCols);

    /// @brief Appends a control to the current row with specified column and row span.
    /// @param ctrl The control to append.
    /// @param colSpan Number of columns the control should span.
    /// @param hAlign Horizontal alignment of the control within its cell.
    /// @param rowSpan Number of rows the control should span.
    /// @param vAlign Vertical alignment of the control within its cell.
    /// @return Reference to this composer for chaining.
    GridComposer& appendCol(Control& ctrl, td::BYTE colSpan, td::HAlignment hAlign = td::HAlignment::Left, td::BYTE rowSpan=1, td::VAlignment vAlign = td::VAlignment::Center);

    /// @brief Appends a fixed-size space cell to the grid.
    /// @param horizontalSpace Width of the space in pixels.
    /// @param verticalSpace Height of the space in pixels.
    /// @return Reference to this composer for chaining.
    GridComposer& appendSpace(td::UINT2 horizontalSpace, td::UINT2 verticalSpace);

    /// @brief Appends a flexible spacer cell to the grid.
    /// @param horizontalSpace Base horizontal space size.
    /// @param nHor Number of horizontal spacer units.
    /// @param verticalSpace Base vertical space size.
    /// @param nVert Number of vertical spacer units.
    /// @return Reference to this composer for chaining.
    GridComposer& appendSpacer(td::UINT2 horizontalSpace, td::BYTE nHor, td::UINT2 verticalSpace = 0, td::BYTE nVert = 0);

    /// @brief Stream operator to append a control to the next column.
    /// @param ctrl The control to append.
    /// @return Reference to this composer for chaining.
    GridComposer& operator << (Control& ctrl);

};

} //namespace gui
