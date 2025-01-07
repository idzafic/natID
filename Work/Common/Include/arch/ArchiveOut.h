// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <arch/Archive.h>
#include <crpt/CRC32.h>

namespace arch
{	
	class ArchiveOut : public Archive
	{		
	protected:
		crpt::CRC32 _crc32;
		bool _calcCRC = false;
		bool _countPayload = true;
		//ArchiveOut& store(const td::Variant& var)
		//{
		//	putGuardedByte(var);
		//	td::DataType dt(var.getType());
		//	td::BYTE vt((td::BYTE) dt);
		//	put((const char*)&vt, 1);
		//	switch (dt)
		//	{
		//	case td::boolean:  return operator << (var.boolVal()); 
		//	case td::byte:  return operator << (var.byteVal());
		//	case td::word:  return operator << (var.wordVal());
		//	case td::int2:  return operator << (var.i2Val());
		//	case td::int4:  return operator << (var.i4Val());
		//	case td::uint4:  return operator << (var.u4Val());
		//	case td::lint8:  return operator << (var.li8Val());
		//	case td::luint8:  return operator << (var.lu8Val());
		//	case td::string8:  return operator << (var.strVal());
		//	case td::decimal0:  
		//	case td::decimal1:
		//	case td::decimal2:
		//	case td::decimal3:
		//	case td::decimal4:
		//		return operator << (var.li8Val());
		//	case td::smallDecimal0:
		//	case td::smallDecimal1:
		//	case td::smallDecimal2:
		//	case td::smallDecimal3:
		//	case td::smallDecimal4:
		//		return operator << (var.i4Val());
		//	case td::ch7:
		//		return operator << (var.chFixVal());
		//	default:
		//		assert(false);
		//	}
		//	return *this;
		//}

		void operator << (const char* )
		{		
			//this is forbidden
		}

		bool encodeHeader(char compressionLevel, bool bGuarded)
		{
			//_guarded = false;
//#ifdef _DEBUG
//			td::LUINT8 transfInitial = _transferedBytes;
//#endif
			try
			{
				char headerData[ARCH_HEADER_SIZE];
				_header.encode(headerData, compressionLevel, bGuarded);
				
				//memset(headerData, 0, 32);
				////memcpy(tmp, _maj, 8);
				//magicWordToString(headerData, _majorVersion); //first 4 bytes
				////bytes 0-9			
				//compressionLevel = compressionLevel & 0x0F;
				//_serializer.setCompression(compressionLevel);

				//compressionLevel <<= 4;

				//char g=0;
				//char endianityAndCompression = (char) SystemEndianInteger;
				//
				//if (bGuarded)
				//{
				//	g= 1;
				//	g <<= 3;
				//	_guarded = false;
				//}
				//endianityAndCompression |= g;				
				//endianityAndCompression |= compressionLevel;			

				////calculate hash key
				//td::UINT4 hashKey = mu::Utils::calcHashNo(headerData, 4, 0);
				//hashKey = mu::Utils::calcHashNo(_messageID.val.str, 16, hashKey);
				//td::WORD w = (td::WORD) (hashKey & 0x0000FFFF);

				//memcpy(headerData+4, &w, 2);			//hash - checking correctness

				//headerData[6] = endianityAndCompression;				
				//headerData[7] = (char) _minorVersion;
				//memcpy(headerData+8, &_payload, 4);		//payload
				//memcpy(headerData+12, &_mask, 4);		//mask
				//memcpy(&headerData[16], _messageID.val.str, 16);

				put(headerData, _header.size());
				//_guarded = bGuarded;
//#ifdef _DEBUG
//				td::LUINT8 deltaTransf = _transferedBytes - transfInitial;
//				assert(deltaTransf == _header.size());
//#endif

				//header
				//total 32 bytes
			}			
			catch (...)
			{
				close();
				return false;
			}

			return true;			
		}		

		template <typename T>
		inline void putGuardedByte(const T& val)
		{
			if (_header.guarded)
			{
				td::BYTE bG = (td::BYTE)(td::getType(val));
				put((const char*)&bG, 1);			
			}
		}

		inline void putGuardedByteSer(const ISerializable& s)
		{
			if (_header.guarded)
			{
				td::BYTE bG =s.getGuardedByte();
				put((const char*)&bG, 1);			
			}
		}
		
	public:
		template <size_t size>
		ArchiveOut(const char(&magicWord)[size], IBinSerializer& serializer, td::BYTE minorVersion = 0)
			: Archive(serializer, minorVersion)			
		{
			//assert(strlen(strMajorVersion) == 4);
			static_assert(size != 4, "Magic word used to sync archives has to be 4 chars long!!");
			_header.majorVersion = calcMagicWord(&magicWord[0]);			
			//_header.messageID.reset();
		}

		ArchiveOut(const Header* pExtHeader, IBinSerializer& serializer, bool calcCRC)
			: Archive(serializer, pExtHeader->minorVersion)
			, _calcCRC(calcCRC)
		{			
			//_header.majorVersion = pExtHeader->majorVersion;
			_header = *pExtHeader;
			
		}

		template <class TSTR>
		void onClose(TSTR& f)
		{
			char tmp[ARCH_HEADER_SIZE];
			td::UINT4 crc32 = 0;
			if (_calcCRC)
				crc32 = _crc32.value();
			_countPayload = false;
			_header.encode(tmp, crc32, (td::UINT4) _totalUsedBytes);
			f.seekp(0, std::ios::beg);
			f.write(tmp, ARCH_HEADER_SIZE);
		}

