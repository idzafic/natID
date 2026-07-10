// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DataViewHeader.h
 * @brief Canvas that renders the column header row of a DataView control.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <td/String.h>
#include <gui/Canvas.h>
#include <cnt/PushBackVector.h>
#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/Columns.h>

namespace gui
{

class DataView;
class LineNumbers;

/// @brief Draws the header row of a DataView and manages column resizing via drag interaction.
class NATGUI_API DataViewHeader : public Canvas
{
    friend class DataView;
    friend class LineNumbers;

protected:
    //const cnt::SafeFullVector<gui::Column>& _columns;

    DataView* _pDataView;       ///< Pointer to the owning DataView control.
    gui::Size _size;            ///< Current size of the header canvas.
    gui::CoordType _dx = 0;     ///< Horizontal scroll offset applied to the header.
private:
    gui::Point _startDrag;      ///< Cursor position recorded when a column-resize drag begins.
    int _toolTipIndex = -1;     ///< Index of the column whose tooltip is currently shown, or -1.
public:
    static int spaceBetweenCols; ///< Pixel gap rendered between adjacent column headers.
private:
    static td::ColorID txtColor; ///< Color used to render column header text.
    bool _inResizing = false;    ///< True while the user is actively dragging a column separator.
private:
    /// @brief Returns the index of the column currently under the cursor.
    /// @param x Horizontal cursor coordinate in view space.
    /// @return Column index, or -1 if none.
    int getHoveringColumn(gui::CoordType x) const;
    /// @brief Returns the index of the column separator nearest the cursor.
    /// @param x Horizontal cursor coordinate in view space.
    /// @return Separator index, or -1 if none.
    int getSeparator(gui::CoordType x) const;
protected:
    //void sendSelChangeMessage();
    /// @brief Measures the preferred height of the header canvas.
    /// @param cell Cell info structure to populate with size constraints.
    void measure(CellInfo&) override;
    /// @brief Re-measures the header canvas after layout changes.
    /// @param cell Cell info structure to update.
    void reMeasure(CellInfo&) override;

    /// @brief Handles resize events and updates the internal size state.
    /// @param newSize The new size of the canvas.
    void onResize(const gui::Size& newSize) override;

    /// @brief Returns the current size of the header canvas.
    /// @return Reference to the internal size object.
    const Size& getSize() const;

    /// @brief Draws all column headers within the given dirty rectangle.
    /// @param rect The rectangle region that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Handles a primary (left) button press to begin column resize or sorting.
    /// @param inputDevice Input device state at the time of the press.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    //void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    /// @brief Handles cursor drag to resize the column under the separator.
    /// @param inputDevice Input device state during the drag.
    void onCursorDragged(const gui::InputDevice& inputDevice) override;

    /// @brief Called when the cursor enters the header canvas.
    /// @param inputDevice Input device state at entry.
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the cursor moves within the header canvas.
    /// @param inputDevice Input device state during movement.
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
    /// @brief Called when the cursor exits the header canvas.
    /// @param inputDevice Input device state at exit.
    void onCursorExited(const gui::InputDevice& inputDevice) override;

    //void setContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    //void forwardContextMenuEventsTo(int pos, Frame* pFrame);
    //void setNonRemovable(int viewPos, bool nonRemovable);
    /// @brief Called when the system color mode (light/dark) changes.
    /// @param bDarkMode True if the new mode is dark mode.
    void systemColorModeChanged(bool bDarkMode) override;

    /// @brief Updates the horizontal scroll offset of the header to match the body.
    /// @param dx New horizontal scroll offset in pixels.
    void setScrollOffset(gui::CoordType dx);
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    /// @brief Constructs a DataViewHeader attached to the given DataView.
    /// @param pDataView Pointer to the owning DataView.
    DataViewHeader(DataView* pDataView);
    /// @brief Destructor.
    ~DataViewHeader();
};

} //namespace gui
