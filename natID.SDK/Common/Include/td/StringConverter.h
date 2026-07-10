// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StringConverter.h
    @brief Provides toString() and fromString() bridge implementations for Date, Time, DateTime, Color, and Decimal. */
#pragma once
#include <td/String.h>
#include <td/DateTime.h>
#include <cnt/StringBuilder.h>
#include <td/Color.h>

namespace td
{
	/// @brief Converts a Date to its canonical string representation.
	/// @return A String containing the formatted date.
	inline td::String Date::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	/// @brief Converts a Time to its canonical string representation.
	/// @return A String containing the formatted time.
	inline td::String Time::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	/// @brief Converts a DateTime to its canonical string representation.
	/// @return A String containing the formatted date and time.
	inline td::String DateTime::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

    /// @brief Converts a Color to its canonical string representation.
    /// @return A String containing the formatted color value.
    inline td::String Color::toString() const
    {
        char tmp[16];
        int nChars = formc_str(tmp);
        td::String str;
        str.fromKnownString(&tmp[0], nChars);
        return str;
    }

	/// @brief Converts a Decimal to its canonical string representation.
	/// @tparam THOLDER The underlying integer storage type.
	/// @tparam NDEC    Number of decimal places.
	/// @return A String containing the formatted decimal value.
	template <typename THOLDER, int NDEC>
	inline td::StringBase<UTF8, td::EncodingUTF8, 4, false> Decimal<THOLDER, NDEC>::toString() const
	{
		char tmp[32];
		int nChars = formc_str(tmp);
		td::String str;
		str.fromKnownString(&tmp[0], nChars);
		return str;
	}

	/// @brief Parses a Date from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Date::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	/// @brief Sets a Date value from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Date::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		setValueFromString(str.c_str());
	}

	/// @brief Parses a Time from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Time::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	/// @brief Sets a Time value from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void Time::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		setValueFromString(str.c_str());
	}

	/// @brief Parses a DateTime from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
	template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
	void DateTime::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
	{
		fromString(str.c_str());
	}

	/// @brief Sets a DateTime value from a StringBase.
	/// @tparam T_CHAR Character type of the string.
	/// @tparam Encoding String encoding identifier.
	/// @tparam SPACE_FOR_SIZE Internal size storage parameter.
	/// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
	/// @param str The source string to parse.
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

    /// @brief Parses a Decimal from a StringBase.
    /// @tparam THOLDER The underlying integer storage type.
    /// @tparam NDEC    Number of decimal places.
    /// @tparam T_CHAR Character type of the string.
    /// @tparam Encoding String encoding identifier.
    /// @tparam SPACE_FOR_SIZE Internal size storage parameter.
    /// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
    /// @param str The source string to parse.
    template <typename THOLDER, int NDEC>
    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    void Decimal<THOLDER, NDEC>::fromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
    {
        fromString(str.c_str());
    }

    /// @brief Sets a Decimal value from a StringBase.
    /// @tparam THOLDER The underlying integer storage type.
    /// @tparam NDEC    Number of decimal places.
    /// @tparam T_CHAR Character type of the string.
    /// @tparam Encoding String encoding identifier.
    /// @tparam SPACE_FOR_SIZE Internal size storage parameter.
    /// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
    /// @param str The source string to parse.
    template <typename THOLDER, int NDEC>
    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    void Decimal<THOLDER, NDEC>::setValueFromString(const td::StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>& str)
    {
        setValueFromString(str.c_str());
    }

    /// @brief Computes a hash number for a StringBase using the mu::Utils helper.
    /// @tparam T_CHAR Character type of the string.
    /// @tparam Encoding String encoding identifier.
    /// @tparam SPACE_FOR_SIZE Internal size storage parameter.
    /// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
    /// @return A 32-bit hash value derived from the string contents.
    template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE, bool EXTERN_ALLOCATOR>
    td::UINT4 StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, EXTERN_ALLOCATOR>::calcHashNo() const
    {
        return mu::Utils::calcHashNo(c_str());
    }

    /// @brief Returns a size_t hash value for use with standard hash containers.
    /// @tparam T_CHAR Character type of the string.
    /// @tparam Encoding String encoding identifier.
    /// @tparam SPACE_FOR_SIZE Internal size storage parameter.
    /// @tparam EXTERN_ALLOCATOR Whether the string uses an external allocator.
    /// @return A size_t hash value derived from the string contents.
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
