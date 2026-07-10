// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Conv.h
    @brief String-to-number and number-to-string conversion utilities for td types. */
#pragma once

#include <td/Types.h>
#include <mu/Machine.h>
#include <tuple>
#include <charconv>
#include <system_error>
#include <cstdlib>
#include <cstring>
#include <type_traits>
#include <mu/mu.h>

namespace td
{

// Check if we can use from_chars (c++ 17)
#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    #ifndef __APPLE__
        #define TD_USE_STD_FROM_CHARS 1
    #else
        // On macOS, check if deployment target is macOS 13+
        #ifdef __MAC_OS_X_VERSION_MIN_REQUIRED
            #if __MAC_OS_X_VERSION_MIN_REQUIRED >= 130000
                #define TD_USE_STD_FROM_CHARS 1
            #endif
        #else
            // If no deployment target specified, assume older macOS
            #define TD_USE_STD_FROM_CHARS 0
        #endif
    #endif
#else
    #define TD_USE_STD_FROM_CHARS 0
#endif

/// @brief Converts a C string to an arithmetic value of type T.
/// @param str Pointer to the null-terminated source string.
/// @param strLen Length of the string, or -1 to compute automatically.
/// @return Parsed value of type T, or T{0} on failure.
template<typename T>
T toNumber(const char* str, int strLen=-1)
{
    static_assert(std::is_arithmetic<T>::value, "T must be arithmetic type");

    if (!str)
        return T{0};

    if (strLen < 0)
        strLen = (int) strlen(str);

    if (strLen == 0)
        return T{0};

    T result = T{0};

#if TD_USE_STD_FROM_CHARS
    auto [ptr, ec] = std::from_chars(str, str + strLen, result);
    if (ec != std::errc())
        result = 0;  // or throw
    return result;
#else
    // Fallback implementation (used on all platforms if from_chars unavailable)
    if constexpr (std::is_same<T, bool>::value) {
        result = (str[0] == '1' || str[0] == 't' || str[0] == 'T' ||
                  str[0] == 'y' || str[0] == 'Y');
    }
    else if constexpr (std::is_integral<T>::value) {
        if constexpr (std::is_unsigned<T>::value) {
            result = static_cast<T>(std::strtoull(str, nullptr, 10));
        } else {
            result = static_cast<T>(std::strtoll(str, nullptr, 10));
        }
    }
    else if constexpr (std::is_floating_point<T>::value) {
        result = static_cast<T>(std::strtod(str, nullptr));
    }

    return result;
#endif
}

/// @brief Converts a character string to a boolean value by checking for "true".
/// @param pStr Pointer to the character array.
/// @param nLen Length of the string.
/// @return True if the string equals "true" (case-sensitive first char check), false otherwise.
template <typename TCH>
inline bool toBoolean(const TCH* pStr, int nLen)
{
    if (nLen == 4)
    {
        if ( ((pStr[0] == 't') || (pStr[0] == 'T')) && (pStr[1] == 'r') && (pStr[2] == 'u') && (pStr[3] == 'e'))
            return true;
    }
    return false;
}


/// @brief Converts a C string to a floating-point value of type TFLOAT.
/// @param str Pointer to the null-terminated source string.
/// @return Parsed floating-point value.
template <typename TFLOAT>
inline TFLOAT toFloat(const char* str)
{
    return TFLOAT(mu::toDouble(str));
}

/// @brief Converts a C string to an integer value of type TINT.
/// @param str Pointer to the null-terminated source string.
/// @return Parsed integer value.
template <typename TINT>
inline TINT toInt(const char* str)
{
    return (TINT) atol(str);
}

/// @brief Converts a C string to a floating-point value, storing the result in val.
/// @param str Pointer to the null-terminated source string.
/// @param val Reference to the output floating-point variable.
template <typename TFLOAT>
inline void toFloat(const char* str, TFLOAT& val)
{
    if (!str)
    {
        val = 0;
        return;
    }

    val = TFLOAT(mu::toDouble(str));
}

/// @brief Converts a C string to an integer value, storing the result in val.
/// @param str Pointer to the null-terminated source string.
/// @param val Reference to the output integer variable.
template <typename TINT>
inline void toInt(const char* str, TINT& val)
{
    val = (TINT) atol(str);
}

/// @brief Converts a C string to a boolean value by comparing to "true" (case-insensitive).
/// @param str Pointer to the null-terminated source string.
/// @return True if the string equals "true", false otherwise.
inline bool toBoolean(const char* str)
{
    auto len = strlen(str);
    if (len != 4)
        return false;
    if (td::compareNoCase(str, "true") == 0)
        return true;

    return false;
}

//prebaceno u gui/adv/Converter.h
/// @brief Converts a string to a td::HAlignment value.
/// @param str Source string containing "Left", "Right", or "Center".
/// @return Corresponding HAlignment enum value; defaults to Left.
template <typename TSTR>
inline td::HAlignment toHAlignment(const TSTR& str)
{
//    if (str.cCompareConstStr("Left"))
//        return td::HAlignment::Left;
    auto nChars = str.length();
    if (nChars < 5)
        return td::HAlignment::Left;
    auto ch = str.getAt(0);
    if (ch == 'R' || ch=='r')
    {
        if (str.compareConstStr("ight", 1))
            return td::HAlignment::Right;
    }
    else if (ch == 'C' || ch=='c')
    {
        if (str.compareConstStr("enter", 1))
            return td::HAlignment::Center;
    }
    return td::HAlignment::Left;
}

/// @brief Converts a string to a td::VAlignment value.
/// @param str Source string containing "Top", "Bottom", or "Center".
/// @return Corresponding VAlignment enum value; defaults to Top.
template <typename TSTR>
inline td::VAlignment toVAlignment(const TSTR& str)
{
//    if (str.cCompareConstStr("Top"))
//        return td::VAlignment::Top;
    auto nChars = str.length();
    if (nChars < 6)
        return td::VAlignment::Top;

    auto ch = str.getAt(0);
    if (ch == 'B' || ch=='b')
    {
        if (str.compareConstStr("ottom", 1))
            return td::VAlignment::Bottom;
    }
    else if (ch == 'C' || ch=='c')
    {
        if (str.compareConstStr("enter", 1))
            return td::VAlignment::Center;
    }

    return td::VAlignment::Top;
}

/// @brief Converts a string to a td::TextAlignment value.
/// @param str Source string containing "Left", "Right", "Center", or "Justify".
/// @return Corresponding TextAlignment enum value; defaults to Left.
template <typename TSTR>
inline td::TextAlignment toTextAlignment(const TSTR& str)
{
//    if (str.cCompareConstStr("Left"))
//        return td::TextAlignment::Left;
    auto nChars = str.length();
    if (nChars < 5)
        return td::TextAlignment::Left;

    auto ch = str.getAt(0);
    if (ch == 'R' || ch=='r')
    {
        if (str.compareConstStr("ight", 1))
            return td::TextAlignment::Right;
    }
    else if (ch == 'C' || ch=='c')
    {
        if (str.compareConstStr("enter", 1))
            return td::TextAlignment::Center;
    }
    else if (ch == 'J' || ch=='j')
    {
        if (str.compareConstStr("ustify", 1))
            return td::TextAlignment::Justified;
    }
    return td::TextAlignment::Left;
}

/// @brief Converts a string to a td::TextWrap value.
/// @param str Source string containing "No", "Word", or "Any".
/// @return Corresponding TextWrap enum value; defaults to No.
template <typename TSTR>
inline td::TextWrap toTextWrap(const TSTR& str)
{
    auto nChars = str.length();
    if (nChars < 3 || nChars > 4)
        return td::TextWrap::No;
    auto ch = str.getAt(0);
    if (ch == 'W' || ch=='w')
    {
        if (str.compareConstStr("ord", 1))
            return td::TextWrap::Word;
    }
    else if (ch == 'A' || ch=='a')
    {
        if (str.compareConstStr("ny", 1))
            return td::TextWrap::Any;
    }
    return td::TextWrap::No;
}

/// @brief Converts a td::HAlignment value to its string representation.
/// @param ha The HAlignment enum value.
/// @return Pointer to a constant string ("Left", "Center", "Right", or "NHA").
inline const char* toString(td::HAlignment ha)
{
    switch(ha)
    {
        case td::HAlignment::Left: return "Left";
        case td::HAlignment::Center: return "Center";
        case td::HAlignment::Right: return "Right";
        default: return "NHA";
    }
}

/// @brief Converts a td::TextAlignment value to its string representation.
/// @param ta The TextAlignment enum value.
/// @return Pointer to a constant string ("Left", "Center", "Right", "Justified", or "NTA").
inline const char* toString(td::TextAlignment ta)
{
    switch(ta)
    {
        case td::TextAlignment::Left: return "Left";
        case td::TextAlignment::Center: return "Center";
        case td::TextAlignment::Right: return "Right";
        case td::TextAlignment::Justified: return "Justified";
        default: return "NTA";
    }
}

/// @brief Converts a td::VAlignment value to its string representation.
/// @param va The VAlignment enum value.
/// @return Pointer to a constant string ("Top", "Center", "Bottom", or "NVA").
inline const char* toString(td::VAlignment va)
{
    switch(va)
    {
        case td::VAlignment::Top: return "Top";
        case td::VAlignment::Center: return "Center";
        case td::VAlignment::Bottom: return "Bottom";
        default: return "NVA";
    }
}

/// @brief Converts a td::TextWrap value to its string representation.
/// @param tw The TextWrap enum value.
/// @return Pointer to a constant string ("No", "Word", "Any", or "NTW").
inline const char* toString(td::TextWrap tw)
{
    switch(tw)
    {
        case td::TextWrap::No: return "No";
        case td::TextWrap::Word: return "Word";
        case td::TextWrap::Any: return "Any";
        default: return "NTW";
    }
}
//inline td::tVAlignment toVAlignment(const td::String& str)
//{
//	return toVAlignment(str.c_str());
//}

