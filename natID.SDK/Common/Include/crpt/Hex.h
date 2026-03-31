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
#include <td/String.h>

namespace crpt
{
	//caller has to allocate 2*inpLen in outLen
	inline void toHex(const td::BYTE* inpStr, size_t inpLen, char* outStr)
	{
		const char pHex[] = "0123456789ABCDEF";
		const td::BYTE* pCh= inpStr;
		for (size_t i = 0; i < inpLen; ++i)
		{
			td::BYTE ch = pCh[i];
			//extract upper part
			td::BYTE upper = (ch >> 4) & 0x0F;
			size_t j = 2 * i;
			outStr[j] = pHex[upper];
			td::BYTE lower = ch & 0x0F;
			outStr[j+1] = pHex[lower];
		}
	}
	inline void toHex(const td::String& str, char* outStr)
	{
		toHex((const td::BYTE*) str.c_str(), str.length(), outStr);
	}

	inline void toHex(const td::String& str, td::String& outStr)
	{
		size_t len = str.length();
		if (len == 0)
			return;
		outStr.reserve(2 * len);
		toHex((const td::BYTE*) str.c_str(), str.length(), outStr.begin());
	}

	template <size_t size>	
	inline void toHex(const td::BYTE (&strIn)[size], td::String& outStr)
	{
		outStr.reserve(2 * size);
		toHex((const td::BYTE*) &strIn[0], size, outStr.begin());
	}

	
	inline void toHex(const td::BYTE* strIn, size_t sizeIn, td::String& outStr)
	{
		outStr.reserve(2 * sizeIn);
		toHex((const td::BYTE*) &strIn[0], sizeIn, outStr.begin());
	}

	template <size_t size>
	inline void toHex(const td::BYTE(&strIn)[size], char(&outStr)[size * 2])
	{		
		toHex((const td::BYTE*) &strIn[0], size, &outStr[0]);
	}

	//caller has to allocate inpLen/2 in outLen
	inline void fromHex(const char* inpStr, size_t inpLen, td::BYTE* outStr)
	{
		assert(inpLen % 2 == 0);
		const td::BYTE pDec[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
		const td::BYTE* pCh = (const td::BYTE*)inpStr;
		for (size_t i = 0, j=0; i < inpLen; i+=2, ++j)
		{
			td::BYTE ch1 = pCh[i];
			assert(ch1 >= '0' && ch1 <= 'F');
			td::BYTE ch2 = pCh[i+1];
			assert(ch2 >= '0' && ch2 <= 'F');
			if (ch1 >= 'A')
				ch1 -= ('A' - 10);
			else
				ch1 -= '0';

			if (ch2 >= 'A')
				ch2 -= ('A' - 10);
			else
				ch2 -= '0';
			
			td::BYTE out = (ch1 << 4) | ch2;
			outStr[j] = out;
		}
	}

	inline void fromHex(const td::String& str, td::BYTE* outStr)
	{
		fromHex( str.c_str(), str.length(), outStr);
	}

	inline void fromHex(const td::String& str, td::String& outStr)
	{
		size_t len = str.length();
		if (len <= 1)
			return;
		if (len % 2 != 0)
			return;
		outStr.reserve(len / 2);
		fromHex(str.c_str(), str.length(), (td::BYTE*) outStr.begin());
	}
}