// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Types.h
    @brief Platform-independent fundamental type aliases, string utility functions, color helpers, and core enumerations for the td namespace. */
//
//  Created by Izudin Dzafic on 18/04/2008.
//  Copyright © 2008-2021 IDz. All rights reserved.
//


#pragma once
#include <compiler/Definitions.h>

#ifdef _MSC_VER
#define SIZE_T_FMT "%Iu"
#else
#define SIZE_T_FMT "%zu"
#endif
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

/// @brief Portable snprintf wrapper for non-Windows platforms.
/// @param buffer Destination character buffer.
/// @param sizeOfBuffer Total size of the destination buffer in bytes.
/// @param count Maximum number of characters to write (use _TRUNCATE to fill buffer).
/// @param format printf-style format string.
/// @return Number of characters written, or -1 if the output was truncated.
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

/// @brief Portable strcpy_s replacement for non-Windows platforms.
/// @param buffer Destination character buffer.
/// @param sizeOfBuffer Total capacity of the destination buffer in bytes.
/// @param sIn Source null-terminated string to copy.
/// @return Number of characters copied.
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

/// @brief Portable wmemcpy_s replacement for non-Windows platforms.
/// @param dest Destination wide-character buffer.
/// @param buffLen Capacity of the destination buffer in wide characters.
/// @param src Source wide-character buffer.
/// @param count Number of wide characters to copy.
/// @return Pointer to the destination buffer.
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
            #if !defined(MU_ARM) && !defined(__aarch64__)
                #define USE_SIMD
            #endif
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
	typedef unsigned int UINT4;  ///< Unsigned 32-bit integer.
	typedef int INT4;            ///< Signed 32-bit integer.
	typedef unsigned char BYTE;  ///< Unsigned 8-bit byte.
    typedef unsigned char UINT1; ///< Unsigned 8-bit integer.
	typedef char INT1;           ///< Signed 8-bit integer.
	typedef unsigned short WORD; ///< Unsigned 16-bit word.
	typedef signed short INT2;   ///< Signed 16-bit integer.
	typedef unsigned short UINT2;///< Unsigned 16-bit integer.
	typedef unsigned int TPSKEY; ///< Key type for TPS lookups.
	typedef long long LINT8;     ///< Signed 64-bit integer.
    typedef long long INT8;      ///< Signed 64-bit integer (alias for LINT8).
	typedef unsigned long long LUINT8; ///< Unsigned 64-bit integer.
    typedef unsigned long long UINT8;  ///< Unsigned 64-bit integer (alias for LUINT8).
	typedef float Coord;         ///< Single-precision floating-point coordinate.
	typedef std::complex<double> cmplx;  ///< Double-precision complex number.
	typedef std::complex<float> cmplxf;  ///< Single-precision complex number.

#ifdef MU_64BIT
	typedef  LUINT8 PTRHOLDER;   ///< Pointer-sized unsigned integer (64-bit build).
    #define STRING_BASE_ADDRESS  8
#else
	typedef  UINT4 PTRHOLDER;    ///< Pointer-sized unsigned integer (32-bit build).
    #define STRING_BASE_ADDRESS  4
#endif

    /// @brief String encoding enumeration identifying the Unicode encoding used.
    typedef enum _StringEncoding:BYTE {
        EncodingUTF8=0, ///< UTF-8 encoding.
        EncodingUTF16,  ///< UTF-16 encoding.
        EncodingUTF32   ///< UTF-32 encoding.
    } StringEncoding;

	const char ENDL = '\n'; ///< End-of-line character constant.
	const char endl = '\n'; ///< End-of-line character constant (lowercase alias).

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


/// @brief Pack red, green, and blue components into a 32-bit OpenGL-style RGBA value with full opacity.
/// @tparam TINT Integral type for color component values.
/// @param r Red component (0–255).
/// @param g Green component (0–255).
/// @param b Blue component (0–255).
/// @return Packed RGBA color with alpha set to 0xFF.
template <typename TINT>
inline td::UINT4 GLRGB(TINT r, TINT g, TINT b)
{
    return (td::UINT4) (0xFF000000 |
                ((td::UINT4)(r)) |
                ((td::UINT4)(g)<<8) |
                ((td::UINT4)(b)<<16));
}

