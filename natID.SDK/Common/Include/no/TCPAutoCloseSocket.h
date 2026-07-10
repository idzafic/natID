// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TCPAutoCloseSocket.h
 * @brief RAII TCP socket that automatically closes itself on destruction.
 */
//
//  TCPSocket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/TCPSocket.h>

namespace no
{

/// @brief Extends TCPSocket with automatic close semantics: the socket is closed
///        when the object goes out of scope.
class TCPAutoCloseSocket : public TCPSocket
{

protected:
    /// @brief Default constructor; creates an invalid, unclosed socket.
    TCPAutoCloseSocket()
    {}

public:
    /// @brief Constructs a TCPAutoCloseSocket that wraps an existing OS socket ID.
    /// @param sockID OS socket file descriptor to wrap.
    TCPAutoCloseSocket(int sockID)
    : TCPSocket(sockID)
    {}

    /// @brief Copy constructor; copies the socket descriptor from another TCPSocket.
    /// @param sock Source TCPSocket to copy.
    TCPAutoCloseSocket(const TCPSocket& sock)
    : TCPSocket(sock)
    {}

    /// @brief Assignment operator; replaces the current descriptor with another TCPSocket's.
    /// @param sock Source TCPSocket to assign from.
    void operator = (const TCPSocket& sock)
    {
        TCPSocket::operator = (sock);
    }

    /// @brief Destructor; automatically closes the underlying socket.
    ~TCPAutoCloseSocket()
    {
        TCPSocket::close();
    }

};

} //namespace no