    /// @brief Converts a string to a td::DataType enum value.
    /// @param str Source string containing the data type name (e.g., "int4", "real8", "string").
    /// @return Corresponding DataType enum value, or TD_NONE if not recognized.
	template <typename TSTR>
	inline td::DataType toDataType(const TSTR& str)
	{
	    if (str.compareConstStr("string"))
	        return td::string8;
	    if (str.compareConstStr("string8"))
	        return td::string8;
	    if (str.compareConstStr("word"))
	        return td::word;
	    if (str.compareConstStr("int2"))
	        return td::int2;
	    if (str.compareConstStr("uint4"))
	        return td::uint4;
	    if (str.compareConstStr("int"))
	        return td::int4;
	    if (str.compareConstStr("int4"))
	        return td::int4;
	    if (str.compareConstStr("lint"))
	        return td::lint8;
	    if (str.compareConstStr("lint8"))
	        return td::lint8;
	    if (str.compareConstStr("luint"))
	        return td::luint8;
	    if (str.compareConstStr("luint8"))
	        return td::luint8;
	    if (str.compareConstStr("real4"))
	        return td::real4;
	    if (str.compareConstStr("real8"))
	        return td::real8;
	    if (str.compareConstStr("boolean"))
	        return td::boolean;
	    if (str.compareConstStr("bool"))
	        return td::boolean;
	    if (str.compareConstStr("byte"))
	        return td::byte;
	    if (str.compareConstStr("enumerator"))
	        return td::enumerator;
	    if (str.compareConstStr("date"))
	        return td::date;
	    if (str.compareConstStr("time"))
	        return td::time;
	    if (str.compareConstStr("dateTime"))
	        return td::dateTime;
	    if (str.compareConstStr("decimal0"))
	        return td::decimal0;
	    if (str.compareConstStr("decimal1"))
	        return td::decimal1;
	    if (str.compareConstStr("decimal2"))
	        return td::decimal2;
	    if (str.compareConstStr("decimal3"))
	        return td::decimal3;
	    if (str.compareConstStr("decimal4"))
	        return td::decimal4;
	    if (str.compareConstStr("smallDecimal0"))
	        return td::smallDecimal0;
	    if (str.compareConstStr("smallDecimal1"))
	        return td::smallDecimal1;
	    if (str.compareConstStr("smallDecimal2"))
	        return td::smallDecimal2;
	    if (str.compareConstStr("smallDecimal3"))
	        return td::smallDecimal3;
	    if (str.compareConstStr("smallDecimal4"))
	        return td::smallDecimal4;
	    if (str.compareConstStr("ch"))
	        return td::ch;
	    if (str.compareConstStr("nch"))
	        return td::nch;
	    if (str.compareConstStr("ch7"))
	        return td::ch7;
	    if (str.compareConstStr("chFix"))
	        return td::chFix;
	    if (str.compareConstStr("nchFix"))
	        return td::nchFix;
	    if (str.compareConstStr("binary"))
	        return td::binary;
	    if (str.compareConstStr("color"))
	        return td::color;

	    return td::TD_NONE;
	}