/// @brief Pack red, green, blue, and alpha components into a 32-bit OpenGL-style RGBA value.
/// @tparam TINT Integral type for color component values.
/// @param r Red component (0–255).
/// @param g Green component (0–255).
/// @param b Blue component (0–255).
/// @param a Alpha component (0–255).
/// @return Packed RGBA color value.
template <typename TINT>
inline td::UINT4 GLRGBA(TINT r, TINT g, TINT b, TINT a)
{
    return (td::UINT4) (((td::UINT4)(a) << 24) |
        ((td::UINT4)(r)) |
        ((td::UINT4)(g)<<8) |
        ((td::UINT4)(b)<<16));
}

/// @brief Pack red, green, blue, and alpha into a GDI-compatible ARGB 32-bit value (B and R channels swapped).
/// @tparam TINT Integral type for color component values.
/// @param r Red component (0–255).
/// @param g Green component (0–255).
/// @param b Blue component (0–255).
/// @param a Alpha component (0–255).
/// @return Packed GDI ARGB color value.
template <typename TINT>
inline td::UINT4 GDIARGB(TINT r, TINT g, TINT b, TINT a)
{
    return (td::UINT4) (((td::UINT4)(a) << 24) |
        ((td::UINT4)(b)) |
        ((td::UINT4)(g)<<8) |
        ((td::UINT4)(r)<<16));
}

/// @brief Extract the red component from a packed 32-bit color.
/// @param color Packed 32-bit color value.
/// @return Red channel value (0–255).
inline td::UINT4 GetRColor(td::UINT4 color)
{
    return color & 0x000000FF;
}

/// @brief Extract the green component from a packed 32-bit color.
/// @param color Packed 32-bit color value.
/// @return Green channel value (0–255).
inline td::UINT4 GetGColor(td::UINT4 color)
{
    return ((color & 0x0000FF00) >> 8);
}

/// @brief Extract the blue component from a packed 32-bit color.
/// @param color Packed 32-bit color value.
/// @return Blue channel value (0–255).
inline td::UINT4 GetBColor(td::UINT4 color)
{
    return ((color & 0x00FF0000) >> 16);
}

/// @brief Extract the alpha component from a packed 32-bit color.
/// @param color Packed 32-bit color value.
/// @return Alpha channel value (0–255).
inline td::UINT4 GetAColor(td::UINT4 color)
{
    return ((color & 0xFF000000) >> 24);
}

/// @brief Convert an OpenGL RGBA color to native GDI ARGB by swapping R and B channels.
/// @param color OpenGL-style RGBA packed color.
/// @return GDI-compatible ARGB packed color.
inline td::UINT4 GLRGBAToNativeGDI(td::UINT4 color)
{
    //rotate R and B
    td::UINT4 r = GetRColor(color);
    td::UINT4 b = GetBColor(color);
    td::UINT4 a = color & 0xFF000000;
    td::UINT4 g = color & 0x0000FF00;
    return (a | (r << 16) | g | b);
}

/// @brief Compare two null-terminated C strings case-insensitively.
/// @param s1 First string to compare.
/// @param s2 Second string to compare.
/// @return 0 if equal, negative if s1 < s2, positive if s1 > s2.
inline int compareNoCase(const char* s1, const char* s2)
{
    while (*s1 != '\0' && *s2 != '\0' && tolower(*s1) == tolower(*s2))
    {
        s1++;
        s2++;
    }

    return tolower(*s1) - tolower(*s2);
}

/// @brief Compare the beginning of a string to a substring, case-insensitively.
/// @param str The string to search within.
/// @param subStr The substring prefix to match.
/// @return 0 if str starts with subStr (case-insensitive), non-zero otherwise, -1 if subStr is empty.
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

