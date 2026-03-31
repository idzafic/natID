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

namespace arch
{
	class ArchiveIn : public Archive
	{
	protected:		
		bool _revertBytes;
		bool _bMainHeaderRead;
		bool _bMessageHeaderRead;
		//std::set<td::UINT4> _allowedMajorVersions;	
		td::UINT4 _allowedMajorVersion = 0;
		//td::LUINT8 _firstErrorPosition = 0;
	protected:
		bool decodeHeader(char* pHeader = nullptr)
		{	
			bool checkHeaderSize = false;
			try
			{				
				mem::PointerDeleter<char> pData;
				//td::GUID prevGuid;
				td::UINT4 prevMajorVersion;

				if (_bMainHeaderRead)
				{
					//prevGuid = _header.messageID;
					prevMajorVersion = _header.majorVersion;
				}
				
				if (pHeader == nullptr)
				{
					checkHeaderSize = true;
					pHeader = new char[_header.size()];
					pData = pHeader;				
					get(pHeader, _header.size());
				}			

				if (!_header.decode(pHeader))
				{
					assert(false);
					close();
					return false;
				}
				//std::set<td::UINT4>::const_iterator itEnd(_allowedMajorVersions.end());

				//if (_allowedMajorVersions.find(_header.majorVersion) == itEnd)
				if (_allowedMajorVersion != 0)
				{
					if (_header.majorVersion != _allowedMajorVersion)
					{
						assert(false);
						close();
						return false;
					}
				}								

				//char endianityAndCompression = pHeader[6];
				//char compression = endianityAndCompression >> 4;
				//_serializer.setCompression(compression);

				//char endianity = endianityAndCompression & 0x0F;				

				//bool bOrigGuarded= false;
				//
				//if (endianity & 0x08)
				//{
				//	bOrigGuarded = true;
				//	endianity &= 0x07;
				//}				

				//if (endianity & 0x04)
				//{
				//	_serializer.markAsLastPayload();
				//	endianity &= 0x03; 
				//}

				//if (endianity > 1)
				//{
				//	close();
				//	return false;
				//}

				//if (endianity != (char) SystemEndianInteger)
				//	_revertBytes = true;
				//else
				//	_revertBytes = false;				

				//td::UINT4 hashKey = mu::Utils::calcHashNo(pHeader, 4);
				//hashKey = mu::Utils::calcHashNo(pHeader+16, 16, hashKey);
				//td::WORD w = (td::WORD) (hashKey & 0xFFFF);
				//				
				//td::WORD wIn = 0;
				//memcpy(&wIn, pHeader+4, 2);
				//if (_revertBytes)
				//	Machine::swapEndian(wIn);
				//if (w != wIn)
				//{
				//	close();
				//	return false;				
				//}

				//td::GUID guid;
				//memcpy(guid.val.str, pHeader + 16, 16);
				//_minorVersion = (td::BYTE) pHeader[7];

				//memcpy(&_payload, pHeader+8, 4);
				//memcpy(&_mask, pHeader+12, 4);

				//if (_revertBytes)
				//{
				//	Machine::swapEndian(_payload);
				//	Machine::swapEndian(_mask);
				//}

				//_serializer.setPayloadSize(_payload);
				//_serializer.setMask(_mask);


				////header
				////total 32 bytes
				//_guarded = bOrigGuarded;				
				if (!_bMainHeaderRead && checkHeaderSize)
				{
					assert(_transferedBytes == _header.size());
				}				
			
				if (_bMainHeaderRead)
				{
					if (_header.majorVersion != prevMajorVersion)
					{
						close();
						return false;
					}

					//if (_header.messageID != prevGuid)
					//{
					//	close();
					//	return false;
					//}
				}
				//else
				//{
				//	_majorVersion = magicWord;
				//	_messageID = guid;
				//}
			}			
			catch (...)
			{
				close();
				return false;
			}

			_bMainHeaderRead = true;
			return true;			
		}

		

		template<typename T>
		inline void checkGuard(T val)
		{
			if (_header.guarded)
			{
				td::BYTE bG;
				get((char*) &bG, 1);
				if (bG != (td::BYTE) td::getType(val))
				{
					close();
					Exception e;
					cnt::StringBuilder<td::String> str;
					str.appendCString("Unexpected guard byte! Expected: ");
					str << mu::Utils::c_str(td::getType(val));
					str.appendCString(" Obtained: ");
					str << mu::Utils::c_str( (td::DataType) bG);
					str.getString(e.str);
					
					throw e;
				}
			}
		}

