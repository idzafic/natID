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

namespace mu
{
	class CRC64
	{
	protected:
		td::LUINT8 _poly;
		td::BYTE _order;
		td::LUINT8 _crc;
	public:
		CRC64(td::LUINT8 poly, td::BYTE order)
			: _poly(poly)
			, _order(order)
			, _crc(0)
		{
		}

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

		td::LUINT8 getValue() const
		{
			return _crc;
		}

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