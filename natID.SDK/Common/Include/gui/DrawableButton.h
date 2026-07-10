// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableButton.h
 * @brief Base class for custom-drawn button controls.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Control.h>
#include <gui/IPopoverButton.h>
namespace gui
{
class PopoverView;
class DrawableButtonHelper;

/// @brief A button control whose visual appearance is rendered entirely through a virtual draw callback.
class NATGUI_API DrawableButton : public Control
{
    friend class DrawableButtonHelper;
private:
    std::function<void()> _onClick; ///< Callback invoked when the button is clicked.
protected:
    gui::Size _size; ///< Current size of the button.
protected:
    /// @brief Performs initial measurement to determine the preferred cell size.
    /// @param cellInfo Cell layout information to populate.
    void measure(CellInfo&) override;

    /// @brief Performs a re-measurement pass when the layout changes.
    /// @param cellInfo Cell layout information to update.
    void reMeasure(CellInfo&) override;

    /// @brief Called when the button is resized.
    /// @param newSize The new dimensions of the button.
    virtual void onResize(const gui::Size& newSize);

    /// @brief Called to render the button's custom appearance.
    /// @param rect The dirty rectangle that needs to be redrawn.
    virtual void onDraw(const gui::Rect& rect);

    DrawableButton(const DrawableButton&) = delete;
    DrawableButton& operator =(const DrawableButton&) = delete;

    /// @brief Internal constructor used by derived classes.
    /// @param unused Disambiguation tag (unused value).
    DrawableButton(int);

    /// @brief Default constructor for use by subclasses.
    DrawableButton();
public:
    /// @brief Destructor.
    ~DrawableButton();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::DrawableButton.
    gui::ObjType getObjType() const override { return ObjType::DrawableButton;}

    /// @brief Returns the associated popover canvas, if any.
    /// @return Pointer to the PopoverCanvas, or nullptr if not applicable.
    virtual PopoverCanvas* getPopoverCanvas();

    /// @brief Returns the associated popover button interface, if any.
    /// @return Pointer to the IPopoverButton interface, or nullptr if not applicable.
    virtual gui::IPopoverButton* getPopoverButton();

    /// @brief Returns the index of the currently selected item in the associated popover.
    /// @return Zero-based index of the current selection.
    virtual td::UINT2 getPopoverCurrentSelection();

    /// @brief Returns the registered click callback.
    /// @return Reference to the stored std::function click handler.
    const std::function<void()>& getClickHandler() const;

    /// @brief Registers a callback to be invoked when the button is clicked.
    /// @param fnToCall The function to call on click.
    void onClick(const std::function<void()>& fnToCall);
};

} //namespace gui
