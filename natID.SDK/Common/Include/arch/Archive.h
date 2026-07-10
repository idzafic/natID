// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Archive.h
    @brief Defines the base Archive class and the Exception class for archive error handling. */
#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <td/DateTime.h>
#include <mu/Machine.h>
#include <arch/IBinSerializer.h>
#include <arch/ISerializable.h>
#include <mem/PointerDeleter.h>
#include <mem/PointerReleaser.h>
//#include "random/IntRandom.h"
#include <mu/Utils.h>
#include <td/Decimal.h>
#include <cnt/StringBuilder.h>
#include <td/GUID.h>
#include <arch/Header.h>
#include <td/Point.h>
#include <td/Rect.h>
#include <arch/EnumSerializer.h>
//#include <td/Variant.h>
//#include <cnt/List.h>

namespace arch
{
	/// @brief Exception class thrown when a binary archive encounters a deserialization error.
	class Exception
	{
	public:
		td::String str; ///< Human-readable description of the error.

		/// @brief Default constructor; creates an empty exception.
		Exception()
		{
		}

		/// @brief Outputs the exception message to an output stream.
		/// @tparam OSTREAM Type of the output stream.
		/// @param o The output stream to write the error message to.
		template<class OSTREAM>
		void show(OSTREAM& o) const
		{
			o << str.c_str() << std::endl;
		}
	};

	/// @brief Base class for binary archives providing common header management and helper utilities.
	class Archive
	{
	protected:
		td::LUINT8 _transferedBytes;  ///< Running count of bytes transferred through the serializer.
		td::LUINT8 _totalUsedBytes;   ///< Total payload bytes written (excluding header).
		IBinSerializer& _serializer;  ///< Reference to the underlying binary serializer.
		Header _header;               ///< Decoded or encoded archive header.

	protected:
		/// @brief Converts a 4-character string into a packed 32-bit magic word (little-endian).
		/// @param str Pointer to a 4-character string.
		/// @return Packed 32-bit integer representation.
		inline td::UINT4 calcMagicWord(const char* str) const
		{
			assert(strlen(str) == 4);
			td::UINT4 mw = str[3];
			mw <<= 8;
			mw |= str[2];
			mw <<= 8;
			mw |= str[1];
			mw <<= 8;
			mw |= str[0];
			return mw;
		}

		/// @brief Converts a packed 32-bit magic word back into a 4-character string buffer.
		/// @param buf Output buffer of at least 4 bytes.
		/// @param mw The packed magic word to convert.
		inline void magicWordToString(char* buf, td::UINT4 mw) const
		{
			buf[0] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[1] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[2] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[3] = (char)(mw & 0x000000FF);
		}
	public:
		/// @brief Constructs an Archive with a reference to a binary serializer and an optional minor version.
		/// @param serializer The binary serializer to use for I/O operations.
		/// @param minorVersion Minor version to store in the archive header.
		Archive(IBinSerializer& serializer, td::BYTE minorVersion = 0)
			: _transferedBytes(0)
			, _totalUsedBytes(0)
			, _serializer(serializer)
		{
			_header.minorVersion = minorVersion;
		}

		/// @brief Returns a pointer to the archive header.
		/// @return Const pointer to the Header structure.
		const Header* getHeader() const
		{
			return &_header;
		}
		//void setVersion(td::UINT4 ver)
		//{
		//	_currentVersion = ver;
		//}

		//td::UINT4 clientID() const
		//{
		//	return _clientID;
		//}

		//void clientID(td::UINT4 clientKey)
		//{
		//	_clientID = clientKey;
		//}

		//const td::GUID& getID() const
		//{
			//const Header* pHeader = getHeader();
			//assert(pHeader);
			//return pHeader->messageID;
			//return _header.messageID;
		//}

		//void setID(const td::GUID&)
		//{
			//_header.messageID = messageKey;
		//}

		/// @brief Returns the total number of bytes transferred so far.
		/// @return Number of bytes read or written through this archive.
		td::LUINT8 transferedBytes() const
		{
			return _transferedBytes;
		}

		/// @brief Returns the minor version stored in the archive header.
		/// @return Minor version byte.
		td::BYTE getMinorVersion() const
		{
			return _header.minorVersion;
		}

		/// @brief Returns the major version (magic word) stored in the archive header.
		/// @return Major version as a packed 32-bit integer.
		td::UINT4 getMajorVersion() const
		{
			return _header.majorVersion;
		}

		//td::UINT4 getFileVersion()
		//{
		//	return _fileVersion;
		//}

		/// @brief Seeks the underlying serializer to the specified byte position.
		/// @param pos Target byte offset in the stream.
		/// @return True if seeking succeeded, false otherwise.
		virtual bool goTo(td::LUINT8 pos)
		{
			return _serializer.goTo(pos);
		}


		/// @brief Closes the underlying binary serializer.
		void close()
		{
			_serializer.close();
		}
	};
}
