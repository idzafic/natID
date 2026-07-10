// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CRC16.h
    @brief Provides a CRC-16 checksum computation class. */
#pragma once
#include <td/Types.h>
#include <crpt/Hex.h>
#include <td/String.h>
#include <mu/Machine.h>

namespace crpt
{
	/// @brief Computes a CRC-16 checksum over incremental data chunks.
	class CRC16
	{
		td::WORD _crc16 = 0xFFFF; ///< Current CRC-16 accumulator value.

	public:
		/// @brief Default constructor; initializes the CRC-16 accumulator to 0xFFFF.
		CRC16()
		{

		}

		/// @brief Resets the CRC-16 accumulator to its initial value (0xFFFF).
		void init()
		{
			_crc16 = 0xFFFF;
		}

		/// @brief Feeds a byte buffer into the CRC-16 computation.
		/// @param buf Pointer to the input byte buffer.
		/// @param size Number of bytes to process.
		void consume(const td::BYTE *buf, size_t size)
		{
			for (size_t i=0; i<size; ++i)
			{
				td::BYTE x = _crc16 >> 8 ^ buf[i];
				x ^= x>>4;
				td::WORD w = x;
				_crc16 = (_crc16 << 8) ^ (w << 12) ^ (w <<5) ^ w;
			}
		}

		/// @brief Feeds the content of a string into the CRC-16 computation.
		/// @param str Input string whose bytes are to be processed.
		void consume(const td::String& str)
		{
			consume((const td::BYTE*) str.c_str(), str.length());
		}

		/// @brief Returns the current CRC-16 checksum value.
		/// @return Current 16-bit CRC value.
		td::WORD value() const
		{
			return _crc16;
		}

		/// @brief Returns the current CRC-16 value as an uppercase hexadecimal string.
		/// @return Hex-encoded string representation of the CRC-16 value in big-endian byte order.
		td::String toString() const
		{
			td::WORD currValue = value();
			if (mu::Machine::getEndianInteger() == mu::Machine::Endian::Little)
			{
				mu::Machine::swapEndian(currValue);
			}
			td::String str;
			crpt::toHex((td::BYTE*) &currValue, 2, str);
			return str;
		}
	};
}

