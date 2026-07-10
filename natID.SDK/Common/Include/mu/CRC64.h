// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CRC64.h
    @brief Provides a configurable forward CRC-64 calculation class. */
#pragma once
#include <td/Types.h>

namespace mu
{
	/// @brief Computes a CRC-64 checksum using a configurable polynomial and bit order.
	class CRC64
	{
	protected:
		td::LUINT8 _poly; ///< Generator polynomial used for CRC computation.
		td::BYTE _order; ///< Bit order (number of bits) of the CRC, typically 64.
		td::LUINT8 _crc; ///< Current accumulated CRC value.
	public:
		/// @brief Constructs a CRC64 instance with the specified polynomial and bit order.
		/// @param poly The generator polynomial for the CRC algorithm.
		/// @param order The number of bits in the CRC (e.g., 64).
		CRC64(td::LUINT8 poly, td::BYTE order)
			: _poly(poly)
			, _order(order)
			, _crc(0)
		{
		}

		/// @brief Updates the CRC state by processing one byte in the forward direction.
		/// @param byte The byte to feed into the CRC shift register.
		inline void updateForward(td::BYTE byte)
		{
			/*unsigned __int64 poly,
			unsigned __int8  order,
			unsigned __int64 *pCRC,
			unsigned __int8  byte)*/

			td::LUINT8 reg = _crc;
			td::LUINT8 inputBit = 0;

			for(int i=0; i<8; ++i)
			{
				inputBit = ((td::LUINT8)(byte & 0x80)) << (_order - 8);
				byte     = byte << 1;

				if ((reg & ((td::LUINT8)1 << (_order-1)))^(inputBit))
				{
					reg = (reg << 1) ^ _poly;
				}
				else
				{
					reg = (reg << 1);
				}
			}

			td::LUINT8 n = 2;
			n = n << _order;
			_crc = (reg & (n - 1));
		}

		/// @brief Returns the current accumulated CRC value.
		/// @return The CRC-64 checksum computed so far.
		td::LUINT8 getValue() const
		{
			return _crc;
		}

		/// @brief Computes the CRC over a C string or a fixed-length byte buffer.
		/// @param pStr Pointer to the data to process.
		/// @param len Number of bytes to process; if negative, strlen is used.
		inline void calc(const char* pStr, int len=-1)
		{
			if (len < 0)
				len = (int) strlen(pStr);
			if (len <= 0)
				return;
			for (int i=0; i<len; ++i)
			{
				updateForward((td::BYTE) pStr[i]);
			}
		}
	};
}
