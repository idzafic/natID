// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Hash.h
 *  @brief Inline hash functions for computing polynomial rolling hash values from C strings.
 */
#pragma once
#include <td/Types.h>

namespace mu
{
	/// @brief Computes a hash number for a null-terminated C string.
	/// @param pStr Pointer to the null-terminated input string.
	/// @return 32-bit hash value computed from all characters in the string.
	inline td::UINT4 calcHashNo(const char* pStr)
	{
		td::UINT4 hashNo = 0;
		while (*pStr)
		{
			td::UINT4 highorder = hashNo & 0xf8000000;    // extract high-order 5 bits from hashNo
			hashNo <<= 5;									 // shift hashNo left by 5 bits
			hashNo ^= (highorder >> 27);					 // move the highorder 5 bits to the low-order
			// end and XOR into hashNo
			hashNo ^= (td::UINT4) (*pStr);
			++pStr;
		}
		return hashNo;
	}

	/// @brief Updates an existing hash value by folding in a new 32-bit value.
	/// @param prevHashNo The previously computed hash value to update.
	/// @param hashUpdate The new 32-bit value to fold into the hash.
	/// @return Updated 32-bit hash value.
	inline td::UINT4 calcHashNo(td::UINT4 prevHashNo, td::UINT4 hashUpdate)
	{
		td::UINT4 hashNo = prevHashNo;

		td::UINT4 highorder = hashNo & 0xf8000000;    // extract high-order 5 bits from hashNo
		hashNo <<= 5;									 // shift hashNo left by 5 bits
		hashNo ^= (highorder >> 27);					 // move the highorder 5 bits to the low-order

		// end and XOR into hashNo
		hashNo ^= hashUpdate;

		return hashNo;
	}

	/// @brief Computes or continues a hash over a fixed-length character buffer.
	/// @param pStr Pointer to the character buffer.
	/// @param nChar Number of characters to process.
	/// @param prevHashNo Starting hash value; defaults to 0 for a fresh computation.
	/// @return 32-bit hash value after processing nChar characters.
	inline td::UINT4 calcHashNo(const char* pStr, int nChar, td::UINT4 prevHashNo = 0)
	{
		td::UINT4 hashNo = prevHashNo;

		for (int i = 0; i < nChar; ++i)
		{
			td::UINT4 highorder = hashNo & 0xf8000000;    // extract high-order 5 bits from hashNo
			hashNo <<= 5;									 // shift hashNo left by 5 bits
			hashNo ^= (highorder >> 27);					 // move the highorder 5 bits to the low-order
			// end and XOR into hashNo
			hashNo ^= (td::UINT4) (pStr[i]);
		}
		return hashNo;
	}

	/// @brief Computes a hash for a C string up to (but not including) a sentinel character.
	/// @param pStr Pointer to the input string.
	/// @param untilCh Character at which hashing stops.
	/// @return 32-bit hash value of the prefix before the sentinel character.
	inline td::UINT4 calcHashNo(const char* pStr, char untilCh)
	{
		td::UINT4 hashNo = 0;
		while (*pStr != untilCh)
		{
			td::UINT4 highorder = hashNo & 0xf8000000;    // extract high-order 5 bits from hashNo
			hashNo <<= 5;									 // shift hashNo left by 5 bits
			hashNo ^= (highorder >> 27);					 // move the highorder 5 bits to the low-order
			// end and XOR into hashNo
			hashNo ^= (td::UINT4) (*pStr);
			++pStr;
		}
		return hashNo;
	}
}
