// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cstdarg>
#include <td/Types.h>
#include <mu/UTFConverter.h>
#include <mu/Regionals.h>
#include <td/BufferString.h>
#include <set>
#include <cmath>
#include <typeinfo>
#include <mem/Utils.h>

namespace cnt
{
	template <class TUSEDSTRING, td::UINT4 MINBUFFLEN>
	class StringBuilder;
}

namespace td
{	
	
	class BinaryString
	{
	protected:
		template <class TUSEDSTRING, td::UINT4 MINBUFFLEN> 
		friend class cnt::StringBuilder;

		td::BYTE* buffer;
	public:
		typedef td::BYTE* iterator;
		typedef const td::BYTE* const_iterator;
		typedef char CHAR_TYPE;

		iterator begin()
		{
			return (iterator) getBegin();
		}

		const_iterator begin() const
		{
			return (const_iterator) getBegin();
		}

		const_iterator end() const
		{
			return (const_iterator) getEnd();
		}

	protected:

		inline void cleanBuffer()
		{
			if (buffer)
			{
				delete [] buffer;
				buffer = 0;
			}
		}		

		//input is only space required for characters without zero ch and space for size
		inline bool prepareBuffer(int size) 
		{
			assert(size <= 0x0FFFFFFF);  //256MB boundary

			if (buffer)
			{
				delete [] buffer;
				buffer = 0;
			}

			if (size > 0)
			{
				int nToReserve = size + 4;			

				buffer = new td::BYTE [nToReserve];

				td::INT4* pLen = reinterpret_cast<td::INT4*>(buffer);
				*pLen = size;				

				return true;
			}
			return false;
		}

		td::BYTE* getBegin() const
		{
			if (buffer)
			{
				return buffer + 4;				
			}
			return 0;
		}

		td::BYTE* getEnd() const
		{
			if (buffer)
			{
				int size = length();
				return buffer + 4 + size;
			}
			return 0;
		}


		inline void setString(const td::BYTE* pStr, int inputLen)
		{			
			if (prepareBuffer(inputLen))
			{
				mem::fastCopy(getBegin(), pStr, inputLen);
			}			
		}
		
	public:
		~BinaryString()			
		{
			if (buffer)
				delete [] buffer;
		}

		BinaryString()
			: buffer(0)
		{
		}


		BinaryString(const td::BYTE* pStr, int inputLen)
			: buffer(0)
		{			
			setString(pStr, inputLen);
		}		
		
		void fromKnownString(const td::BYTE* str, int inputUnitLen)
		{
			if (inputUnitLen < 0)
				inputUnitLen = 0;

			setString(str, inputUnitLen);
		}
				

		const td::BYTE* c_str() const
		{
			if (buffer)			
				return (buffer + 4);				
			return 0;
		}

		void clean()
		{
			if (buffer)
			{
				delete [] buffer;
				buffer = 0;
			}
		}

		int length() const
		{
			if (buffer)
			{
				td::INT4* pSize= reinterpret_cast<td::INT4*>(buffer);
				return (int) (*pSize);
			}
			return 0;
		}

		int unitLength() const
		{
			return length();
		}
		
	};	

	inline DataType getType(const BinaryString&) {return binary;}	

};