	//Izbacio treba vidjeti koristi li se gdje
	//	inline td::DataType toDataType2(const char* str)
	//	{
	//		if (td::compareNoCase(str, "boolean") == 0)
	//			return td::boolean;
	//		if (td::compareNoCase(str, "bool") == 0)
	//			return td::boolean;
	//		if (td::compareNoCase(str, "byte") == 0)
	//			return td::byte;
	//		if (td::compareNoCase(str, "word") == 0)
	//			return td::word;
	//		if (td::compareNoCase(str, "int2") == 0)
	//			return td::int2;
	//		if (td::compareNoCase(str, "uint4") == 0)
	//			return td::uint4;
	//		if (td::compareNoCase(str, "int") == 0)
	//			return td::int4;
	//		if (td::compareNoCase(str, "int4") == 0)
	//			return td::int4;
	//		if (td::compareNoCase(str, "lint") == 0)
	//			return td::lint8;
	//		if (td::compareNoCase(str, "lint8") == 0)
	//			return td::lint8;
	//		if (td::compareNoCase(str, "luint") == 0)
	//			return td::luint8;
	//		if (td::compareNoCase(str, "luint8") == 0)
	//			return td::luint8;
	//		if (td::compareNoCase(str, "real4") == 0)
	//			return td::real4;
	//		if (td::compareNoCase(str, "real8") == 0)
	//			return td::real8;
	//		if (td::compareNoCase(str, "string") == 0)
	//			return td::string8;
	//		if (td::compareNoCase(str, "string8") == 0)
	//			return td::string8;
	//		if (td::compareNoCase(str, "enumerator") == 0)
	//			return td::enumerator;
	//		if (td::compareNoCase(str, "date") == 0)
	//			return td::date;
	//		if (td::compareNoCase(str, "time") == 0)
	//			return td::time;
	//		if (td::compareNoCase(str, "dateTime") == 0)
	//			return td::dateTime;
	//		if (td::compareNoCase(str, "decimal0") == 0)
	//			return td::decimal0;
	//		if (td::compareNoCase(str, "decimal1") == 0)
	//			return td::decimal1;
	//		if (td::compareNoCase(str, "decimal2") == 0)
	//			return td::decimal2;
	//		if (td::compareNoCase(str, "decimal3") == 0)
	//			return td::decimal3;
	//		if (td::compareNoCase(str, "decimal4") == 0)
	//			return td::decimal4;
	//		if (td::compareNoCase(str, "smallDecimal0") == 0)
	//			return td::smallDecimal0;
	//		if (td::compareNoCase(str, "smallDecimal1") == 0)
	//			return td::smallDecimal1;
	//		if (td::compareNoCase(str, "smallDecimal2") == 0)
	//			return td::smallDecimal2;
	//		if (td::compareNoCase(str, "smallDecimal3") == 0)
	//			return td::smallDecimal3;
	//		if (td::compareNoCase(str, "smallDecimal4") == 0)
	//			return td::smallDecimal4;
	//		if (td::compareNoCase(str, "ch") == 0)
	//			return td::ch;
	//		if (td::compareNoCase(str, "nch") == 0)
	//			return td::nch;
	//		if (td::compareNoCase(str, "ch7") == 0)
	//			return td::ch7;
	//		if (td::compareNoCase(str, "chFix") == 0)
	//			return td::chFix;
	//		if (td::compareNoCase(str, "nchFix") == 0)
	//			return td::nchFix;
	//		if (td::compareNoCase(str, "binary") == 0)
	//			return td::binary;
	//
	//		return td::TD_NONE;
	//	}

