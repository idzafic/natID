// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Header.h
    @brief Defines the archive Header structure that encodes metadata at the start of a binary stream. */
#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <mu/Machine.h>
#include <mu/Utils.h>
#include <td/GUID.h>
#include <crpt/CRC16.h>

#define ARCH_HEADER_SIZE 16

namespace arch
{
	//HEADER DESCRIPTION
	//bytes	desc
	//0-3	document major version (PSVD, MSG, ACKN, REQU)					{4 bytes}
	//4-5	crc16 of header													{2 bytes}
	//6		compression, guarded, last payload, and endianity				{1 bytes} cccchgle, where bits are: c-compression_or_other, h-hash of header is calculated, g-guarded, l-last payload, e-endianity
	//7		minor version													{1 bytes}
	//8-11	payload size													{4 bytes}
	//12-15 mask (crc32 of payload, return value, additional info, etc)		{4 bytes} 1byte header size,

	template <class TARCHIVE, bool bWrite>
	class MemorySerializer;

	/// @brief Represents the 16-byte binary archive header that stores versioning, compression, and integrity metadata.
	class Header
	{
		//begins with majorVersion 4 bytes
		template <class TARCHIVE, bool bWrite>
		friend class MemorySerializer;
	public:
		td::UINT4 majorVersion; ///< Four-character magic word identifying the archive type (e.g. "PSVD").
		td::UINT4 payloadSize;  ///< Size of the payload data in bytes.
		td::UINT4 mask;         ///< Mask field: CRC32 of payload, return value, or additional info.
		bool revertBytes;       ///< True if byte order must be reversed (endianness mismatch).
		bool guarded;           ///< True if type guard bytes are written before each field.
		bool lastPayload;       ///< True if this is the last payload in the stream.
		bool calcHeaderCRC;     ///< True if a CRC16 checksum of the header should be calculated.
		td::WORD hashKey;       ///< CRC16 hash of the header for integrity validation.
		td::BYTE minorVersion;  ///< Minor version number of the archive format.
		td::BYTE compression;   ///< Compression level, stored in the lower 4 bits.
		bool _isOK = true;      ///< Internal flag indicating whether the last decode operation succeeded.

		//td::GUID messageID;

		/// @brief Default constructor; initializes all header fields to safe zero/false values.
		Header()
			: majorVersion(0)
			, payloadSize(0)
			, mask(0)
			, revertBytes(false)
			, guarded(false)
			, lastPayload(false)
			, calcHeaderCRC(false)
			, hashKey(0)
			, minorVersion(0)
			, compression(0)
		{
		}

		/// @brief Constructs a Header using a 4-character magic word and optional format flags.
		/// @tparam size Size of the magic word character array (must be exactly 4 characters).
		/// @param magicWord Four-character string identifying the archive type.
		/// @param minorVersion Minor version of the archive format.
		/// @param compression Compression level (lower 4 bits used).
		/// @param calcHeadCRC Whether to calculate a CRC16 checksum of the header.
		/// @param guarded Whether guard bytes are written around each serialized field.
		template <size_t size>
		Header(const char(&magicWord)[size], td::BYTE minorVersion = 1, td::BYTE compression = 0, bool calcHeadCRC = false, bool guarded = false)
			: majorVersion(0)
			, payloadSize(0)
			, mask(0)
			, revertBytes(false)
			, guarded(guarded)
			, lastPayload(false)
			, calcHeaderCRC(calcHeadCRC)
			, hashKey(0)
			, minorVersion(minorVersion)
			, compression(compression & 0x0F)
		{
			//assert(strlen(strMajorVersion) == 4);
			static_assert(size != 4, "Magic word used to sync archives has to be 4 chars long!!");
			majorVersion = calcMagicWord(&magicWord[0]);
			//_header.messageID.reset();
		}

		/// @brief Updates the major version field from a new 4-character magic word.
		/// @tparam size Size of the magic word character array.
		/// @param magicWord New four-character magic word to set as the major version.
		template <size_t size>
		inline void resetMajorVersion(const char(&magicWord)[size])
		{
			static_assert(size != 4, "Magic word used to sync archives has to be 4 chars long!!");
			majorVersion = calcMagicWord(&magicWord[0]);
		}
	private:

		/// @brief Returns the byte offset of the hash/CRC field in the header buffer.
		/// @return Byte position of the 2-byte hash field.
		inline int getHashPos() const
		{
			//2Bytes
			return 4;
		}

