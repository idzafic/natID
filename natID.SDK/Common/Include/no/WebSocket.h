// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WebSocket.h
 *  @brief WebSocket frame types, message types, and message container for the no networking layer.
 */
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
/// @brief Represents the type of a raw WebSocket frame as received from the wire.
enum class Frame : unsigned char {Continuation=0, ///< Continuation frame (not final)
                                   ContinuationFin, ///< Final continuation frame
                                   TxtSequence,     ///< Non-final text data frame
                                   TxtFinal,        ///< Final text data frame
                                   BinSequence,     ///< Non-final binary data frame
                                   BinFinal,        ///< Final binary data frame
                                   Incomplete,       ///< Frame data is incomplete
                                   ConnClose,        ///< Connection close frame
                                   Ping,             ///< Ping control frame
                                   Pong,             ///< Pong control frame
                                   Error             ///< Error condition frame
};

/// @brief Encapsulates a WebSocket message along with its type, optional command, and raw payload pointer.
class Message
{
public:
    /// @brief Specifies the sequencing/framing type of a WebSocket message.
    enum class Type : unsigned char {Continuation=0, ///< Continuation (not final)
                                      ContinuationFin, ///< Final continuation
                                      TxtSequence,     ///< Non-final text message
                                      TxtFinal,        ///< Final text message
                                      BinSequence,     ///< Non-final binary message
                                      BinFinal         ///< Final binary message
    };
    /// @brief Identifies the consumer role that will process the message.
    enum class Consumer : unsigned char {StandardClient=0, ///< Standard built-in client consumer
                                          CustomClient,     ///< Custom user-defined client consumer
                                          Server            ///< Server-side consumer
    };
    const char* pMsg = nullptr; ///< Pointer to the raw message payload (not owned).
    td::UINT4 len = 0;          ///< Length in bytes of the payload pointed to by pMsg.
    Command cmd;                ///< Optional command parsed from the message prefix.
    Type type = Type::TxtFinal; ///< Framing/sequencing type of this message.

    /// @brief Default constructor. Creates an empty text-final message with no payload.
    Message()
    : pMsg(nullptr)
    , type(Type::TxtFinal)
    {
    }

    /// @brief Constructs a message with an explicit command, raw pointer payload, and type.
    /// @param cmdIn    Command associated with this message.
    /// @param pMsgIn   Pointer to the raw payload data.
    /// @param lenIn    Length of the payload in bytes.
    /// @param typeIn   Framing type (defaults to TxtFinal).
    Message(Command cmdIn, const char* pMsgIn, td::UINT4 lenIn, Type typeIn = Type::TxtFinal)
    : pMsg(pMsgIn)
    , len(lenIn)
    , cmd(cmdIn)
    , type(typeIn)
    {
    }

    /// @brief Constructs a message from a raw pointer payload without an explicit command.
    /// @param pMsgIn   Pointer to the raw payload data.
    /// @param lenIn    Length of the payload in bytes.
    /// @param typeIn   Framing type (defaults to TxtFinal).
    Message(const char* pMsgIn, td::UINT4 lenIn, Type typeIn = Type::TxtFinal)
    : pMsg(pMsgIn)
    , len(lenIn)
    , type(typeIn)
    {
    }

    /// @brief Copy-constructs a message from another, optionally extracting a leading command prefix.
    /// @param msgIn        Source message to copy from.
    /// @param detectComand If true, the first Command::length() bytes are interpreted as the command.
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

    /// @brief Constructs a message from a td::String with an explicit command and type.
    /// @param cmdIn    Command associated with this message.
    /// @param str      String whose internal buffer provides the payload.
    /// @param typeIn   Framing type (defaults to TxtFinal).
    Message(Command cmdIn, const td::String& str, Type typeIn = Type::TxtFinal)
    : pMsg(str.c_str())
    , len(str.length())
    , cmd(cmdIn)
    , type(typeIn)
    {
    }

    /// @brief Constructs a message from a td::String without an explicit command.
    /// @param str    String whose internal buffer provides the payload.
    /// @param typeIn Framing type (defaults to TxtFinal).
    Message(const td::String& str, Type typeIn = Type::TxtFinal)
    : pMsg(str.c_str())
    , len(str.length())
    , type(typeIn)
    {
    }

    /// @brief Returns the command stored in this message.
    /// @return The Command value parsed from or assigned to this message.
    Command getCommand() const
    {
        return cmd;
    }
};

} //namespace WebSocket

} //namespace no
