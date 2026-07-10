// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TCPServer.h
 * @brief TCP server socket that listens for and accepts incoming connections.
 */
//
//  TCPServer.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/Socket.h>
#include <td/String.h>

namespace no
{

/// @brief Listening TCP server socket that binds to a local address and accepts clients.
class NETOP_API TCPServer : public Socket
{
protected:
    /// @brief Copy constructor is disabled; TCPServer is non-copyable.
    /// @param server Source server (not usable).
    TCPServer(const TCPServer& server);

    /// @brief Assignment operator is disabled; TCPServer is non-copyable.
    /// @param server Source server (not usable).
    void operator = (const TCPServer& server);
public:
    /// @brief Default constructor; creates an unbound server socket.
    TCPServer();

    /// @brief Destructor; stops listening and closes the socket.
    ~TCPServer();

    /// @brief Binds to a local address and starts listening for incoming connections.
    /// @param localAddress Null-terminated local IP address to bind (nullptr or "" for INADDR_ANY).
    /// @param localPort Local port number to listen on.
    /// @param queueLen Maximum number of pending connections in the accept queue.
    /// @param nonBlocking If true the socket operates in non-blocking mode.
    /// @return true if binding and listening succeeded, false otherwise.
    bool start(const char* localAddress, td::UINT2 localPort, int queueLen = 5, bool nonBlocking = false);

    /// @brief Binds to a local address (td::String) and starts listening.
    /// @param localAddress Local IP address string to bind.
    /// @param localPort Local port number to listen on.
    /// @param queueLen Maximum number of pending connections in the accept queue.
    /// @param nonBlocking If true the socket operates in non-blocking mode.
    /// @return true if binding and listening succeeded, false otherwise.
    bool start(const td::String& localAddress, td::UINT2 localPort, int queueLen = 5, bool nonBlocking = false);

    /// @brief Accepts the next pending client connection.
    /// @return OS socket file descriptor for the accepted client, or -1 on error.
    int accept();

    /// @brief Stops listening and closes the server socket.
    void stop();
};

} //namespace no
