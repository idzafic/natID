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
#include <crpt/Hex.h>
#include <td/String.h>
#include <mu/Machine.h>

namespace crpt
{
	class CRC16
	{		
		td::WORD _crc16 = 0xFFFF;

	public:
		CRC16()
		{

		}
		void init()
		{
			_crc16 = 0xFFFF;
		}

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

		void consume(const td::String& str)
		{
			consume((const td::BYTE*) str.c_str(), str.length());
		}

		td::WORD value() const
		{			
			return _crc16;
		}

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

