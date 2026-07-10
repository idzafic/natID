// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CellInfo.h
 @brief Stores layout metadata for a single cell in a GUI grid, including sizing, alignment, and span settings. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <gui/natGUI.h>
#include <gui/Types.h>
#include <tuple>

namespace gui
{

/// @brief Holds sizing, alignment, and span information for a GUI layout cell.
class NATGUI_API CellInfo
{
public:
    td::UINT2 minHor = 0;     ///< Minimum horizontal size of the cell in pixels.
    td::UINT2 minVer = 0;     ///< Minimum vertical size of the cell in pixels.
    td::BYTE nResHor = 0;     ///< Number of horizontal resize units allocated to this cell.
    td::BYTE nResVer = 0;     ///< Number of vertical resize units allocated to this cell.
    td::BYTE hAlign : 2;      ///< Horizontal alignment of the cell content.
    td::BYTE vAlign : 2;      ///< Vertical alignment of the cell content.
    td::BYTE _rowSpan : 1;    //set if cell is spanned over several rows
    td::BYTE _colSpan : 1;    //set if cell is spanned over several cols
    //td::BYTE spanRedistributed: 1; //set if initial redistribution was done
    td::BYTE frozen : 1;            ///< Indicates whether the cell is frozen (non-resizable).
    td::BYTE keepAspectRatio : 1;   ///< Indicates whether the cell should maintain its aspect ratio when resized.
//    td::BYTE handledInitialAppearance : 1;
    //td::BYTE autoResize: 1; //

    //td::BYTE fixedHeightByOtherRows : 1; //mozda za ImageView
public:
    /// @brief Resets all cell info fields to their default values.
    void clean();
    /// @brief Copies layout properties from another CellInfo instance.
    /// @param ci The source CellInfo to copy from.
    void operator = (const CellInfo& ci);
    /// @brief Checks whether the cell is currently visible.
    /// @return True if the cell is visible, false otherwise.
    bool isVisible() const;
    /// @brief Sets the row and column span flags for this cell.
    /// @param rowSpan Non-zero if the cell spans multiple rows.
    /// @param colSpan Non-zero if the cell spans multiple columns.
    /// @param redistributed Non-zero if initial span redistribution has already been performed.
    void setSpanInfo(td::BYTE rowSpan=0, td::BYTE colSpan=0, td::BYTE redistributed=0);
    /// @brief Sets the horizontal and vertical alignment of the cell content.
    /// @param hAlign The desired horizontal alignment.
    /// @param vAlign The desired vertical alignment.
    void setAlignment(td::HAlignment hAlign, td::VAlignment vAlign);

    /// @brief Returns the horizontal alignment of the cell content.
    /// @return The horizontal alignment value.
    td::HAlignment getHAlignment() const;
    /// @brief Returns the vertical alignment of the cell content.
    /// @return The vertical alignment value.
    td::VAlignment getVAlignment() const;
    /// @brief Returns both horizontal and vertical alignment as a tuple.
    /// @return A tuple containing horizontal and vertical alignment values.
    std::tuple<td::HAlignment, td::VAlignment> getAlignment() const;
    /// @brief Checks whether this cell spans multiple rows.
    /// @return True if the cell has a row span, false otherwise.
    bool isRowSpan() const;
    /// @brief Checks whether this cell spans multiple columns.
    /// @return True if the cell has a column span, false otherwise.
    bool isColSpan() const;
    /// @brief Checks whether the cell can be resized in both horizontal and vertical directions.
    /// @return True if the cell is resizable in all directions, false otherwise.
    bool isResizableInAllDirections() const;
};

} // namespace gui