/// @brief Convert a boolean value to its C string representation.
/// @param bVal Boolean value to convert.
/// @return "true" if bVal is true, "false" otherwise.
inline const char* c_str(bool bVal)
{
    return bVal ? "true" : "false";
}

/// @brief Check whether a C string equals "true".
/// @param str Null-terminated string to test.
/// @return true if str equals "true", false otherwise.
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


/// @brief Extract the low 32 bits from a 64-bit signed integer.
/// @param val The 64-bit value to decompose.
/// @return Low 32-bit signed integer portion.
inline td::INT4 getLow(td::LINT8 val)
{
    td::LINT8 tmp = 0x00000000FFFFFFFF & val;
    return (td::INT4) tmp;
}

/// @brief Extract the high 32 bits from a 64-bit signed integer.
/// @param val The 64-bit value to decompose.
/// @return High 32-bit signed integer portion.
inline td::INT4 getHi(td::LINT8 val)
{
    td::LINT8 tmp = 0xFFFFFFFF00000000 & val;
    return (td::INT4) tmp;
}

/// @brief Return the length of a null-terminated UTF-8 string, safely handling null pointers.
/// @param pStr Pointer to the null-terminated string, or nullptr.
/// @return Number of characters in the string, or 0 if pStr is nullptr.
inline size_t strLen(const char* pStr)
{
    if (!pStr)
        return 0;
    return ::strlen(pStr);
}

/// @brief Return the length of a null-terminated wide-character string (UTF-16 or UTF-32).
/// @tparam TCH Character type (must be td::UTF16 or td::UTF32).
/// @param pStr Pointer to the null-terminated wide string.
/// @return Number of characters in the string.
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

/// @brief Test whether a character is an alphabetic letter or underscore.
/// @tparam TCH Character type.
/// @param val Character value to test.
/// @return true if val is 'A'–'Z', 'a'–'z', or '_'.
template <typename TCH>
bool isAlpha(TCH val)
{
    return ((val == '_') || (val >= 'a' && val <= 'z') || (val >= 'A' && val <= 'Z'));
}

/// @brief Test whether a character is a decimal digit.
/// @tparam TCH Character type.
/// @param val Character value to test.
/// @return true if val is '0'–'9'.
template <typename TCH>
bool isNumeric(TCH val)
{
    return (val >= '0' && val <= '9');
}

/// @brief Test whether a character is a lowercase English vowel.
/// @tparam TCH Character type.
/// @param val Character value to test.
/// @return true if val is 'a', 'e', 'i', 'o', or 'u'.
template <typename TCH>
bool isVowel(TCH val)
{
    return ((val == TCH('a') ) || (val == TCH('e') ) || (val == TCH('i') ) || (val == TCH('o')) || (val == TCH('u')));
}


/// @brief Search for a character in a string scanning left to right from a given position.
/// @tparam TCH Character type.
/// @param pStr Null-terminated string to search.
/// @param ch Character to locate.
/// @param iPos Zero-based start position offset.
/// @return Zero-based index of the first match relative to iPos, or -1 if not found.
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

/// @brief Search for a character in a fixed-length buffer scanning left to right.
/// @tparam TCH Character type.
/// @param pStr Buffer to search (need not be null-terminated).
/// @param ch Character to locate.
/// @param nBuffLen Number of characters to examine.
/// @param iPos Zero-based start position offset.
/// @return Zero-based index of the first match relative to iPos, or -1 if not found.
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

/// @brief Search for a character in a string scanning right to left.
/// @tparam TCH Character type.
/// @param pStr Null-terminated string to search.
/// @param ch Character to locate.
/// @param inStartPos Zero-based start position for the search, or -1 to start from the end.
/// @return Zero-based number of steps from the start position to the match, or -1 if not found.
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

/// @brief Delete all heap-allocated objects pointed to by elements of a container and clear it.
/// @tparam T Container type whose value type is a pointer.
/// @param v Container of pointers whose targets will be deleted.
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

