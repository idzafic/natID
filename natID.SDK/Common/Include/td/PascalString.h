// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "td/Types.h"
#include <cstdarg>
#include <cassert>

namespace td
{
	template <int SIZEINSIDE>
	class PascalString
	{
		char* buffer;
		bool prepareBuffer(int size) 
		{
			assert(size <= 0x0000FFFF);

			if (buffer)
			{
				delete buffer;
				buffer = 0;
			}

			if (size > 0)
			{
				int nToReserve = size + SIZEINSIDE + 1;
				assert(nToReserve > 0);
				
				buffer = new char[nToReserve];

				return true;
			}
			return false;
		}

		char* getBegin() const
		{
			if (buffer)
			{				
				return buffer + SIZEINSIDE;
			}
			return 0;
		}

		inline void fillString(const char* str, int nLen)
		{
			if (prepareBuffer(nLen))
			{
				STRCPY(getBegin(), nLen+1, str);

				if (SIZEINSIDE == 1)
				{
					td::BYTE* pSize = (td::BYTE*) buffer;
					*pSize = (td::BYTE) nLen;
				}
				else if (SIZEINSIDE == 2)
				{
					td::WORD* pSize = (td::WORD*) buffer;
					*pSize = (td::WORD) nLen;
				}				
				else if (SIZEINSIDE == 4)
				{
					td::INT4* pSize = (td::INT4*) buffer;
					*pSize = (td::INT4) nLen;
				}
			}			
		}

	public:
		PascalString(): buffer(0){}
		PascalString(const char* str)
			: buffer(0)
		{

			int nLen = (int) strlen(str);
			fillString(str, nLen);
			
		}

		PascalString(const char* str, int nLen)
			: buffer(0)
		{
                        assert(strlen(str) == nLen);
			fillString(str, nLen);
		}

		void operator = (const PascalString& str)
		{
			int nLen = str.length();
			fillString(str.c_str(), nLen);
		}

		void operator = (const char* str)
		{
			int nLen = (int) strlen(str);
			fillString(str, nLen);
		}

		~PascalString()
		{
			if (buffer)
				delete [] buffer;
		}

		

		int length() const
		{
			if (buffer)
			{
				if (SIZEINSIDE == 1)
				{
					td::BYTE* pSize = (td::BYTE*) buffer;
					return (int) *pSize;
				}
				else if (SIZEINSIDE == 2)
				{
					td::WORD* pSize = (td::WORD*) buffer;
					return (int) *pSize;
				}				
				else if (SIZEINSIDE == 4)
				{
					td::INT4* pSize = (td::INT4*) buffer;
					return (int) *pSize;
				}
				else
				{
					return (int) strlen(buffer);
				}
			}
			return 0;
		}

		void cFormat(const char* format, ...)
		{
			va_list argptr;	
			va_start(argptr, format);

			char tmp[256];

			int nLen = vsnprintf(tmp, 256, format, argptr);

			va_end(argptr);

			//assert(nLen >= 0);

			if (prepareBuffer(nLen))
			{	
				strcpy(getBegin(), nLen+1, tmp);
			}
		}

		const char* c_str() const
		{
			if (buffer)
			{
				return buffer + SIZEINSIDE;
			}
			return 0;
		}
	};

	typedef PascalString<0> PascStr0;
	typedef PascalString<1> PascStr1;
	typedef PascalString<2> PascStr2;
};
