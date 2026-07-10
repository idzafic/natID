// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Definitions.h
    @brief Defines common type aliases, enumerations, and constants used throughout the network operations library. */
//
//  IConnector.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once

namespace no
{

typedef td::UINT4 TSockPos; ///< Type alias for socket position index, stored as a 32-bit unsigned integer.

/// @brief Enumerates the reasons why a socket connection was disconnected.
enum class DisconnectReason : unsigned char {Other=0, ///< Unspecified or unknown disconnection reason.
    IntentionalClose, ///< The connection was intentionally closed by the local side.
    ClosedByRemote, ///< The connection was closed by the remote end.
    ReadingTimeOut, ///< A read operation timed out.
    WritingTimeout, ///< A write operation timed out.
    ReadingAndWritingTimeout, ///< Both read and write operations timed out.
    ErrorAsyncRead, ///< An error occurred during an asynchronous read operation.
    ErrorAsyncWrite, ///< An error occurred during an asynchronous write operation.
    ReadingError, ///< A general error occurred during reading.
    WritingError, ///< A general error occurred during writing.
    CannotObtainHeader, ///< Failed to obtain the protocol header.
    WrongHeaderSize, ///< The header had an incorrect size.
    WrongHeader, ///< The header content was invalid.
    WrongUserID, ///< The provided user ID was invalid.
    WrongPassword, ///< The provided password was incorrect.
    NoWebSocketManager, ///< No WebSocket manager was available to handle the connection.
    WrongWebSocketHandshake, ///< The WebSocket handshake was invalid.
    FailedAuthentication, ///< Authentication failed.
    WrongWebSocketFrame, ///< A WebSocket frame with an invalid format was received.
    WrongPong, ///< An invalid or unexpected pong frame was received.
    WebSocketClosedByRemote, ///< The WebSocket was closed by the remote end.
    WebSockeAppOnConnectDecision, ///< The application decided to reject the connection during the connect callback.
    SuccessfulUpgradeToWebSocket ///< The connection was successfully upgraded to WebSocket.
};

namespace HTTP
{
/// @brief Enumerates supported HTTP request methods.
enum class Request : unsigned char {UNKNOWN=0, ///< Unknown or unsupported request type.
    GET, ///< HTTP GET request.
    POST ///< HTTP POST request.
};

/// @brief Enumerates supported HTTP protocol versions.
enum class Version : unsigned char {UNKNOWN=0, ///< Unknown HTTP version.
    v1_0=10, ///< HTTP version 1.0.
    v1_1=11, ///< HTTP version 1.1.
    v2=20, ///< HTTP version 2.
    v3=30 ///< HTTP version 3.
};

const td::UINT2 MAX_HEADER_LEN = 4096; ///< Maximum allowed HTTP header length in bytes.

const td::UINT2 DefaultKeepAliveTimeOut = 60000; ///< Default keep-alive timeout in milliseconds (60 seconds).

const td::UINT2 MaxPasswordLength = 32; ///< Maximum allowed password length in characters.

namespace WebSocket
{
/// @brief Enumerates the connection states for a WebSocket upgrade flow.
enum class State : td::BYTE {NotConnected = 0, ///< Not yet connected.
    LoginStage1, ///< First stage of the login/authentication sequence.
    LoginStage2, ///< Second stage of the login/authentication sequence.
    LoginStage3, ///< Third stage of the login/authentication sequence.
    Connected ///< Fully connected and authenticated WebSocket connection.
};
}

} //namespace HTTP

} //namespace no
