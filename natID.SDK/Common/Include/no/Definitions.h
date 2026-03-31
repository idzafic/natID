// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  IConnector.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once

namespace no
{

typedef td::UINT4 TSockPos;

enum class DisconnectReason : unsigned char {Other=0, IntentionalClose, ClosedByRemote, ReadingTimeOut, WritingTimeout, ReadingAndWritingTimeout, ErrorAsyncRead, ErrorAsyncWrite, ReadingError, WritingError, CannotObtainHeader, WrongHeaderSize, WrongHeader, WrongUserID, WrongPassword, NoWebSocketManager, WrongWebSocketHandshake, FailedAuthentication, WrongWebSocketFrame, WrongPong, WebSocketClosedByRemote, WebSockeAppOnConnectDecision, SuccessfulUpgradeToWebSocket};

namespace HTTP
{
enum class Request : unsigned char {UNKNOWN=0, GET, POST};
enum class Version : unsigned char {UNKNOWN=0, v1_0=10, v1_1=11, v2=20, v3=30};
const td::UINT2 MAX_HEADER_LEN = 4096;

const td::UINT2 DefaultKeepAliveTimeOut = 60000; //keep-alive timeout (2 seconds)

const td::UINT2 MaxPasswordLength = 32;

namespace WebSocket
{
enum class State : td::BYTE {NotConnected = 0, LoginStage1, LoginStage2, LoginStage3, Connected};
}

} //namespace HTTP

} //namespace no
