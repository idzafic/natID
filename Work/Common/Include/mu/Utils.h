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
//#include "mu/GraphPrimitives.h"
#include <td/Point.h>
//#include "td/Rect.h"
#include <td/Decimal.h>
#include <td/Color.h>
#include <td/ColorID.h>
#include <td/Variant.h>
#include <td/BoolCh.h>
#include "./Hash.h"

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
class Utils
{
protected:
    char _buf[MU_BUFFLEN + 1];
public:
    Utils()
    {
        memset(_buf, 0, MU_BUFFLEN + 1);
    }

    //do not change this EVER!!!
    inline static td::UINT4 calcHashNo(const char* pStr)
    {
        return mu::calcHashNo(pStr);
    }

    inline static td::UINT4 calcHashNo(td::UINT4 prevHashNo, td::UINT4 hashUpdate)
    {
        return mu::calcHashNo(prevHashNo, hashUpdate);
    }

    inline static td::UINT4 calcHashNo(const char* pStr, int nChar, td::UINT4 prevHashNo = 0)
    {
        return mu::calcHashNo(pStr, nChar, prevHashNo);
    }

    inline static td::UINT4 calcHashNo(const char* pStr, char untilCh)
    {
        return mu::calcHashNo(pStr, untilCh);
    }

    const char* c_str(std::complex<float> val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"

        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "(%g,%g)", val.real(), val.imag()) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }
    const char* c_str(std::complex<double> val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "(%g,%g)", val.real(), val.imag()) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    const char* c_str(float val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"

        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%g", val) <0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    const char* c_str(double val)
    {
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%g", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return _buf;
    }

    const char* c_str(td::INT4 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%d", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::BYTE val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%d", (int) val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::INT2 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%hd", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::UINT4 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%u", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::WORD val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%hu", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::LINT8 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%lld", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }

    const char* c_str(td::LUINT8 val)
    {
        if (SNPRINTF(_buf, MU_BUFFLEN, _TRUNCATE, "%llu", val) < 0)
            _buf[MU_BUFFLEN] = 0;
        return _buf;
    }
#ifndef MU_WINDOWS
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

    template <typename TDEC, int NLEN>
    const char* c_str(const td::Decimal<TDEC, NLEN>& val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    template <typename T>
    const char* c_str(const td::Point<T>& pt)
    {
        pt.c_str(_buf, MU_BUFFLEN+1);
        return _buf;
    }

    static float toFloat(const char* val)
    {
        if (val == 0)
            return 0;

        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        float toRet = (float)atof(val);
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return toRet;
    }

    static double toDouble(const char* val)
    {
        if (val == 0)
            return 0;
        char* old_locale = setlocale(LC_NUMERIC, NULL);  // Save current locale
        setlocale(LC_NUMERIC, "C");  // Set locale to "C"
        double toRet = atof(val);
        setlocale(LC_NUMERIC, old_locale);  // Restore original locale
        return toRet;
    }

    static td::INT4 toINT4(const char* val)
    {
        if (val == 0)
            return 0;
        return atoi(val);
    }

    static td::LINT8 toLINT8(const char* val)
    {
        if (val == 0)
            return 0;
        return std::atoll(val);
    }

    static td::LUINT8 toLUINT8(const char* val)
    {
        if (val == 0)
            return 0;
        return std::atoll(val);
    }

    static td::INT2 toINT2(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::INT2) atoi(val);
    }

    static td::WORD toWORD(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::WORD) atoi(val);
    }

    static td::BYTE toBYTE(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::BYTE) atoi(val);
    }

    static td::INT1 toINT1(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::INT1) atoi(val);
    }

    static td::UINT4 toUINT4(const char* val)
    {
        if (val == 0)
            return 0;
        return (td::UINT4) atoi(val);
    }


    const char* c_str(td::Date val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }
    
    const char* c_str(td::Color val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    const char* c_str(td::Time val)
    {
        val.formc_str(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    const char* c_str(const td::DateTime& val, bool showMS = true)
    {
        if (showMS)
            val.formc_str(_buf, MU_BUFFLEN + 1);
        else
            val.formc_strWoMS(_buf, MU_BUFFLEN + 1);
        return _buf;
    }

    const char* c_str(const td::BoolCh& val)
    {
        return c_str(val());
    }

    static const char* c_str(td::DataType val)
    {
        return td::c_str(val);
    }

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

    const char* toRoman(td::WORD value, bool lower = false)
    {
        _buf[0] = 0;

        if (value >= 4000)
            return nullptr;

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

    static td::UINT4 toColor(const char* str)
    {
        assert(str != 0);
        assert(*str == '#');
        return hexToDec(++str);
    }

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

    static td::Decimal2 toDecimal2(const char* str)
    {
        td::Decimal2 toRet;
        toRet.fromString(str);
        return toRet;
    }

    static td::Decimal3 toDecimal3(const char* str)
    {
        td::Decimal3 toRet;
        toRet.fromString(str);
        return toRet;
    }

    static td::Decimal4 toDecimal4(const char* str)
    {
        td::Decimal4 toRet;
        toRet.fromString(str);
        return toRet;
    }

    static td::Date toDate(const char* str)
    {
        td::Date d;
        d.fromString(str);
        return d;
    }

    static td::Time toTime(const char* str)
    {
        td::Time t;
        t.fromString(str);
        return t;
    }

    static td::DateTime toDateTime(const char* str)
    {
        td::DateTime dt;
        dt.fromString(str);
        return dt;
    }

    static const char* c_str(bool val)
    {
        return td::c_str(val);
    }

    static const char* c_str(const td::String& val)
    {
        return val.c_str();
    }
    
    static const char* c_str(const td::StringExt& val)
    {
        const char* pStr = val.c_str();
        return pStr;
    }

    static const char* c_str(const char* val)
    {
        return val;
    }
    
    static const char* c_str(td::ColorID val)
    {
        return td::toString(val);
    }
    
    static const char* c_str(td::LinePattern val)
    {
        return td::toString(val);
    }
    
    static const char* c_str(td::DotPattern val)
    {
        return td::toString(val);
    }

    const char* c_str(const td::Variant& val)
    {
        if (val.getType() == td::string8)
            return val.strVal().c_str();
        return val.c_str(*this);
    }

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

    inline const char* c_str() const
    {
        return _buf;
    }
};

};
