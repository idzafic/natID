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
	namespace Machine
	{
	//public:
		
        //enum class Endian : td::BYTE {LittleEndian = 0, BigEndian = 1};
        enum class Endian : td::BYTE {Little = 0, Big = 1};

		inline Machine::Endian getEndianInteger()
		{
			int i = 1;
			unsigned char *pc = (unsigned char*)&i;

            return ((pc[0] != (unsigned char) 0x01) ? Endian::Big : Endian::Little); // most significant byte is stored first for big_endian
		}

		inline Endian getEndianSingle()
		{
			float x = 9.3095191e+023f;
			unsigned char *pc = (unsigned char*)&x;
			//pc[0] = (unsigned char) 0x01;
			//pc[1] = (unsigned char) 0x23;
			//pc[2] = (unsigned char) 0x45;
			//pc[3] = (unsigned char) 0x67;


			return ((pc[0] != (unsigned char) 0x01) ? Endian::Big : Endian::Little); // most significant byte is stored first for big_endian
		}

		inline Endian getEndianDouble()
		{
			double x = -3.5986963492447479e+230;
			unsigned char *pc = (unsigned char*)&x;
			//pc[0] = (unsigned char) 0x01;
			//pc[1] = (unsigned char) 0x23;
			//pc[2] = (unsigned char) 0x45;
			//pc[3] = (unsigned char) 0x67;
			//pc[4] = (unsigned char) 0x89;
			//pc[5] = (unsigned char) 0xab;
			//pc[6] = (unsigned char) 0xcd;
			//pc[7] = (unsigned char) 0xef;

			return ((pc[0] != (unsigned char) 0x01) ? Endian::Big : Endian::Little); // most significant byte is stored first for big_endian
		}

    inline void swapEndian(td::BYTE& )
    {
    }
    
		inline void swapEndian(td::WORD& x)
		{
			x = (x>>8) | 
				(x<<8);
		}

		inline void swapEndian(td::UINT4& x)
		{
			x = (x>>24) |
				((x<<8) & 0x00FF0000) |
				((x>>8) & 0x0000FF00) |
				(x<<24);
		}

		inline void swapEndian(td::INT2& x)
		{
			x = (x>>8) | 
				(x<<8);
		}

		inline void swapEndian(td::INT4& x)
		{
			x = (x>>24) | 
				((x<<8) & 0x00FF0000) |
				((x>>8) & 0x0000FF00) |
				(x<<24);
		}

		inline void swapEndian(td::LINT8& x)
		{
			x = (x>>56) | 
                                ((x<<40) & 0x00FF000000000000LL) |
                                ((x<<24) & 0x0000FF0000000000LL) |
                                ((x<<8)  & 0x000000FF00000000LL) |
                                ((x>>8)  & 0x00000000FF000000LL) |
                                ((x>>24) & 0x0000000000FF0000LL) |
                                ((x>>40) & 0x000000000000FF00LL) |
				(x<<56);
		}

		inline void swapEndian(td::LUINT8& x)
		{
			x = (x>>56) | 
				((x<<40) & 0x00FF000000000000LL) |
				((x<<24) & 0x0000FF0000000000LL) |
				((x<<8)  & 0x000000FF00000000LL) |
				((x>>8)  & 0x00000000FF000000LL) |
				((x>>24) & 0x0000000000FF0000LL) |
				((x>>40) & 0x000000000000FF00LL) |
				(x<<56);
		}

		inline void swapEndian(float& val)
		{
			unsigned char* pChar = (unsigned char*) &val;
			//0<->3
			unsigned char tmp = pChar[0];
			pChar[0] = pChar[3];
			pChar[0] = tmp;

			//1<->2
			tmp = pChar[1];
			pChar[1] = pChar[2];
			pChar[2] = tmp;
		}

		inline void swapEndian(double& val)
		{
			unsigned char* pChar = (unsigned char*) &val;
			//0<->7
			unsigned char tmp = pChar[0];
			pChar[0] = pChar[7];
			pChar[7] = tmp;

			//1<->6
			tmp = pChar[1];
			pChar[1] = pChar[6];
			pChar[6] = tmp;

			//2<->5
			tmp = pChar[2];
			pChar[2] = pChar[5];
			pChar[5] = tmp;

			//3<->4
			tmp = pChar[3];
			pChar[3] = pChar[4];
			pChar[4] = tmp;
		}

		//static const Endian SystemEndianInteger;
		//static const Endian SystemEndianSingle;
		//static const Endian SystemEndianDouble;
	}
}
