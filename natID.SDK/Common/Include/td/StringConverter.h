// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <td/DateTime.h>
#include <cnt/StringBuilder.h>
#include <td/Color.h>

namespace td
{
	inline td::String Date::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	inline td::String Time::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	inline td::String DateTime::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

    inline td::String Color::toString() const
    {
        char tmp[16];
        int nChars = formc_str(tmp);
        td::String str;
        str.fromKnownString(&tmp[0], nChars);
        return str;
    }

	template <typename THOLDER, int NDEC>	
	inline td::StringBase<UTF8, td::EncodingUTF8, 4, false> Decimal<THOLDER, NDEC>::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Date::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Date::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		setValueFromString(str.c_str());
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Time::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Time::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		setValueFromString(str.c_str());
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void DateTime::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void DateTime::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		setValueFromString(str.c_str());
	}

//	template <typename THOLDER, int NDEC, typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
//	void Decimal<THOLDER, NDEC>::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
//	{
//		fromString(str.c_str());
//	}

//	template <typename THOLDER, int NDEC, typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
//	void Decimal<THOLDER, NDEC>::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
//	{
//		setValueFromString(str.c_str());
//	}
	
    template <typename THOLDER, int NDEC>
    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    void Decimal<THOLDER, NDEC>::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
    {
        fromString(str.c_str());
    }

    template <typename THOLDER, int NDEC>
    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    void Decimal<THOLDER, NDEC>::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
    {
        setValueFromString(str.c_str());
    }

    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    td::UINT4 StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::calcHashNo() const
    {
        return mu::Utils::calcHashNo(c_str());
    }

    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    size_t StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::hash_value() const
    {
        return (size_t) mu::Utils::calcHashNo(c_str());
    }


//    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
//    size_t StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::hash_value(StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
//    {
//        return (size_t) mu::Utils::calcHashNo(str.c_str());
//    }

	//template <typename TSTRARR>
	//inline void td::StringBase<UTF8, td::EncodingUTF8, 4, false>::fromStringArray(const TSTRARR& arr, UTF8 chSep )
	//{
	//	//static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on StringExt");
	//	//assert(!EXTERN_ALLOCATOR);
	//	//cleanBuffer();
	//	//setString(ctStrIn, (int)size - 1);
	//	cnt::S
	//}
}
