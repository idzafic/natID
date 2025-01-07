// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/mu.h>

namespace td
{
	class GUID
	{
	public:
		union
		{
			char str[16];
            struct
            {
                td::LUINT8 hu;
                td::LUINT8 lu;
            };
			struct
			{
				td::LINT8 hi;
				td::LINT8 lo;				
			};
			struct  
			{
				td::UINT4 i0;
				td::UINT4 i1;
				td::UINT4 i2;
				td::UINT4 i3;
			};
		};
	public:
		GUID()
		{				
			//boost::uuids::uuid::const_iterator it(tmpUID.begin());
			//boost::uuids::uuid::const_iterator itEnd(tmpUID.end());
			//int i=0;			
			//while (it != itEnd)
			//{
			//	str[i++] = *it;
			//	++it;
			//}
		}

		//void fromBinString(const char* pStr)
		//{
		//	memcpy(str, pStr, 16);
		//}

		//void toBinString(char* pStr) const
		//{
		//	memcpy (pStr, str, 16);
		//}

		GUID(const GUID& uid)
		{
			memcpy(str, uid.str, 16);
		}

		GUID( bool)
		{
			reset();
		}

		GUID(td::LUINT8 inpHi, td::LUINT8 inpLo)
			:hi(inpHi)
			,lo(inpLo)
		{			
		}

		//td::LUINT8& hi()
		//{
		//	return hi;
		//}

		//td::LUINT8& lo()
		//{
		//	return lo;
		//}

		void operator = (const GUID& uid)
		{
			memcpy(str, uid.str, 16);
		}
		void reset()
		{
			mu::getGUID(str);
			//boost::uuids::uuid tmpUID = boost::uuids::uuid(boost::uuids::random_generator()()); 
			//memcpy(str, (char*) tmpUID.data, 16);
		}

		bool operator == (const GUID& uid) const
		{
			return ((hi == uid.hi) && (lo == uid.lo));
		}

		bool operator != (const GUID& uid) const
		{
			return ((hi != uid.hi) || (lo != uid.lo));
		}

		bool operator < (const GUID& uid) const
		{
			return ((hi < uid.hi) || ((hi == uid.hi) && (lo < uid.lo)));
		}

		bool operator <= (const GUID& uid) const
		{
			return ((hi <= uid.hi) || ((hi == uid.hi) && (lo <= uid.lo)));
		}

		bool operator >= (const GUID& uid) const
		{
			return ((hi >= uid.hi) || ((hi == uid.hi) && (lo >= uid.lo)));
		}

		bool operator > (const GUID& uid) const
		{
			return ((hi > uid.hi) || ((hi == uid.hi) && (lo > uid.lo)));
		}

		template <class TSTR>
		void show(TSTR& s) const
		{
			s << (td::LUINT8)hi << ":" << (td::LUINT8) lo;
			//s << i0 << "-" << i1 << "-" << i2 << "-" << i3 << "\n";
		}

		template <class TSTR>
		void showSigned(TSTR& s) const
		{
			s << (td::LINT8)hi << ":" << (td::LINT8)lo;
			//s << i0 << "-" << i1 << "-" << i2 << "-" << i3 << "\n";
		}

	protected:
		//output to ostream
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const GUID& guid)
		{
			guid.show(os); //show in simplified form

			//os << std::hex << std::setfill('0')
			//	<< setw(2) << (int)guid.str[0]
			//	<< setw(2) << (int)guid.str[1]
			//	<< setw(2) << (int)guid.str[2]
			//	<< setw(2) << (int)guid.str[3]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[4]
			//	<< setw(2) << (int)guid.str[5]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[6]
			//	<< setw(2) << (int)guid.str[7]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[8]
			//	<< setw(2) << (int)guid.str[9]
			//	<< "-"
			//	<< setw(2) << (int)guid.str[10]
			//	<< setw(2) << (int)guid.str[11]
			//	<< setw(2) << (int)guid.str[12]
			//	<< setw(2) << (int)guid.str[13]
			//	<< setw(2) << (int)guid.str[14]
			//	<< setw(2) << (int)guid.str[15];
			return os;
		}
	};

	inline DataType getType(const GUID&) {return guid;}
}
