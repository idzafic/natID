// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ArchiveIn.h
 * @brief Binary deserialization archive for reading typed data from a stream.
 */
#pragma once
#include <arch/Archive.h>

namespace arch
{
	/// @brief Archive class for reading (deserializing) typed binary data from an input stream.
	class ArchiveIn : public Archive
	{
	protected:
		bool _revertBytes; ///< True if byte order must be reversed when reading multi-byte values.
		bool _bMainHeaderRead; ///< True if the main archive header has been successfully read.
		bool _bMessageHeaderRead; ///< True if the message header has been successfully read.
		//std::set<td::UINT4> _allowedMajorVersions;
		td::UINT4 _allowedMajorVersion = 0; ///< Allowed major version number; 0 means any version is accepted.
		//td::LUINT8 _firstErrorPosition = 0;
	protected:
		/// @brief Decodes the archive header from the stream or from a pre-read buffer.
		/// @param pHeader Pointer to an existing header buffer, or nullptr to read from the stream.
		/// @return True if the header was decoded successfully, false on error.
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



		/// @brief Reads and validates the guard byte for a typed value when guarding is enabled.
		/// @tparam T Type of the value being guarded.
		/// @param val The value whose type is used to check the guard byte.
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

		/// @brief Reads and validates the guard byte for an ISerializable object when guarding is enabled.
		/// @param ser The serializable object whose guard byte is expected.
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
		/// @brief Constructs an ArchiveIn using the provided binary serializer.
		/// @param serializer The binary serializer used to read raw bytes.
		ArchiveIn(IBinSerializer& serializer)
			: Archive(serializer)
			, _revertBytes(false)
			, _bMainHeaderRead(false)
			, _bMessageHeaderRead(false)
		{
			//_allowedMajorVersion = calcMagicWord("MEMS");
		}

		/// @brief Sets the only accepted major version for this archive.
		/// @tparam size Length of the magic word string literal (must be 4 characters + null).
		/// @param magicWord A 4-character magic word identifying the supported major version.
		template <size_t size>
		void setSupportedMajorVersion(const char(&magicWord)[size])
		{
			static_assert(size != 4, "Magic word has to be 4 characters long!");
			td::UINT4 mw = calcMagicWord(&magicWord[0]);
			_allowedMajorVersion = mw;
		}

		/// @brief Reads a block of raw bytes from the underlying serializer.
		/// @param data Pointer to the destination buffer.
		/// @param nBytes Number of bytes to read.
		inline void get(char* data, td::UINT4 nBytes)
		{
			_serializer.read(data, nBytes);
			_transferedBytes += nBytes;
		}

#pragma region Load Data

		/// @brief Reads a bool value from the archive.
		/// @param b Reference to the bool variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a td::BYTE value from the archive.
		/// @param b Reference to the byte variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::BYTE& b)
		{
			checkGuard(b);
			get((char*)&b, 1);
			return *this;
		}

		/// @brief Reads a td::INT1 value from the archive.
		/// @param b Reference to the signed byte variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::INT1& b)
		{
			checkGuard(b);
			get((char*)&b, 1);
			return *this;
		}

		/// @brief Reads a td::WORD (16-bit unsigned) value from the archive.
		/// @param w Reference to the word variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::WORD& w)
		{
			checkGuard(w);
			get((char*)&w, 2);

			if (_revertBytes)
				mu::Machine::swapEndian(w);

			return *this;
		}

		/// @brief Reads a td::INT2 (16-bit signed) value from the archive.
		/// @param s2 Reference to the variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::INT2& s2)
		{
			checkGuard(s2);
			get((char*)&s2, 2);
			if (_revertBytes)
				mu::Machine::swapEndian(s2);
			return *this;
		}

		/// @brief Reads a td::INT4 (32-bit signed) value from the archive.
		/// @param i4 Reference to the variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::INT4& i4)
		{
			checkGuard(i4);
			get((char*)&i4, 4);
			if (_revertBytes)
				mu::Machine::swapEndian(i4);
			return *this;
		}

		/// @brief Reads a td::UINT4 (32-bit unsigned) value from the archive.
		/// @param ui4 Reference to the variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::UINT4& ui4)
		{
			checkGuard(ui4);
			get((char*)&ui4, 4);

			if (_revertBytes)
				mu::Machine::swapEndian(ui4);

			return *this;
		}