    /// @brief Converts a C string to a td::PresentationType enum value.
    /// @param str Pointer to the null-terminated source string ("yesNo", "trueFalse", "checkBox").
    /// @return Corresponding PresentationType value; defaults to PT_SameAsDT.
	inline td::PresentationType toPresentationType(const char* str)
	{
	    if (td::compareNoCase(str, "yesNo") == 0)
	        return td::PresentationType::PT_YesNo;
	    if (td::compareNoCase(str, "trueFalse") == 0)
	        return td::PresentationType::PT_TrueFalse;
	    if (td::compareNoCase(str, "checkBox") == 0)
	        return td::PresentationType::PT_CheckBox;
	    return td::PresentationType::PT_SameAsDT;
	}



    /// @brief Converts a boolean value to its string representation.
    /// @param bVal The boolean value.
    /// @return "true" if bVal is true, "false" otherwise.
    inline const char* boolToStr(bool bVal)
    {
        if (bVal)
            return "true";

        return "false";
    }


    /// @brief Converts a character to uppercase.
    /// @param ch The input character.
    /// @return The uppercase equivalent, or the original character if not a lowercase letter.
    template <typename TCHAR>
    inline TCHAR toUpper(TCHAR ch)
    {
        if ((ch >= 97) && (ch <= 122))
        {
            ch -= 32;
        }
        return ch;
    }

