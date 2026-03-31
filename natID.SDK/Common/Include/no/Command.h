// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class Command
{
    friend class Message;
    
    union
    {
        char _cmdStr[4];
        td::UINT4 _cmdVal;
    };
    
public:
    
    Command()
    : _cmdVal(0)
    {
    }
    
    Command(const Command& cmd)
    : _cmdVal(cmd._cmdVal)
    {
    }
    
    Command(td::UINT4 cmdVal)
    : _cmdVal(cmdVal)
    {
    }
    
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
    
    inline const Command& operator = (td::UINT4 val)
    {
        _cmdVal = val;
        return *this;
    }
    
    inline bool operator == (const Command& cmd) const
    {
        return (_cmdVal == cmd._cmdVal);
    }
    
    inline bool operator != (const Command& cmd) const
    {
        return (_cmdVal != cmd._cmdVal);
    }
    
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
    
    constexpr td::UINT4 length() const
    {
        return 4;
    }
    
    const char* getStr() const
    {
        return &_cmdStr[0];
    }
    
    bool isOk() const
    {
        return (_cmdVal != 0);
    }
    
    td::UINT4 getVal() const
    {
        return _cmdVal;
    }
    
};

} //namespace WebSocket

} //namespace no
