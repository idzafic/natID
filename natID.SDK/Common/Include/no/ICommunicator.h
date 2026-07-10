// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ICommunicator.h
 * @brief Abstract interface for asynchronous network I/O communicators.
 */
//
//  ICommunicator.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/SockDesc.h>
#include <mem/Buffer.h>
#include <no/IProsumer.h>
#include <td/Variant.h>
#include <tuple>

namespace no
{

/// @brief Pure-virtual interface that defines the contract for all asynchronous
///        network communicators (both client-side and server-side).
class ICommunicator
{
public:
    //virtual void init(td::UINT2 nMaxEntries) = 0;

    /// @brief Starts the communicator and binds/connects to the given address and port.
    /// @param pAddress Null-terminated string with the local or remote address.
    /// @param portNo Port number to bind or connect to.
    /// @param additionalInfo Variant carrying any extra start-up parameters.
    /// @return true on success, false on failure.
    virtual bool start(const char* pAddress, td::UINT2 portNo, const td::Variant& additionalInfo) = 0;

    /// @brief Registers a new socket entry with the communicator.
    /// @param sockDesc Descriptor of the socket to add.
    /// @return true if the entry was added successfully, false otherwise.
    virtual bool addEntry(no::SockDesc& sockDesc) = 0; //returns reading buffer

    /// @brief Retrieves the socket descriptor for the current active socket.
    /// @param sockDesc Output descriptor to be filled.
    /// @return true if a valid descriptor was retrieved, false otherwise.
    virtual bool getSockDesc(no::SockDesc& sockDesc) = 0;

    /// @brief Removes a previously registered socket entry from the communicator.
    /// @param sockDesc Descriptor identifying the entry to remove.
    /// @param reason Reason why the socket is being disconnected.
    virtual void removeEntry(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;

    /// @brief Schedules an asynchronous fixed-size read operation.
    /// @param sockDesc Descriptor of the socket to read from.
    /// @param nBytesToRead Exact number of bytes to read.
    /// @param timeOutInMs Optional timeout in milliseconds (0 = no timeout).
    /// @param varUserData Optional user-defined data forwarded to the completion callback.
    /// @return true if the operation was queued successfully, false otherwise.
    virtual bool asyncRead(const no::SockDesc& sockDesc, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    //virtual bool asyncRead(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;

    /// @brief Schedules an asynchronous read that returns as soon as any data is available.
    /// @param sockDesc Descriptor of the socket to read from.
    /// @param timeOutInMs Optional timeout in milliseconds (0 = no timeout).
    /// @param varUserData Optional user-defined data forwarded to the completion callback.
    /// @return true if the operation was queued successfully, false otherwise.
    virtual bool asyncReadSome(const no::SockDesc& sockDesc, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;
    //virtual bool asyncReadSome(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;

    /// @brief Continues an asynchronous fixed-size read into an existing buffer.
    /// @param sockDesc Descriptor of the socket to read from.
    /// @param buffer Buffer into which data will be appended.
    /// @param nBytesToRead Number of additional bytes to read.
    /// @param timeOutInMs Optional timeout in milliseconds.
    /// @param varUserData Optional user-defined data forwarded to the completion callback.
    /// @return true if the continuation was queued successfully, false otherwise.
    virtual bool asyncReadCont(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 nBytesToRead, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;

    /// @brief Continues an asynchronous partial read into an existing buffer.
    /// @param sockDesc Descriptor of the socket to read from.
    /// @param buffer Buffer into which available data will be appended.
    /// @param timeOutInMs Optional timeout in milliseconds.
    /// @param varUserData Optional user-defined data forwarded to the completion callback.
    /// @return true if the continuation was queued successfully, false otherwise.
    virtual bool asyncReadSomeCont(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;

    /// @brief Schedules an asynchronous write of the contents of a buffer.
    /// @param sockDesc Descriptor of the socket to write to.
    /// @param buffer Buffer whose contents will be sent.
    /// @param timeOutInMs Optional timeout in milliseconds.
    /// @param varUserData Optional user-defined data forwarded to the completion callback.
    /// @return true if the write was queued successfully, false otherwise.
    virtual bool asyncWrite(const no::SockDesc& sockDesc, mem::Buffer& buffer, td::UINT4 timeOutInMs = 0, const td::Variant& varUserData = td::Variant()) = 0;

    //virtual bool shouldClose(const no::SockDesc& sockDesc, no::DisconnectReason reason) = 0;
    //virtual void releaseBuffer(mem::Buffer* pBuffer) = 0;

    /// @brief Returns the current number of registered socket entries.
    /// @return Count of active entries managed by this communicator.
    virtual TSockPos getNumberOfEntries() const = 0;

    //stop at the next possible moment
    /// @brief Requests a graceful shutdown at the next convenient point.
    virtual void shutDown() = 0;

    //stop immediatelly
    /// @brief Stops the communicator immediately, aborting any pending operations.
    virtual void stop() = 0;

    /// @brief Releases all resources held by the communicator.
    virtual void release() = 0;

    /// @brief Returns an empty, reusable memory buffer.
    /// @return A newly allocated empty Buffer object.
    virtual mem::Buffer getEmptyBuffer() = 0;

    /// @brief Constructs a buffer by concatenating an array of strings.
    /// @param strings Pointer to an array of td::String objects to concatenate.
    /// @param nStrings Number of strings in the array.
    /// @return A Buffer containing the concatenated string data.
    virtual mem::Buffer makeBuffer(const td::String* strings, int nStrings) = 0;

    /// @brief Provides access to the shared auxiliary read/write buffer.
    /// @return A tuple of (pointer to buffer, buffer length in bytes).
    virtual std::tuple<char*, td::UINT4> getReadWriteAuxilaryBuffer() = 0;
};

} //namespace no
