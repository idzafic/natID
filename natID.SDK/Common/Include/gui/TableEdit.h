// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TableEdit.h
    @brief Editable table control that displays and edits data from an IDataSet. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"
#include <td/Types.h>
#include <dp/IDataSet.h>
#include <gui/Columns.h>
#include <initializer_list>
#include <cnt/SafeFullVector.h>
#include <gui/InputDevice.h>
#include <gui/IDataSetPresenter.h>

namespace gui
{

class TableEditHelper;

/// @brief A control that presents rows and columns of data from an IDataSet and allows in-place editing.
class NATGUI_API TableEdit : public Control
{
public:

    using SelChCallBack = std::function<void(td::INT4)>; ///< Callback type for selection-changed events.
    using ActivateCallBack = std::function<void(td::INT4)>; ///< Callback type for row-activation events.

    /// @brief Controls visibility and numbering style of the row-number column.
    enum class RowNumberVisibility : td::BYTE
    {
        NotVisible = 0,         ///< Row number column is not displayed.
        VisibleZeroBased,       ///< Row number column is visible with 0-based numbering.
        VisibleOneBased         ///< Row number column is visible with 1-based numbering.
    };
private:
    friend class TableEditHelper;
    SelChCallBack _onChangedSelection; ///< Callback invoked when the selected row changes.
    ActivateCallBack _onActivate; ///< Callback invoked when a row is activated (e.g. double-clicked).
private:
    /// @brief Returns a formatted string representation of a cell value.
    /// @param iRow Row index.
    /// @param iCol Column index.
    /// @return Pointer to the formatted value string.
    const char* getFormattedValue(int iRow, int iCol) const;
protected:
    dp::IDataSet* _pData = nullptr; ///< Pointer to the underlying data set.
    cnt::SafeFullVector<gui::Column> _columns; ///< Column descriptors.
    const char* _id; ///< Control identifier string.
    td::WORD _rowNumberWidth = 50; ///< Width of the row-number column in pixels.
    td::UINT2 _contextMenuGroup; ///< System context menu group identifier.
    td::Ownership _dsOwnership; ///< Ownership policy for the data set.
    RowNumberVisibility _rowNumberVisibility; ///< Current visibility mode for row numbers.
    td::BYTE _tmpBlockMsg; ///< Temporary flag to block messaging.
    td::BYTE _disableMsg; ///< Flag to permanently disable messaging.
    BoolStyle _boolStyle = BoolStyle::Text; ///< Visual style used to render boolean values.
    td::BYTE _contextMenuID; ///< Identifier of the context menu attached to this control.
private:
    /// @brief Measures the preferred size of the table control.
    /// @param[out] cell Cell information to fill.
    void measure(CellInfo&) override;

    /// @brief Re-measures the table after a layout change.
    /// @param[out] cell Cell information to update.
    void reMeasure(CellInfo&) override;

    /// @brief Sends a selection-changed message for the given row.
    /// @param rowNo Zero-based index of the newly selected row.
    void sendSelChangedMessage(td::INT4 rowNo);

    /// @brief Sends a row-activation message and returns whether it was handled.
    /// @param rowNo Zero-based index of the activated row.
    /// @return True if the activation was handled.
    bool sendActivateMessage(td::INT4 rowNo);

    /// @brief Displays the context menu triggered by the given input device state.
    /// @param inputDevice Input device state at the time of the right-click.
    void showContextMenu(const gui::InputDevice& inputDevice);

    /// @brief Initialises visual defaults for a column based on its data type.
    /// @param iCol Zero-based column index.
    /// @param dt Data type of the column.
    void initVisualDefaults(size_t iCol, td::DataType dt);

    /// @brief Handles system context-menu action items.
    /// @param aiDesc Descriptor of the triggered action item.
    /// @return True if the action was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override; //system context menus

