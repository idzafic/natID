// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SplitterCell.h
 *  @brief Interactive splitter handle cell that can minimize and restore a panel. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/ISwitch.h>

namespace gui
{

class SplitterLayout;

/// @brief Canvas cell used as a draggable splitter handle that supports minimize/restore of adjacent panels.
class NATGUI_API SplitterCell : public Canvas
{
    friend class SplitterLayout;
public:
    /// @brief Panel state controlled by the splitter.
    enum class Status : unsigned char {Minimized=0, ///< Adjacent panel is hidden/minimized.
                                       Normal        ///< Adjacent panel is at its normal size.
                                      };
private:
    /// @brief Initialises the graphical symbols used for the minimized indicator.
    void initSymbols();
protected:
    SplitterLayout* _pSplitter = nullptr;       ///< Pointer to the owning SplitterLayout.
    NatString _toolTipStr = nullptr;            ///< Tooltip text shown when the panel is minimized.
    ISwitchState* _pSwitchStateUpdater = nullptr; ///< Optional external state observer.
    Shape _minimizedArea;                       ///< Background shape drawn when minimized.
    //Path _minimizedBorder; //border
    Shape _minizedSymbol1;                      ///< First symbol drawn in the minimized state.
    Shape _minizedSymbol2;                      ///< Second symbol drawn in the minimized state.

    Status _status = Status::Normal;            ///< Current minimize/normal status.
    //td::BYTE _dragOperation = 0;
private:
    /// @brief Shows or hides the tooltip depending on the current state.
    /// @param bSet True to show the tooltip, false to hide it.
    void setToolTip(bool bSet);
protected:
    /// @brief Updates the mouse cursor to reflect the splitter interaction mode.
    void setCursor() const;

    /// @brief Draws the splitter cell within the given rectangle.
    /// @param rect The dirty rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Toggles the panel between minimized and normal state.
    void toggle();

    /// @brief Handles the primary button press to begin a drag or toggle.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Handles the primary button release to finish a drag.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
//    virtual void onCursorEntered(const gui::InputDevice& inputDevice);

    /// @brief Handles cursor drag events to resize the adjacent panel.
    /// @param inputDevice The input device that generated the event.
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
//    virtual void onCursorExited(const gui::InputDevice& inputDevice);

    /// @brief Registers an external object that is notified when the switch state changes.
    /// @param pUpdater Pointer to the state updater.
    void setUpdater(ISwitchState* pUpdater);

public:
    /// @brief Constructs a SplitterCell associated with the given SplitterLayout.
    /// @param pSplitter Pointer to the owning SplitterLayout.
    SplitterCell(SplitterLayout* pSplitter);

    /// @brief Destructor.
    ~SplitterCell();

    /// @brief Returns the object type identifier for this cell.
    /// @return ObjType::SplitterCell.
    gui::ObjType getObjType() const override { return ObjType::SplitterCell;}

    /// @brief Measures the preferred size of the splitter handle.
    /// @param cell Output cell layout information.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the splitter handle after layout changes.
    /// @param cell Output cell layout information.
    void reMeasure(CellInfo& cell) override;

    /// @brief Returns the current minimize/normal status.
    /// @return The current Status value.
    SplitterCell::Status getStatus() const;

    /// @brief Sets the minimize/normal status programmatically.
    /// @param status The desired Status value.
    void setStatus(SplitterCell::Status status);

    /// @brief Sets the tooltip string shown when the panel is minimized.
    /// @param str The tooltip text.
    void setToolTipWhenMinimized(const td::String& str);
};

} //namespace gui
