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
#include <tuple>

namespace td
{

template <typename CH>
inline const CH* findFirstNonWhiteSpace(const CH* pBuff)
{
    CH ch = *pBuff;
    while (ch != 0)
    {
        if (td::isWhiteSpace(ch))
        {
            ++pBuff;
            ch = *pBuff;
            continue;
        }
        return pBuff;
    }
    return nullptr;
}

template <typename CH>
inline const CH* findFirstWhiteSpace(const CH* pBuff)
{
    CH ch = *pBuff;
    while (ch != 0)
    {
        if (td::isWhiteSpace(ch))
            return pBuff;
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

template <typename CH>
inline const CH* findChar(const CH* pBuff, CH chToFind, size_t fromPosition)
{
    pBuff += fromPosition;
    CH ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
            return pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

template <typename CH>
inline const CH* findChar(const CH* pBuff, CH chToFind)
{
    CH ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
            return pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

template <typename CH>
inline const CH* skipHorizontalWhiteSpaces(const CH* pStr)
{
    CH ch = *pStr;
    while (ch)
    {
        if (!td::isHorizontalSpace(ch))
            return pStr;
        
        ++pStr;
        ch = *pStr;
    }
    return pStr;
}

template <typename CH>
inline const CH* findCharOrWhiteSpace(const CH* pBuff, CH chToFind)
{
    CH ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
        {
            return pBuff;
        }
        
        if (td::isWhiteSpace(ch))
            return pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

template <typename CH>
inline const CH* findLastNonWhiteSpace(const CH* pBuff, CH chToFind, const CH*& pLastNonWhiteSpace)
{
    pLastNonWhiteSpace = nullptr;
    CH ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
        {
            return pBuff;
        }
        
        if (!td::isWhiteSpace(ch))
            pLastNonWhiteSpace = pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

template <typename CH>
inline size_t getCount(const CH* pStr, CH chToCount)
{
    if (!pStr)
        return 0;
    size_t nCh = 0;
    CH ch = *pStr;
    while (ch != 0)
    {
        if (ch == chToCount)
            ++nCh;
        ++pStr;
        ch = *pStr;
    }
    return nCh;
}


inline size_t strLen(const td::UTF32* pStr)
{
    size_t len = 0;
    while (pStr)
    {
        ++len;
        ++pStr;
    }
    return len;
}

// Function to find the first occurrence of a pattern in a string
template <typename CH>
inline int findPattern(const CH* str, const CH* pattern)
{
    // Get the lengths of the string and the pattern
    size_t str_len = td::strLen(str);
    size_t pattern_len = strlen(pattern);

    // Edge case: if the pattern is longer than the string, return -1
    if (pattern_len > str_len) 
    {
        return -1;
    }

    // Loop through the string to search for the pattern
    for (size_t i = 0; i <= str_len - pattern_len; i++)
    {
        // Check if the substring from i to i + pattern_len matches the pattern
        if (strncmp(&str[i], pattern, pattern_len) == 0) 
        {
            return (int)i; // Return the position where the pattern starts
        }
    }

    // If the pattern is not found, return -1
    return -1;
}

template <typename CH>
inline bool beginsWith(const CH* str, const CH* pattern)
{
    // Get the lengths of the string and the pattern
    size_t str_len = td::strLen(str);
    size_t pattern_len = strlen(pattern);

    // Edge case: if the pattern is longer than the string, return -1
    if (pattern_len > str_len) 
    {
        return false;
    }

    // Loop through the string to search for the pattern
    if (strncmp(&str[0], pattern, pattern_len) == 0)
    {
        return true; // Return the position where the pattern starts
    }

    // If the pattern is not found, return -1
    return false;
}


inline bool isUTF8ContinuationByte(unsigned char byte)
{
    return ((byte & 0xC0) == 0x80); // Top 2 bits are 10
}

///returns len in bytes, and glyphLen
//inline std::tuple<td::UINT4, td::UINT4> getLenUTF8(const char* pStr)
//{
//    if (!pStr)
//        return {0,0};
//    const unsigned char* pUTF8 = (const unsigned char*) pStr;
//    td::UINT4 len =0;
//    td::UINT4 glyphLen = 0;
//    
//    while (*pUTF8 != 0)
//    {
//        unsigned char ch = *pUTF8;
//        
//        if (ch < 0x7F)
//        {
//            ++len;
//            ++glyphLen;
//            ++pUTF8;
//            continue;
//        }
//        if ((ch & 0xE0) == 0xC0)    //2-byte UTF8 character
//        {
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            len += 2;
//            ++glyphLen;
//            ++pUTF8;
//        }
//        else if ((ch & 0xF0) == 0xE0)    // 3-byte character
//        {
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            len += 3;
//            ++glyphLen;
//            ++pUTF8;
//        }
//        else if ((ch & 0xF8) == 0xF0)    // 4-byte character
//        {
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            ++pUTF8;
//            ch = *pUTF8;
//            if (!isUTF8ContinuationByte(ch))
//            {
//                assert(false); //not utf8
//                return {0,0};
//            }
//            len += 4;
//            ++glyphLen;
//            ++pUTF8;
//        }
//        else
//        {
//            assert(false); //not utf8
//            return {0,0};
//        }
//    }
//    return {len, glyphLen};
//}

/// Returns length in bytes and glyph length (number of visible characters)
inline std::tuple<unsigned int, unsigned int> getLenUTF8(const char* pStr)
{
    if (!pStr)
        return {0, 0};

    const unsigned char* pUTF8 = reinterpret_cast<const unsigned char*>(pStr);
    unsigned int len = 0;
    unsigned int glyphLen = 0;

    while (*pUTF8 != 0)
    {
        unsigned char ch = *pUTF8;

        if (ch < 0x80) // 1-byte character (ASCII)
        {
            ++len;
            ++glyphLen;
            ++pUTF8;
        }
        else if ((ch & 0xE0) == 0xC0) // 2-byte UTF-8 character
        {
            if (!isUTF8ContinuationByte(pUTF8[1]))
            {
                assert(false); // Invalid UTF-8 sequence
                return {0, 0};
            }
            len += 2;
            ++glyphLen;
            pUTF8 += 2;
        }
        else if ((ch & 0xF0) == 0xE0) // 3-byte UTF-8 character
        {
            if (!isUTF8ContinuationByte(pUTF8[1]) || !isUTF8ContinuationByte(pUTF8[2]))
            {
                size_t progress = (const char*) pUTF8 - pStr;
                assert(false); // Invalid UTF-8 sequence
                return {0, 0};
            }
            len += 3;
            ++glyphLen;
            pUTF8 += 3;
        }
        else if ((ch & 0xF8) == 0xF0) // 4-byte UTF-8 character
        {
            if (!isUTF8ContinuationByte(pUTF8[1]) || !isUTF8ContinuationByte(pUTF8[2]) || !isUTF8ContinuationByte(pUTF8[3]))
            {
                assert(false); // Invalid UTF-8 sequence
                return {0, 0};
            }
            len += 4;
            ++glyphLen;
            pUTF8 += 4;
        }
        else
        {
            assert(false); // Invalid UTF-8 sequence
            return {0, 0};
        }
    }

    return {len, glyphLen};
}

inline int toUTF8(td::UTF32 ch, td::UTF8 utf[4]) {
    if (ch < 0x80) {  // 1-byte UTF-8
        utf[0] = (unsigned char) ch;
        return 1;
    } else if (ch < 0x800) {  // 2-byte UTF-8
        utf[0] = (unsigned char) (0xC0 | (ch >> 6));
        utf[1] = (unsigned char) (0x80 | (ch & 0x3F));
        return 2;
    } else if (ch < 0x10000) {  // 3-byte UTF-8
        utf[0] = (unsigned char) (0xE0 | (ch >> 12));
        utf[1] = (unsigned char) (0x80 | ((ch >> 6) & 0x3F));
        utf[2] = (unsigned char) (0x80 | (ch & 0x3F));
        return 3;
    } else if (ch < 0x110000) {  // 4-byte UTF-8
        utf[0] = (unsigned char) (0xF0 | (ch >> 18));
        utf[1] = (unsigned char) (0x80 | ((ch >> 12) & 0x3F));
        utf[2] = (unsigned char) (0x80 | ((ch >> 6) & 0x3F));
        utf[3] = (unsigned char) (0x80 | (ch & 0x3F));
        return 4;
    }
    assert(false);
    return 0;  // Invalid character
}

} //namespace td
