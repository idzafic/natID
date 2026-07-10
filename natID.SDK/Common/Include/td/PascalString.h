// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PascalString.h
 @brief Pascal-style string with an inline length prefix stored before the character data. */
#pragma once
#include "td/Types.h"
#include <cstdarg>
#include <cassert>

namespace td
{
	/// @brief A string that stores its length as a fixed-width integer immediately before the character data.
	/// @tparam SIZEINSIDE Number of bytes used to store the length prefix (0 = no prefix, 1 = BYTE, 2 = WORD, 4 = INT4).
	template <int SIZEINSIDE>
	class PascalString
	{
		char* buffer; ///<  Raw buffer pointer; the first SIZEINSIDE bytes hold the length, followed by the null-terminated string.
		/// @brief Allocates a new buffer large enough to hold @p size characters plus the length prefix.
		/// @param size Number of character bytes required (excluding null terminator).
		/// @return true if allocation succeeded and the buffer is ready for writing; false if size is 0.
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

		/// @brief Returns a pointer to the first character of the string, skipping the length prefix bytes.
		/// @return Pointer to the character data, or null if no buffer has been allocated.
		char* getBegin() const
		{
			if (buffer)
			{
				return buffer + SIZEINSIDE;
			}
			return 0;
		}

		/// @brief Copies @p str into the internal buffer and writes the encoded length prefix.
		/// @param str Source null-terminated C string.
		/// @param nLen Number of characters in @p str (not counting the null terminator).
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
		/// @brief Default constructor; creates an empty string with no allocated buffer.
		PascalString(): buffer(0){}
		/// @brief Constructs a PascalString from a null-terminated C string.
		/// @param str Source C string to copy.
		PascalString(const char* str)
			: buffer(0)
		{

			int nLen = (int) strlen(str);
			fillString(str, nLen);

		}

		/// @brief Constructs a PascalString from a C string with an explicitly provided length.
		/// @param str Source C string to copy.
		/// @param nLen Number of characters in @p str.
		PascalString(const char* str, int nLen)
			: buffer(0)
		{
                        assert(strlen(str) == nLen);
			fillString(str, nLen);
		}

		/// @brief Copy-assigns the value of another PascalString.
		/// @param str Source PascalString to copy.
		void operator = (const PascalString& str)
		{
			int nLen = str.length();
			fillString(str.c_str(), nLen);
		}

		/// @brief Assigns a null-terminated C string to this PascalString.
		/// @param str Source C string to copy.
		void operator = (const char* str)
		{
			int nLen = (int) strlen(str);
			fillString(str, nLen);
		}

		/// @brief Destructor; releases the allocated buffer.
		~PascalString()
		{
			if (buffer)
				delete [] buffer;
		}



		/// @brief Returns the number of characters stored in this string.
		/// @return Length of the string in characters, or 0 if the buffer is empty.
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

		/// @brief Formats a string into this PascalString using printf-style formatting.
		/// @param format printf-compatible format string.
		/// @param ... Variable arguments matching the format specifiers.
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

		/// @brief Returns a pointer to the internal null-terminated C string.
		/// @return Pointer to the character data, or null if no buffer has been allocated.
		const char* c_str() const
		{
			if (buffer)
			{
				return buffer + SIZEINSIDE;
			}
			return 0;
		}
	};

	typedef PascalString<0> PascStr0; ///< PascalString with no length prefix.
	typedef PascalString<1> PascStr1; ///< PascalString with a 1-byte (BYTE) length prefix.
	typedef PascalString<2> PascStr2; ///< PascalString with a 2-byte (WORD) length prefix.
};
