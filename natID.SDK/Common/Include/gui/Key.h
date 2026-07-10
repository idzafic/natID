// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Key.h
 * @brief Keyboard key representation including virtual keys, ASCII characters, and modifier states.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include "Types.h"


namespace gui
{
class WinKeyHelper;
class KeyHelper;
class InputDevice;

/// @brief Encapsulates a keyboard key event with its type, virtual key, ASCII value, modifiers, and repeat count.
class NATGUI_API Key
{
    friend class KeyHelper;
    friend class InputDevice;
    friend class WinKeyHelper;

public:
    /// @brief Classifies the type of key event.
    enum class Type : unsigned char {NA=0, ASCII, Virtual, Other};
    /// @brief Enumeration of virtual (non-printable) key identifiers.
    enum class Virtual : unsigned char {NA=0, Tab, Enter, Left, Right, Up, Down, Home, End, PgUp, PgDown, Insert, Delete,
                    Esc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrintScreen, ScrollLock, Pause, Break, BackSpace, VolumeUp, VolumeDown, Mute,
                    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, NumDecimal, NumEnter, NumPlus, NumMinus, NumMultiply, NumDivide,
                    NumInsert, NumDelete, NumEnd, NumDown, NumPgDown, NumLeft, NumRight, NumHome, NumUp, NumPgUp};
private:
    td::UINT4 _keyCode = 0;       ///< UTF-32 key code.
    td::UINT4 _modifiers = 0;     ///< Active modifier flags.
    td::UINT2 _nEvents = 0;       ///< Number of key events (repeat count).
    td::BYTE _extendedMask = 0;   ///< Extended key mask for platform-specific flags.
    char _ascii = 0;              ///< ASCII character value, if applicable.
    Type _type = Type::NA;        ///< Type classification of the key.
    Virtual _virtual = Virtual::NA; ///< Virtual key identifier, if applicable.
private:
    /// @brief Stores a key code and optional repeat count.
    /// @param keyCode UTF-32 key code.
    /// @param nEvents Number of key press events.
    void setCode(td::UINT4 keyCode, td::UINT2 nEvents = 1);

    /// @brief Stores a virtual key with its associated raw code and repeat count.
    /// @param keyCode Raw platform key code.
    /// @param nEvents Number of key press events.
    /// @param virtualKey The virtual key identifier.
    void setVirtual(td::UINT4 keyCode, td::UINT2 nEvents, Virtual virtualKey);

public:
    /// @brief Default constructor. Initialises all members to their default values.
    Key();

    /// @brief Constructs a Key from a platform key code, modifiers, repeat count, and extended mask.
    /// @param keyCode UTF-32 platform key code.
    /// @param keyModifiers Active modifier bitmask.
    /// @param nEvents Number of key press events.
    /// @param extendedMask Platform-specific extended key mask.
    Key(td::UINT4 keyCode, td::UINT4 keyModifiers, td::UINT2 nEvents, td::BYTE extendedMask=0);

    /// @brief Constructs a Key for simulation purposes.
    /// @param type Key type classification.
    /// @param virtID Virtual key identifier.
    /// @param ascii ASCII character value.
    Key(Type type, Virtual virtID, char ascii = 0); //for simulations

    /// @brief Returns the type classification of the key event.
    /// @return Key type enumeration value.
    Key::Type getType() const;

    /// @brief Returns the active modifier bitmask.
    /// @return Modifier flags as td::UINT4.
    td::UINT4 getModifiers() const;

    /// @brief Returns the number of key press events (repeat count).
    /// @return Event count.
    td::UINT2 getNoOfEvents() const;

    /// @brief Checks whether the key produces a printable glyph.
    /// @return True if the key is a printable glyph.
    bool isGlyph() const;

    /// @brief Checks whether any modifier key is currently pressed.
    /// @return True if at least one modifier key is active.
    bool isAnyModifierPressed() const;

    /// @brief Checks whether Alt, Ctrl, or Cmd is pressed.
    /// @return True if any of Alt, Ctrl, or Cmd is pressed.
    bool isAltCtrlOrCmdPressed() const;

    /// @brief Checks whether the Cmd (Command) key is pressed.
    /// @return True if Cmd is pressed.
    bool isCmdPressed() const;

    /// @brief Checks whether Cmd on macOS or Ctrl on other platforms is pressed.
    /// @return True if the platform-specific primary modifier is pressed.
    bool isCmdOnMacOrCtrlOnOtherPressed() const;

    /// @brief Checks whether the Ctrl key is pressed.
    /// @return True if Ctrl is pressed.
    bool isCtrlPressed() const;

    /// @brief Checks whether the Alt key is pressed.
    /// @return True if Alt is pressed.
    bool isAltPressed() const;

    /// @brief Checks whether the Fn key is pressed.
    /// @return True if Fn is pressed.
    bool isFnPressed() const;

    /// @brief Checks whether the Shift key is pressed.
    /// @return True if Shift is pressed.
    bool isShiftPressed() const;

    /// @brief Checks whether Caps Lock is active.
    /// @return True if Caps Lock is on.
    bool isCapsLockPressed() const;

    /// @brief Checks whether the key is an ASCII character key.
    /// @return True if the key produces an ASCII character.
    bool isASCII() const;

    /// @brief Returns the ASCII character for the key.
    /// @return ASCII character value.
    char getChar() const;

    /// @brief Checks whether the key is a virtual (non-printable) key.
    /// @return True if the key is virtual.
    bool isVirtual() const;

    /// @brief Checks whether the key is the Enter key.
    /// @return True if the key is Enter.
    bool isEnter() const;

    /// @brief Checks whether the key is the Delete key.
    /// @return True if the key is Delete.
    bool isDelete() const;

    /// @brief Checks whether the key is the Backspace key.
    /// @return True if the key is Backspace.
    bool isBackSpace() const;

    /// @brief Returns the virtual key identifier.
    /// @return Virtual key enumeration value.
    Virtual getVirtual() const;

    /// @brief Returns the raw UTF-32 key code.
    /// @return Key code as td::UINT4.
    td::UINT4 getCode() const;
};

} //namespace gui
