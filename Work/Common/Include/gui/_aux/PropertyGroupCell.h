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
#include <gui/DrawableString.h>

namespace gui
{

class NATGUI_API PropertyGroupCell : public Canvas
{
public:
    enum class Status : unsigned char {Expanded=0, Collapsed};
private:
protected:
    DrawableString _label;
    Size _contentSize;
    td::UINT2 _width;
    td::UINT2 _height;
    Status _status = Status::Expanded;
    td::BYTE _hover = 0;
protected:
    void updateBackground(bool pointing);
    
    void onDraw(const gui::Rect& rect) override;
    
//    virtual void onClick(const gui::Point& modelPoint, td::UINT4 keyModifiers);
//
//    virtual void onInputDeviceEntered(const gui::Point& modelPoint);
//
//    virtual void onInputDeviceExited(const gui::Point& modelPoint);
    
    //virtual void onInputDeviceMoved(const gui::Point& modelPoint, td::UINT4 keyModifiers);
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override;
    void onCursorEntered(const gui::InputDevice& inputDevice) override;
    void onCursorExited(const gui::InputDevice& inputDevice) override;
    
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    PropertyGroupCell();
public:
    PropertyGroupCell(const td::String& label);
    ~PropertyGroupCell();
    
    PropertyGroupCell::Status getStatus() const;
    //void setStatus(PropertyGroupCell::Status status);
    
    td::UINT2 getArrowWidth() const;
};

} //namespace gui
