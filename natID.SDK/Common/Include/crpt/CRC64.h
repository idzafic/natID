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
	//OLD implementation
	class CRC64
	{
		td::LUINT8 crc64Tab[256];
		td::LUINT8 _crc64 = 0xFFFFFFFFFFFFFFFF;		

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
		CRC64()
		{
			calcCRCTable();
		}

		void init()
		{
			_crc64 = 0xFFFFFFFFFFFFFFFF;
		}

		void consume(const td::BYTE *buf, size_t size)
		{	
			for (size_t i = 0; i < size; i++) 
			{
				td::BYTE byte = buf[i];
				_crc64 = crc64Tab[(td::BYTE)_crc64 ^ byte] ^ (_crc64 >> 8);
			}
		}

		void consume(const td::String& str)
		{
			consume((const td::BYTE*) str.c_str(), str.length());
		}

		td::LUINT8 value() const
		{			
			td::LUINT8 zero = 0;
			zero = ~zero;
			td::LUINT8 toRet = _crc64 ^ zero;
			return toRet;
		}

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