    /// @brief Converts a character to lowercase.
    /// @param ch The input character.
    /// @return The lowercase equivalent, or the original character if not an uppercase letter.
    template <typename TCHAR>
    inline TCHAR toLower(TCHAR ch)
    {
        if ((ch >= 65) && (ch <= 90))
        {
            ch += 32;
        }
        return ch;

    }

    /// @brief Converts all characters in a string to uppercase in-place.
    /// @param pStr Pointer to the null-terminated string to modify.
    template <typename TCHAR>
    void toUpper(TCHAR* pStr)
    {
        if (!pStr)
            return;
        while(*pStr != 0)
        {
            //TCHAR chIn = *pStr;
            if ((*pStr >= 97) && (*pStr <= 122))
            {
                *pStr -= 32;
            }
            ++pStr;
        }
    }

    /// @brief Converts all characters in a string to lowercase in-place.
    /// @param pStr Pointer to the null-terminated string to modify.
    template <typename TCHAR>
    void toLower(TCHAR* pStr)
    {
        if (!pStr)
            return;

        while(*pStr != 0)
        {
            //TCHAR chIn = *pStr;
            if ((*pStr >= 65) && (*pStr <= 90))
            {
                *pStr += 32;
            }
            ++pStr;
        }
    }

    /// @brief Converts a single hexadecimal character to its decimal byte value.
    /// @param hex The hexadecimal character ('0'-'9', 'a'-'f', 'A'-'F').
    /// @return Decimal value (0-15), or 0 for unrecognized input.
    inline td::BYTE hexToDec(char hex)
    {
        if (hex >= '0' && hex <= '9')
        {
            return (td::BYTE) (hex - '0');
        }
        else
        {
            hex = td::toUpper(hex);
            td::BYTE baseV = 10;
            if (hex >= 'A' && hex <= 'F')
            {
                return (baseV + (td::BYTE) (hex - 'A'));
            }
        }
        return 0;
    }