		inline void checkGuardSer(ISerializable& ser)
		{
			if (_header.guarded)
			{
				td::BYTE bG;
				get((char*) &bG, 1);
				if (bG != ser.getGuardedByte())
				{
					close();
					Exception e;
					cnt::StringBuilder<td::String> str;
					str.appendCString("Unexpected guard byte! Expected (ISerializable): ");					

					mu::Utils u;
					
					str << u.c_str(ser.getGuardedByte());
					str.appendCString(" Obtained: ");
					str << u.c_str(bG);
					str.getString(e.str);

					throw e;
				}
			}
		}

	public:
		ArchiveIn(IBinSerializer& serializer)
			: Archive(serializer)
			, _revertBytes(false)
			, _bMainHeaderRead(false)
			, _bMessageHeaderRead(false)
		{
			//_allowedMajorVersion = calcMagicWord("MEMS");
		}		
		
		template <size_t size>
		void setSupportedMajorVersion(const char(&magicWord)[size])
		{
			static_assert(size != 4, "Magic word has to be 4 characters long!");
			td::UINT4 mw = calcMagicWord(&magicWord[0]);
			_allowedMajorVersion = mw;
		}

		inline void get(char* data, td::UINT4 nBytes)
		{
			_serializer.read(data, nBytes);
			_transferedBytes += nBytes;
		}

#pragma region Load Data

		ArchiveIn& operator >> (bool& b)
		{			
			checkGuard(b);
			td::BYTE b1;
			get((char*)&b1, 1);
			if (b1 > 1)
			{
				Exception e;
				e.str = "Value for bool out of scope!";
				throw e;
			}

			if (b1)
				b= true;
			else
				b=false;

			return *this;
		}

		ArchiveIn& operator >> (td::BYTE& b)
		{			
			checkGuard(b);
			get((char*)&b, 1);
			return *this;
		}

		ArchiveIn& operator >> (td::INT1& b)
		{
			checkGuard(b);
			get((char*)&b, 1);
			return *this;
		}

		ArchiveIn& operator >> (td::WORD& w)
		{			
			checkGuard(w);
			get((char*)&w, 2);

			if (_revertBytes)
				mu::Machine::swapEndian(w);

			return *this;
		}

		ArchiveIn& operator >> (td::INT2& s2)
		{		
			checkGuard(s2);
			get((char*)&s2, 2);
			if (_revertBytes)
				mu::Machine::swapEndian(s2);
			return *this;
		}

		ArchiveIn& operator >> (td::INT4& i4)
		{
			checkGuard(i4);
			get((char*)&i4, 4);
			if (_revertBytes)
				mu::Machine::swapEndian(i4);
			return *this;
		}

		ArchiveIn& operator >> (td::UINT4& ui4)
		{			
			checkGuard(ui4);
			get((char*)&ui4, 4);

			if (_revertBytes)
				mu::Machine::swapEndian(ui4);

			return *this;
		}

		ArchiveIn& operator >> (td::LINT8& li8)
		{			
			checkGuard(li8);
			get((char*)&li8, 8);
			if (_revertBytes)
				mu::Machine::swapEndian(li8);
			return *this;
		}

		ArchiveIn& operator >> (td::LUINT8& lui8)
		{			
			checkGuard(lui8);
			get((char*)&lui8, 8);
			if (_revertBytes)
				mu::Machine::swapEndian(lui8);
			return *this;
		}

		ArchiveIn& operator >> (td::String& s8)
		{				
			checkGuard(s8);
			if (s8.spaceForSize() == 2)
			{
				td::WORD len;
				//operator >> (len);
				get((char*) &len, 2);
				if (_revertBytes)
					mu::Machine::swapEndian(len);
				if (s8.prepareBuffer(len))
					get(s8.getBegin(), len);
			}
			else
			{
				td::UINT4 len;
				//operator >> (len);
				get((char*) &len, 4);
				if (_revertBytes)
					mu::Machine::swapEndian(len);
				if (s8.prepareBuffer(len))
					get(s8.getBegin(), len);
			}

			//if(s8.spaceForSize() == 2)
			//{
			//	td::WORD w;
			//	get((char*)&w, 2);

			//	if (_revertBytes)
			//		Machine::swapEndian(w);

			//	s8.prepareBuffer(w);			
			//	get(s8.getBegin(), w);	
			//}
			//else
			//{
			//	//duzina 4 bytea
			//	td::INT4 w;

			//	get((char*)&w, 4);

			//	if (_revertBytes)
			//		Machine::swapEndian(w);

			//	s8.prepareBuffer(w);			
			//	get(s8.getBegin(), w);	
			//}
			return *this;
		}

