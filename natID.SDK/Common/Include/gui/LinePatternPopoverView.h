// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LinePatternPopoverView.h
    @brief Popover canvas that displays and draws selectable line pattern options. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PopoverCanvas.h>
//#include <gui/Image.h>
//#include <gui/Symbol.h>
#include <cnt/Array.h>
#include <gui/Shape.h>
//#include <gui/DrawableString.h>

namespace gui
{

/// @brief Popover canvas view that presents all available line patterns for user selection.
class NATGUI_API LinePatternPopoverView : public PopoverCanvas
{
protected:
    cnt::Array<td::String, (size_t) td::LinePattern::NA> _labels; ///< Display labels for each line pattern option.
    Shape _line;          ///< Reusable shape used to render the line preview in each cell.
    td::UINT2 _maxLblLen; ///< Maximum label length in pixels, used for layout calculations.
private:
    /// @brief Draws the currently selected pattern onto the parent popover button.
    /// @param sz The size of the button area to draw into.
    void drawCurrentItemOnPOButton(const Size& sz) override;
protected:

    /// @brief Returns the display label for a given item position.
    /// @param itemPos Zero-based index of the line pattern item.
    /// @return Const reference to the label string for that item.
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;

    /// @brief Returns the tooltip text for a given item position.
    /// @param itemPos Zero-based index of the line pattern item.
    /// @return Const reference to the tooltip string for that item.
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;

    /// @brief Draws the visual representation of the line pattern at the specified cell.
    /// @param itemPos Zero-based index of the item to draw.
    /// @param r Rectangle defining the cell area in which to draw.
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;

    /// @brief Draws the tooltip text for the line pattern at the specified cell.
    /// @param itemPos Zero-based index of the item whose tooltip text is drawn.
    /// @param r Rectangle defining the tooltip text area.
    void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r) override;

    LinePatternPopoverView(const LinePatternPopoverView&) = delete;
    LinePatternPopoverView& operator =(const LinePatternPopoverView&) = delete;
public:
    /// @brief Constructs the LinePatternPopoverView and initialises all pattern labels.
    LinePatternPopoverView();
};

} //namespace gui
