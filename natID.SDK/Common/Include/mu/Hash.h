// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Types.h>

namespace mu
{
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