// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file InputDevice.h
 * @brief Abstraction for all input devices including mouse, trackpad, stylus, and keyboard.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <gui/Key.h>

namespace gui
{

class InputDeviceHelper;

/// @brief Represents an input device event, providing access to pointer position, button state, and keyboard modifiers.
class NATGUI_API InputDevice
{
    friend class InputDeviceHelper;

public:
    /// @brief Bitmask of events that an input device can generate.
    enum class Event : td::UINT2 {NA=0, CursorEnterLeave=1, CursorShape=2, PrimaryClicks=4, PrimaryDblClick =8, SecondaryClicks =16, CursorMove = 32, CursorDrag=64, Zoom=128, Keyboard=256};
    /// @brief Physical type of the input device.
    enum class Type : unsigned char {NA=0, Mouse, TrackPad, Stylus, FingerTouch, Keyboard, Other};
    /// @brief Button identifier on the input device.
    enum class Button : unsigned char {NA=0, Primary=1, Secondary=2, Wheel=128};
protected:
    static double _zoomIncrement; ///< Default zoom increment applied per scroll step.
    gui::Point _modelPoint;       ///< Point in model (logical) coordinates.
    gui::Point _framePoint;       ///< Point in frame (physical) coordinates.
    gui::Point _containingWindowPoint; ///< Point relative to the containing window.
    gui::Point _scrollDelta;      ///< Scroll delta values for the current event.
    double _scale;                ///< Current scale factor.
    Key _key;                     ///< Keyboard key associated with the event.
    td::UINT2 _nEvents;           ///< Number of events (e.g. click count).
    Type _type;                   ///< Type of input device.
    Button _button;               ///< Button involved in the event.
    td::BYTE _modelPointOK = 1;   ///< Flag indicating whether the model point is valid.
    td::BYTE _pointerCaptured = 0; ///< Non-zero when the cursor is invisible and captured.

protected:
    /// @brief Sets the model-space point for this event.
    /// @param modelPoint The point in model coordinates.
    void setModelPoint(const gui::Point& modelPoint);

    /// @brief Sets keyboard modifier flags.
    /// @param modifiers Bitmask of active keyboard modifiers.
    void setKeyboardModifiers(td::UINT4 modifiers);

    /// @brief Sets the device type.
    /// @param type The input device type.
    void setType(InputDevice::Type type);

    /// @brief Sets the button involved in the event.
    /// @param button The button identifier.
    void setButton(InputDevice::Button button);

    /// @brief Sets the scale factor.
    /// @param scale The scale value to apply.
    void setScale(double scale);

    /// @brief Sets the number of events (e.g. click count).
    /// @param nEvents Number of events.
    void setNoOfEvents(td::UINT2 nEvents);

    /// @brief Sets the character produced by a key press.
    /// @param chPressed UTF-32 character code.
    void setChar(td::UINT4 chPressed);

    /// @brief Default constructor. Initialises members to default values.
    InputDevice();
    InputDevice(const InputDevice&) = delete;
    InputDevice& operator =(const InputDevice&) = delete;
public:
    /// @brief Constructs an InputDevice with full pointer and modifier information.
    /// @param modelPoint Point in model coordinates.
    /// @param framePoint Point in frame coordinates.
    /// @param ptRelToParent Point relative to the parent window.
    /// @param modifiers Active keyboard modifier bitmask.
    /// @param type Type of the input device.
    /// @param button Button involved in the event.
    InputDevice(const gui::Point& modelPoint, const gui::Point& framePoint, const gui::Point& ptRelToParent, td::UINT4 modifiers, Type type, Button button);

    /// @brief Destructor.
    ~InputDevice();

    //getModelPoint is available on gui::Canvas only (on other views asserts), check isModelPointAvailable()
    /// @brief Returns the event position in model coordinates (scroll and scale applied).
    /// @return Reference to the model-space point.
    const gui::Point& getModelPoint() const; //includes scroll, scale,...

    //getFramePoint - returns point relative to the frame in logical pixels (always available)
    //for openGL and other GPU views use this one and then convert to model coordinates if necessary
    /// @brief Returns the event position in frame coordinates (always available).
    /// @return Reference to the frame-space point in logical pixels.
    const gui::Point& getFramePoint() const; //in frame coordinates (always available),

    /// @brief Returns the event position relative to the containing window.
    /// @return Reference to the point in containing-window coordinates.
    const gui::Point& getPointRelativeToContainingWindow() const; //in coordinates of containig window (parent)

    /// @brief Returns the scroll delta for the current event.
    /// @return Reference to the scroll delta point.
    const gui::Point& getScrollDelta() const;

    /// @brief Returns the type of the input device.
    /// @return Device type enumeration value.
    InputDevice::Type getType() const;

    /// @brief Returns the button involved in the event.
    /// @return Button enumeration value.
    InputDevice::Button getButton() const;

    /// @brief Returns the current scale factor.
    /// @return Scale as a double.
    double getScale() const;

    /// @brief Returns the number of events (e.g. click count).
    /// @return Number of events.
    td::UINT2 getNoOfEvents() const;

    /// @brief Sets the application-wide default zoom increment.
    /// @param increment Zoom increment value.
    static void setDefaultZoomIncrement(double increment);

    /// @brief Returns the application-wide default zoom increment.
    /// @return Zoom increment value.
    static double getDefaultZoomIncrement();

    /// @brief Checks whether the model-space point is valid for this event.
    /// @return True if the model point is available, false otherwise.
    bool isModelPointAvailable() const;
    //key part
    /// @brief Returns the bitmask of active keyboard modifiers.
    /// @return Modifier flags as td::UINT4.
    td::UINT4 getKeyboardModifiers() const;

    /// @brief Checks whether any modifier key is currently pressed.
    /// @return True if at least one modifier key is pressed.
    bool isAnyModifierPressed() const;

    /// @brief Checks whether the Alt key is pressed.
    /// @return True if Alt is pressed.
    bool isAltPressed() const;

    /// @brief Checks whether the Ctrl key is pressed.
    /// @return True if Ctrl is pressed.
    bool isCtrlPressed() const;

    /// @brief Checks whether the Cmd (Command) key is pressed.
    /// @return True if Cmd is pressed.
    bool isCmdPressed() const;

    /// @brief Checks whether Cmd on macOS or Ctrl on other platforms is pressed.
    /// @return True if the platform-specific primary modifier key is pressed.
    bool isCmdOnMacOrCtrlOnOtherPressed() const;

    /// @brief Returns the keyboard key associated with this event.
    /// @return Reference to the Key object.
    const Key& getKey() const;

    /// @brief Checks whether the pointer is currently captured (cursor invisible).
    /// @return True if the pointer is captured.
    bool isPointerCaptured() const;
};

} //namespace gui
