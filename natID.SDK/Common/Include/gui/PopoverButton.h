// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PopoverButton.h @brief Declares the PopoverButton control that displays a popover canvas when activated. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/DataCtrl.h>
#include <gui/IPopoverButton.h>

namespace gui
{

class PopoverCanvas;

/// @brief A button control that opens an associated PopoverCanvas for item selection.
class NATGUI_API PopoverButton : public DataCtrl, public IPopoverButton
{
private:
    std::function<void()> _onChangedSelection; ///< Callback invoked when the selected item changes.
protected:
    PopoverCanvas* _popoverCanvas = nullptr; ///< The popover canvas displayed when the button is clicked.

protected:
    /// @brief Measures the preferred cell size for this button.
    /// @param ci Cell information structure to fill.
    void measure(CellInfo&) override;

    /// @brief Re-measures the cell size after layout changes.
    /// @param ci Cell information structure to fill.
    void reMeasure(CellInfo&) override;

    /// @brief Sets the current selection inside the popover.
    /// @param pos Zero-based position of the selected item.
    /// @param closePopover Whether to close the popover after selection.
    void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) override;

    /// @brief Sends a message notification for the popover selection change.
    void sendPopoverMessage() override;

    /// @brief Closes the popover if it is open.
    void closePopover() override;

    /// @brief Internal constructor used by derived classes specifying a data type.
    /// @param dt Data type carried by this control.
    PopoverButton(td::DataType dt);
    PopoverButton(const PopoverButton&) = delete;
    PopoverButton& operator =(const PopoverButton&) = delete;
public:
    /// @brief Constructs a PopoverButton with an associated popover canvas.
    /// @param popoverView Pointer to the PopoverCanvas that will be shown.
    PopoverButton(PopoverCanvas* popoverView);

    /// @brief Destructor.
    ~PopoverButton();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::PopoverButton.
    gui::ObjType getObjType() const override { return ObjType::PopoverButton;}

    /// @brief Returns a pointer to the associated popover canvas.
    /// @return Pointer to the PopoverCanvas.
    PopoverCanvas* getPopoverCanvas();

    /// @brief Returns the index of the currently selected item.
    /// @return Zero-based index of the current selection.
    td::UINT2 getCurrentSelection() const;

    /// @brief Enables or disables the popover interaction.
    /// @param bEnable True to enable, false to disable.
    void enablePopover(bool bEnable) override;

    /// @brief Returns the registered selection-change callback.
    /// @return Const reference to the selection-change handler function.
    const std::function<void()>& getChangedSelectionHandler() const;

    /// @brief Registers a callback to be called when the selection changes.
    /// @param fnToCall Function to invoke on selection change.
    void onChangedSelection(const std::function<void()>& fnToCall);
};

} //namespace gui
