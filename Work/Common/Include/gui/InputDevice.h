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
#include "Types.h"
#include "Key.h"

namespace gui
{

class InputDeviceHelper;

class NATGUI_API InputDevice
{
    friend class InputDeviceHelper;
    
public:
    enum class Event : td::UINT2 {NA=0, CursorEnterLeave=1, CursorShape=2, PrimaryClicks=4, PrimaryDblClick =8, SecondaryClicks =16, CursorMove = 32, CursorDrag=64, Zoom=128, Keyboard=256};
    enum class Type : unsigned char {NA=0, Mouse, TrackPad, Stylus, FingerTouch, Keyboard, Other};
    enum class Button : unsigned char {NA=0, Primary=1, Secondary=2, Wheel=128};
protected:
    static double _zoomIncrement;
    gui::Point _modelPoint;
    gui::Point _framePoint;
    gui::Point _containingWindowPoint;
    gui::Point _scrollDelta;
//    void* _pEvent;
    double _scale;
    Key _key;
//    td::UINT4 _modifiers;
//    td::UINT4 _char; //UTF32
    td::UINT2 _nEvents;
    Type _type;
    Button _button;
    td::BYTE _modelPointOK = 1;
    
protected:
    void setModelPoint(const gui::Point& modelPoint);
    void setKeyboardModifiers(td::UINT4 modifiers);
    void setType(InputDevice::Type type);
    void setButton(InputDevice::Button button);
    void setScale(double scale);
    void setNoOfEvents(td::UINT2 nEvents);
    void setChar(td::UINT4 chPressed);
    InputDevice();
public:
    InputDevice(const gui::Point& modelPoint, const gui::Point& framePoint, const gui::Point& ptRelToParent, td::UINT4 modifiers, Type type, Button button);
    ~InputDevice();
    
    //getModelPoint is available on gui::Canvas only (on other views asserts), check isModelPointAvailable()
    const gui::Point& getModelPoint() const; //includes scroll, scale,...
    
    //getFramePoint - returns point relative to the frame in logical pixels (always available)
    //for openGL and other GPU views use this one and then convert to model coordinates if necessary
    const gui::Point& getFramePoint() const; //in frame coordinates (always available),
    
    const gui::Point& getPointRelativeToContainingWindow() const; //in coordinates of containig window (parent)
    
    const gui::Point& getScrollDelta() const;
    
    InputDevice::Type getType() const;
    InputDevice::Button getButton() const;
    
    double getScale() const;
    td::UINT2 getNoOfEvents() const;
    
    static void setDefaultZoomIncrement(double increment);
    static double getDefaultZoomIncrement();
    bool isModelPointAvailable() const;
    //key part
    td::UINT4 getKeyboardModifiers() const;
    bool isAnyModifierPressed() const;
    const Key& getKey() const;
};

} //namespace gui
