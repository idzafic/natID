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

namespace gui
{

class KeyHelper;
class InputDevice;

class NATGUI_API Key
{
    friend class KeyHelper;
    friend class InputDevice;
    
public:
    enum class Type : unsigned char {NA=0, ASCII, Virtual, Other};
    enum class Virtual : unsigned char {NA=0, Tab, Enter, Left, Right, Up, Down, Home, End, PgUp, PgDown, Insert, Delete,
                    Esc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, PrintScreen, ScrollLock, Pause, Break, BackSpace, VolumeUp, VolumeDown, Mute,
                    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, NumDecimal, NumEnter, NumPlus, NumMinus, NumMultiply, NumDivide,
                    NumInsert, NumDelete, NumEnd, NumDown, NumPgDown, NumLeft, NumRight, NumHome, NumUp, NumPgUp};
private:
    td::UINT4 _keyCode = 0; //UTF32
    td::UINT4 _modifiers = 0;
    td::UINT2 _nEvents = 0;
    td::BYTE _extendedMask = 0;
    char _ascii = 0;
    Type _type = Type::NA;
    Virtual _virtual = Virtual::NA;
private:
    void setCode(td::UINT4 keyCode, td::UINT2 nEvents = 1);
    void setVirtual(td::UINT4 keyCode, td::UINT2 nEvents, Virtual virtualKey);
    
public:
    Key();
    Key(td::UINT4 keyCode, td::UINT4 keyModifiers, td::UINT2 nEvents, td::BYTE extendedMask=0);
    Key(Type type, Virtual virtID, char ascii = 0); //for simulations
    
    Key::Type getType() const;
    td::UINT4 getModifiers() const;
    td::UINT2 getNoOfEvents() const;
    bool isGlyph() const;
    bool isAnyModifierPressed() const;
    bool isAltCtrlOrCmdPressed() const;
    bool isCmdPressed() const;
    bool isCmdOnMacOrCtrlOnOtherPressed() const;
    bool isCtrlPressed() const;
    bool isAltPressed() const;
    bool isFnPressed() const;
    bool isShiftPressed() const;
    bool isCapsLockPressed() const;
    bool isASCII() const;
    char getChar() const;
    bool isVirtual() const;
    bool isEnter() const;
    bool isDelete() const;
    bool isBackSpace() const;
    Virtual getVirtual() const;
    td::UINT4 getCode() const;
};

} //namespace gui
