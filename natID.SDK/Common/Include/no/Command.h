// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Command.h
    @brief Defines the WebSocket Command type representing a 4-byte command identifier. */
//
//  Header.h
//
//  Created by Izudin Dzafic on 10/10/22.
//  Copyright 2022 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <cassert>
#include <td/Types.h>
//#include <td/String.h>
//#include <map>

namespace no
{
namespace WebSocket
{

class Message;

/// @brief Represents a 4-byte WebSocket command identifier, stored as either a 4-character string or a 32-bit unsigned integer.
class Command
{
    friend class Message;

    union
    {
        char _cmdStr[4]; ///< Command represented as a 4-character string.
        td::UINT4 _cmdVal; ///< Command represented as a 32-bit unsigned integer value.
    };

public:

    /// @brief Default constructor. Initializes the command to zero (empty/invalid command).
    Command()
    : _cmdVal(0)
    {
    }

    /// @brief Copy constructor.
    /// @param cmd The command to copy.
    Command(const Command& cmd)
    : _cmdVal(cmd._cmdVal)
    {
    }

    /// @brief Constructs a command from a 32-bit unsigned integer value.
    /// @param cmdVal The integer value representing the command.
    Command(td::UINT4 cmdVal)
    : _cmdVal(cmdVal)
    {
    }

    /// @brief Constructs a command from a C-string. The string must be exactly 4 characters long.
    /// @param pStrCmd Pointer to the 4-character command string.
    Command (const char* pStrCmd)
    {
        auto nLen = strlen(pStrCmd);
        assert(nLen == 4);
        _cmdVal = 0;
        if (nLen > 4)
            nLen = 4;
        memcpy(_cmdStr, pStrCmd, nLen);
    }

//    template <size_t size>
//    Command(const char(&str)[size])
//    {
//        static_assert(size == 4);
//        memcpy(_cmdStr, str, size);
//    }

    /// @brief Copy assignment operator.
    /// @param cmd The command to copy.
    /// @return Reference to this command.
    inline const Command& operator = (const Command& cmd)
    {
        _cmdVal = cmd._cmdVal;
        return *this;
    }

//    template <size_t size>
//    inline const Command& operator = (const char(&str)[size])
//    {
//        static_assert(size-1 == 4, "Command must be 4 bytes long");
//        memcpy(_cmdStr, str, size);
//        return *this;
//    }

    /// @brief Assigns the command value from a 32-bit unsigned integer.
    /// @param val The integer value to assign.
    /// @return Reference to this command.
    inline const Command& operator = (td::UINT4 val)
    {
        _cmdVal = val;
        return *this;
    }

    /// @brief Equality comparison operator.
    /// @param cmd The command to compare against.
    /// @return True if both commands have the same value.
    inline bool operator == (const Command& cmd) const
    {
        return (_cmdVal == cmd._cmdVal);
    }

    /// @brief Inequality comparison operator.
    /// @param cmd The command to compare against.
    /// @return True if the commands differ.
    inline bool operator != (const Command& cmd) const
    {
        return (_cmdVal != cmd._cmdVal);
    }

    /// @brief Inequality comparison against a C-string command.
    /// @param str The 4-character string to compare against.
    /// @return True if this command differs from the given string command.
    inline bool operator != (const char* str) const
    {
        Command cmd(str);
        return operator != (cmd);
    }

//    template <size_t size>
//    inline bool operator == (const char(&str)[size]) const
//    {
//        Command cmd(str);
//        return operator == (cmd);
//    }

//    template <size_t size>
//    inline bool operator != (const char(&str)[size]) const
//    {
//        Command cmd(str);
//        return operator != (cmd);
//    }

    /// @brief Returns the fixed length of a command in bytes.
    /// @return Always returns 4, since commands are always 4 bytes.
    constexpr td::UINT4 length() const
    {
        return 4;
    }

    /// @brief Returns a pointer to the command string.
    /// @return Pointer to the 4-character command string.
    const char* getStr() const
    {
        return &_cmdStr[0];
    }

    /// @brief Checks whether the command is valid (non-zero).
    /// @return True if the command value is non-zero.
    bool isOk() const
    {
        return (_cmdVal != 0);
    }

    /// @brief Returns the integer value of the command.
    /// @return The 32-bit unsigned integer representation of the command.
    td::UINT4 getVal() const
    {
        return _cmdVal;
    }

};

} //namespace WebSocket

} //namespace no