		inline void put(const char* data, td::UINT4 nBytes)
		{
			_serializer.write(data, nBytes);
			_transferedBytes += nBytes;
			if (_countPayload && (_transferedBytes > ARCH_HEADER_SIZE))
			{
				_totalUsedBytes += nBytes;
				if (_calcCRC)
					_crc32.consume((td::BYTE*) data, nBytes);
			}				
		}

#pragma region Save Data
		//ArchiveOut& operator << (const td::Variant& var)
		//{
		//	return store(var);			
		//}

		ArchiveOut& operator << (bool b)
		{
			putGuardedByte(b);			
			td::BYTE val = (b) ? (1): (0);			
			put((const char*)&val, 1);
			return *this;
		}

		ArchiveOut& operator << (td::BYTE b)
		{			
			putGuardedByte(b);
			put((const char*)&b, 1);			
			return *this;
		}

		ArchiveOut& operator << (td::INT1 b)
		{
			putGuardedByte(b);
			put((const char*)&b, 1);
			return *this;
		}

		ArchiveOut& operator << (td::WORD w)
		{
			putGuardedByte(w);
			put((const char*)&w, 2);
			return *this;
		}

		ArchiveOut& operator << (td::INT2 s2)
		{
			putGuardedByte(s2);
			put((const char*)&s2, 2);			
			return *this;
		}

		ArchiveOut& operator << (td::INT4 i4)
		{
			putGuardedByte(i4);
			put((const char*)&i4, 4);			
			return *this;
		}

		ArchiveOut& operator << (td::UINT4 ui4)
		{
			putGuardedByte(ui4);
			put((const char*)&ui4, 4);			
			return *this;
		}

		ArchiveOut& operator << (const td::LINT8& li8)
		{
			putGuardedByte(li8);
			put((const char*)&li8, 8);			
			return *this;
		}

		ArchiveOut& operator << (const td::LUINT8& lui8)
		{
			putGuardedByte(lui8);
			put((const char*)&lui8, 8);			
			return *this;
		}

		template <typename TVAL>
		ArchiveOut& operator << (const td::Rect<TVAL>& r)
		{
			putGuardedByte(r);
			putGuardedByte(r.left);			
			put((const char*)&r, sizeof(r) );
			return *this;
		}

		ArchiveOut& operator << (const td::String& s8)
		{
			putGuardedByte(s8);
			if (s8.spaceForSize() == 2)
			{
				td::WORD len =(td::WORD) s8.length();
				//operator << (len);
				put( (const char*) &len, 2);
				put(s8.c_str(), len);				
			}
			else
			{
				td::UINT4 len =(td::UINT4) s8.length();
				//operator << (len);
				put((const char*) &len, 4);
				put(s8.c_str(), len);				
			}
			return *this;
		}

		ArchiveOut& operator << (const td::Color& c)
		{
			putGuardedByte(c);
			td::UINT4 val = c.getValue();
			put((const char*)&val, 4);			
			return *this;
		}
        
        ArchiveOut& operator << (const td::ColorID& c)
        {
            putGuardedByte(c);
            td::BYTE val = (td::BYTE) c;
            put((const char*)&val, 1);
            return *this;
        }

		ArchiveOut& operator << (const td::Date& d)
		{
			putGuardedByte(d);
			td::INT4 val = d.getValue();
			put((const char*)&val, 4);			
			return *this;
		}

		ArchiveOut& operator << (const td::Time& t)
		{
			putGuardedByte(t);
			td::INT4 val = t.getValue();
			put((const char*)&val, 4);
			return *this;
		}

		ArchiveOut& operator << (const td::DateTime& dt)
		{	
			putGuardedByte(dt);
			td::INT4 val = dt.getDate();
			put((const char*)&val, 4);

			val = dt.getTime();
			put((const char*)&val, 4);
			return *this;
		}

		ArchiveOut& operator << (const double& dbl)
		{		
			putGuardedByte(dbl);
			put((const char*)&dbl, 8);
			return *this;
		}			

		ArchiveOut& operator << (const void* ptr)
		{
			putGuardedByte(ptr);
			put((const char*)&ptr, sizeof(void*));
			return *this;
		}

		ArchiveOut& operator << (float flt)
		{		
			putGuardedByte(flt);
			put((const char*)&flt, 4);
			return *this;
		}

		template<typename THOLDER, int NDEC>
		inline ArchiveOut& operator << (const td::Decimal<THOLDER, NDEC>& dec)
		{
			putGuardedByte(dec);
			THOLDER val = dec.getValue();
			put((const char*)(&val), sizeof(THOLDER));
			return *this;
		}
        
        ArchiveOut& putGuarded(td::BYTE* pBytes, td::UINT4 nBytes)
        {
            putGuardedByte(nBytes);
            put((const char*)&nBytes, 4);
            put((const char*)pBytes, nBytes);
            return *this;
        }

		ArchiveOut& operator << (const ISerializable& s)
		{				
			putGuardedByteSer(s);
			s.serialize(*this);
			return *this;
		}

		ArchiveOut& operator << (const ISerializable* s)
		{	
			putGuardedByteSer(*s);
			
			s->serialize(*this);
			return *this;
		}		

		template <class TSERIALIZABLE>
		inline ArchiveOut& operator << (const TSERIALIZABLE& s)
		{				
			s.saveToBinaryStream(*this);
			return *this;
		}

#pragma endregion
	};
}
