// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SockDesc.h
 * @brief Socket descriptor structure carrying identity information for a managed socket.
 */
//
//  IP4Address.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <td/DateTime.h>
#include <no/Definitions.h>

namespace no
{

/// @brief Lightweight value type that uniquely identifies a socket within a communicator.
class SockDesc
{
public:
    int socketID;           ///< Underlying OS socket file descriptor (-1 if not connected).
    TSockPos sockEntryPos;  ///< Position of this socket in the communicator's entry table.
    //TSockPos sockPos;
    TSockPos webSockPos;    ///< WebSocket slot index (0 if this is a plain TCP socket).
    td::BYTE consumerPos;   ///< Index of the IProsumer that owns this socket (1-based).
    td::BYTE version;       ///< Version counter used to detect stale descriptors.

    /// @brief Constructs a descriptor for an optionally supplied socket ID.
    /// @param sockID OS socket file descriptor; defaults to -1 (not connected).
    SockDesc(int sockID = -1)
    : socketID(sockID)
    , sockEntryPos(0)
    //, sockPos(0)
    , webSockPos(0)
    , consumerPos(0)
    , version(0)
    //, webSock(0)
    {
    }

    /// @brief Constructs a fully-specified socket descriptor.
    /// @param sockID OS socket file descriptor.
    /// @param entryPos Position in the communicator's entry table.
    /// @param webSockPos WebSocket slot index (0 for plain TCP).
    /// @param consumPos IProsumer index that owns this socket.
    /// @param versionNo Version counter value.
    SockDesc(int sockID, TSockPos entryPos, TSockPos webSockPos, td::BYTE consumPos, td::BYTE versionNo)
    //: creationDT(creationDateTime)
    : socketID(sockID)
    , sockEntryPos(entryPos)
    , webSockPos(webSockPos)
    , consumerPos(consumPos)
    , version(versionNo)
    {
    }


    //td::BYTE webSock;

    /// @brief Equality operator; compares socket ID, entry position, and WebSocket position.
    /// @param sd Other descriptor to compare against.
    /// @return true if all identifying fields are equal, false otherwise.
    bool operator == (const SockDesc& sd)
    {
        return (socketID == sd.socketID && sockEntryPos == sd.sockEntryPos && webSockPos == sd.webSockPos);
    }

    /// @brief Assignment operator; copies all fields from another descriptor.
    /// @param sd Source descriptor.
    /// @return Const reference to this descriptor.
    const SockDesc& operator = (const SockDesc& sd)
    {
        //creationDT = sd.creationDT;
        socketID = sd.socketID;
        sockEntryPos = sd.sockEntryPos;
        //sockPos = sd.sockPos;
        webSockPos = sd.webSockPos;
        consumerPos = sd.consumerPos;
        version = sd.version;
        //webSock = sd.webSock;
        return *this;
    }

    /// @brief Returns whether the socket is currently connected.
    /// @return true if socketID is a non-negative value, false otherwise.
    bool isConnected() const
    {
        return (socketID >= 0);
    }

    /// @brief Returns whether this descriptor refers to a WebSocket connection.
    /// @return true if webSockPos is non-zero, false for plain TCP.
    bool isWebSocket() const
    {
        return (webSockPos > 0);
    }

    /// @brief Resets the identifying fields to their disconnected defaults.
    void clean()
    {
        //creationDT = td::DateTime();
        socketID = -1;
        sockEntryPos = 0;
        //sockPos = 0;
        consumerPos = 0;
        version = 0;
        //webSockPos = 0;
        //webSock = 0;
    }

};

} //namespace no