    TableEdit(const TableEdit&) = delete;
    TableEdit& operator =(const TableEdit&) = delete;
public:
    /// @brief Constructs a TableEdit control.
    /// @param dataSetOwnerShip Ownership policy for the attached data set.
    /// @param rowNumberVisibility Initial visibility mode for the row-number column.
    TableEdit(td::Ownership dataSetOwnerShip = td::Ownership::Extern, RowNumberVisibility rowNumberVisibility = RowNumberVisibility::NotVisible);

    /// @brief Destroys the TableEdit and releases owned resources.
    ~TableEdit();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::TableEdit.
    virtual gui::ObjType getObjType() const override { return ObjType::TableEdit;}

    //set/get using specified iRwo
    //virtual bool setValue(int iRow, int iCol, const td::Variant& val);
    //virtual bool getValue(int iRow, int iCol, td::Variant& val, bool checkType = false) const;

    //set/get using current selection
    //virtual bool setValue(int iCol, const td::Variant& val);
    //virtual bool getValue(int iCol, td::Variant& val, bool checkType = false) const;

    /// @brief Clears the displayed data and optionally sends a change message.
    /// @param sendMessage If true, fires a selection-changed notification.
    virtual void clean(bool sendMessage = false);

    /// @brief Clears the display and detaches (without deleting) the data set.
    void cleanAndDetachDataSet();

    /// @brief Binds all columns of the given data set to the control.
    /// @param pData Pointer to the data set to display.
    void init(dp::IDataSet* pData);

    /// @brief Binds selected columns of the given data set by position list.
    /// @param pData Pointer to the data set to display.
    /// @param colPos Initializer list of zero-based column positions to show.
    /// @return True if initialisation succeeded.
    bool init(dp::IDataSet* pData, const std::initializer_list<int>& colPos);

    /// @brief Binds the first N columns of the given data set.
    /// @param pData Pointer to the data set to display.
    /// @param nFirstCols Number of leading columns to bind.
    void init(dp::IDataSet* pData, size_t nFirstCols);

    /// @brief Binds all columns of the given shared data set.
    /// @param pData Shared pointer to the data set to display.
    void init(dp::IDataSetPtr& pData);

    /// @brief Binds selected columns of the given shared data set by position list.
    /// @param pData Shared pointer to the data set to display.
    /// @param colPos Initializer list of zero-based column positions to show.
    /// @return True if initialisation succeeded.
    bool init(dp::IDataSetPtr& pData, const std::initializer_list<int>& colPos);

    /// @brief Binds the first N columns of the given shared data set.
    /// @param pData Shared pointer to the data set to display.
    /// @param nFirstCols Number of leading columns to bind.
    void init(dp::IDataSetPtr& pData, size_t nFirstCols);

    /// @brief Reloads the display from the current data set.
    /// @return True if the reload succeeded.
    bool reload();

    /// @brief Begins a batch update, suppressing intermediate redraws.
    void beginUpdate();

    /// @brief Ends a batch update and triggers a redraw.
    void endUpdate();

    /// @brief Appends the last row of the data set to the display.
    /// @param sendMessage If true, fires a selection-changed notification.
    void push_back(bool sendMessage = false);

    /// @brief Refreshes the display of the specified row.
    /// @param iRow Zero-based row index to update.
    void updateRow(int iRow);

    /// @brief Inserts a new row at the given position in the display.
    /// @param iRow Zero-based position at which to insert.
    /// @param sendMessage If true, fires a selection-changed notification.
    void insertRow(int iRow, bool sendMessage = false);

    /// @brief Removes the row at the given position from the display.
    /// @param iRow Zero-based index of the row to remove.
    /// @param sendMessage If true, fires a selection-changed notification.
    void removeRow(int iRow, bool sendMessage = true);

    /// @brief Returns the zero-based index of the first selected row.
    /// @return Selected row index, or -1 if nothing is selected.
    int getFirstSelectedRow() const;

    /// @brief Programmatically selects the specified row.
    /// @param iRow Zero-based index of the row to select.
    /// @param sendMessage If true, fires a selection-changed notification.
    /// @param bMakeVisible If true, scrolls the row into view.
    void selectRow(int iRow, bool sendMessage = false, bool bMakeVisible = true);