		/// @brief Returns the byte offset of the endianity/compression/flags field.
		/// @return Byte position of the combined flags byte.
		inline int getEndianityPos() const
		{
			//1 Byte
			return 6;
		}

		/// @brief Returns the byte offset of the minor version field.
		/// @return Byte position of the minor version byte.
		inline int getMinorVersionPos() const
		{
			//1 Byte
			return 7;
		}

		/// @brief Returns the byte offset of the payload size field.
		/// @return Byte position of the 4-byte payload size.
		inline int getPayloadPos() const
		{
			//4 bytes
			return 8;
		}

		/// @brief Returns the byte offset of the mask field.
		/// @return Byte position of the 4-byte mask.
		inline int getMaskPos() const
		{
			//4 bytes
			return 12;
		}

		//inline int getGUIDPos() const
		//{
		//	//16 bytes
		//	return 16;
		//}

		/// @brief Packs a 4-character string into a 32-bit magic word integer using little-endian order.
		/// @param str Pointer to the 4-character string.
		/// @return Packed 32-bit integer representation of the magic word.
		inline td::UINT4 calcMagicWord(const char* str) const
		{
			td::UINT4 mw = str[3];
			mw <<= 8;
			mw |= str[2];
			mw <<= 8;
			mw |= str[1];
			mw <<= 8;
			mw |= str[0];
			return mw;
		}

		/// @brief Unpacks a 32-bit magic word integer into a 4-character string buffer.
		/// @param buf Output buffer of at least 4 bytes to receive the characters.
		/// @param mw The packed magic word integer to convert.
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

		/// @brief Sets the last-payload flag bit in the endianity byte of the header buffer.
		/// @param buf Pointer to the raw header byte buffer.
		inline void markAsLastPayload(char* buf)
		{
			int nCh = getEndianityPos();
			char ch = buf[nCh];
			ch = ch | 0x02;
			buf[nCh] = ch;
			lastPayload = true;
		}

		/// @brief Writes the payload size into the appropriate field of the header buffer.
		/// @param buf Pointer to the raw header byte buffer (may be nullptr to only update the member).
		/// @param size Payload size in bytes to store.
		inline void writePayloadSize(char* buf, td::UINT4 size)
		{
			payloadSize = size;
			if (buf)
			{
				char* pPayload = buf + getPayloadPos();
				memcpy(pPayload, (const char*)&payloadSize, 4);
			}
		}



	public:

		/// @brief Returns whether the last decode() call succeeded.
		/// @return True if the header is valid, false if a CRC or format error was detected.
		inline bool isOK() const
		{
			return _isOK;
		}

		/// @brief Returns the fixed size of the binary header in bytes.
		/// @return Header size, always equal to ARCH_HEADER_SIZE (16).
		inline int size() const
		{
			return ARCH_HEADER_SIZE;
		}

		/// @brief Decodes a raw 16-byte header buffer into the header fields.
		/// @param pHeader Pointer to the 16-byte header data to decode.
		/// @return True if decoding succeeded (including optional CRC check), false on format error.
		bool decode(const char* pHeader)
		{
			_isOK = true;

			majorVersion = calcMagicWord(pHeader);

			memcpy(&hashKey, pHeader + 4, 2);

			char endianityAndCompression = pHeader[6];
			compression = endianityAndCompression >> 4;

			char lowPart = endianityAndCompression & 0x0F;

			if (lowPart & 0x08)
			{
				calcHeaderCRC = true;
				lowPart &= 0x07;
			}
			else
				calcHeaderCRC = false;

			if (lowPart & 0x04)
			{
				guarded = true;
				lowPart &= 0x03;
			}
			else
				guarded = false;

			if (lowPart & 0x02)
			{
				lastPayload = true;
				lowPart &= 0x01;
			}
			else
				lastPayload = false;

			if (lowPart != (char) mu::Machine::getEndianInteger())
				revertBytes = true;
			else
				revertBytes = false;

			memcpy(&payloadSize, pHeader + 8, 4);
			memcpy(&mask, pHeader + 12, 4);

			//td::GUID guid;
			//memcpy(guid.str, pHeader + 16, 16);
			minorVersion = (td::BYTE) pHeader[7];

			if (revertBytes)
			{
				mu::Machine::swapEndian(payloadSize);
				mu::Machine::swapEndian(mask);
				mu::Machine::swapEndian(hashKey);
			}
			if (calcHeaderCRC)
			{
				crpt::CRC16 crcHeader;
				crcHeader.consume((td::BYTE*) pHeader, 4);
				crcHeader.consume((td::BYTE*) pHeader + 6, 10);
				td::WORD  hashKeyCtrl = crcHeader.value();
				if (hashKey != hashKeyCtrl)
				{
					_isOK = false;
					return false;
				}
			}
			return true;
		}

