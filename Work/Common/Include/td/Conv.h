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
#include <mu/Machine.h>
#include <tuple>

namespace td
{

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


template <typename TFLOAT>
inline TFLOAT toFloat(const char* str)
{
    return (TFLOAT) atof(str);
}

template <typename TINT>
inline TINT toInt(const char* str)
{
    return (TINT) atol(str);
}

template <typename TFLOAT>
inline void toFloat(const char* str, TFLOAT& val)
{
     val = (TFLOAT) atof(str);
}

template <typename TINT>
inline void toInt(const char* str, TINT& val)
{
    val = (TINT) atol(str);
}

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
	
	


    inline const char* boolToStr(bool bVal)
    {
        if (bVal)
            return "true";

        return "false";
    }


    template <typename TCHAR>
    inline TCHAR toUpper(TCHAR ch)
    {
        if ((ch >= 97) && (ch <= 122))
        {
            ch -= 32;
        }
        return ch;
    }

    template <typename TCHAR>
    inline TCHAR toLower(TCHAR ch)
    {
        if ((ch >= 65) && (ch <= 90))
        {
            ch += 32;
        }
        return ch;

    }

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

    inline void charToHex(unsigned char c, unsigned char hex[2])
    {
        unsigned char hex1 = c / 16;
        unsigned char hex2 = c % 16;
        hex1 += hex1 <= 9 ? '0' : 'a' - 10;
        hex2 += hex2 <= 9 ? '0' : 'a' - 10;
        hex[0] = hex1;
        hex[1] = hex2;
    }

    template <typename TSTR>
    inline void toHex(const char* pStr, size_t len, TSTR& outStr)
    {
        outStr.reserve(len * 2);
        toHex(pStr, len, outStr.begin());
    }

    ///pHexOut must be at least 2 * sizeof(T) in size
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

    inline td::LUINT8 toUint8(const char* pStr)
    {
        char *pStop = 0;
#ifdef MU_WINDOWS
        return _strtoui64(pStr, &pStop, 10);
#else
        return strtoull(pStr, &pStop, 10);
#endif
    }

    inline td::LUINT8 toInt8(const char* pStr)
    {
        char *pStop = 0;
#ifdef MU_WINDOWS
        return _strtoi64(pStr, &pStop, 10);
#else
        return strtoll(pStr, &pStop, 10);
#endif
    }

    template <typename T>
    inline void toZero(T& val)
    {
        memset((void*)&val, 0, sizeof(T));
    }

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

    template <size_t NBUFLEN>
    inline size_t toAnsiCNumber(const char* pStrIn, size_t nLenIn, char (&buff) [NBUFLEN], char decPoint, char thSep)
    {
        static_assert(NBUFLEN >= 25);
        return toAnsiCNumber(pStrIn, nLenIn, buff, NBUFLEN, decPoint, thSep);
    }

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

