// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 18/04/2008.
//  Copyright © 2008-2021 IDz. All rights reserved.
//


#pragma once
#include <compiler/Definitions.h>

//#include <ctype> 
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <complex>
#include <stdarg.h>

//#include <malloc.h>

//#define BOOST_FILESYSTEM_VERSION 3

#ifndef EIGEN_VECTORIZE
#define EIGEN_VECTORIZE
#endif

#ifndef EIGEN_VECTORIZE_SSE3
#define EIGEN_VECTORIZE_SSE3
#endif

#ifndef MU_DEBUG
#if defined(DEBUG) || defined(_DEBUG)
#define MU_DEBUG
#endif
#endif

#ifdef TRACE_APP
//#define TraceApplication true
//#else
//#define TraceApplication false
#endif


//#ifdef _WINDOWS
//#ifndef _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
//#endif
//#endif

#ifdef MU_WINDOWS

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

//typedef int ssize_t;
//#define _snprintf_s(a,b,c,...) snprintf(a,b,__VA_ARGS__)
#define SNPRINTF _snprintf_s
#define SPRINTF sprintf_s
#define STRCPY strcpy_s
#define WCSCPY wcscpy_s
#define WMEMCPYS wmemcpy_s
#define SSCANF sscanf_s
#define FMT_LUINT8 "%I64u"
#define FMT_LINT8 "%I64d"
#define FMT_LINT8_EXT "I64d"
#define CSTR_TO_LINT8 _strtoi64
#define CSTR_TO_LUINT8 _strtoui64
#define DUPENV _dupenv_s
#define STRERROR strerror_s
#define EXECV _wexecv
#else
 //MAC i Linux
// #ifdef MU_LINUX
// #ifndef MU_X64
// #include <stdint.h>
// #if UINTPTR_MAX == 0xffffffffffffffff
// #define MU_X64
// #endif
// #endif //MU_X64
// #else //MU_LINUX
// #define MU_CLANG
// #endif

#if !defined(_TRUNCATE)
#define _TRUNCATE ((size_t)-1)
#endif

inline int _snprintf_s(
	char *buffer,
	size_t sizeOfBuffer,
	size_t count,
	const char *format,
	...
	)
{
	int retval;
	va_list ap;

	if ((count != _TRUNCATE) && (count < sizeOfBuffer)) 
	{
		sizeOfBuffer = count;
	}

	va_start(ap, format);
	retval = vsnprintf(buffer, sizeOfBuffer, format, ap);
	va_end(ap);

	if ((0 <= retval) && (sizeOfBuffer <= (size_t)retval)) 
	{
		retval = -1;
	}

	return retval;
}

inline int strcpy_s( char *buffer, [[maybe_unused]] size_t sizeOfBuffer, const char *sIn)
{
    int nLen = (int) strlen(sIn);
#ifdef MU_DEBUG
    assert(nLen < sizeOfBuffer);
#endif
    if (nLen >= sizeOfBuffer)
    {
        nLen = (int) sizeOfBuffer-1;
        strncpy(buffer, sIn, nLen);
        buffer[sizeOfBuffer-1] = 0;
        return nLen;
    }
    strncpy(buffer, sIn, nLen);
    buffer[nLen] = 0;
    return nLen;
}

inline wchar_t* wmemcpy_s( wchar_t* dest, [[maybe_unused]] size_t buffLen, const wchar_t* src, size_t count )
{
#ifdef MU_DEBUG
    assert(count < buffLen);
#endif
    return wmemcpy(dest, src, count);
}
//#define sscanf_s( buffer, fmt, ... ) sscanf( buffer, fmt, __VA_ARGS__ )

#define SNPRINTF _snprintf_s
#define SPRINTF sprintf
#define STRCPY strcpy_s
#define SSCANF sscanf
#define WCSCPY wcscpy
#define WMEMCPYS wmemcpy_s
#define VSNPRINTF vsnprintf
#define FMT_LUINT8 "%llu"
#define FMT_LINT8 "%lld"
#define FMT_LINT8_EXT "lld"
#define CSTR_TO_LINT8 strtoll
#define CSTR_TO_LUINT8 strtoull
#define DUPENV _dupenv
#define STRERROR strerror
#define EXECV execv

#endif

