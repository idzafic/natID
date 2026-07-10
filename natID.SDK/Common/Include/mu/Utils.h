// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Utils.h
    @brief Utility class and free functions for type conversions, string formatting, and hash computation. */
#pragma once
#include <td/Types.h>
#include <td/String.h>
#include <td/DateTime.h>
//#include "mu/GraphPrimitives.h"
#include <td/Point.h>
//#include "td/Rect.h"
#include <td/Decimal.h>
#include <td/Color.h>
#include <td/ColorID.h>
#include <td/Variant.h>
#include <td/BoolCh.h>
#include "./Hash.h"
#include <mu/mu.h>

//
//namespace td
//{
//	inline td::String Date::toString() const
//	{
//		char tmp[16];
//		int nLen = SNPRINTF(tmp, 16, _TRUNCATE, "%04d-%02d-%02d", getYear(), getMonth(), getDay());
//		if (nLen < 0)
//		{
//			tmp[15] = 0;
//		}
//		td::String str(tmp, nLen);
//		return str;
//	}
//}
namespace mu
{

/// @brief Reverses the first size elements of the array arr in place.
/// @tparam T Element type of the array.
/// @param arr  Pointer to the first element of the array to reverse.
/// @param size Number of elements to reverse.
template <typename T>
inline void reverse(T* arr, size_t size)
{
    if (size <= 1)
        return;

    size_t start = 0;
    size_t end = size - 1;

    while (start < end)
    {
        // Swap elements at start and end indices
        T temp = arr[start];
        arr[start++] = arr[end];
        arr[end--] = temp;
    }
}

/// @brief Searches for the first occurrence of substring toFind within buf.
/// @param buf     Buffer to search in.
/// @param toFind  Null-terminated substring to locate.
/// @param bufLen  Number of characters in buf to search.
/// @return Zero-based byte offset of the first match, or -1 if not found.
inline int findString(const char* buf, const char* toFind, int bufLen)
{
    int nToFind = (int) strlen(toFind);
    //char chToFind = toFind[0];

    for (int i=0; i<bufLen; ++i)
    {
        char ch = buf[i];
        if (ch == toFind[0])
        {
            bool found = true;
            for (int k=1; k<nToFind; ++k)
            {
                char ch2 = buf[i+k];
                if (ch2 != toFind[k])
                {
                    found = false;
                    break;
                }
            }

            if (found)
                return i;
        }
    }
    return -1;
}

#define MU_BUFFLEN 63
//template <int MU_BUFFLEN = 63>
/// @brief Collection of static and instance helper methods for formatting values as C-strings and for type conversion.
class Utils
{
protected:
    char _buf[MU_BUFFLEN + 1]; ///< Internal character buffer used for formatting output.
public:
    /// @brief Default constructor; zeroes the internal formatting buffer.
    Utils()
    {
        memset(_buf, 0, MU_BUFFLEN + 1);
    }

    /// @brief Computes a hash number for the given null-terminated string.
    /// @param pStr Null-terminated string to hash.
    /// @return 32-bit hash value.
    //do not change this EVER!!!
    inline static td::UINT4 calcHashNo(const char* pStr)
    {
        return mu::calcHashNo(pStr);
    }

    /// @brief Combines two hash values into a new hash.
    /// @param prevHashNo Accumulated hash value.
    /// @param hashUpdate New value to fold in.
    /// @return Updated 32-bit hash value.
    inline static td::UINT4 calcHashNo(td::UINT4 prevHashNo, td::UINT4 hashUpdate)
    {
        return mu::calcHashNo(prevHashNo, hashUpdate);
    }

    /// @brief Computes a hash number for the first nChar characters of pStr, optionally seeded.
    /// @param pStr       String data to hash.
    /// @param nChar      Number of characters to include.
    /// @param prevHashNo Seed value for the hash (defaults to 0).
    /// @return 32-bit hash value.
    inline static td::UINT4 calcHashNo(const char* pStr, int nChar, td::UINT4 prevHashNo = 0)
    {
        return mu::calcHashNo(pStr, nChar, prevHashNo);
    }

