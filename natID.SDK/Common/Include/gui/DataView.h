// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DataView.h
    @brief Scrollable tabular data view widget backed by a dataset. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "View.h"
#include <td/Types.h>
#include <dp/IDataSet.h>
#include <gui/Columns.h>
#include <initializer_list>
#include <cnt/SafeFullVector.h>
#include <gui/InputDevice.h>
#include <gui/GridLayout.h>
#include <gui/IDataSetPresenter.h>
#include <gui/Canvas.h>
#include <gui/ViewScroller.h>
#include <gui/_aux/DataViewHeader.h>
#include <gui/_aux//DataViewBody.h>
#include <gui/Label.h>
#include <gui/_aux/LineNumbers.h>
#include <gui/IScrollConsumer.h>

namespace gui
{

//class DataViewHelper;

/// @brief Widget that displays rows and columns of a dataset with scrolling support.
class NATGUI_API DataView : public View, public IScrollConsumer
{
    friend class DataViewBody;
    friend class DataViewHeader;
public:
    using SelChCallBack = std::function<void(td::INT4)>; ///< Callback type for selection change events.
    using ActivateCallBack = std::function<void(td::INT4)>; ///< Callback type for row activation events.

    /// @brief Controls whether and how row numbers are displayed.
    enum class RowNumberVisibility : td::BYTE
    {
        NotVisible = 0,         ///< Row number column is not displayed.
        VisibleZeroBased,       ///< Row number column is visible with 0-based numbering.
        VisibleOneBased         ///< Row number column is visible with 1-based numbering.
    };
private:
    friend class TableEditHelper;
    SelChCallBack _onChangedSelection; ///< Callback invoked when the row selection changes.
    ActivateCallBack _onActivate; ///< Callback invoked when a row is activated.
protected:
    dp::IDataSet* _pData = nullptr; ///< Pointer to the underlying dataset.
    gui::Label _lblRecNo; ///< Label used to display the current record number.
    cnt::SafeFullVector<gui::Column> _columns; ///< Column descriptors for the view.
    gui::DataViewBody _body; ///< The scrollable body portion of the data view.
    gui::ViewScroller _scroller; ///< Scroller managing horizontal and vertical scroll bars.
    gui::LineNumbers _lineNumbers; ///< Widget displaying row line numbers.
    gui::DataViewHeader _header; ///< Header row with column titles.

    gui::GridLayout _gridLayout; ///< Internal grid layout organising body, header, and line numbers.

    const char* _id; ///< Identifier string for the data view.
    //int _selectedRow = -1;
    td::WORD _rowNumberWidth = 50; ///< Width in pixels of the row-number column.
    td::UINT2 _contextMenuGroup; ///< System context menu group identifier.
    td::Ownership _dsOwnership; ///< Ownership policy for the attached dataset.
    RowNumberVisibility _rowNumberVisibility; ///< Current row-number visibility mode.
    td::BYTE _tmpBlockMsg; ///< Temporary flag to block messaging during bulk operations.
    td::BYTE _disableMsg; ///< Flag to disable all messaging.
    td::BYTE _contextMenuID; ///< Identifier of the context menu to display.
private:
    /// @brief Internal measurement pass.
    /// @param ci Cell information to fill.
    void measure(CellInfo&) override;

    /// @brief Internal re-measurement pass.
    /// @param ci Cell information to update.
    void reMeasure(CellInfo&) override;

    /// @brief Sends a selection-changed message for the given row.
    /// @param rowNo Zero-based row index that was selected.
    void sendSelChangedMessage(td::INT4 rowNo);

    /// @brief Sends an activation message for the given row.
    /// @param rowNo Zero-based row index that was activated.
    /// @return True if the message was handled.
    bool sendActivateMessage(td::INT4 rowNo);

    /// @brief Displays the context menu at the position described by the input device.
    /// @param inputDevice Input device providing the cursor position.
    void showContextMenu(const gui::InputDevice& inputDevice);

    /// @brief Sets default visual properties for a column based on its data type.
    /// @param iCol Column index.
    /// @param dt Data type of the column.
    void initVisualDefaults(size_t iCol, td::DataType dt);

    /// @brief Handles system context menu action items.
    /// @param aiDesc Action item descriptor.
    /// @return True if the action was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;

    /// @brief Initialises internal state; called from constructors.
    /// @return True on success.
    bool init();

