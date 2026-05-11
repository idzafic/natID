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
class NATGUI_API DataViewBody : public Canvas
{
    friend class DataView;
    friend class LineNumbers;

protected:
    DataView* _pDataView;
    gui::ILineNumbers* _pLineNumbers = nullptr;
    dp::IDataSet* _pDS = nullptr;
    //const cnt::SafeFullVector<gui::Column>& _columns;
    gui::Size _size;
    int _selectedRow = -1;
    static int lineHeight;
    BoolStyle _boolStyle = BoolStyle::Text;
private:
    static td::ColorID alternateRowColor;
private:
    const char* getFormattedValue(const td::Variant& val, int iVisCol) const;
    td::BYTE _inFocus = 0;
protected:
    
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;

    void onResize(const gui::Size& newSize) override;
    
    const Size& getSize() const;
    bool getModelSize(gui::Size& modelSize) const override;
    
    void onReload();
    void onUpdate();
    
    void onDraw(const gui::Rect& rect) override;
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    
    void onFocusIn() override;
    void onFocusOut() override;
//    void onCursorDragged(const gui::InputDevice& inputDevice) override;
//    
//    void onCursorEntered(const gui::InputDevice& inputDevice) override;
//    void onCursorMoved(const gui::InputDevice& inputDevice) override;
//    void onCursorExited(const gui::InputDevice& inputDevice) override;
    
    void calcVisibleRect(gui::Rect& r, int iRow) const;
    bool onKeyPressed(const gui::Key& key) override;
    
    void setContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    void forwardContextMenuEventsTo(int pos, Frame* pFrame);
    //void setNonRemovable(int viewPos, bool nonRemovable);
    void systemColorModeChanged(bool bDarkMode) override;
    bool hasFocus() const;
    
    //void selectRow(int iRow, bool bSendMessage);
    void moveSelectionUp();
    void moveSelectionDown();
    void moveSelectionPgUp();
    void moveSelectionPgDown();
    void moveSelectionToFirst();
    void moveSelectionToLast();
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    DataViewBody(DataView* pDV);
    ~DataViewBody();
    
    void setLineNumbers(gui::ILineNumbers* pLN);
    void setDataSet(dp::IDataSet* pDS);
    gui::CoordType getFirstVisibleRow(td::UINT4& lineNo) const;
    size_t getNumberOfRows() const;
    int getLineHeight() const;
    void setBoolStyle(BoolStyle boolStyle);
    BoolStyle getBoolStale() const;
    int getFirstSelectedRow() const;
    void selectRow(int iRow, bool bSendMessage, bool bMakeVisible);

    gui::CoordType getLineNumbersInfo(td::UINT4& startingLinee);

};

} //namespace gui