    /// @brief Computes a hash number for the characters of pStr up to (but not including) untilCh.
    /// @param pStr    String data to hash.
    /// @param untilCh Stop character (not included in the hash).
    /// @return 32-bit hash value.
    inline static td::UINT4 calcHashNo(const char* pStr, char untilCh)
    {
        return mu::calcHashNo(pStr, untilCh);
    }

    /// @brief Formats a complex<float> value as "(real,imag)" into the internal buffer.
    /// @param val Complex float value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(std::complex<float> val)
    {
        char* oldLocale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"

        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "(%g,%g)", val.real(), val.imag()) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, oldLocale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a complex<double> value as "(real,imag)" into the internal buffer.
    /// @param val Complex double value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(std::complex<double> val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "(%g,%g)", val.real(), val.imag()) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a float value using %g notation into the internal buffer.
    /// @param val Float value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(float val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"

        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%g", val) <0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a double value using %g notation into the internal buffer.
    /// @param val Double value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(double val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%g", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a float value with a fixed number of decimal places into the internal buffer.
    /// @param val  Float value to format.
    /// @param nDec Number of digits after the decimal point.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(float val, int nDec)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"

        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%.*f", nDec, val) <0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a double value with a fixed number of decimal places into the internal buffer.
    /// @param val  Double value to format.
    /// @param nDec Number of digits after the decimal point.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(double val, int nDec)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%.*lf", nDec, val) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    /// @brief Formats a td::INT4 value as a decimal string into the internal buffer.
    /// @param val Integer value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::INT4 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%d", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::BYTE value as a decimal string into the internal buffer.
    /// @param val Byte value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::BYTE val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%d", (int) val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::INT2 value as a signed short decimal string into the internal buffer.
    /// @param val Short integer value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::INT2 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%hd", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::UINT4 value as an unsigned decimal string into the internal buffer.
    /// @param val Unsigned 32-bit integer value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::UINT4 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%u", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::WORD value as an unsigned short decimal string into the internal buffer.
    /// @param val Unsigned short value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::WORD val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%hu", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::LINT8 value as a signed 64-bit decimal string into the internal buffer.
    /// @param val Signed 64-bit integer value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::LINT8 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%lld", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    /// @brief Formats a td::LUINT8 value as an unsigned 64-bit decimal string into the internal buffer.
    /// @param val Unsigned 64-bit integer value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    const char* c_str(td::LUINT8 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%llu", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }
#ifndef MU_WINDOWS
        /// @brief Formats a size_t value as a decimal string into the internal buffer (non-Windows only).
        /// @param val size_t value to format.
        /// @return Pointer to the internal buffer containing the formatted string.
        const char* c_str(size_t val)
        {
#ifdef MU_X64
            if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%llu", val) < 0)
                _buf[MU_BUFFLEN] = 0;
#else
            if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%lu", val) < 0)
                _buf[MU_BUFFLEN] = 0;
#endif
            return _buf;
        }
#endif

    /// @brief Formats a Decimal value as a C-string into the internal buffer.
    /// @tparam TDEC  Underlying integer type of the Decimal.
    /// @tparam NLEN  Number of decimal places.
    /// @param val Decimal value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    template <typename TDEC, int NLEN>
    const char* c_str(const td::Decimal<TDEC, NLEN>& val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    /// @brief Formats a 2D point as a C-string into the internal buffer.
    /// @tparam T Coordinate type of the point.
    /// @param pt Point value to format.
    /// @return Pointer to the internal buffer containing the formatted string.
    template <typename T>
    const char* c_str(const td::Point<T>& pt)
    {
        pt.c_str(_buf, MU_BUFFLEN+1);
        return _buf;
    }

    /// @brief Parses a C-string as a float value using locale-independent conversion.
    /// @param strToFloat Null-terminated string to parse.
    /// @return Parsed float value.
    static float toFloat(const char* strToFloat)
    {
        return float(mu::toDouble(strToFloat));
    }

    /// @brief Parses a C-string as a double value using locale-independent conversion.
    /// @param strToDbl Null-terminated string to parse.
    /// @return Parsed double value.
    static double toDouble(const char* strToDbl)
    {
        return mu::toDouble(strToDbl);
    }

    /// @brief Parses a C-string as a td::INT4 (32-bit signed integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed integer value, or 0 if val is nullptr.
    static td::INT4 toINT4(const char* val)
    {
        if (val == 0)
            return 0;
        return atoi(val);
    }

    /// @brief Parses a C-string as a td::LINT8 (64-bit signed integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed 64-bit integer value, or 0 if val is nullptr.
    static td::LINT8 toLINT8(const char* val)
    {
        if (val == 0)
            return 0;
        return std::atoll(val);
    }

    /// @brief Parses a C-string as a td::LUINT8 (64-bit unsigned integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed 64-bit unsigned integer value, or 0 if val is nullptr.
    static td::LUINT8 toLUINT8(const char* val)
    {
        if (val == 0)
            return 0;
        return std::atoll(val);
    }

    /// @brief Parses a C-string as a td::INT2 (16-bit signed integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed 16-bit integer value, or 0 if val is nullptr.
    static td::INT2 toINT2(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::INT2) atoi(val);
    }

