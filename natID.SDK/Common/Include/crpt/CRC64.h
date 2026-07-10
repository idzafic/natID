// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CRC64.h
    @brief Provides a CRC-64 checksum computation class using the ECMA polynomial and a precomputed lookup table. */
#pragma once
#include <td/Types.h>
#include <crpt/Hex.h>
#include <td/String.h>
#include <mu/Machine.h>

namespace crpt
{
	//OLD implementation
	/// @brief Computes a CRC-64 checksum over incremental data chunks using a precomputed table.
	class CRC64
	{
		td::LUINT8 crc64Tab[256]; ///< Precomputed CRC-64 lookup table.
		td::LUINT8 _crc64 = 0xFFFFFFFFFFFFFFFF; ///< Current CRC-64 accumulator value.

		/// @brief Precomputes the 256-entry CRC-64 lookup table using the ECMA-182 polynomial.
		void calcCRCTable()
		{
			td::LUINT8 POLYNOMIAL = 0xC96C5795D7870F42;

			for (td::LUINT8 i = 0; i<256; ++i)
			{
				td::LUINT8 crc = i;

				for (int j = 0; j<8; ++j)
				{
					// is current coefficient set?
					if (crc & 1)
					{
						// yes, then assume it gets zero'd (by implied x^64 coefficient of dividend)
						crc >>= 1;

						// and add rest of the divisor
						crc ^= POLYNOMIAL;
					}
					else
					{
						// no? then move to next coefficient
						crc >>= 1;
					}
				}
				crc64Tab[i] = crc;
			}
		}
	public:
		/// @brief Constructor; precomputes the CRC-64 lookup table and initializes the accumulator.
		CRC64()
		{
			calcCRCTable();
		}

		/// @brief Resets the CRC-64 accumulator to its initial value (0xFFFFFFFFFFFFFFFF).
		void init()
		{
			_crc64 = 0xFFFFFFFFFFFFFFFF;
		}

		/// @brief Feeds a byte buffer into the CRC-64 computation.
		/// @param buf Pointer to the input byte buffer.
		/// @param size Number of bytes to process.
		void consume(const td::BYTE *buf, size_t size)
		{
			for (size_t i = 0; i < size; i++)
			{
				td::BYTE byte = buf[i];
				_crc64 = crc64Tab[(td::BYTE)_crc64 ^ byte] ^ (_crc64 >> 8);
			}
		}

		/// @brief Feeds the content of a string into the CRC-64 computation.
		/// @param str Input string whose bytes are to be processed.
		void consume(const td::String& str)
		{
			consume((const td::BYTE*) str.c_str(), str.length());
		}

		/// @brief Returns the final CRC-64 value after XOR-finalisation.
		/// @return Finalized 64-bit CRC value.
		td::LUINT8 value() const
		{
			td::LUINT8 zero = 0;
			zero = ~zero;
			td::LUINT8 toRet = _crc64 ^ zero;
			return toRet;
		}

		/// @brief Returns the CRC-64 value as an uppercase hexadecimal string.
		/// @return Hex-encoded string representation of the CRC-64 value in big-endian byte order.
		td::String toString() const
		{
			td::LUINT8 currValue = value();
			if (mu::Machine::getEndianInteger() == mu::Machine::Endian::Little)
			{
				mu::Machine::swapEndian(currValue);
			}
			td::String str;
			crpt::toHex((td::BYTE*) &currValue, 8, str);
			return str;
		}
	};
}

//
//	//		/* Test vectors. */
//#define TEST1 "123456789"
//#define TESTLEN1 9
//#define TESTCRC1 UINT64_C(0x995dc9bbdf1939fa)
//#define TEST2 "This is a test of the emergency broadcast system."
//#define TESTLEN2 49
//#define TESTCRC2 UINT64_C(0x27db187fc15bbc72)
//
//	int test()
//	{
//		crc64_test((const td::BYTE*) TEST1, TESTLEN1, TESTCRC1);
//		crc64_test((const td::BYTE*) TEST2, TESTLEN2, TESTCRC2);
//		return 0;
//	}
//};