#ifdef MU_64BIT
    #ifndef __MINGW32__
        #ifndef MU_MACOS
            #define USE_SIMD
        #endif
    #endif
#endif


#ifdef USE_SIMD
    #include <emmintrin.h> //SSE2
    #include <pmmintrin.h> //SSE3
    
    #ifdef MU_WINDOWS
    #ifdef __MINGW32__
    #include <x86intrin.h>
    #else
        #include <intrin.h>
    #endif //MINGW
    #pragma intrinsic ( _mm_hsub_pd )
    #pragma intrinsic ( _mm_hadd_pd )
    #else //for other platforms
        #include <pmmintrin.h> //SSE3
    #endif //MU_WINDOWS

#endif //USE_SIMD

#define COLOR_TRANSPARENT 0x00ffffff

namespace td
{
	typedef unsigned int UINT4;
	typedef int INT4;
	typedef unsigned char BYTE;
    typedef unsigned char UINT1;
	typedef char INT1;
	typedef unsigned short WORD;	
	typedef signed short INT2;
	typedef unsigned short UINT2;
	typedef unsigned int TPSKEY;
	typedef long long LINT8;
    typedef long long INT8;
	typedef unsigned long long LUINT8;
    typedef unsigned long long UINT8;
	typedef float Coord;
	typedef std::complex<double> cmplx;
	typedef std::complex<float> cmplxf;

#ifdef MU_64BIT
	typedef  LUINT8 PTRHOLDER;
    #define STRING_BASE_ADDRESS  8
#else
	typedef  UINT4 PTRHOLDER;
    #define STRING_BASE_ADDRESS  4
#endif

    typedef enum _StringEncoding:BYTE {EncodingUTF8=0, EncodingUTF16, EncodingUTF32} StringEncoding;
	const char ENDL = '\n';
	const char endl = '\n';

#ifdef MU_WINDOWS		

#ifdef USE_SIMD	
	#define SIMD_MULD _mm_mul_pd
	#define SIMD_LOADD _mm_load_pd
	#define SIMD_LOADD_REVERT _mm_loadr_pd
	#define SIMD_STORED _mm_storeu_pd
	#define SIMD_STORED_REVERT _mm_storeh_pd
#endif	

	#define TD_LF (10)
    #define TD_CR (13)

	//for fo::getLine
	#define TD_TO_NEWLINE (10)
	#define TD_TO_LINEBEG (13)

	#define TD_EOF (-1)
	typedef wchar_t NativeUnicode;  //Tip u koji kompajler pretvara unesene znake nakon makroa L
	typedef char	UTF8;
	typedef wchar_t	UTF16;
	typedef INT4	UTF32;
	typedef unsigned long ULONGPTR ;

