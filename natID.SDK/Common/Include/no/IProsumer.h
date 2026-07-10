// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IProsumer.h
 * @brief Pure-virtual producer/consumer interface for asynchronous socket events.
 */
//
//  IProsumer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <mem/Buffer.h>
#include <td/Variant.h>

namespace no
{

class ICommunicator;

/// @brief Abstract interface that an application must implement to receive
///        asynchronous notifications from an ICommunicator.
class IProsumer
{
public:
    /// @brief Connection state enumeration.
    enum class ConnStatus
    {
        NotConnected = 0, ///< Socket is not connected.
        Connected         ///< Socket is connected and ready for I/O.
    };

    /// @brief Called when a new incoming connection requires handling before it is accepted.
    /// @param sockDesc Descriptor of the newly connected socket.
    /// @param userData Variant that may be populated with user-defined data.
    /// @return true to accept the connection, false to reject it.
    virtual bool handleConnect(no::SockDesc& sockDesc, td::Variant& userData) = 0;

    /// @brief Called after a connection has been fully established and accepted.
    /// @param sockDesc Descriptor of the connected socket.
    /// @param userData User-defined data associated with this connection.
    virtual void onConnect(no::SockDesc& sockDesc, const td::Variant& userData) = 0;

    /// @brief Queries whether the given connection should be closed.
    /// @param sockDesc Descriptor of the socket under consideration.
    /// @param reason Reason code that triggered the disconnect check.
    /// @return true to proceed with disconnection, false to keep the connection open.
    virtual bool shouldDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;

    /// @brief Called when a connection has been fully disconnected.
    /// @param sockDesc Descriptor of the now-closed socket.
    /// @param reason Reason for the disconnection.
    virtual void onDisconnect(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;

    /// @brief Called when an exact-size asynchronous read has completed.
    /// @param sockDesc Descriptor of the socket that supplied the data.
    /// @param buffer Buffer containing the received data.
    /// @param varUserData User-defined data that was passed to the read request.
    virtual void onBufferRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) = 0;

    /// @brief Called when a partial asynchronous read has completed.
    /// @param sockDesc Descriptor of the socket that supplied the data.
    /// @param buffer Buffer containing the received data.
    /// @param varUserData User-defined data that was passed to the read request.
    virtual void onBufferReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, const td::Variant& varUserData) = 0;

    /// @brief Called when an asynchronous write has been fully transmitted.
    /// @param sockDesc Descriptor of the socket on which the data was sent.
    /// @param varUserData User-defined data that was passed to the write request.
    virtual void onBufferSent(const no::SockDesc& sockDesc, const td::Variant& varUserData) = 0;

    /// @brief Called periodically when the communicator event loop is idle.
    virtual void onIdle() = 0;
};

} //namespace no
