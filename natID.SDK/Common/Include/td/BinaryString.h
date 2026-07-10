// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BinaryString.h
    @brief Defines the BinaryString class for storing and managing raw binary data as a length-prefixed byte buffer. */
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

	/// @brief Stores and manages a raw binary byte sequence using a length-prefixed heap-allocated buffer.
	class BinaryString
	{
	protected:
		template <class TUSEDSTRING, td::UINT4 MINBUFFLEN>
		friend class cnt::StringBuilder;

		td::BYTE* buffer; ///< Pointer to the heap-allocated buffer; the first 4 bytes store the length, followed by the raw data bytes.

	public:
		typedef td::BYTE* iterator;             ///< Mutable iterator type for traversing the binary data.
		typedef const td::BYTE* const_iterator; ///< Immutable iterator type for traversing the binary data.
		typedef char CHAR_TYPE;                 ///< Character type alias used for compatibility.

		/// @brief Returns an iterator to the first byte of the binary data.
		/// @return A mutable iterator pointing to the beginning of the data.
		iterator begin()
		{
			return (iterator) getBegin();
		}

		/// @brief Returns a const iterator to the first byte of the binary data.
		/// @return An immutable iterator pointing to the beginning of the data.
		const_iterator begin() const
		{
			return (const_iterator) getBegin();
		}

		/// @brief Returns a const iterator one past the last byte of the binary data.
		/// @return An immutable iterator pointing past the end of the data.
		const_iterator end() const
		{
			return (const_iterator) getEnd();
		}

	protected:

		/// @brief Deallocates the internal buffer and resets the pointer to null.
		inline void cleanBuffer()
		{
			if (buffer)
			{
				delete [] buffer;
				buffer = 0;
			}
		}

		//input is only space required for characters without zero ch and space for size
		/// @brief Allocates a new internal buffer with enough capacity for the given number of data bytes plus the 4-byte length header.
		/// @param size Number of data bytes the buffer must accommodate (must not exceed 256 MB).
		/// @return True if allocation succeeded and size is positive, false otherwise.
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

		/// @brief Returns a pointer to the first data byte, past the 4-byte length header.
		/// @return Pointer to the start of the binary data, or null if no buffer is allocated.
		td::BYTE* getBegin() const
		{
			if (buffer)
			{
				return buffer + 4;
			}
			return 0;
		}

		/// @brief Returns a pointer one past the last data byte in the buffer.
		/// @return Pointer past the end of the binary data, or null if no buffer is allocated.
		td::BYTE* getEnd() const
		{
			if (buffer)
			{
				int size = length();
				return buffer + 4 + size;
			}
			return 0;
		}


		/// @brief Allocates a buffer and copies the given byte array into it.
		/// @param pStr Pointer to the source byte array to copy.
		/// @param inputLen Number of bytes to copy from pStr.
		inline void setString(const td::BYTE* pStr, int inputLen)
		{
			if (prepareBuffer(inputLen))
			{
				mem::fastCopy(getBegin(), pStr, inputLen);
			}
		}

	public:
		/// @brief Destructor that frees the internal buffer.
		~BinaryString()
		{
			if (buffer)
				delete [] buffer;
		}

		/// @brief Default constructor that initializes an empty BinaryString with no buffer.
		BinaryString()
			: buffer(0)
		{
		}


		/// @brief Constructs a BinaryString by copying the given byte array.
		/// @param pStr Pointer to the source byte array.
		/// @param inputLen Number of bytes to copy from pStr.
		BinaryString(const td::BYTE* pStr, int inputLen)
			: buffer(0)
		{
			setString(pStr, inputLen);
		}

		/// @brief Assigns the binary string content from a known-length byte array.
		/// @param str Pointer to the source byte array.
		/// @param inputUnitLen Number of bytes to copy; clamped to 0 if negative.
		void fromKnownString(const td::BYTE* str, int inputUnitLen)
		{
			if (inputUnitLen < 0)
				inputUnitLen = 0;

			setString(str, inputUnitLen);
		}


		/// @brief Returns a pointer to the raw binary data.
		/// @return Pointer to the first data byte, or null if no data is stored.
		const td::BYTE* c_str() const
		{
			if (buffer)
				return (buffer + 4);
			return 0;
		}

		/// @brief Releases the internal buffer and resets the BinaryString to an empty state.
		void clean()
		{
			if (buffer)
			{
				delete [] buffer;
				buffer = 0;
			}
		}

		/// @brief Returns the number of bytes stored in this BinaryString.
		/// @return The length of the binary data in bytes, or 0 if no data is stored.
		int length() const
		{
			if (buffer)
			{
				td::INT4* pSize= reinterpret_cast<td::INT4*>(buffer);
				return (int) (*pSize);
			}
			return 0;
		}

		/// @brief Returns the number of storage units (bytes) used by this BinaryString.
		/// @return The same value as length(), since each unit is one byte.
		int unitLength() const
		{
			return length();
		}

	};

	/// @brief Returns the DataType identifier for BinaryString values.
	/// @return The td::binary DataType constant.
	inline DataType getType(const BinaryString&) {return binary;}

};