/// @brief Check whether a UTF-8 character is a Unicode line-break character (always false for UTF-8).
/// @param ch UTF-8 character to test.
/// @return Always false for UTF-8 characters.
inline bool isUnidodeLineBreak(td::UTF8)
{
    return false;
}

/// @brief Check whether a UTF-16 character is the Unicode LINE SEPARATOR (U+2028).
/// @param ch UTF-16 character to test.
/// @return true if ch equals U+2028.
inline bool isUnidodeLineBreak(td::UTF16 ch)
{
    td::UINT2 iVal = (td::UINT2) ch;
    if (iVal == 8232) //Unicode Character 'LINE SEPARATOR' (U+2028)
        return true;
    return false;
}

/// @brief Check whether a UTF-32 character is the Unicode LINE SEPARATOR (U+2028).
/// @param ch UTF-32 character to test.
/// @return true if ch equals U+2028.
inline bool isUnidodeLineBreak(td::UTF32 ch)
{
    td::UINT4 iVal = (td::UINT4) ch;
    if (iVal == 8232) //Unicode Character 'LINE SEPARATOR' (U+2028)
        return true;
    return false;
}

/// @brief Test whether a character is whitespace (space, tab, newline, carriage return, or Unicode line separator).
/// @tparam TCH Character type.
/// @param ch Character to test.
/// @return true if ch is considered whitespace.
template <typename TCH>
bool isWhiteSpace(TCH ch)
{
    if (isUnidodeLineBreak(ch)) //special unicode for HTML
        return true;

    return ((ch == (TCH)' ') || (ch == (TCH)'\t') || (ch == (TCH)'\n') || (ch == (TCH)'\r'));
}

/// @brief Test whether a character is a newline (LF or Unicode line separator).
/// @tparam TCH Character type.
/// @param ch Character to test.
/// @return true if ch is a newline character.
template <typename TCH>
bool isNewLine(TCH ch)
{
    if (isUnidodeLineBreak(ch)) //special unicode for HTML
        return true;

    return (ch == (TCH)'\n');
}

/// @brief Test whether a character is a horizontal space (space or tab).
/// @tparam TCH Character type.
/// @param ch Character to test.
/// @return true if ch is a space or tab character.
template <typename TCH>
bool isHorizontalSpace(TCH ch)
{
    return ((ch == (TCH)' ') || (ch == (TCH)'\t'));
}

/// @brief Advance a string pointer past any leading whitespace characters.
/// @tparam TCH Character type.
/// @param pStr Pointer to the start of the string.
/// @return Pointer to the first non-whitespace character, or nullptr if pStr is nullptr.
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

/// @brief Count the number of trailing whitespace characters in a string.
/// @tparam TCH Character type.
/// @param pStr Null-terminated string to inspect.
/// @param fromPos Upper bound position to start scanning backwards, or -1 to use the end of the string.
/// @return Number of trailing whitespace characters found.
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

/// @brief Remove trailing whitespace characters from a mutable string in place.
/// @tparam TCH Character type.
/// @param pStr Null-terminated string to trim in place.
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

/// @brief Remove leading and trailing whitespace from a mutable string in place.
/// @tparam TCH Character type.
/// @param pStr Mutable string to trim.
/// @return Pointer to the first non-whitespace character after leading trim.
template <typename TCH>
TCH* trimWhiteSpaces(TCH* pStr)
{
    pStr = skipWhiteSpaces(pStr);
    if (pStr)
        trimTrailingWhiteSpaces(pStr);
    return pStr;
}

/// @brief Find the first occurrence of a character in a null-terminated string starting from a given position.
/// @tparam TCH Character type.
/// @param pStr Null-terminated string to search.
/// @param ch Character to find.
/// @param searchStartPos Zero-based index to begin searching.
/// @return Zero-based index of the first match, or -1 if not found.
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

/// @brief Count occurrences of a character within a string of known length.
/// @tparam TCH Character type.
/// @param pStr String to search (need not be null-terminated if strLen is non-zero).
/// @param strLen Number of characters to examine; if 0, strlen is used.
/// @param ch Character to count.
/// @return Number of occurrences of ch in the string.
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

