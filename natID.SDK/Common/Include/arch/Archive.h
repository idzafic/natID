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
#include <td/String.h>
#include <td/DateTime.h>
#include <mu/Machine.h>
#include <arch/IBinSerializer.h>
#include <arch/ISerializable.h>
#include <mem/PointerDeleter.h>
#include <mem/PointerReleaser.h>
//#include "random/IntRandom.h"
#include <mu/Utils.h>
#include <td/Decimal.h>
#include <cnt/StringBuilder.h>
#include <td/GUID.h>
#include <arch/Header.h>
#include <td/Point.h>
#include <td/Rect.h>
#include <arch/EnumSerializer.h>
//#include <td/Variant.h>
//#include <cnt/List.h>

namespace arch
{
	class Exception
	{
	public:
		td::String str;	
		Exception()			
		{
		}

		template<class OSTREAM>
		void show(OSTREAM& o) const
		{			
			o << str.c_str() << std::endl;
		}
	};

	class Archive
	{
	protected:			
		td::LUINT8 _transferedBytes;
		td::LUINT8 _totalUsedBytes;
		IBinSerializer& _serializer;		
		Header _header;
			
	protected:
		inline td::UINT4 calcMagicWord(const char* str) const
		{
			assert(strlen(str) == 4);
			td::UINT4 mw = str[3];
			mw <<= 8;
			mw |= str[2];
			mw <<= 8;
			mw |= str[1];
			mw <<= 8;
			mw |= str[0];
			return mw;
		}

		inline void magicWordToString(char* buf, td::UINT4 mw) const
		{
			buf[0] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[1] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[2] = (char)(mw & 0x000000FF);
			mw >>= 8;
			buf[3] = (char)(mw & 0x000000FF);	
		}
	public:
		Archive(IBinSerializer& serializer, td::BYTE minorVersion = 0)
			: _transferedBytes(0)
			, _totalUsedBytes(0)
			, _serializer(serializer)			
		{
			_header.minorVersion = minorVersion;
		}		

		const Header* getHeader() const
		{
			return &_header;
		}
		//void setVersion(td::UINT4 ver)
		//{
		//	_currentVersion = ver;
		//}

		//td::UINT4 clientID() const
		//{
		//	return _clientID;
		//}

		//void clientID(td::UINT4 clientKey) 
		//{
		//	_clientID = clientKey;
		//}

		//const td::GUID& getID() const
		//{
			//const Header* pHeader = getHeader();
			//assert(pHeader);
			//return pHeader->messageID;
			//return _header.messageID;
		//}

		//void setID(const td::GUID&)
		//{
			//_header.messageID = messageKey;
		//}

		td::LUINT8 transferedBytes() const
		{
			return _transferedBytes;
		}

		td::BYTE getMinorVersion() const
		{
			return _header.minorVersion;
		}		

		td::UINT4 getMajorVersion() const
		{
			return _header.majorVersion;
		}
		
		//td::UINT4 getFileVersion()
		//{
		//	return _fileVersion;
		//}		

		virtual bool goTo(td::LUINT8 pos)
		{
			return _serializer.goTo(pos);
		}


		void close()
		{
			_serializer.close();
		}
	};	
}
