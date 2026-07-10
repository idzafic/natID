// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file netOp.h
 *  @brief Public API for creating network communicators and performing security operations in the no layer.
 */
//
//  netOp.h
//
//  Created by Izudin Dzafic on 10/10/20.
//  Copyright 2021 Izudin Dzafic. All rights reserved.
//

#pragma once
#include <no/netOpLib.h>
#include <no/NetworkInterfaces.h>
#include <no/SockDesc.h>

namespace no
{
class ICommunicator;
class IConnector;
class IAcceptor;
class IProsumer;

/// @brief Selects the internal memory allocator strategy used by communicator socket buffers.
enum class Allocator : unsigned char {Small=0, ///< Small buffer allocator for low-traffic connections.
                                       Medium,  ///< Medium buffer allocator for moderate traffic.
                                       Large    ///< Large buffer allocator for high-throughput connections.
};

//collect all newtork cards and interfaces
/// @brief Enumerates all available network cards and interfaces on the host machine.
/// @param nifs              Output structure populated with discovered network interface data.
/// @param populateWiFiInfo  If true, additional Wi-Fi specific information is gathered.
/// @return True on success, false if enumeration failed.
NETOP_API bool getNetworkInterfaces(NetworkInterfaces& nifs, bool populateWiFiInfo);

//create multiplexing communicators
/// @brief Creates a multiplexing client-side communicator that manages multiple outgoing connections.
/// @param nMaxSocketEntries     Maximum number of simultaneous socket entries managed by this communicator.
/// @param sizeOfReadWriteBuffer Size in bytes of the read/write buffer allocated per socket.
/// @param pProsumer             Array of prosumer interface pointers that handle connection events.
/// @param nProsumers            Number of entries in the pProsumer array.
/// @param allocType             Buffer allocator strategy (defaults to Small).
/// @return Pointer to the newly created ICommunicator, or nullptr on failure.
NETOP_API no::ICommunicator* createClientCommunicator(no::TSockPos nMaxSocketEntries, td::UINT4 sizeOfReadWriteBuffer, no::IProsumer** pProsumer, size_t nProsumers, no::Allocator allocType = no::Allocator::Small/*, td::UINT4 sizeOfReadSomeDataChunk = 1024*/);

/// @brief Creates a multiplexing server-side communicator that manages multiple accepted connections.
/// @param nMaxSocketEntries     Maximum number of simultaneous socket entries managed by this communicator.
/// @param sizeOfReadWriteBuffer Size in bytes of the read/write buffer allocated per socket.
/// @param pProsumer             Array of prosumer interface pointers that handle connection events.
/// @param nProsumers            Number of entries in the pProsumer array.
/// @param allocType             Buffer allocator strategy (defaults to Small).
/// @return Pointer to the newly created ICommunicator, or nullptr on failure.
NETOP_API no::ICommunicator* createServerCommunicator(no::TSockPos nMaxSocketEntries, td::UINT4 sizeOfReadWriteBuffer, no::IProsumer** pProsumer, size_t nProsumers, no::Allocator allocType = no::Allocator::Small/*, td::UINT4 sizeOfReadSomeDataChunk = 1024*/);

//ecrypt/decrypt (security) methods
namespace secu
{
/// @brief Fills a buffer with cryptographically suitable random bytes for use as keys or tokens.
/// @param buffer Output buffer to fill with random data.
/// @param len    Number of random bytes to generate.
/// @return Number of bytes written on success, or a negative error code.
NETOP_API int genString(unsigned char* buffer, int len);

/// @brief Encrypts plaintext using AES with the provided key and initialisation vector.
/// @param plaintext      Pointer to the input plaintext data.
/// @param plaintextLen   Length of the plaintext in bytes.
/// @param key            Encryption key buffer (must match the expected key length).
/// @param iv             Initialisation vector buffer.
/// @param outCipherText  Output buffer that receives the encrypted ciphertext.
/// @param outBuffLen     Size in bytes of the outCipherText buffer.
/// @return Length of the produced ciphertext in bytes, or a negative error code.
NETOP_API int encrypt(const unsigned char* plaintext, int plaintextLen, const unsigned char* key, const unsigned char* iv, unsigned char* outCipherText, int outBuffLen);

/// @brief Decrypts ciphertext using AES with the provided key and initialisation vector.
/// @param ciphertext     Pointer to the input ciphertext data.
/// @param ciphertextLen  Length of the ciphertext in bytes.
/// @param key            Decryption key buffer (must match the expected key length).
/// @param iv             Initialisation vector buffer used during encryption.
/// @param outPlainText   Output buffer that receives the decrypted plaintext.
/// @return Length of the produced plaintext in bytes, or a negative error code.
NETOP_API int decrypt(const unsigned char* ciphertext, int ciphertextLen, const unsigned char* key, const unsigned char* iv, unsigned char* outPlainText);
} //namespace secu

} //namespace no
