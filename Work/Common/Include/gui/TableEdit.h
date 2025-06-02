// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

namespace gui
{

class TableEditHelper;

class NATGUI_API TableEdit : public Control
{
public:
    enum class BoolStyle : td::BYTE {Text=0, CheckCrossGreenRed, CircleGreenRed};
    
    using SelChCallBack = std::function<void(td::INT4)>;
    using ActivateCallBack = std::function<void(td::INT4)>;
    
    enum class RowNumberVisibility : td::BYTE
    {
        NotVisible = 0,         // Row number column is not displayed
        VisibleZeroBased,   // Row number column is visible with 0-based numbering
        VisibleOneBased     // Row number column is visible with 1-based numbering
    };
private:
    friend class TableEditHelper;
    SelChCallBack _onChangedSelection;
    ActivateCallBack _onActivate;
private:
    const char* getFormattedValue(int iRow, int iCol) const;
protected:
    dp::IDataSet* _pData = nullptr;
    cnt::SafeFullVector<gui::Column> _columns;
    const char* _id;
    td::WORD _rowNumberWidth = 50;
    td::UINT2 _contextMenuGroup; //system context menu group
    td::Ownership _dsOwnership;
    RowNumberVisibility _rowNumberVisibility;
    td::BYTE _tmpBlockMsg;
    td::BYTE _disableMsg;
    BoolStyle _boolStyle = BoolStyle::Text;
    td::BYTE _contextMenuID;
private:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    void sendSelChangedMessage(td::INT4 rowNo);
    bool sendActivateMessage(td::INT4 rowNo);
    void showContextMenu(const gui::InputDevice& inputDevice);
    void initVisualDefaults(size_t iCol, td::DataType dt);
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override; //system context menus
public:
    TableEdit(td::Ownership dataSetOwnerShip = td::Ownership::Extern, RowNumberVisibility rowNumberVisibility = RowNumberVisibility::NotVisible);
    ~TableEdit();
    virtual gui::ObjType getObjType() const override { return ObjType::TableEdit;}
    
    //set/get using specified iRwo
    //virtual bool setValue(int iRow, int iCol, const td::Variant& val);
    //virtual bool getValue(int iRow, int iCol, td::Variant& val, bool checkType = false) const;
    
    //set/get using current selection
    //virtual bool setValue(int iCol, const td::Variant& val);
    //virtual bool getValue(int iCol, td::Variant& val, bool checkType = false) const;
    
    virtual void clean(bool sendMessage = false);
    void init(dp::IDataSet* pData);
    bool init(dp::IDataSet* pData, const std::initializer_list<int>& colPos);
    void init(dp::IDataSet* pData, size_t nFirstCols);
    void init(dp::IDataSetPtr& pData);
    bool init(dp::IDataSetPtr& pData, const std::initializer_list<int>& colPos);
    void init(dp::IDataSetPtr& pData, size_t nFirstCols);
    bool reload();
    void beginUpdate();
    void endUpdate();
    void push_back(bool sendMessage = false);
    void updateRow(int iRow);
    void insertRow(int iRow, bool sendMessage = false);
    void removeRow(int iRow, bool sendMessage = true);
    int getFirstSelectedRow() const;
    void selectRow(int iRow, bool sendMessage = false, bool bMakeVisible = true);
    void disableMessaging();
    void enableMessaging();
    //size_t getNoOfRows() const;
    size_t getNoOfColumns() const;
    dp::IDataSet* getDataSet();
    const dp::IDataSet* getDataSet() const;
    cnt::SafeFullVector<gui::Column>& allocBindColumns(size_t nColumns);
    const cnt::SafeFullVector<gui::Column>& getColumnDesc() const;
    dp::IDataSet::Row getCurrentRow();
    dp::IDataSet::Row getEmptyRow();
    //bool getDRow(size_t iRow, dp::RowWrapper& rowData);

    const SelChCallBack& getChangedSelectionHandler() const;
    void onChangedSelection(const SelChCallBack& fnToCall);
    void onActivate(const ActivateCallBack& fnToCall);
    RowNumberVisibility getRowNumberVisibility() const;
    void setColumnWidthWithRowNumber(td::WORD width);
    td::WORD getColumnWidthWithRowNumber() const;
    void setColumnNumericFormat(int iCol, td::FormatFloat fmt, int nDec = 6);
    
    void setColumnWidth(int iCol, td::WORD width);
    td::WORD getColumnWidth(int iCol) const;
    
    void setEnumStrings(int iCol, const cnt::SafeFullVector<td::String>* pEnumStrings);
    
    void setBoolStyle(BoolStyle boolStyle);
    BoolStyle getBoolStale() const;
    
    void setContextMenu(td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
};

class IDataSetPresenter
{
public:
    virtual void show(dp::IDataSet* pDS) = 0;
};

} //namespace gui