		ArchiveIn& operator >> (td::Color& c)
		{
			checkGuard(c);			
			get((char*)&c._color, 4);	
			if (_revertBytes)
				mu::Machine::swapEndian(c._color);
			return *this;
		}

		ArchiveIn& operator >> (td::Date& d)
		{				
			checkGuard(d);
			td::INT4 date;
			get((char*)&date, 4);
			if (_revertBytes)
				mu::Machine::swapEndian(date);
			d.setValue(date);
			return *this;
		}

		ArchiveIn& operator >> (td::Time& t)
		{				
			checkGuard(t);
			td::INT4 val;

			get((char*)&val, 4);

			if (_revertBytes)
				mu::Machine::swapEndian(val);

			t.setValue(val);

			return *this;
		}

		ArchiveIn& operator >> (td::DateTime& dt)
		{				
			checkGuard(dt);
			td::INT4 date;
			get((char*)&date, 4);

			td::INT4 time;
			get((char*)&time, 4);

			if (_revertBytes)
			{
				mu::Machine::swapEndian(date);
				mu::Machine::swapEndian(time);
			}

			dt.setDateTime(date, time);

			return *this;
		}

		ArchiveIn& operator >> (double& dbl)
		{		
			checkGuard(dbl);
			get((char*)&dbl, 8);
			if (_revertBytes)
			{			
				mu::Machine::swapEndian(dbl);
			}
			return *this;
		}

		ArchiveIn& operator >> (float& flt)
		{				
			checkGuard(flt);
			get((char*)&flt, 4);
			if (_revertBytes)
			{			
				mu::Machine::swapEndian(flt);
			}
			return *this;
		}

		ArchiveIn& operator >> (void*& ptr)
		{
			checkGuard(ptr);
			get((char*)&ptr, sizeof(void*));
			return *this;
		}

		ArchiveIn& operator >> (td::ColorID& clr)
		{
			//checkGuard(clr);
            td::BYTE iClr = (td::BYTE) td::ColorID::SysText;
            checkGuard(iClr);
			*this >> iClr;
            clr = (td::ColorID) iClr;
			return *this;
		}

		ArchiveIn& operator >> (td::LinePattern& lp)
		{
			//checkGuard(clr);
			td::BYTE iPattern = (td::BYTE)td::LinePattern::Solid;
			checkGuard(iPattern);
			*this >> iPattern;
			lp = (td::LinePattern)iPattern;
			return *this;
		}
        
        ArchiveIn& operator >> (td::DotPattern& lp)
        {
            //checkGuard(clr);
            td::BYTE iPattern = (td::BYTE)td::DotPattern::Pixel;
            checkGuard(iPattern);
            *this >> iPattern;
            lp = (td::DotPattern)iPattern;
            return *this;
        }
        
        ArchiveIn& operator >> (td::Anchor& anchor)
        {
            //checkGuard(clr);
            td::BYTE iAnchor = (td::BYTE)td::Anchor::Fixed;
            checkGuard(iAnchor);
            *this >> iAnchor;
            anchor = (td::Anchor)iAnchor;
            return *this;
        }

		template<typename THOLDER, int NDEC>
		ArchiveIn& operator >> (td::Decimal<THOLDER, NDEC>& dec)
		{
			checkGuard(dec);
			THOLDER val=0;
			get((char*)(&val), sizeof(THOLDER));
			dec.setValue(val);
			//dec = val;
			return *this;
		}

		ArchiveIn& operator >> (ISerializable& s)
		{		
			checkGuardSer(s);
			s.serialize(*this);
			return *this;
		}

		ArchiveIn& operator >> (ISerializable* s)
		{	
			checkGuardSer(*s);
			s->serialize(*this);
			return *this;
		}

		//this is method that all own seriazable classes should implement if they want to be loaded from binary archive
		template <class TSERIALIZABLE>
		ArchiveIn& operator >> (TSERIALIZABLE& s)
		{	
			if (!s.loadFromBinaryStream(*this))
			{
				assert(false);
				//mozda vratiti dummy arhivu
				//do something here
			}
			return *this;
		}

#pragma endregion
	};	
}
