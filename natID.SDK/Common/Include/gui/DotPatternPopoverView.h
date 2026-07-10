// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DotPatternPopoverView.h
 * @brief Popover canvas that renders selectable dot-pattern items.
 */
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
#include <td/DotPattern.h>
//#include <gui/DrawableString.h>

namespace gui
{

/// @brief Popover canvas that displays all available dot patterns for selection.
class NATGUI_API DotPatternPopoverView : public PopoverCanvas
{
protected:
    cnt::Array<td::String, (size_t) td::DotPattern::NA> _labels; ///< Display labels for each dot-pattern item.
    cnt::Array<Shape, (size_t) td::DotPattern::NA> _shapes;      ///< Prebuilt shapes used to render each dot-pattern item.
    td::UINT2 _maxLblLen; ///< Maximum label length (in characters) among all items.
private:
    /// @brief Draws the currently selected item onto the associated popover button.
    /// @param sz Size of the button area to draw into.
    void drawCurrentItemOnPOButton(const Size& sz) override;
protected:

    /// @brief Returns the display label for the item at the given position.
    /// @param itemPos Zero-based index of the item.
    /// @return Reference to the item's label string.
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;

    /// @brief Returns the tooltip text for the item at the given position.
    /// @param itemPos Zero-based index of the item.
    /// @return Reference to the item's tooltip string.
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;

    //virtual std::tuple<const td::String&, const td::String&> getPopoverLabelAndToolTip(td::UINT2 itemPos) const;

    /// @brief Draws a single item inside its allocated rectangle.
    /// @param itemPos Zero-based index of the item.
    /// @param r       Bounding rectangle for the item.
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;

    /// @brief Draws the tooltip text for an item inside its allocated rectangle.
    /// @param itemPos Zero-based index of the item.
    /// @param r       Bounding rectangle for the tooltip area.
    void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r) override;

    /// @brief Indicates whether the popover view can be resized by the user.
    /// @return True if the view is resizable.
    bool isResizible() const override;

    /// @brief Returns the minimum button width required to display the current selection.
    /// @return Required button width in pixels.
    td::UINT2 getRequiredButtonWidth() const override;

    DotPatternPopoverView(const DotPatternPopoverView&) = delete;
    DotPatternPopoverView& operator =(const DotPatternPopoverView&) = delete;
public:
    /// @brief Constructs a DotPatternPopoverView and initialises all pattern items.
    DotPatternPopoverView();
};

} //namespace gui
