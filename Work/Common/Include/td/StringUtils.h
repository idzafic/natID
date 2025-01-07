// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace td
{

inline const char* findFirstNonWhiteSpace(const char* pBuff)
{
    char ch = *pBuff;
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

inline const char* findFirstWhiteSpace(const char* pBuff)
{
    char ch = *pBuff;
    while (ch != 0)
    {
        if (td::isWhiteSpace(ch))
            return pBuff;
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

inline const char* findChar(const char* pBuff, char chToFind, size_t fromPosition)
{
    pBuff += fromPosition;
    char ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
            return pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

inline const char* findChar(const char* pBuff, char chToFind)
{
    char ch = *pBuff;
    while (ch)
    {
        if (ch == chToFind)
            return pBuff;
        
        ++pBuff;
        ch = *pBuff;
    }
    return nullptr;
}

inline const char* findCharOrWhiteSpace(const char* pBuff, char chToFind)
{
    char ch = *pBuff;
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

inline const char* findLastNonWhiteSpace(const char* pBuff, char chToFind, const char*& pLastNonWhiteSpace)
{
    pLastNonWhiteSpace = nullptr;
    char ch = *pBuff;
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

inline size_t getCount(const char* pStr, char chToCount)
{
    if (!pStr)
        return 0;
    size_t nCh = 0;
    auto ch = *pStr;
    while (ch != 0)
    {
        if (ch == chToCount)
            ++nCh;
        ++pStr;
        ch = *pStr;
    }
    return nCh;
}

// Function to find the first occurrence of a pattern in a string
inline int findPattern(const char* str, const char* pattern)
{
    // Get the lengths of the string and the pattern
    size_t str_len = strlen(str);
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

inline bool beginsWith(const char* str, const char* pattern)
{
    // Get the lengths of the string and the pattern
    size_t str_len = strlen(str);
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

}
