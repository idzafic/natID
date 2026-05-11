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
#include <gui/Columns.h>

namespace gui
{

class DataView;
class LineNumbers;
class NATGUI_API DataViewHeader : public Canvas
{
    friend class DataView;
    friend class LineNumbers;
    
protected:
    //const cnt::SafeFullVector<gui::Column>& _columns;
    
    DataView* _pDataView;
    gui::Size _size;
    gui::CoordType _dx = 0;
private:
    gui::Point _startDrag;
    int _toolTipIndex = -1;
public:
    static int spaceBetweenCols;
private:
    static td::ColorID txtColor;
    bool _inResizing = false;
private:
    int getHoveringColumn(gui::CoordType x) const;
    int getSeparator(gui::CoordType x) const;
protected:
    //void sendSelChangeMessage();
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;

    void onResize(const gui::Size& newSize) override;
    
    const Size& getSize() const;

    void onDraw(const gui::Rect& rect) override;
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    //void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
    
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    void onCursorMoved(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    
    //void setContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    //void forwardContextMenuEventsTo(int pos, Frame* pFrame);
    //void setNonRemovable(int viewPos, bool nonRemovable);
    void systemColorModeChanged(bool bDarkMode) override;
    
    void setScrollOffset(gui::CoordType dx);
//    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
public:
    DataViewHeader(DataView* pDataView);
    ~DataViewHeader();
};

} //namespace gui