/// @brief Format a string into a caller-supplied buffer using printf-style format.
/// @param buffer Destination character buffer.
/// @param bufferSize Total size of the destination buffer in bytes.
/// @param format printf-style format string.
/// @return Number of characters written (excluding the null terminator), or negative on error.
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

/// @brief Format a string into a statically-sized array buffer using printf-style format.
/// @tparam bufferSize Compile-time size of the destination array.
/// @param buffer Destination character array.
/// @param format printf-style format string.
/// @return Number of characters written (excluding the null terminator), or negative on error.
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

/// @brief Union allowing a 32-bit value to be interpreted as byte, word, signed int, or unsigned int.
typedef union
{
    td::BYTE ui1;   ///< Unsigned 8-bit view.
    td::WORD ui2;   ///< Unsigned 16-bit view.
    td::INT4 i4;    ///< Signed 32-bit view.
    td::UINT4 ui4;  ///< Unsigned 32-bit view.
} Int4Union;

/// @brief Union allowing a 64-bit value to be interpreted as signed or unsigned 64-bit integer.
typedef union
{
    td::LINT8 li8;   ///< Signed 64-bit view.
    td::LUINT8 lui8; ///< Unsigned 64-bit view.
} Int8Union;

/// @brief Line style enumeration for graphical rendering.
typedef enum _tLineStyle : BYTE {
    SOLID = 0, ///< Solid line.
    DASH,      ///< Dashed line.
    DOTDASH,   ///< Dot-dash alternating line.
    DOT        ///< Dotted line.
} LineStyle;

/// @brief Data type enumeration identifying the runtime type of a Variant or database column.
typedef enum _tDataType : BYTE {TD_NONE=0, boolean=1, byte, word, int2, uint4, int4, lint8, luint8, real4, real8, string8, string16, string32, enumerator, date, time, dateTime, decimal0, decimal1, decimal2, decimal3, decimal4, ch, nch, chFix, nchFix, ch7,  dbDate, dbTime, dbDateTime, dbDecimal, binary, guid, color, colorID, hAlignment, vAlignment, textAlignment, linePattern, dotPattern, anchor, smallDecimal0, smallDecimal1, smallDecimal2, smallDecimal3, smallDecimal4, rect, point, circle, variant,
    ptrBoolean, ptrByte, ptrWord, ptrInt2, ptrUint4, ptrInt4, ptrLint8, ptrLuint8, ptrReal4, ptrReal8, ptrString8, ptrString16, ptrString32, ptrEnumerator, ptrDate, ptrTime, ptrDateTime, ptrDecimal0, ptrDecimal1, ptrDecimal2, ptrDecimal3, ptrDecimal4, ptrCh, ptrNch, ptrChFix, ptrSmallChFix, ptrNchFix, ptrDbDate, ptrDbTime, ptrDbDateTime, ptrDbDecimal, ptrBinary, ptrGuid, ptrColor, ptrSmalldecimal0, ptrSmalldecimal1, ptrSmalldecimal2, ptrSmalldecimal3, ptrSmalldecimal4, ptrRect, ptrPoint, ptrVariant, ptrVoid,
    TD_LAST } DataType; //ch-ansi char, nch-unicode char

/// @brief Presentation type controlling how a value is displayed in UI widgets.
enum class PresentationType : unsigned char {
    PT_SameAsDT=0, ///< Use the same representation as the data type.
    PT_YesNo,      ///< Display as Yes/No text.
    PT_TrueFalse,  ///< Display as True/False text.
    PT_CheckBox    ///< Display as a check box.
};

/// @brief Floating-point display format.
enum class FormatFloat : unsigned char {
    Decimal=0,  ///< Standard decimal notation.
    Scientific  ///< Scientific (exponential) notation.
};