    /// @brief Removes all rows and resets internal state.
    void removeAll();

    /// @brief Updates the line-number display to show the given line.
    /// @param lineNo Line number to display.
    void setLineNumber(td::UINT4 lineNo);

    /// @brief Called when the scroller position changes.
    /// @param pScroller Scroller that changed.
    /// @param newOrigin New scroll origin in logical coordinates.
    void onScroller(gui::ViewScroller* pScroller, const gui::Point& newOrigin) override;

    /// @brief Computes the auxiliary size occupied by headers and line numbers.
    /// @param sz Size structure to fill.
    void getAuxSize(gui::Size& sz);

    /// @brief Reacts to focus changes within the view.
    void handleFocusChange();

    /// @brief Returns whether this view currently has keyboard focus.
    /// @return True if focused.
    bool hasFocus() const;

    /// @brief Called when the column header configuration changes.
    /// @param modelSizeChanged True if the model size was also modified.
    void onHeaderChange(bool modelSizeChanged);

    DataView(const DataView&) = delete;
    DataView& operator =(const DataView&) = delete;
public:
    /// @brief Constructs a DataView.
    /// @param dataSetOwnerShip Ownership policy for the dataset (default Extern).
    /// @param rowNumberVisibility Initial row-number display mode.
    DataView(td::Ownership dataSetOwnerShip = td::Ownership::Extern, RowNumberVisibility rowNumberVisibility = RowNumberVisibility::VisibleOneBased);

    /// @brief Destructor. Releases resources and optionally the owned dataset.
    ~DataView();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::TableEdit.
    virtual gui::ObjType getObjType() const override { return ObjType::TableEdit;}

    /// @brief Clears all displayed rows.
    /// @param sendMessage If true, a selection-changed message is sent after clearing.
    virtual void clean(bool sendMessage = false);

    /// @brief Clears rows and detaches the dataset without deleting it.
    void cleanAndDetachDataSet();

    /// @brief Initialises the view with a dataset and auto-detects all columns.
    /// @param pData Pointer to the dataset to display.
    void init(dp::IDataSet* pData);

    /// @brief Initialises the view with a dataset and a subset of column positions.
    /// @param pData Pointer to the dataset.
    /// @param colPos Initializer list of column indices to include.
    /// @return True on success.
    bool init(dp::IDataSet* pData, const std::initializer_list<int>& colPos);

    /// @brief Initialises the view with the first N columns of a dataset.
    /// @param pData Pointer to the dataset.
    /// @param nFirstCols Number of leading columns to display.
    void init(dp::IDataSet* pData, size_t nFirstCols);

    /// @brief Initialises the view with a shared dataset pointer.
    /// @param pData Shared dataset pointer.
    void init(dp::IDataSetPtr& pData);

    /// @brief Initialises the view with a shared dataset and a subset of column positions.
    /// @param pData Shared dataset pointer.
    /// @param colPos Column indices to include.
    /// @return True on success.
    bool init(dp::IDataSetPtr& pData, const std::initializer_list<int>& colPos);

    /// @brief Initialises the view with the first N columns of a shared dataset.
    /// @param pData Shared dataset pointer.
    /// @param nFirstCols Number of leading columns to display.
    void init(dp::IDataSetPtr& pData, size_t nFirstCols);

    /// @brief Reloads the view from the current dataset.
    /// @return True if the reload succeeded.
    bool reload();

    /// @brief Begins a batch-update; suppresses redraws until endUpdate is called.
    void beginUpdate();

    /// @brief Ends a batch-update and triggers a redraw.
    void endUpdate();

    /// @brief Appends a new row from the dataset to the end of the view.
    /// @param sendMessage If true, fires a selection-changed message.
    void push_back(bool sendMessage = false);

    /// @brief Redraws the row at the given index.
    /// @param iRow Zero-based row index to refresh.
    void updateRow(int iRow);

    /// @brief Inserts a row into the view at the specified position.
    /// @param iRow Zero-based insertion index.
    /// @param sendMessage If true, fires a selection-changed message.
    void insertRow(int iRow, bool sendMessage = false);

    /// @brief Removes the row at the specified position.
    /// @param iRow Zero-based index of the row to remove.
    /// @param sendMessage If true, fires a selection-changed message.
    void removeRow(int iRow, bool sendMessage = true);

    /// @brief Returns the zero-based index of the first selected row.
    /// @return Row index, or -1 if nothing is selected.
    int getFirstSelectedRow() const;