    /// @brief Converts a hexadecimal string to a binary value of type T.
    /// @param hex Pointer to the hexadecimal string (must be even length).
    /// @param lenHex Number of hex characters to read.
    /// @param val Output value of type T filled from the hex data.
    /// @return True on success, false if the hex length is odd or too large for T.
    template <typename T>
    inline bool hexToDec(const char* hex, size_t lenHex, T& val)
    {
        val = 0;
        if (lenHex % 2 != 0)
        {
            assert(false);
            return false;
        }
        size_t n = lenHex / 2;
        char* pOut = (char*) &val;
        size_t nOut = sizeof(T);
        //memset(pOut, 0, nOut);

        if (nOut < n)
        {
            return false;
        }

        for (size_t i = 0; i < n; ++i)
        {
            unsigned char ch1 = (unsigned char) hex[i * 2 + 0];
            unsigned char ch2 = (unsigned char) hex[i * 2 + 1];
            td::BYTE bVal = (td::hexToDec(ch1) << 4) | (td::hexToDec(ch2));
            pOut[i] = bVal;
        }

        if (mu::Machine::getEndianInteger() == mu::Machine::Endian::Little)
        {
            mu::Machine::swapEndian(val);
        }
        return true;
    }

    ///pHexOut must be at least 2 * len in size
    /// @brief Converts a binary byte array to its hexadecimal string representation.
    /// @param pStr Pointer to the input binary data.
    /// @param len Number of bytes to convert.
    /// @param outStr Output buffer; must be at least 2*len characters.
    inline void toHex(const char* pStr, size_t len, char* outStr)
    {
        const char pHex[] = "0123456789ABCDEF";
        const td::BYTE* pCh = (const td::BYTE*) pStr;
        for (size_t i = 0; i < len; ++i)
        {
            td::BYTE ch = pCh[i];
            //extract upper part
            td::BYTE upper = (ch >> 4) & 0x0F;
            size_t j = 2 * i;
            outStr[j] = pHex[upper];
            td::BYTE lower = ch & 0x0F;
            outStr[j + 1] = pHex[lower];
        }
    }

    /// @brief Converts a single byte to two hexadecimal characters.
    /// @param c The input byte.
    /// @param hex Output array of two characters representing the byte in hex.
    inline void charToHex(unsigned char c, unsigned char hex[2])
    {
        unsigned char hex1 = c / 16;
        unsigned char hex2 = c % 16;
        hex1 += hex1 <= 9 ? '0' : 'a' - 10;
        hex2 += hex2 <= 9 ? '0' : 'a' - 10;
        hex[0] = hex1;
        hex[1] = hex2;
    }

    /// @brief Converts binary data to hex and appends to a string object.
    /// @param pStr Pointer to the input binary data.
    /// @param len Number of bytes to convert.
    /// @param outStr Output string that receives the hex representation.
    template <typename TSTR>
    inline void toHex(const char* pStr, size_t len, TSTR& outStr)
    {
        outStr.reserve(len * 2);
        toHex(pStr, len, outStr.begin());
    }