//ubaceno u qtu definitions
//	typedef enum _tHAlignment { Left=0x0001, Right =0x0002, HCenter=0x0004} tHAlignment;
//
//	typedef enum _tVAlignment { Top=0x0020, Bottom=0x0040, VCenter = 0x0080} tVAlignment;

/// @brief Horizontal alignment options for UI elements.
enum class HAlignment : unsigned char {
    Left=0, ///< Align to the left.
    Center, ///< Center horizontally.
    Right   ///< Align to the right.
};

/// @brief Vertical alignment options for UI elements.
enum class VAlignment : unsigned char {
    Top=0,  ///< Align to the top.
    Center, ///< Center vertically.
    Bottom  ///< Align to the bottom.
};

/// @brief Text alignment options within a text-rendering region.
enum class TextAlignment : unsigned char {
    Left=0,    ///< Left-align text.
    Center,    ///< Center text.
    Right,     ///< Right-align text.
    Justified  ///< Justify text on both sides.
};

/// @brief Text wrapping mode.
enum class TextWrap : unsigned char {
    No=0, ///< No wrapping.
    Word, ///< Wrap at word boundaries.
    Any   ///< Wrap at any character boundary.
};

/// @brief Text ellipsize mode for truncated text.
enum class TextEllipsize : unsigned char {
    None=0, ///< No ellipsis.
    Start,  ///< Ellipsis at the beginning.
    Middle, ///< Ellipsis in the middle.
    End     ///< Ellipsis at the end.
};

/// @brief Thousand-separator display preference.
enum class ThousandSep : unsigned char {
    SystemDefault, ///< Use the system locale default.
    Show,          ///< Always show the separator.
    DoNotShow      ///< Never show the separator.
};

/// @brief Convert a horizontal alignment value to an equivalent text alignment value.
/// @param hAling Horizontal alignment to convert.
/// @return Corresponding TextAlignment value.
inline TextAlignment getTextAlignment(HAlignment hAling)
{
    switch (hAling)
    {
    case HAlignment::Left: return TextAlignment::Left;
    case HAlignment::Right: return TextAlignment::Right;
    case HAlignment::Center: return TextAlignment::Center;
    default: return TextAlignment::Left;
    }
    return TextAlignment::Left;
}

/// @brief Check whether a DataType represents an integer type (including decimal0 and smallDecimal0).
/// @param type DataType value to test.
/// @return true if type is an integer-compatible DataType.
inline bool isInteger(td::DataType type)
{
    return (((type >= td::byte) && (type <= td::luint8)) || (type == td::decimal0) || (type == td::smallDecimal0));
}

/// @brief Check whether a DataType represents any numeric type (integer, float, or decimal).
/// @param type DataType value to test.
/// @return true if type is numeric.
inline bool isNumeric(td::DataType type)
{
    return (((type >= td::byte) && (type <= td::real8)) || ((type >= td::decimal0) && (type <= td::decimal4)) || ((type >= td::smallDecimal0) && (type <= td::smallDecimal4)));
}

/// @brief Check whether a DataType represents a date or time type.
/// @param type DataType value to test.
/// @return true if type is date, time, or dateTime.
inline bool isDateTime(td::DataType type)
{
    return ((type >= td::date) && (type <= td::dateTime));
}

/// @brief Check whether a DataType represents a floating-point type.
/// @param type DataType value to test.
/// @return true if type is real4 or real8.
inline bool isFloat(td::DataType type)
{
    return ((type >= td::real4) && (type <= td::real8));
}

/// @brief Check whether a DataType represents a fixed-precision decimal type.
/// @param type DataType value to test.
/// @return true if type is decimal0 through decimal4.
inline bool isDecimal(td::DataType type)
{
    return ( (type >= td::decimal0) && (type <= td::decimal4)  );
}

/// @brief Check whether a DataType represents a small (INT4-backed) decimal type.
/// @param type DataType value to test.
/// @return true if type is smallDecimal0 through smallDecimal4.
inline bool isSmallDecimal(td::DataType type)
{
    return ( (type >= td::smallDecimal0) && (type <= td::smallDecimal4) );
}

