// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyGroupCell.h
 *  @brief Collapsible group header cell used in property views. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Canvas.h>
#include <gui/DrawableString.h>

namespace gui
{

/// @brief Canvas-based cell that displays a collapsible property group with a label and expand/collapse arrow.
class NATGUI_API PropertyGroupCell : public Canvas
{
public:
    /// @brief Expansion state of the property group cell.
    enum class Status : unsigned char {Expanded=0, ///< Group is expanded and children are visible.
                                       Collapsed    ///< Group is collapsed and children are hidden.
                                      };
private:
protected:
    DrawableString _label;         ///< Text label displayed in the group header.
    Size _contentSize;             ///< Cached size of the cell content area.
    td::UINT2 _width;              ///< Width of the cell in pixels.
    td::UINT2 _height;             ///< Height of the cell in pixels.
    Status _status = Status::Expanded; ///< Current expand/collapse status.
    td::BYTE _hover = 0;           ///< Non-zero when the cursor is hovering over the cell.
protected:
    /// @brief Updates the background appearance based on pointer hover state.
    /// @param pointing True if the cursor is currently over the cell.
    void updateBackground(bool pointing);

    /// @brief Draws the cell content within the given rectangle.
    /// @param rect The dirty rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

//    virtual void onClick(const gui::Point& modelPoint, td::UINT4 keyModifiers);
//
//    virtual void onInputDeviceEntered(const gui::Point& modelPoint);
//
//    virtual void onInputDeviceExited(const gui::Point& modelPoint);

    //virtual void onInputDeviceMoved(const gui::Point& modelPoint, td::UINT4 keyModifiers);

    /// @brief Handles the primary (left) button press to toggle expand/collapse.
    /// @param inputDevice The input device that generated the event.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor enters the cell area.
    /// @param inputDevice The input device that generated the event.
    void onCursorEntered(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor exits the cell area.
    /// @param inputDevice The input device that generated the event.
    void onCursorExited(const gui::InputDevice& inputDevice) override;

    /// @brief Measures the preferred size of the cell and fills in the CellInfo structure.
    /// @param cell Output cell layout information.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the cell size after content or state changes.
    /// @param cell Output cell layout information.
    void reMeasure(CellInfo& cell) override;

    /// @brief Default constructor (protected — use the labeled constructor from derived classes).
    PropertyGroupCell();
public:
    /// @brief Constructs a PropertyGroupCell with the given label text.
    /// @param label The text to display in the group header.
    PropertyGroupCell(const td::String& label);

    /// @brief Destructor.
    ~PropertyGroupCell();

    /// @brief Returns the current expand/collapse status.
    /// @return The current Status value.
    PropertyGroupCell::Status getStatus() const;
    //void setStatus(PropertyGroupCell::Status status);

    /// @brief Returns the width reserved for the expand/collapse arrow.
    /// @return Width of the arrow area in pixels.
    td::UINT2 getArrowWidth() const;
};

} //namespace gui
