// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Socket.h
 * @brief Base socket class wrapping a platform OS socket descriptor.
 */
//
//  Socket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <td/String.h>

namespace no
{

/// @brief IP protocol version selector.
enum class Protocol : td::BYTE {IPv4=0, IPv6};

/// @brief Thin RAII wrapper around a platform socket file descriptor.
class NETOP_API Socket
{
public:
    /// @brief Transport protocol type.
    enum class Type : td::BYTE {TCP=0, UDP};
protected:
	int _socketID; ///< Underlying OS socket file descriptor (-1 when invalid).
    //td::BYTE _autoClose : 1;

protected:

public:
    /// @brief Default constructor; creates an invalid (closed) socket.
    Socket();

    /// @brief Constructs a Socket wrapping an existing OS socket descriptor.
    /// @param sockID OS socket file descriptor to wrap.
    Socket(int sockID);

    /// @brief Copy constructor; copies the socket descriptor (no duplication at OS level).
    /// @param sock Source socket to copy from.
    Socket(const Socket& sock);

    /// @brief Destructor; does not automatically close the socket.
    ~Socket();

    /// @brief Closes the underlying OS socket.
    void close();

    /// @brief Creates a new OS socket for the given protocol and transport type.
    /// @param ipProtocol IP version (IPv4 or IPv6).
    /// @param type Transport type (TCP or UDP).
    /// @return true if the socket was created successfully, false otherwise.
    bool create(Protocol ipProtocol, Type type);

    /// @brief Assignment operator; copies the socket descriptor.
    /// @param sock Source socket to copy from.
    void operator = (const Socket& sock);

    /// @brief Returns the underlying OS socket file descriptor.
    /// @return Socket file descriptor, or -1 if the socket is not open.
    int getID() const;

    //void setAutoClose(bool autoClose);
    //bool isAutoClose() const;

    /// @brief Returns whether the socket is currently open and valid.
    /// @return true if the socket descriptor is valid, false otherwise.
    bool isOK() const;

    /// @brief Puts the socket into non-blocking mode.
    /// @return true if the operation succeeded, false otherwise.
    bool makeNonBlocking();
};

} //namespace no
