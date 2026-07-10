// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TCPSocket.h
 * @brief TCP socket with synchronous send/receive and address-query helpers.
 */
//
//  TCPSocket.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/Socket.h>
#include <no/NetworkInterfaces.h>
#include <td/String.h>
#include <mem/Buffer.h>

namespace no
{

class TCPServer;

/// @brief Extends Socket with TCP-specific send, receive, and address-query operations.
class NETOP_API TCPSocket : public Socket
{
    friend class TCPServer;

protected:
    /// @brief Default constructor; creates an invalid TCP socket.
    TCPSocket();

    /// @brief Puts the socket into non-blocking mode (internal helper).
    void setAsNonBlocking();
public:
    /// @brief Constructs a TCPSocket wrapping an existing OS socket descriptor.
    /// @param sockID OS socket file descriptor to wrap.
    TCPSocket(int sockID);

    /// @brief Copy constructor; copies the socket descriptor.
    /// @param sock Source TCPSocket to copy.
    TCPSocket(const TCPSocket& sock);

    /// @brief Assignment operator; copies the socket descriptor.
    /// @param sock Source TCPSocket to assign from.
    void operator = (const TCPSocket& sock);

    /// @brief Synchronously receives up to @p nToReceive bytes into a raw buffer.
    /// @param buffer Destination buffer for received data.
    /// @param nToReceive Maximum number of bytes to receive.
    /// @return Number of bytes actually received, or a negative value on error.
	ssize_t receive(char* buffer, ssize_t nToReceive) const;

    /// @brief Synchronously receives data into a mem::Buffer object.
    /// @param buffer Destination buffer; filled with available data.
    /// @return Number of bytes received, or a negative value on error.
    ssize_t receive(mem::Buffer& buffer);

    /// @brief Synchronously receives exactly @p nToReceive bytes and returns them as a string.
    /// @param nToReceive Number of bytes to receive.
    /// @return td::String containing the received bytes.
	td::String receive(ssize_t nToReceive) const;

    /// @brief Synchronously sends bytes from a raw buffer.
    /// @param buffer Source buffer containing data to send.
    /// @param nToWrite Number of bytes to send from @p buffer.
    /// @return Number of bytes actually sent, or a negative value on error.
	ssize_t send(const char* buffer, ssize_t nToWrite);

    /// @brief Synchronously sends the contents of a mem::Buffer.
    /// @param buffer Buffer whose data will be sent.
    /// @return Number of bytes sent, or a negative value on error.
    ssize_t send(mem::Buffer& buffer);

    /// @brief Synchronously sends a td::String.
    /// @param str String to send.
    /// @return Number of bytes sent, or a negative value on error.
    ssize_t send(const td::String& str);

    /// @brief Returns whether the socket is in non-blocking mode.
    /// @return true if non-blocking, false if blocking.
    bool isNonBlocking() const;

    /// @brief Looks up the MAC address of the local interface via the OS.
    /// @return MAC address as a formatted td::String.
    td::String getMACAddress() const;

    /// @brief Looks up the MAC address using a pre-built network-interface list.
    /// @param nifs Collection of network interfaces to search.
    /// @return MAC address as a formatted td::String.
    td::String getMACAddress(const no::NetworkInterfaces& nifs) const;

    /// @brief Retrieves the local IP address and port number for this socket.
    /// @param ipAddress Output string to receive the local IP address.
    /// @param portNo Output variable to receive the local port number.
    /// @return true on success, false if the query failed.
    bool getLocalIPAddressAndPortNumber(td::String& ipAddress, td::UINT2& portNo) const;

    /// @brief Returns the local IP address for this socket.
    /// @return Local IP address as a td::String.
    td::String getLocalIPAddress() const;

    /// @brief Returns the local port number for this socket.
    /// @return Local port number.
    td::UINT2 getLocalPortNumber() const;

    /// @brief Retrieves the remote (foreign) IP address and port number.
    /// @param ipAddress Output string to receive the remote IP address.
    /// @param portNo Output variable to receive the remote port number.
    /// @return true on success, false if the query failed.
    bool getForeignIPAddressAndPortNumber(td::String& ipAddress, td::UINT2& portNo) const;

    /// @brief Returns the remote IP address for this socket.
    /// @return Remote IP address as a td::String.
    td::String getForeignIPAddress() const;

    /// @brief Returns the remote port number for this socket.
    /// @return Remote port number.
    td::UINT2 getForeignPortNumber() const;

};

} //namespace no