		/// @brief Reads a td::LINT8 (64-bit signed) value from the archive.
		/// @param li8 Reference to the variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::LINT8& li8)
		{
			checkGuard(li8);
			get((char*)&li8, 8);
			if (_revertBytes)
				mu::Machine::swapEndian(li8);
			return *this;
		}

		/// @brief Reads a td::LUINT8 (64-bit unsigned) value from the archive.
		/// @param lui8 Reference to the variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::LUINT8& lui8)
		{
			checkGuard(lui8);
			get((char*)&lui8, 8);
			if (_revertBytes)
				mu::Machine::swapEndian(lui8);
			return *this;
		}

		/// @brief Reads a td::String value from the archive.
		/// @param s8 Reference to the string to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a td::Color value from the archive.
		/// @param c Reference to the color variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::Color& c)
		{
			checkGuard(c);
			get((char*)&c._color, 4);
			if (_revertBytes)
				mu::Machine::swapEndian(c._color);
			return *this;
		}

		/// @brief Reads a td::Date value from the archive.
		/// @param d Reference to the date variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a td::Time value from the archive.
		/// @param t Reference to the time variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a td::DateTime value from the archive.
		/// @param dt Reference to the date-time variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a double-precision floating-point value from the archive.
		/// @param dbl Reference to the double variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a single-precision floating-point value from the archive.
		/// @param flt Reference to the float variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads a raw pointer value from the archive.
		/// @param ptr Reference to the pointer variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (void*& ptr)
		{
			checkGuard(ptr);
			get((char*)&ptr, sizeof(void*));
			return *this;
		}

		/// @brief Reads a td::ColorID enum value from the archive.
		/// @param clr Reference to the ColorID variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::ColorID& clr)
		{
			//checkGuard(clr);
            td::BYTE iClr = (td::BYTE) td::ColorID::SysText;
            checkGuard(iClr);
			*this >> iClr;
            clr = (td::ColorID) iClr;
			return *this;
		}

		/// @brief Reads a td::LinePattern enum value from the archive.
		/// @param lp Reference to the LinePattern variable to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (td::LinePattern& lp)
		{
			//checkGuard(clr);
			td::BYTE iPattern = (td::BYTE)td::LinePattern::Solid;
			checkGuard(iPattern);
			*this >> iPattern;
			lp = (td::LinePattern)iPattern;
			return *this;
		}

		/// @brief Reads a td::DotPattern enum value from the archive.
		/// @param lp Reference to the DotPattern variable to populate.
		/// @return Reference to this archive for chaining.
        ArchiveIn& operator >> (td::DotPattern& lp)
        {
            //checkGuard(clr);
            td::BYTE iPattern = (td::BYTE)td::DotPattern::Pixel;
            checkGuard(iPattern);
            *this >> iPattern;
            lp = (td::DotPattern)iPattern;
            return *this;
        }

		/// @brief Reads a td::Anchor enum value from the archive.
		/// @param anchor Reference to the Anchor variable to populate.
		/// @return Reference to this archive for chaining.
        ArchiveIn& operator >> (td::Anchor& anchor)
        {
            //checkGuard(clr);
            td::BYTE iAnchor = (td::BYTE)td::Anchor::Fixed;
            checkGuard(iAnchor);
            *this >> iAnchor;
            anchor = (td::Anchor)iAnchor;
            return *this;
        }

		/// @brief Reads a td::Decimal value from the archive.
		/// @tparam THOLDER Underlying integer type holding the decimal.
		/// @tparam NDEC Number of decimal places.
		/// @param dec Reference to the decimal variable to populate.
		/// @return Reference to this archive for chaining.
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

		/// @brief Reads an ISerializable object from the archive by reference.
		/// @param s Reference to the ISerializable object to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (ISerializable& s)
		{
			checkGuardSer(s);
			s.serialize(*this);
			return *this;
		}

		/// @brief Reads an ISerializable object from the archive by pointer.
		/// @param s Pointer to the ISerializable object to populate.
		/// @return Reference to this archive for chaining.
		ArchiveIn& operator >> (ISerializable* s)
		{
			checkGuardSer(*s);
			s->serialize(*this);
			return *this;
		}

		//this is method that all own seriazable classes should implement if they want to be loaded from binary archive
		/// @brief Reads any serializable class that implements loadFromBinaryStream().
		/// @tparam TSERIALIZABLE Type that provides a loadFromBinaryStream() method.
		/// @param s Reference to the object to populate.
		/// @return Reference to this archive for chaining.
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