    ///pHexOut must be at least 2 * sizeof(T) in size
    /// @brief Converts a typed value to its hexadecimal string representation.
    /// @param val The input value to convert.
    /// @param outStr Output buffer; must be at least 2*sizeof(T) characters.
    template <typename T>
    inline void toHex(T& val, char* outStr)
    {
        T currValue = val;
        if (mu::Machine::getEndianInteger() == mu::Machine::Endian::Little)
        {
            mu::Machine::swapEndian(currValue);
        }
        size_t inpLen = sizeof(T);
        toHex((const char*)(&currValue), inpLen, outStr);

        //const char pHex[] = "0123456789ABCDEF";
        //const td::BYTE* pCh = (const td::BYTE*) (&currValue);
        //size_t inpLen = sizeof(T);
        //for (size_t i = 0; i < inpLen; ++i)
        //{
        //	td::BYTE ch = pCh[i];
        //	//extract upper part
        //	td::BYTE upper = (ch >> 4) & 0x0F;
        //	size_t j = 2 * i;
        //	outStr[j] = pHex[upper];
        //	td::BYTE lower = ch & 0x0F;
        //	outStr[j + 1] = pHex[lower];
        //}
    }

    /// @brief Converts a C string to an unsigned 64-bit integer.
    /// @param pStr Pointer to the null-terminated decimal string.
    /// @return Parsed LUINT8 value.
    inline td::LUINT8 toUint8(const char* pStr)
    {
        char *pStop = 0;
#ifdef MU_WINDOWS
        return _strtoui64(pStr, &pStop, 10);
#else
        return strtoull(pStr, &pStop, 10);
#endif
    }

    /// @brief Converts a C string to a signed 64-bit integer.
    /// @param pStr Pointer to the null-terminated decimal string.
    /// @return Parsed LINT8 (signed 64-bit) value.
    inline td::LUINT8 toInt8(const char* pStr)
    {
        char *pStop = 0;
#ifdef MU_WINDOWS
        return _strtoi64(pStr, &pStop, 10);
#else
        return strtoll(pStr, &pStop, 10);
#endif
    }

    /// @brief Zeroes out the memory of a value.
    /// @param val Reference to the value to clear.
    template <typename T>
    inline void toZero(T& val)
    {
        memset((void*)&val, 0, sizeof(T));
    }

