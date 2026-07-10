// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ILineNumbers.h
    @brief Interface for a line-number gutter widget used alongside text editing views. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include <gui/Types.h>
#include <td/ColorID.h>

namespace gui
{

/// @brief Abstract interface for a line-number display component that synchronises with a text view.
class ILineNumbers
{
public:
    /// @brief Sets the total number of lines to display.
    /// @param totalNoOfLines Total line count in the associated document.
    /// @param forceRedraw If true, forces an immediate redraw even if the count has not changed.
    virtual void setTotalLines(td::UINT4 totalNoOfLines, bool forceRedraw = false) = 0;

    /// @brief Returns the total number of lines currently tracked.
    /// @return Total line count.
    virtual td::UINT4 getTotalLines() const = 0;

    /// @brief Sets the currently active (cursor) line number.
    /// @param lineNo One-based line number to mark as current.
    virtual void setCurrentLine(td::UINT4 lineNo) = 0;

    /// @brief Returns the currently active line number.
    /// @return One-based index of the current line.
    virtual td::UINT4 getCurrentLine() const = 0;

    /// @brief Updates the vertical scroll position to keep line numbers in sync with the text view.
    /// @param dy Vertical scroll offset in coordinate units.
    virtual void setScrollPos(CoordType dy) = 0;

    /// @brief Notifies the component that the font has changed and metrics need recalculating.
    virtual void fontChanged() = 0;

    /// @brief Returns the width offset that the line-number gutter occupies on the right side.
    /// @return Width of the right-side gutter in coordinate units.
    virtual gui::CoordType getOffsetOnRightSide() const = 0;

    /// @brief Returns the colour used to draw normal line numbers.
    /// @return Colour identifier for line numbers.
    virtual td::ColorID getColor() const = 0;

    /// @brief Returns the colour used to highlight the currently selected line number.
    /// @return Colour identifier for the selected line number.
    virtual td::ColorID getColorForSelectedLine() const = 0;
};

}  //namespace gui