	#define STRFMT  "%s"			//format za stampanje UTF8 stringova
	#define _G_SIZE_OF_WCHAR_ 2
        #define WINDOWS_UNICODE 1
#elif MINGW
        #define TD_LF (10)
        #define TD_CR (13)
        #define TD_EOF (-1)
        typedef wchar_t NativeUnicode;  //Tip u koji kompajler pretvara unesene znake nakon makroa L
        typedef char	UTF8;
        typedef wchar_t	UTF16;
        typedef INT4	UTF32;
        #define STRFMT  "%s"			//format za stampanje UTF8 stringova
        #define _G_SIZE_OF_WCHAR_ 2
        #define WINDOWS_UNICODE 1
		typedef UINT4 ULONGPTR;
#else
	#define TD_LF (10)
	#define TD_CR (13)	
	#define TD_EOF (-1)
	typedef wchar_t NativeUnicode;  //Tip u koji kompajler pretvara unesene znake nakon makroa L
	typedef char	UTF8;
	typedef UINT2	UTF16;
	typedef wchar_t	UTF32;
	typedef UINT4 ULONGPTR;
	#define STRFMT  "%s"			//format za stampanje UTF8 stringova
	#define _G_SIZE_OF_WCHAR_ 4
    #define TD_TO_NEWLINE (10)
    #define TD_TO_LINEBEG (13)
#ifndef HINSTANCE
    typedef void* HINSTANCE;
#endif
#endif


template <typename TINT>
inline td::UINT4 GLRGB(TINT r, TINT g, TINT b)
{
    return (td::UINT4) (0xFF000000 |
                ((td::UINT4)(r)) |
                ((td::UINT4)(g)<<8) |
                ((td::UINT4)(b)<<16));
}

template <typename TINT>
inline td::UINT4 GLRGBA(TINT r, TINT g, TINT b, TINT a)
{
    return (td::UINT4) (((td::UINT4)(a) << 24) |
        ((td::UINT4)(r)) |
        ((td::UINT4)(g)<<8) |
        ((td::UINT4)(b)<<16));
}

template <typename TINT>
inline td::UINT4 GDIARGB(TINT r, TINT g, TINT b, TINT a)
{
    return (td::UINT4) (((td::UINT4)(a) << 24) |
        ((td::UINT4)(b)) |
        ((td::UINT4)(g)<<8) |
        ((td::UINT4)(r)<<16));
}

inline td::UINT4 GetRColor(td::UINT4 color)
{
    return color & 0x000000FF;
}

inline td::UINT4 GetGColor(td::UINT4 color)
{
    return ((color & 0x0000FF00) >> 8);
}

inline td::UINT4 GetBColor(td::UINT4 color)
{
    return ((color & 0x00FF0000) >> 16);
}

inline td::UINT4 GetAColor(td::UINT4 color)
{
    return ((color & 0xFF000000) >> 24);
}

inline td::UINT4 GLRGBAToNativeGDI(td::UINT4 color)
{
    //rotate R and B
    td::UINT4 r = GetRColor(color);
    td::UINT4 b = GetBColor(color);
    td::UINT4 a = color & 0xFF000000;
    td::UINT4 g = color & 0x0000FF00;
    return (a | (r << 16) | g | b);
}

inline int compareNoCase(const char* s1, const char* s2)
{
    while (*s1 != '\0' && *s2 != '\0' && tolower(*s1) == tolower(*s2))
    {
        s1++;
        s2++;
    }

    return tolower(*s1) - tolower(*s2);
}

inline int compareSubStrNoCase(const char* str, const char* subStr)
{
    size_t nToCompare = std::strlen(subStr);
    
    if (nToCompare == 0)
        return -1;

    while (nToCompare > 0 && *str != '\0' && *subStr != '\0' && tolower(*str) == tolower(*subStr))
    {
        str++;
        subStr++;
        --nToCompare;
    }

    if (nToCompare == 0)
        return 0;
    
    return tolower(*str) - tolower(*subStr);
}

inline const char* c_str(bool bVal)
{
    return bVal ? "true" : "false";
}

inline bool isTrue(const char* str)
{
    if (strcmp(str, "true") == 0)
        return true;
    return false;
}

//template <typename TELEM>
//inline void cleanVector(TELEM* vect, int n)
//{
//	memset(vect, 0, sizeof(TELEM) * n);
//}


inline td::INT4 getLow(td::LINT8 val)
{
    td::LINT8 tmp = 0x00000000FFFFFFFF & val;
    return (td::INT4) tmp;
}

inline td::INT4 getHi(td::LINT8 val)
{
    td::LINT8 tmp = 0xFFFFFFFF00000000 & val;
    return (td::INT4) tmp;
}

inline size_t strLen(const char* pStr)
{
    return ::strlen(pStr);
}

template <typename TCH>
int strLen(const TCH* pStr)
{
#ifdef _DEBUG
    assert( (typeid(TCH) == typeid(td::UTF16)) || (typeid(TCH) == typeid(td::UTF32)));
#endif
    int i=0;
    while(*pStr != 0)
    {
        ++pStr;
        ++i;
    }
    return i;
}

//size_t strLen(const UTF16* pStr)
//{
//#ifdef MU_WINDOWS
//    return ::wcslen(pStr);
//#else
//    size_t i=0;
//
//#endif
//}

template <typename TCH>
bool isAlpha(TCH val)
{
    return ((val == '_') || (val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z'));
}

template <typename TCH>
bool isNumeric(TCH val)
{
    return (val >= '0' && val <= '9');
}

template <typename TCH>
bool isVowel(TCH val)
{
    return ((val == TCH('a') ) || (val == TCH('e') ) || (val == TCH('i') ) || (val == TCH('o')) || (val == TCH('u')));
}


template <typename TCH>
int findFromLeft(const TCH* pStr, TCH ch, int iPos=0)
{
    pStr += iPos;
    int i=0;
    while(pStr[i] != 0)
    {
        if (pStr[i] == ch)
        {
            return i;
        }
        ++i;
    }
    return -1;
}

template <typename TCH>
int findFromLeftNoZeroEnd(const TCH* pStr, TCH ch, int nBuffLen, int iPos = 0)
{
    pStr += iPos;
    int i = 0;
    while (i < nBuffLen)
    {
        if (pStr[i] == ch)
        {
            return i;
        }
        ++i;
    }
    return -1;
}

template <typename TCH>
int findFromRight(const TCH* pStr, TCH ch, int inStartPos = -1)
{
    size_t nStartPos = 0;
    if (inStartPos == -1)
    {
        nStartPos = strLen(pStr) - 1;
    }
    else
        nStartPos = (size_t)inStartPos;
    int i = 0;
    while(nStartPos >= 0)
    {
        if (pStr[nStartPos] == ch)
            return i;

        ++i;
        --nStartPos;
    }
    return -1;
}

template <class T>
void cleanVectorOfPointers(T& v)
{
    typename T::iterator it(v.begin());
    typename T::const_iterator itEnd(v.end());
    while (it != itEnd)
    {
        if (*it)
            delete *it;
        ++it;
    }
}

inline bool isUnidodeLineBreak(td::UTF8)
{
    return false;
}

inline bool isUnidodeLineBreak(td::UTF16 ch)
{
    td::UINT2 iVal = (td::UINT2) ch;
    if (iVal == 8232) //Unicode Character 'LINE SEPARATOR' (U+2028)
        return true;
    return false;
}

inline bool isUnidodeLineBreak(td::UTF32 ch)
{
    td::UINT4 iVal = (td::UINT4) ch;
    if (iVal == 8232) //Unicode Character 'LINE SEPARATOR' (U+2028)
        return true;
    return false;
}

template <typename TCH>
bool isWhiteSpace(TCH ch)
{
    if (isUnidodeLineBreak(ch)) //special unicode for HTML
        return true;
    
    return ((ch == (TCH)' ') || (ch == (TCH)'\t') || (ch == (TCH)'\n') || (ch == (TCH)'\r'));
}

template <typename TCH>
bool isNewLine(TCH ch)
{
    if (isUnidodeLineBreak(ch)) //special unicode for HTML
        return true;
    
    return (ch == (TCH)'\n');
}

template <typename TCH>
bool isHorizontalSpace(TCH ch)
{
    return ((ch == (TCH)' ') || (ch == (TCH)'\t'));
}

template <typename TCH>
TCH* skipWhiteSpaces(TCH* pStr)
{
    if (!pStr)
        return nullptr;
    TCH* toRet = pStr;
    while (toRet != nullptr)
    {
        if (isWhiteSpace(*toRet))
            ++toRet;
        else
            return toRet;
    }
    return toRet;
}

template <typename TCH>
size_t getNumberOfTrailingWhiteSpaces(const TCH* pStr, int fromPos = -1)
{
    if (!pStr)
        return 0;
    
    int len = (int) strLen(pStr);
    --len;
    if (fromPos > 0)
    {
        if (len > fromPos)
            len = fromPos;
    }
    size_t noOfTrailing = 0;
    const TCH* pCh = pStr+len;
    while (len > 0)
    {
        if (isWhiteSpace(*pCh))
        {
            --pCh;
            ++noOfTrailing;
        }
        else
            return noOfTrailing;
        --len;
    }
    return noOfTrailing;
}

template <typename TCH>
void trimTrailingWhiteSpaces(TCH* pStr)
{
    if (!pStr)
        return;
    int len = (int) strLen(pStr);
    --len;
    
    TCH* pCh = pStr+len;
    while (len > 0)
    {
        if (isWhiteSpace(*pCh))
        {
            *pCh = 0;
            --pCh;
        }
        else
            return;
        --len;
    }
    return;
}

template <typename TCH>
TCH* trimWhiteSpaces(TCH* pStr)
{
    pStr = skipWhiteSpaces(pStr);
    if (pStr)
        trimTrailingWhiteSpaces(pStr);
    return pStr;
}

template <typename TCH>
inline int find(const TCH* pStr, TCH ch, td::UINT4 searchStartPos = 0)
{
    int nLen = (int) strLen(pStr);
    for (int i = searchStartPos; i < nLen; ++i)
    {
        TCH myCh = pStr[i];
        if (ch == myCh)
            return i;
    }
    return -1;
}

template <typename TCH>
inline size_t count(const TCH* pStr, size_t strLen, TCH ch)
{
    if (strLen == 0)
        strLen = strlen(pStr);
    if (strLen == 0)
        return 0;
    size_t n = 0;
    for (size_t i = 0; i < strLen; ++i)
    {
        TCH myCh = pStr[i];
        if (ch == myCh)
            return ++n;
    }
    return n;
}

inline int format(char* buffer, size_t bufferSize, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    // Use vsnprintf to format the string into the buffer
    int nLen = vsnprintf(buffer, bufferSize, format, args);
//    int nLen = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args);
    va_end(args);
    //terminate, just in case
    buffer[bufferSize-1] = 0;
    return nLen;
}

template <size_t bufferSize>
inline int format(char(&buffer)[bufferSize], const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    // Use vsnprintf to format the string into the buffer
    int nLen = vsnprintf(buffer, bufferSize, format, args);
//    int nLen = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args);
    va_end(args);
    //terminate, just in case
    buffer[bufferSize-1] = 0;
    return nLen;
}

typedef union
{
    td::BYTE ui1;
    td::WORD ui2;
    td::INT4 i4;
    td::UINT4 ui4;
} Int4Union;

typedef union
{
    td::LINT8 li8;
    td::LUINT8 lui8;
} Int8Union;

typedef enum _tLineStyle : BYTE {SOLID = 0, DASH, DOTDASH, DOT} LineStyle;

typedef enum _tDataType : BYTE {TD_NONE=0, boolean=1, byte, word, int2, uint4, int4, lint8, luint8, real4, real8, string8, string16, string32, enumerator, date, time, dateTime, decimal0, decimal1, decimal2, decimal3, decimal4, ch, nch, chFix, nchFix, ch7,  dbDate, dbTime, dbDateTime, dbDecimal, binary, guid, color, colorID, hAlignment, vAlignment, textAlignment, linePattern, dotPattern, anchor, smallDecimal0, smallDecimal1, smallDecimal2, smallDecimal3, smallDecimal4, rect, point, circle, variant,
    ptrBoolean, ptrByte, ptrWord, ptrInt2, ptrUint4, ptrInt4, ptrLint8, ptrLuint8, ptrReal4, ptrReal8, ptrString8, ptrString16, ptrString32, ptrEnumerator, ptrDate, ptrTime, ptrDateTime, ptrDecimal0, ptrDecimal1, ptrDecimal2, ptrDecimal3, ptrDecimal4, ptrCh, ptrNch, ptrChFix, ptrSmallChFix, ptrNchFix, ptrDbDate, ptrDbTime, ptrDbDateTime, ptrDbDecimal, ptrBinary, ptrGuid, ptrColor, ptrSmalldecimal0, ptrSmalldecimal1, ptrSmalldecimal2, ptrSmalldecimal3, ptrSmalldecimal4, ptrRect, ptrPoint, ptrVariant, ptrVoid,
    TD_LAST } DataType; //ch-ansi char, nch-unicode char

enum class PresentationType : unsigned char {PT_SameAsDT=0, PT_YesNo, PT_TrueFalse, PT_CheckBox};

enum class FormatFloat : unsigned char {Decimal=0, Scientific};

//ubaceno u qtu definitions
//	typedef enum _tHAlignment { Left=0x0001, Right =0x0002, HCenter=0x0004} tHAlignment;
//
//	typedef enum _tVAlignment { Top=0x0020, Bottom=0x0040, VCenter = 0x0080} tVAlignment;

enum class HAlignment : unsigned char {Left=0, Center, Right};
enum class VAlignment : unsigned char {Top=0, Center, Bottom};
enum class TextAlignment : unsigned char {Left=0, Center, Right, Justified};
enum class TextWrap : unsigned char {No=0, Word, Any};
enum class TextEllipsize : unsigned char {None=0, Start, Middle, End};

enum class ThousandSep : unsigned char {SystemDefault, Show, DoNotShow};

inline bool isInteger(td::DataType type)
{
    return (((type >= td::byte) && (type <= td::luint8)) || (type == td::decimal0) || (type == td::smallDecimal0));
}

inline bool isNumeric(td::DataType type)
{
    return (((type >= td::byte) && (type <= td::real8)) || ((type >= td::decimal0) && (type <= td::decimal4)) || ((type >= td::smallDecimal0) && (type <= td::smallDecimal4)));
}

inline bool isDateTime(td::DataType type)
{
    return ((type >= td::date) && (type <= td::dateTime));
}

inline bool isFloat(td::DataType type)
{
    return ((type >= td::real4) && (type <= td::real8));
}

inline bool isDecimal(td::DataType type)
{
    return ( (type >= td::decimal0) && (type <= td::decimal4)  );
}

inline bool isSmallDecimal(td::DataType type)
{
    return ( (type >= td::smallDecimal0) && (type <= td::smallDecimal4) );
}

inline bool isUnsigned(td::DataType type)
{
    return ((type == td::byte) || (type == td::word) || (type == td::uint4) || (type == td::luint8));
}

inline bool isString(td::DataType type)
{
    if (type >= td::string8 && type <= td::string32)
        return true;
    return false;
}

inline bool isDBString(td::DataType type)
{
    if ((type >= td::ch) && (type <= td::nchFix))
        return true;
    return false;
}

inline const char* const toString(bool bVal)
{
    return bVal ? "true" : "false";
}

template <size_t size, typename TINT>
void iToA(const char(&buffer)[size], TINT decValue)
{
#ifdef MU_WINDOWS
    itoa(decValue, buffer, 10);
#else
    SPRINTF(buffer, "%d", decValue);
#endif
}

enum class Ownership : td::BYTE {Extern = 0, Intern};

inline DataType getType(bool){	return boolean;}
inline DataType getType(char){	return ch;}
inline DataType getType(td::BYTE){	return byte;}
inline DataType getType(td::WORD ){	return word;}
inline DataType getType(td::INT2 ){return int2;}
inline DataType getType(td::INT4 ){	return int4;}
inline DataType getType(td::UINT4 ){return uint4;}
inline DataType getType(td::LINT8) {return lint8;}
inline DataType getType(td::LUINT8) {return luint8;}
inline DataType getType(float) {return real4;}
inline DataType getType(double) {return real8;}
inline DataType getType(void*) { return ptrVoid; }
inline DataType getType(const void*) { return ptrVoid; }

inline const char* c_str(td::DataType val)
{
    switch(val)
    {
        case td::boolean: return "bool";
        case td::byte: return "byte";
        case td::word: return "word";
        case td::int2: return "int2";
        case td::uint4: return "uint4";
        case td::int4: return "int4";
        case td::lint8: return "lint8";
        case td::luint8: return "luint8";
        case td::real4: return "real4";
        case td::real8: return "real8";
        case td::string8: return "string8";
        case td::string16: return "strin16";
        case td::string32: return "string32";
        case td::enumerator: return "enumerator";
        case td::date: return "date";
        case td::time: return "time";
        case td::dateTime: return "dateTime";
        case td::decimal0: return "decimal0";
        case td::decimal1: return "decimal1";
        case td::decimal2: return "decimal2";
        case td::decimal3: return "decimal3";
        case td::decimal4: return "decimal4";
        case td::smallDecimal0: return "smalldecimal0";
        case td::smallDecimal1: return "smalldecimal1";
        case td::smallDecimal2: return "smalldecimal2";
        case td::smallDecimal3: return "smalldecimal3";
        case td::smallDecimal4: return "smalldecimal4";
        case td::ch: return "ch";
        case td::nch: return "nch";
        case td::chFix: return "chFix";
        case td::ch7: return "ch7";
        case td::nchFix: return "nchFix";
        case td::dbDate: return "dbDate";
        case td::dbTime: return "dbTime";
        case td::dbDateTime: return "dbDateTime";
        case td::dbDecimal: return "dbDecimal";
        case td::binary: return "binary";
        case td::guid: return "guid";
        case td::color: return "color";
        case td::colorID: return "colorID";
        case td::linePattern: return "linePattern";
        case td::dotPattern: return "dotPattern";
        case td::hAlignment: return "hAlign";
        case td::vAlignment: return "vAlign";
        case td::textAlignment: return "textAlign";
    default:
        return "none";
    }
    return "none";
}

}; //namespace td