/// @brief Check whether a DataType represents an unsigned integer type.
/// @param type DataType value to test.
/// @return true if type is byte, word, uint4, or luint8.
inline bool isUnsigned(td::DataType type)
{
    return ((type == td::byte) || (type == td::word) || (type == td::uint4) || (type == td::luint8));
}

/// @brief Check whether a DataType represents a variable-length string type.
/// @param type DataType value to test.
/// @return true if type is string8, string16, or string32.
inline bool isString(td::DataType type)
{
    if (type >= td::string8 && type <= td::string32)
        return true;
    return false;
}

/// @brief Check whether a DataType represents a database fixed-length or variable-length character type.
/// @param type DataType value to test.
/// @return true if type is ch, nch, chFix, or nchFix.
inline bool isDBString(td::DataType type)
{
    if ((type >= td::ch) && (type <= td::nchFix))
        return true;
    return false;
}

/// @brief Convert a boolean value to a const C string "true" or "false".
/// @param bVal Boolean value to convert.
/// @return Pointer to the string "true" or "false".
inline const char* const toString(bool bVal)
{
    return bVal ? "true" : "false";
}

/// @brief Convert a decimal integer to a string stored in a statically-sized buffer.
/// @tparam size Compile-time size of the destination array.
/// @tparam TINT Integer type to convert.
/// @param buffer Destination character array.
/// @param decValue Integer value to convert.
template <size_t size, typename TINT>
void iToA(const char(&buffer)[size], TINT decValue)
{
#ifdef MU_WINDOWS
    itoa(decValue, buffer, 10);
#else
    SPRINTF(buffer, "%d", decValue);
#endif
}

/// @brief Ownership semantics for resource management.
enum class Ownership : td::BYTE {
    Extern = 0, ///< Resource is owned externally and must not be freed here.
    Intern      ///< Resource is owned internally and will be freed on destruction.
};

/// @brief Return the DataType tag corresponding to a boolean value.
/// @return td::boolean.
inline DataType getType(bool){	return boolean;}
/// @brief Return the DataType tag corresponding to a char value.
/// @return td::ch.
inline DataType getType(char){	return ch;}
/// @brief Return the DataType tag corresponding to a BYTE value.
/// @return td::byte.
inline DataType getType(td::BYTE){	return byte;}
/// @brief Return the DataType tag corresponding to a WORD value.
/// @return td::word.
inline DataType getType(td::WORD ){	return word;}
/// @brief Return the DataType tag corresponding to an INT2 value.
/// @return td::int2.
inline DataType getType(td::INT2 ){return int2;}
/// @brief Return the DataType tag corresponding to an INT4 value.
/// @return td::int4.
inline DataType getType(td::INT4 ){	return int4;}
/// @brief Return the DataType tag corresponding to a UINT4 value.
/// @return td::uint4.
inline DataType getType(td::UINT4 ){return uint4;}
/// @brief Return the DataType tag corresponding to a LINT8 value.
/// @return td::lint8.
inline DataType getType(td::LINT8) {return lint8;}
/// @brief Return the DataType tag corresponding to a LUINT8 value.
/// @return td::luint8.
inline DataType getType(td::LUINT8) {return luint8;}
/// @brief Return the DataType tag corresponding to a float value.
/// @return td::real4.
inline DataType getType(float) {return real4;}
/// @brief Return the DataType tag corresponding to a double value.
/// @return td::real8.
inline DataType getType(double) {return real8;}
/// @brief Return the DataType tag corresponding to a void pointer.
/// @return td::ptrVoid.
inline DataType getType(void*) { return ptrVoid; }
/// @brief Return the DataType tag corresponding to a const void pointer.
/// @return td::ptrVoid.
inline DataType getType(const void*) { return ptrVoid; }

/// @brief Return the C string name of a DataType value.
/// @param val DataType value to convert.
/// @return Null-terminated string name of the DataType, or "none" for unrecognized values.
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
