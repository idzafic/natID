// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TCPClient.h
 * @brief TCP client socket with synchronous connect support.
 */
//
//  TCPClient.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/TCPSocket.h>

namespace no
{
	/// @brief TCP client socket that adds hostname-based connect operations to TCPSocket.
	class NETOP_API TCPClient : public TCPSocket
	{
	private:

	public:
        /// @brief Default constructor; creates an unconnected TCP client socket.
        TCPClient();

        /// @brief Constructs a TCPClient wrapping an existing OS socket descriptor.
        /// @param sockID OS socket file descriptor to wrap.
        TCPClient(int);

        /// @brief Destructor; releases any resources held by the client socket.
        ~TCPClient();

        /// @brief Connects to a remote host by C-string hostname and port.
        /// @param hostName Null-terminated hostname or dotted-decimal IP address.
        /// @param portNo Remote port number to connect to.
        /// @param nonBlocking If true the socket is placed into non-blocking mode before connecting.
        /// @return true if the connection was established (or initiated for non-blocking), false on error.
        bool connect(const char* hostName, td::UINT2 portNo, bool nonBlocking = false);

        /// @brief Connects to a remote host by td::String hostname and port.
        /// @param hostName Hostname or dotted-decimal IP address string.
        /// @param portNo Remote port number to connect to.
        /// @param nonBlocking If true the socket is placed into non-blocking mode before connecting.
        /// @return true if the connection was established (or initiated for non-blocking), false on error.
        bool connect(const td::String& hostName, td::UINT2 portNo, bool nonBlocking = false);
	};
}