    /// @brief Temporarily disables all message notifications from this control.
    void disableMessaging();

    /// @brief Re-enables message notifications from this control.
    void enableMessaging();

    /// @brief Returns the number of columns in the control.
    /// @return Column count.
    size_t getNumberOfColumns() const;

    /// @brief Returns the number of data rows currently displayed.
    /// @return Row count.
    size_t getNumberOfRows() const;

    /// @brief Returns a pointer to the attached data set.
    /// @return Pointer to the IDataSet.
    dp::IDataSet* getDataSet();

    /// @brief Returns a const pointer to the attached data set.
    /// @return Const pointer to the IDataSet.
    const dp::IDataSet* getDataSet() const;

    /// @brief Allocates and returns the column descriptor vector for binding.
    /// @param nColumns Number of columns to allocate.
    /// @return Reference to the allocated column descriptor vector.
    cnt::SafeFullVector<gui::Column>& allocBindColumns(size_t nColumns);

    /// @brief Returns a const reference to the column descriptor vector.
    /// @return Const reference to the column vector.
    const cnt::SafeFullVector<gui::Column>& getColumnDesc() const;

    /// @brief Returns a row wrapper for the currently selected row.
    /// @return Row wrapper object.
    dp::IDataSet::Row getCurrentRow();

    /// @brief Returns an empty row wrapper for building new data.
    /// @return Empty row wrapper object.
    dp::IDataSet::Row getEmptyRow();
    //bool getDRow(size_t iRow, dp::RowWrapper& rowData);

    /// @brief Returns the selection-changed callback.
    /// @return Const reference to the stored callback function.
    const SelChCallBack& getChangedSelectionHandler() const;

    /// @brief Registers a callback invoked when the row selection changes.
    /// @param fnToCall Function accepting the new zero-based row index.
    void onChangedSelection(const SelChCallBack& fnToCall);

    /// @brief Registers a callback invoked when a row is activated.
    /// @param fnToCall Function accepting the activated zero-based row index.
    void onActivate(const ActivateCallBack& fnToCall);

    /// @brief Returns the current row-number visibility mode.
    /// @return RowNumberVisibility value.
    RowNumberVisibility getRowNumberVisibility() const;

    /// @brief Sets the pixel width of the row-number column.
    /// @param width Desired column width in pixels.
    void setColumnWidthWithRowNumber(td::WORD width);

    /// @brief Returns the pixel width of the row-number column.
    /// @return Column width in pixels.
    td::WORD getColumnWidthWithRowNumber() const;

    /// @brief Sets the numeric format for a floating-point column.
    /// @param iCol Zero-based column index.
    /// @param fmt Float format specifier.
    /// @param nDec Number of decimal places.
    void setColumnNumericFormat(int iCol, td::FormatFloat fmt, int nDec = 6);

    /// @brief Sets the pixel width of the specified column.
    /// @param iCol Zero-based column index.
    /// @param width Desired column width in pixels.
    void setColumnWidth(int iCol, td::WORD width);

    /// @brief Returns the pixel width of the specified column.
    /// @param iCol Zero-based column index.
    /// @return Column width in pixels.
    td::WORD getColumnWidth(int iCol) const;

    /// @brief Associates an enum-string lookup vector with the specified column.
    /// @param iCol Zero-based column index.
    /// @param pEnumStrings Pointer to the vector of display strings.
    void setEnumStrings(int iCol, const cnt::SafeFullVector<td::String>* pEnumStrings);

    /// @brief Sets the visual style used to render boolean columns.
    /// @param boolStyle Desired BoolStyle value.
    void setBoolStyle(BoolStyle boolStyle);

    /// @brief Returns the current visual style used to render boolean columns.
    /// @return Current BoolStyle value.
    BoolStyle getBoolStale() const;

    /// @brief Attaches a context menu to this control.
    /// @param contextMenuID Identifier of the context menu.
    /// @param contextMenuGroup Group identifier for the context menu.
    void setContextMenu(td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);


};

} //namespace gui