    /// @brief Programmatically selects a row.
    /// @param iRow Zero-based row index to select.
    /// @param sendMessage If true, fires a selection-changed message.
    /// @param bMakeVisible If true, scrolls the view to make the row visible.
    void selectRow(int iRow, bool sendMessage = false, bool bMakeVisible = true);

    /// @brief Disables all messaging from this view.
    void disableMessaging();

    /// @brief Re-enables messaging from this view.
    void enableMessaging();

    /// @brief Returns the total number of displayed columns.
    /// @return Column count.
    size_t getNumberOfColumns() const;

    /// @brief Returns the total number of displayed rows.
    /// @return Row count.
    size_t getNumberOfRows() const;

    /// @brief Returns a non-const pointer to the underlying dataset.
    /// @return Pointer to the dataset, or nullptr if not set.
    dp::IDataSet* getDataSet();

    /// @brief Returns a const pointer to the underlying dataset.
    /// @return Const pointer to the dataset.
    const dp::IDataSet* getDataSet() const;

    /// @brief Allocates and returns a vector of column descriptors for binding.
    /// @param nColumns Number of columns to allocate.
    /// @return Reference to the column descriptor vector.
    cnt::SafeFullVector<gui::Column>& allocBindColumns(size_t nColumns);

    /// @brief Returns a const reference to the column descriptors.
    /// @return Const reference to the column vector.
    const cnt::SafeFullVector<gui::Column>& getColumnDesc() const;

    /// @brief Returns a non-const reference to the column descriptors.
    /// @return Reference to the column vector.
    cnt::SafeFullVector<gui::Column>& getColumnDesc();

    /// @brief Returns a row wrapper for the currently selected row.
    /// @return Row wrapper object.
    dp::IDataSet::Row getCurrentRow();

    /// @brief Returns a row wrapper representing an empty (new) row.
    /// @return Empty row wrapper.
    dp::IDataSet::Row getEmptyRow();

    /// @brief Returns the current selection-changed callback.
    /// @return Const reference to the callback function.
    const SelChCallBack& getChangedSelectionHandler() const;

    /// @brief Registers a callback to be invoked when the selection changes.
    /// @param fnToCall Callback function receiving the new row index.
    void onChangedSelection(const SelChCallBack& fnToCall);

    /// @brief Registers a callback to be invoked when a row is activated (e.g. double-clicked).
    /// @param fnToCall Callback function receiving the activated row index.
    void onActivate(const ActivateCallBack& fnToCall);

    /// @brief Returns the current row-number visibility setting.
    /// @return Row-number visibility enum value.
    RowNumberVisibility getRowNumberVisibility() const;

    /// @brief Sets the width of the row-number column.
    /// @param width Width in pixels.
    void setColumnWidthWithRowNumber(td::WORD width);

    /// @brief Returns the width of the row-number column.
    /// @return Width in pixels.
    td::WORD getColumnWidthWithRowNumber() const;

    /// @brief Sets the numeric display format for a column.
    /// @param iCol Column index.
    /// @param fmt Floating-point format specifier.
    /// @param nDec Number of decimal places.
    void setColumnNumericFormat(int iCol, td::FormatFloat fmt, int nDec = 6);

    /// @brief Sets the pixel width of the specified column.
    /// @param iCol Column index.
    /// @param width Width in pixels.
    void setColumnWidth(int iCol, td::WORD width);

    /// @brief Returns the pixel width of the specified column.
    /// @param iCol Column index.
    /// @return Width in pixels.
    td::WORD getColumnWidth(int iCol) const;

    /// @brief Associates an enum string table with a column.
    /// @param iCol Column index.
    /// @param pEnumStrings Pointer to the vector of display strings.
    void setEnumStrings(int iCol, const cnt::SafeFullVector<td::String>* pEnumStrings);

    /// @brief Sets the display style used for boolean columns.
    /// @param boolStyle Desired bool display style.
    void setBoolStyle(BoolStyle boolStyle);

    /// @brief Returns the current bool display style.
    /// @return Bool display style value.
    BoolStyle getBoolStale() const;

    /// @brief Attaches a context menu to the data view.
    /// @param contextMenuID Identifier of the context menu to show.
    /// @param contextMenuGroup Optional group ID for the context menu.
    void setContextMenu(td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
};

} //namespace gui
