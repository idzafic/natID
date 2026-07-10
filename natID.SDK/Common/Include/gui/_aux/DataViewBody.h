// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DataViewBody.h
 * @brief Scrollable canvas that renders the data rows of a DataView control.
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
#include <gui/ILineNumbers.h>
#include <dp/IDataSet.h>
#include <gui/Columns.h>


namespace gui
{
class DataView;
class LineNumbers;

/// @brief Renders the scrollable body area of a DataView, handling row drawing, selection, and keyboard navigation.
class NATGUI_API DataViewBody : public Canvas
{
    friend class DataView;
    friend class LineNumbers;

protected:
    DataView* _pDataView;                       ///< Pointer to the owning DataView control.
    gui::ILineNumbers* _pLineNumbers = nullptr; ///< Optional line-number sidebar attached to this body.
    dp::IDataSet* _pDS = nullptr;               ///< Data source providing the rows to display.
    //const cnt::SafeFullVector<gui::Column>& _columns;
    gui::Size _size;                            ///< Current size of the body canvas.
    int _selectedRow = -1;                      ///< Zero-based index of the currently selected row, or -1 if none.
    static int lineHeight;                      ///< Shared line height in pixels used for all rows.
    BoolStyle _boolStyle = BoolStyle::Text;     ///< Rendering style used for boolean column values.
private:
    static td::ColorID alternateRowColor;       ///< Background color applied to alternating rows.
private:
    /// @brief Formats a variant value for display in the given visible column.
    /// @param val The variant value to format.
    /// @param iVisCol Zero-based visible column index.
    /// @return Pointer to a null-terminated formatted string.
    const char* getFormattedValue(const td::Variant& val, int iVisCol) const;
    td::BYTE _inFocus = 0; ///< Non-zero when this canvas has keyboard focus.
protected:

    /// @brief Measures the preferred size of the body canvas.
    /// @param cell Cell info structure to populate with size constraints.
    void measure(CellInfo&) override;
    /// @brief Re-measures the body canvas after layout changes.
    /// @param cell Cell info structure to update.
    void reMeasure(CellInfo&) override;

    /// @brief Handles resize events and updates internal size state.
    /// @param newSize The new size of the canvas.
    void onResize(const gui::Size& newSize) override;

    /// @brief Returns the current size of the body canvas.
    /// @return Reference to the internal size object.
    const Size& getSize() const;
    /// @brief Calculates the logical (model) size required to display all data rows.
    /// @param modelSize Output parameter receiving the computed model size.
    /// @return True if the model size was successfully computed.
    bool getModelSize(gui::Size& modelSize) const override;

    /// @brief Responds to a full data reload by refreshing the display.
    void onReload();
    /// @brief Responds to incremental data updates by refreshing affected rows.
    void onUpdate();

    /// @brief Draws all visible rows within the given dirty rectangle.
    /// @param rect The rectangle region that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    /// @brief Handles a primary (left) button press to change row selection.
    /// @param inputDevice Input device state at the time of the press.
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    /// @brief Handles a secondary (right) button press to trigger context menus.
    /// @param inputDevice Input device state at the time of the press.
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;

    /// @brief Called when keyboard focus enters the body canvas.
    void onFocusIn() override;
    /// @brief Called when keyboard focus leaves the body canvas.
    void onFocusOut() override;
//    void onCursorDragged(const gui::InputDevice& inputDevice) override;
//
//    void onCursorEntered(const gui::InputDevice& inputDevice) override;
//    void onCursorMoved(const gui::InputDevice& inputDevice) override;
//    void onCursorExited(const gui::InputDevice& inputDevice) override;

    /// @brief Calculates the bounding rectangle for a specific row in model coordinates.
    /// @param r Output rectangle to receive the computed bounds.
    /// @param iRow Zero-based row index.
    void calcVisibleRect(gui::Rect& r, int iRow) const;
    /// @brief Handles keyboard navigation and editing key events.
    /// @param key The key event that was pressed.
    /// @return True if the key event was consumed, false otherwise.
    bool onKeyPressed(const gui::Key& key) override;

    /// @brief Assigns a context menu to a specific column position.
    /// @param pos Zero-based column position.
    /// @param contextMenuID Identifier of the context menu to assign.
    /// @param contextMenuGroup Optional group identifier for the context menu.
    void setContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    /// @brief Forwards context menu events from a column position to a frame handler.
    /// @param pos Zero-based column position.
    /// @param pFrame The frame that will handle the forwarded events.
    void forwardContextMenuEventsTo(int pos, Frame* pFrame);
    //void setNonRemovable(int viewPos, bool nonRemovable);
    /// @brief Called when the system color mode (light/dark) changes.
    /// @param bDarkMode True if the new mode is dark mode.
    void systemColorModeChanged(bool bDarkMode) override;
    /// @brief Returns whether this canvas currently holds keyboard focus.
    /// @return True if focused, false otherwise.
    bool hasFocus() const;

    //void selectRow(int iRow, bool bSendMessage);
    /// @brief Moves the selection to the previous row.
    void moveSelectionUp();
    /// @brief Moves the selection to the next row.
    void moveSelectionDown();
    /// @brief Moves the selection one page up.
    void moveSelectionPgUp();
    /// @brief Moves the selection one page down.
    void moveSelectionPgDown();
    /// @brief Moves the selection to the first row.
    void moveSelectionToFirst();
    /// @brief Moves the selection to the last row.
    void moveSelectionToLast();
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    /// @brief Constructs a DataViewBody attached to the given DataView.
    /// @param pDV Pointer to the owning DataView.
    DataViewBody(DataView* pDV);
    /// @brief Destructor. Cleans up resources owned by the body canvas.
    ~DataViewBody();

    /// @brief Attaches a line-number sidebar to this body.
    /// @param pLN Pointer to the ILineNumbers implementation.
    void setLineNumbers(gui::ILineNumbers* pLN);
    /// @brief Sets the data source used to populate the rows.
    /// @param pDS Pointer to the data set interface.
    void setDataSet(dp::IDataSet* pDS);
    /// @brief Returns the vertical coordinate of the first visible row and its line number.
    /// @param lineNo Output parameter receiving the one-based line number.
    /// @return Vertical coordinate of the first visible row.
    gui::CoordType getFirstVisibleRow(td::UINT4& lineNo) const;
    /// @brief Returns the total number of rows in the current data set.
    /// @return Row count.
    size_t getNumberOfRows() const;
    /// @brief Returns the height of a single row in pixels.
    /// @return Line height in pixels.
    int getLineHeight() const;
    /// @brief Sets the display style used for boolean cell values.
    /// @param boolStyle The desired BoolStyle.
    void setBoolStyle(BoolStyle boolStyle);
    /// @brief Returns the current display style for boolean cell values.
    /// @return The current BoolStyle.
    BoolStyle getBoolStale() const;
    /// @brief Returns the zero-based index of the first selected row.
    /// @return Selected row index, or -1 if no row is selected.
    int getFirstSelectedRow() const;
    /// @brief Selects a specific row, optionally notifying listeners and scrolling it into view.
    /// @param iRow Zero-based row index to select.
    /// @param bSendMessage True to send a selection-change notification.
    /// @param bMakeVisible True to scroll the row into the visible area.
    void selectRow(int iRow, bool bSendMessage, bool bMakeVisible);

    /// @brief Returns the starting vertical coordinate and line number for the line-numbers sidebar.
    /// @param startingLinee Output parameter receiving the starting line number.
    /// @return Vertical coordinate offset for the sidebar.
    gui::CoordType getLineNumbersInfo(td::UINT4& startingLinee);

};

} //namespace gui
