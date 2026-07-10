// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ColorPopoverView.h
    @brief A popover canvas view that renders a color palette for selection by a ColorPicker. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PopoverCanvas.h>

namespace gui
{

class ColorPicker;

/// @brief A canvas-based popover view that displays a color swatch grid for use with ColorPicker.
class NATGUI_API ColorPopoverView : public PopoverCanvas
{
private:
    /// @brief Draws the currently selected color on the popover button face using a size.
    /// @param sz The size of the button area.
    void drawCurrentItemOnPOButton(const Size& sz) override;
    /// @brief Draws the currently selected color on the popover button face using a rect.
    /// @param r The bounding rectangle of the button area.
    void drawCurrentItemOnPOButton(const Rect& r) override;
protected:
    /// @brief Returns the label string for a given color item position.
    /// @param itemPos The index of the item in the palette.
    /// @return Const reference to the label string.
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;
    /// @brief Returns the tooltip string for a given color item position.
    /// @param itemPos The index of the item in the palette.
    /// @return Const reference to the tooltip string.
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;

    /// @brief Draws the entire color palette canvas.
    /// @param rect The bounding rectangle of the canvas.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Draws a single color swatch at the given position.
    /// @param itemPos The index of the color item to draw.
    /// @param r The bounding rectangle for this swatch.
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;

    ColorPopoverView(const ColorPopoverView&) = delete;
    ColorPopoverView& operator =(const ColorPopoverView&) = delete;
public:
    /// @brief Default constructor.
    ColorPopoverView();
    /// @brief Destructor.
    ~ColorPopoverView();;
};

} //namespace gui