    /// @brief Converts a localized number string to ANSI C locale format (dot as decimal separator).
    /// @param pStrIn Input string in locale-specific format.
    /// @param nLenIn Length of the input string.
    /// @param buff Output buffer; must be at least 25 bytes.
    /// @param buffLen Size of the output buffer.
    /// @param decPoint Decimal point character used in the input.
    /// @param thSep Thousands separator character used in the input.
    /// @return Number of characters written to buff.
    inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char* buff, size_t buffLen, char decPoint, char thSep)
    {
        assert(buffLen >= 25);
        memset(buff, 0, buffLen);
        size_t  iPos = 0;

        for (size_t i=0; i<nLenIn && iPos < 24; ++i)
        {
            char ch = pStrIn[i];
            if (ch == decPoint)
            {
                buff[iPos++] = '.';
            }
            else if (ch != thSep)
            {
                buff[iPos++] = ch;
            }
        }
        return iPos;
    }

    /// @brief Template overload of toAnsiCNumber using a fixed-size array buffer.
    /// @param pStrIn Input string in locale-specific format.
    /// @param nLenIn Length of the input string.
    /// @param buff Fixed-size output character array (at least 25 bytes required).
    /// @param decPoint Decimal point character used in the input.
    /// @param thSep Thousands separator character used in the input.
    /// @return Number of characters written to buff.
    template <size_t NBUFLEN>
    inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char (&buff) [NBUFLEN], char decPoint, char thSep)
    {
        static_assert(NBUFLEN >= 25);
        return toAnsiCNumber(pStrIn, nLenIn, buff, NBUFLEN, decPoint, thSep);
    }

    /// @brief Extracts numeric characters from a string, handling sign, decimal point and optional scientific notation.
    /// @param pData Input data string.
    /// @param nDataLen Length of the input data.
    /// @param dt DataType used to determine float/integer handling.
    /// @param buff Output buffer for the extracted number (at least 25 bytes).
    /// @param nBuffLen Size of the output buffer.
    /// @param decPoint Decimal point character.
    /// @param thSep Thousands separator character.
    /// @param sci If true, allow scientific notation characters (e/E/+/-).
    /// @return True if any non-numeric characters were encountered (input changed), false otherwise.
    inline bool extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char* buff, size_t nBuffLen, char decPoint, char thSep, bool sci= false)
    {
        assert(nBuffLen >= 25);
        memset(buff, 0, nBuffLen);
        buff[0]=' ';
        //auto nLen = length();
        //const char* pData = c_str();
        bool neg = false;
        bool decPointDet = false;
        bool changed = false;
        size_t  iPos = 1;
        for (size_t i=0; i<nDataLen && iPos < 24; ++i)
        {
            char ch = pData[i];
            if (!sci && (ch == '-'))
            {
                neg = !neg;
                if (neg)
                    buff[0] = '-';
                else
                    buff[0] = ' ';
                if (i > 0)
                    changed = true;
                continue;
            }
            if (ch == thSep)
            {
                buff[iPos++] = ch;
                continue;
            }
            if (ch == decPoint)
            {
                if (decPointDet)
                    return changed;
                if (td::isFloat(dt))
                {
                    buff[iPos++] = ch;
                    decPointDet = true;
                    continue;
                }
                return changed;
            }
            if ( (ch >= '0') && (ch <= '9'))
            {
                buff[iPos++] = ch;
                continue;
            }
            if (sci)
            {
                if ( (ch == 'e') || (ch == 'E') )
                    buff[iPos++] = ch;
                else if ((ch == '+') || (ch == '-'))
                    buff[iPos++] = ch;
                else
                    changed = true;
            }
            else
                changed = true;
        }
        return changed;
    }

    /// @brief Template overload of extractNumbers using a fixed-size array buffer.
    /// @param pData Input data string.
    /// @param nDataLen Length of the input data.
    /// @param dt DataType for float/integer handling.
    /// @param buff Fixed-size output character array (at least 25 bytes required).
    /// @param decPoint Decimal point character.
    /// @param thSep Thousands separator character.
    /// @param sci If true, allow scientific notation characters.
    /// @return Tuple of (number of characters written, whether the input was changed).
    template <size_t NCHBUFF>
    std::tuple<size_t, bool> extractNumbers(const char* pData, size_t nDataLen, td::DataType dt, char (&buff)[NCHBUFF], char decPoint, char thSep, bool sci= false)
    {
        static_assert(NCHBUFF >= 25);
        memset(buff, 0, NCHBUFF);
        buff[0]=' ';
        //auto nLen = length();
        //const char* pData = c_str();
        bool neg = false;
        bool decPointDet = false;
        bool changed = false;
        size_t  iPos = 1;
        for (size_t i=0; i<nDataLen && iPos < 24; ++i)
        {
            char ch = pData[i];
            if (!sci && (ch == '-'))
            {
                neg = !neg;
                if (neg)
                    buff[0] = '-';
                else
                    buff[0] = ' ';
                if (i > 0)
                    changed = true;
                continue;
            }
            if (ch == thSep)
            {
                buff[iPos++] = ch;
                continue;
            }
            if (ch == decPoint)
            {
                if (decPointDet)
                    return {iPos, changed};
                if (td::isFloat(dt))
                {
                    buff[iPos++] = ch;
                    decPointDet = true;
                    continue;
                }
                return {iPos, changed};
            }
            if ( (ch >= '0') && (ch <= '9'))
            {
                buff[iPos++] = ch;
                continue;
            }
            if (sci)
            {
                if ( (ch == 'e') || (ch == 'E') )
                    buff[iPos++] = ch;
                else if ((ch == '+') || (ch == '-'))
                    buff[iPos++] = ch;
                else
                    changed = true;
            }
            else
                changed = true;
        }
        return {iPos, changed};
    }

} //namespace td
