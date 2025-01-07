// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  WebSocket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/Command.h>

namespace no
{

namespace WebSocket
{
enum class Frame : unsigned char {Continuation=0, ContinuationFin, TxtSequence, TxtFinal, BinSequence, BinFinal, Incomplete, ConnClose, Ping, Pong, Error};

class Message
{
public:
    enum class Type : unsigned char {Continuation=0, ContinuationFin, TxtSequence, TxtFinal, BinSequence, BinFinal};
    enum class Consumer : unsigned char {StandardClient=0, CustomClient, Server};
    const char* pMsg = nullptr;
    td::UINT4 len = 0;
    Command cmd;
    Type type = Type::TxtFinal;
    
    Message()
    : pMsg(nullptr)
    , type(Type::TxtFinal)
    {
    }
    
    Message(Command cmdIn, const char* pMsgIn, td::UINT4 lenIn, Type typeIn = Type::TxtFinal)
    : pMsg(pMsgIn)
    , len(lenIn)
    , cmd(cmdIn)
    , type(typeIn)
    {
    }
    
    Message(const char* pMsgIn, td::UINT4 lenIn, Type typeIn = Type::TxtFinal)
    : pMsg(pMsgIn)
    , len(lenIn)
    , type(typeIn)
    {
    }
    
    Message(const Message& msgIn, bool detectComand)
    : pMsg(msgIn.pMsg)
    , len(msgIn.len)
    , cmd(msgIn.cmd)
    , type(msgIn.type)
    {
        if (detectComand)
        {
            if (msgIn.len >= cmd.length())
            {
                memcpy(cmd._cmdStr, pMsg, cmd.length());
                if (msgIn.len == 4)
                    pMsg = nullptr;
                else
                    pMsg += cmd.length();
                len -= cmd.length();
            }
        }
    }
    
    Message(Command cmdIn, const td::String& str, Type typeIn = Type::TxtFinal)
    : pMsg(str.c_str())
    , len(str.length())
    , cmd(cmdIn)
    , type(typeIn)
    {
    }
    
    Message(const td::String& str, Type typeIn = Type::TxtFinal)
    : pMsg(str.c_str())
    , len(str.length())
    , type(typeIn)
    {
    }
    
    Command getCommand() const
    {
        return cmd;
    }
};

} //namespace WebSocket

} //namespace no
