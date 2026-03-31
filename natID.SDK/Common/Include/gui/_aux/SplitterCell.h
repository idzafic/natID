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
#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <gui/ISwitch.h>

namespace gui
{

class SplitterLayout;

class NATGUI_API SplitterCell : public Canvas
{
    friend class SplitterLayout;
public:
    enum class Status : unsigned char {Minimized=0, Normal};
private:
    void initSymbols();
protected:
    SplitterLayout* _pSplitter = nullptr;
    NatString _toolTipStr = nullptr;
    ISwitchState* _pSwitchStateUpdater = nullptr;
    Shape _minimizedArea; //background
    //Path _minimizedBorder; //border
    Shape _minizedSymbol1;
    Shape _minizedSymbol2;
    
    Status _status = Status::Normal;
    //td::BYTE _dragOperation = 0;
private:
    void setToolTip(bool bSet);
protected:
    void setCursor() const;
    
    void onDraw(const gui::Rect& rect) override;
    
    void toggle();
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onPrimaryButtonReleased(const gui::InputDevice& inputDevice) override;
//    virtual void onCursorEntered(const gui::InputDevice& inputDevice);
    void onCursorDragged(const gui::InputDevice& inputDevice) override;
//    virtual void onCursorExited(const gui::InputDevice& inputDevice);
    
    void setUpdater(ISwitchState* pUpdater);
    
public:
    SplitterCell(SplitterLayout* pSplitter);
    ~SplitterCell();
    gui::ObjType getObjType() const override { return ObjType::SplitterCell;}
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    SplitterCell::Status getStatus() const;
    void setStatus(SplitterCell::Status status);
    void setToolTipWhenMinimized(const td::String& str);
};

} //namespace gui