		/// @brief Encodes the header into a 16-byte buffer with compression and guard flags.
		/// @param headerData Output buffer of ARCH_HEADER_SIZE bytes to receive the encoded header.
		/// @param compressionLevel Compression level (lower 4 bits used).
		/// @param bGuarded Whether guard bytes are enabled in this archive.
		void encode(char* headerData, char compressionLevel, bool bGuarded)
		{
			guarded = bGuarded;

			//char headerData[32];
			memset(headerData, 0, ARCH_HEADER_SIZE);
			//memcpy(tmp, _maj, 8);
			magicWordToString(headerData, majorVersion); //first 4 bytes
			//bytes 0-9
			compressionLevel = compressionLevel & 0x0F;
			//_serializer.setCompression(compressionLevel);

			compressionLevel <<= 4;

			char endianityAndCompression = (char) mu::Machine::getEndianInteger();

			if (calcHeaderCRC)
			{
				char g = 0x08;
				endianityAndCompression |= g;
			}

			if (guarded)
			{
				char g = 0x04;
				endianityAndCompression |= g;
			}

			if (lastPayload)
			{
				char g = 0x02;
				endianityAndCompression |= g;
			}

			endianityAndCompression |= compressionLevel;

			headerData[6] = endianityAndCompression;
			headerData[7] = (char) minorVersion;
			memcpy(headerData+8, &payloadSize, 4);		//payload
			memcpy(headerData+12, &mask, 4);		//mask

			if (calcHeaderCRC)
			{
				crpt::CRC16 crcHeader;
				crcHeader.consume((td::BYTE*) headerData, 4);
				crcHeader.consume((td::BYTE*) headerData + 6, 10);
				hashKey = crcHeader.value();
				memcpy(headerData + 4, &hashKey, 2);
			}
		}

		/// @brief Encodes the header into a buffer, updating mask and payload size fields.
		/// @param headerData Output buffer of ARCH_HEADER_SIZE bytes to receive the encoded header.
		/// @param maskArh Mask value to store (e.g. CRC32 of payload).
		/// @param payLoadSize Total payload size in bytes.
		void encode(char* headerData, td::UINT4 maskArh, td::UINT4 payLoadSize)
		{
			//guarded = bGuarded;
			mask = maskArh;
			payloadSize = payLoadSize;

			//char headerData[32];
			memset(headerData, 0, ARCH_HEADER_SIZE);
			//memcpy(tmp, _maj, 8);
			magicWordToString(headerData, majorVersion); //first 4 bytes
														 //bytes 0-9
			//compressionLevel = compressionLevel & 0x0F;
			td::BYTE compressionLevel = compression;
			//_serializer.setCompression(compressionLevel);

			compressionLevel <<= 4;

			char endianityAndCompression = (char)mu::Machine::getEndianInteger();

			if (calcHeaderCRC)
			{
				char g = 0x08;
				endianityAndCompression |= g;
			}

			if (guarded)
			{
				char g = 0x04;
				endianityAndCompression |= g;
			}

			if (lastPayload)
			{
				char g = 0x02;
				endianityAndCompression |= g;
			}

			endianityAndCompression |= compressionLevel;

			headerData[6] = endianityAndCompression;
			headerData[7] = (char)minorVersion;
			memcpy(headerData + 8, &payloadSize, 4);		//payload
			memcpy(headerData + 12, &mask, 4);		//mask
													//if (msgIDRequired)
													//memcpy(&headerData[16], messageID.str, 16);
			if (calcHeaderCRC)
			{
				crpt::CRC16 crcHeader;
				crcHeader.consume((td::BYTE*) headerData, 4);
				crcHeader.consume((td::BYTE*) headerData + 6, 10);
				hashKey = crcHeader.value();
				memcpy(headerData + 4, &hashKey, 2);
			}
		}
	};
};
