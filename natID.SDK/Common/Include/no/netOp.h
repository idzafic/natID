// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

enum class Allocator : unsigned char {Small=0, Medium, Large};

//collect all newtork cards and interfaces
NETOP_API bool getNetworkInterfaces(NetworkInterfaces& nifs, bool populateWiFiInfo);

//create multiplexing communicators
NETOP_API no::ICommunicator* createClientCommunicator(no::TSockPos nMaxSocketEntries, td::UINT4 sizeOfReadWriteBuffer, no::IProsumer** pProsumer, size_t nProsumers, no::Allocator allocType = no::Allocator::Small/*, td::UINT4 sizeOfReadSomeDataChunk = 1024*/);
NETOP_API no::ICommunicator* createServerCommunicator(no::TSockPos nMaxSocketEntries, td::UINT4 sizeOfReadWriteBuffer, no::IProsumer** pProsumer, size_t nProsumers, no::Allocator allocType = no::Allocator::Small/*, td::UINT4 sizeOfReadSomeDataChunk = 1024*/);

//ecrypt/decrypt (security) methods
namespace secu
{
NETOP_API int genString(unsigned char* buffer, int len);
NETOP_API int encrypt(const unsigned char* plaintext, int plaintextLen, const unsigned char* key, const unsigned char* iv, unsigned char* outCipherText, int outBuffLen);
NETOP_API int decrypt(const unsigned char* ciphertext, int ciphertextLen, const unsigned char* key, const unsigned char* iv, unsigned char* outPlainText);
} //namespace secu

} //namespace no