    /// @brief Parses a C-string as a td::WORD (16-bit unsigned integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed unsigned 16-bit value, or 0 if val is nullptr.
    static td::WORD toWORD(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::WORD) atoi(val);
    }

    /// @brief Parses a C-string as a td::BYTE (8-bit unsigned integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed byte value, or 0 if val is nullptr.
    static td::BYTE toBYTE(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::BYTE) atoi(val);
    }

    /// @brief Parses a C-string as a td::INT1 (8-bit signed integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed signed byte value, or 0 if val is nullptr.
    static td::INT1 toINT1(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::INT1) atoi(val);
    }

    /// @brief Parses a C-string as a td::UINT4 (32-bit unsigned integer).
    /// @param val Null-terminated string to parse, or nullptr.
    /// @return Parsed unsigned 32-bit value, or 0 if val is nullptr.
    static td::UINT4 toUINT4(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::UINT4) atoi(val);
    }


    /// @brief Formats a td::Date value as a C-string into the internal buffer.
    /// @param val Date value to format.
    /// @return Pointer to the internal buffer containing the formatted date string.
    const char* c_str(td::Date val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    /// @brief Formats a td::Color value as a C-string into the internal buffer.
    /// @param val Color value to format.
    /// @return Pointer to the internal buffer containing the formatted color string.
    const char* c_str(td::Color val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    /// @brief Formats a td::Time value as a C-string into the internal buffer.
    /// @param val Time value to format.
    /// @return Pointer to the internal buffer containing the formatted time string.
    const char* c_str(td::Time val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    /// @brief Formats a td::DateTime value as a C-string into the internal buffer.
    /// @param val    DateTime value to format.
    /// @param showMS If true (default), milliseconds are included in the output.
    /// @return Pointer to the internal buffer containing the formatted datetime string.
    const char* c_str(const td::DateTime& val, bool showMS = true)
    {
        if (showMS)
            val.formc_str(_buf, MU_BUFFLEN + 1);
        else
            val.formc_strWoMS(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    /// @brief Formats a td::BoolCh value as a C-string into the internal buffer.
    /// @param val BoolCh value to format.
    /// @return Pointer to the internal buffer containing the formatted boolean string.
    const char* c_str(const td::BoolCh& val)
    {
        return c_str(val());
    }

    /// @brief Returns the string representation of a td::DataType enumeration value.
    /// @param val DataType enumeration value to convert.
    /// @return Pointer to a static string describing the data type.
    static const char* c_str(td::DataType val)
    {
        return td::c_str(val);
    }

    /// @brief Converts an unsigned 32-bit integer to a lowercase hexadecimal string stored in the internal buffer.
    /// @param val      Value to convert.
    /// @param startPos Starting position within the internal buffer (defaults to 0).
    /// @return Pointer to the internal buffer at startPos, or nullptr if startPos exceeds 50.
    const char* decToHex(td::UINT4 val, int startPos=0)
    {
        if (startPos > 50)
            return 0;

        int pos = startPos;
        //for each 4 bits
        while (pos < 8+startPos)
        {
            td::UINT4 rest = val & 0xF0000000;
            rest >>= 28;
            char chRest = (char) rest;
            if ( rest <= 9 )
            {
                _buf[pos] = '0' + chRest;
            }
            else
            {
                _buf[pos] = 'a' + chRest - 10;
            }
            val <<= 4;
            ++pos;
        }
        _buf[pos] = '\0';
        return _buf;
    }

    /// @brief Converts a numeric value to its Roman numeral representation.
    /// @param value Value to convert; must be less than 4000.
    /// @param lower If true, the result is returned in lowercase letters.
    /// @return Pointer to the internal buffer containing the Roman numeral string,
    ///         or nullptr if value is 4000 or greater.
    const char* toRoman(td::WORD value, bool lower = false)
    {
        _buf[0] = 0;

        if (value >= 4000)
            return nullptr;

        /// @brief Maps a decimal value to its Roman numeral string representation.
        struct tRomanData
        {
            td::WORD value;
            char const* numeral;
            //char const* numLen;
        };

        const struct tRomanData RomanData[] =
        {
            {1000, "M" }, {900, "CM" },
            { 500, "D" }, {400, "CD" },
            { 100, "C"},  { 90, "XC" },
            {  50, "L" }, { 40, "XL" },
            {  10, "X" }, {  9, "IX" },
            {   5, "V" }, {  4, "IV" },
            {   1, "I" }, {  0, nullptr } // end marker
        };

        int pos = 0;

        for (const tRomanData* pCurrent = RomanData; pCurrent->value > 0; ++pCurrent)
        {
            while (value >= pCurrent->value)
            {
                STRCPY(_buf + pos, MU_BUFFLEN - pos, pCurrent->numeral);
                pos += (int) strlen(pCurrent->numeral);
                value -= pCurrent->value;
            }
        }

        _buf[MU_BUFFLEN] = 0;

        if (lower)
            td::toLower(_buf);

        return _buf;
    }

    /// @brief Parses a CSS-style color string (starting with '#') into a td::UINT4 color value.
    /// @param str Null-terminated string starting with '#' followed by 8 hex digits.
    /// @return Parsed 32-bit color value.
    static td::UINT4 toColor(const char* str)
    {
        assert(str != 0);
        assert(*str == '#');
        return hexToDec(++str);
    }

    /// @brief Converts a hexadecimal string to an unsigned 32-bit integer.
    /// @param str Null-terminated string containing only lowercase hex digits (0-9, a-f).
    /// @return Parsed 32-bit value, or 0 if str is nullptr.
    static td::UINT4 hexToDec(const char* str)
    {
        if (str == 0)
            return 0;

        td::UINT4 res = 0;
        while (*str != '\0')
        {
            char ch = *str;
            td::UINT4 hexDigit = 0;
            if ( (ch >= '0') && (ch <= '9') )
            {
                hexDigit = ch - '0';
            }
            else if ( (ch >= 'a') && (ch <= 'f') )
            {
                hexDigit = ch - 'a' + 10;
            }
            else
                return res;
            res = res << 4;
            res |= hexDigit;
            ++str;
        }
        return res;
    }

    /// @brief Parses a string into a td::Decimal2 (2 decimal places) value.
    /// @param str Null-terminated decimal string to parse.
    /// @return Parsed Decimal2 value.
    static td::Decimal2 toDecimal2(const char* str)
    {
        td::Decimal2 toRet;
        toRet.fromString(str);
        return toRet;
    }

    /// @brief Parses a string into a td::Decimal3 (3 decimal places) value.
    /// @param str Null-terminated decimal string to parse.
    /// @return Parsed Decimal3 value.
    static td::Decimal3 toDecimal3(const char* str)
    {
        td::Decimal3 toRet;
        toRet.fromString(str);
        return toRet;
    }

    /// @brief Parses a string into a td::Decimal4 (4 decimal places) value.
    /// @param str Null-terminated decimal string to parse.
    /// @return Parsed Decimal4 value.
    static td::Decimal4 toDecimal4(const char* str)
    {
        td::Decimal4 toRet;
        toRet.fromString(str);
        return toRet;
    }

    /// @brief Parses a string into a td::Date value.
    /// @param str Null-terminated date string to parse.
    /// @return Parsed Date value.
    static td::Date toDate(const char* str)
    {
        td::Date d;
        d.fromString(str);
        return d;
    }

    /// @brief Parses a string into a td::Time value.
    /// @param str Null-terminated time string to parse.
    /// @return Parsed Time value.
    static td::Time toTime(const char* str)
    {
        td::Time t;
        t.fromString(str);
        return t;
    }

    /// @brief Parses a string into a td::DateTime value.
    /// @param str Null-terminated datetime string to parse.
    /// @return Parsed DateTime value.
    static td::DateTime toDateTime(const char* str)
    {
        td::DateTime dt;
        dt.fromString(str);
        return dt;
    }

    /// @brief Returns "true" or "false" as a C-string for a boolean value.
    /// @param val Boolean value to convert.
    /// @return Pointer to a static string "true" or "false".
    static const char* c_str(bool val)
    {
        return td::c_str(val);
    }

    /// @brief Returns the underlying C-string of a td::String.
    /// @param val String whose C-string pointer is returned.
    /// @return Pointer to the null-terminated character data of val.
    static const char* c_str(const td::String& val)
    {
        return val.c_str();
    }

    /// @brief Returns the underlying C-string of a td::StringExt.
    /// @param val StringExt whose C-string pointer is returned.
    /// @return Pointer to the null-terminated character data of val.
    static const char* c_str(const td::StringExt& val)
    {
        const char* pStr = val.c_str();
        return pStr;
    }

    /// @brief Returns the given C-string pointer unchanged.
    /// @param val Null-terminated C-string.
    /// @return The same pointer val.
    static const char* c_str(const char* val)
    {
        return val;
    }

    /// @brief Returns the string representation of a td::ColorID enumeration value.
    /// @param val ColorID enumeration value to convert.
    /// @return Pointer to a static string naming the color.
    static const char* c_str(td::ColorID val)
    {
        return td::toString(val);
    }

    /// @brief Returns the string representation of a td::LinePattern enumeration value.
    /// @param val LinePattern enumeration value to convert.
    /// @return Pointer to a static string describing the line pattern.
    static const char* c_str(td::LinePattern val)
    {
        return td::toString(val);
    }

    /// @brief Returns the string representation of a td::DotPattern enumeration value.
    /// @param val DotPattern enumeration value to convert.
    /// @return Pointer to a static string describing the dot pattern.
    static const char* c_str(td::DotPattern val)
    {
        return td::toString(val);
    }

    /// @brief Returns the string representation of a td::Anchor enumeration value.
    /// @param val Anchor enumeration value to convert.
    /// @return Pointer to a static string describing the anchor.
    static const char* c_str(td::Anchor val)
    {
        return td::toString(val);
    }

    /// @brief Returns the C-string representation of a td::Variant value.
    /// @param val Variant value to format; string8 variants return their stored pointer directly.
    /// @return Pointer to a C-string representation of the variant's value.
    const char* c_str(const td::Variant& val)
    {
        if (val.getType() == td::string8)
            return val.strVal().c_str();
        return val.c_str(*this);
    }

    /// @brief Formats a string into the internal buffer using a printf-style format and variable arguments.
    /// @param format printf-style format string.
    /// @param ...    Arguments matching the format string.
    /// @return Pointer to the internal buffer containing the formatted string.
    inline const char* cFormat(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
#ifdef MU_WINDOWS
        int nLen = vsnprintf_s(_buf, _TRUNCATE, format, argptr);
#else
        int nLen = vsnprintf(_buf, MU_BUFFLEN, format, argptr);
#endif

        va_end(argptr);

        _buf[MU_BUFFLEN] = 0;
        if (nLen >= 0)
            return _buf;
        _buf[0] = 0;
        return _buf;
    }

    /// @brief Returns the current contents of the internal formatting buffer.
    /// @return Pointer to the null-terminated contents of the internal buffer.
    inline const char* c_str() const
    {
        return _buf;
    }
};

};
