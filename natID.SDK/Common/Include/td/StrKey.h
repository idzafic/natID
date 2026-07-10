// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StrKey.h
    @brief Defines compact numeric-backed string keys for efficient map lookups. */
#pragma once
#include <td/Types.h>
#include <cstring>
#include <cassert>

namespace td
{

/// @brief Stores a short string of up to KeyLen characters packed into a numeric integer key.
/// @tparam NumType The underlying integer type used to store the packed key (e.g. UINT4 or LUINT8).
/// @tparam KeyLen  Maximum number of characters the key can hold.
template <typename NumType, td::UINT2 KeyLen>
class StrKeyBase
{
private:
    union
    {
        NumType _key;          ///< Packed numeric representation of the string key.
        char _str[sizeof(NumType)]; ///< Byte-level view of the key for memcpy operations.
    };

public:
    static constexpr td::UINT2 Length = KeyLen; ///< Compile-time maximum key length.

    /// @brief Default constructor; initialises the key to zero.
    StrKeyBase()
    : _key(0)
    {
    }

    /// @brief Constructs from a C-string with an optional explicit length.
    /// @param pStr Pointer to the source string.
    /// @param strLen Length of the string, or -1 to use strlen.
    StrKeyBase(const char* pStr, int strLen = -1)
    {
        set(pStr, strLen);
    }

    /// @brief Constructs from any string type that provides c_str() and length().
    /// @tparam TSTR The string type.
    /// @param str The source string object.
    template <class TSTR>
    StrKeyBase(const TSTR& str)
    {
        set(str.c_str(), int(str.length()));
    }

    /// @brief Returns the fixed key length (compile-time value).
    /// @return KeyLen.
    constexpr td::UINT2 length()
    {
        return KeyLen;
    }

    /// @brief Returns the numeric key value.
    /// @return The packed integer key.
    inline NumType getKey() const
    {
        return _key;
    }

    /// @brief Sets the key from a C-string with an explicit length.
    /// @param pStr Pointer to the source string (must not be null).
    /// @param strLen Length of the string; if negative, strlen is used.
    inline void set(const char* pStr, int strLen)
    {
        _key = 0;
        if (strLen < 0)
            strLen = int(strlen(pStr));

        assert(pStr);
        assert(strLen > 0);
        if (!pStr)
            return;

        if (strLen == 0)
            return;
//        assert(strLen <= KeyLen);
        if (strLen > KeyLen)
            strLen = KeyLen;
        if constexpr (KeyLen == 9)
        {
            if (strLen == KeyLen)
            {
                td::LUINT8 key = 0;
                for (td::UINT2 i=0; i<KeyLen; ++i)
                {
                    td::LUINT8 ch = pStr[i];

                    size_t nLshift = ((KeyLen-i-1)*7);
                    if (nLshift > 0)
                    {
                        ch <<= (nLshift+1);
                    }
                    else
                    {
                        ch &= 0x7F;
                    }

                    key |= ch;
                }
                _key = key;
                return;
            }
        }
        memcpy(&_key, pStr, strLen);
    }

    /// @brief Sets the key from any string type that provides c_str() and length().
    /// @tparam TSTR The string type.
    /// @param str The source string object.
    template <class TSTR>
    inline void set(const TSTR& str)
    {
        set(str.c_str(), str.length());
    }

    /// @brief Assigns from another StrKeyBase of the same type.
    /// @param key The source key.
    void operator = (const StrKeyBase<NumType, KeyLen>& key)
    {
        _key = key._key;
    }

    /// @brief Extracts the key as a null-terminated C-string into a caller-supplied buffer.
    /// @param key Output buffer of at least KeyLen+1 characters.
    void get(char key[KeyLen+1]) const
    {
        key[KeyLen] = 0;
        key[KeyLen-1] = 0;
        if constexpr (KeyLen == 9)
        {
            char chLast = _str[7];
            if (chLast != 0)
            {
                bool allAscii = true;
                for (td::UINT2 i=0; i<8; ++i)
                {
                    char ch = _str[i];
                    if (ch & 0x80)
                    {
                        allAscii = false;
                        break;
                    }
                }
                if (!allAscii)
                {
                    td::LUINT8 keyVal = _key;
                    for (td::UINT2 i = 0; i < KeyLen; ++i)
                    {
                        size_t shift = (KeyLen - i - 1) * 7;
                        td::LUINT8 ch;
                        if (shift > 0)
                            ch = (keyVal >> (shift + 1)) & 0x7F;
                        else
                            ch = keyVal & 0x7F;
                        key[i] = static_cast<char>(ch);
                    }
                    return;
                }
            }
            memcpy(key, _str, 8);
        }
        else
            memcpy(key, _str, KeyLen);
    }

    /// @brief Converts the key to a string object via assignment.
    /// @tparam TSTR The string type to assign to.
    /// @param str Output string that receives the key characters.
    template <class TSTR>
    void toString(TSTR& str) const
    {
        char tmp[KeyLen+1];
        get(tmp);
        str = &tmp[0];
    }

};

/// @brief Less-than operator for StrKeyBase; compares the packed numeric keys.
/// @tparam NumType Underlying integer type.
/// @tparam KeyLen  Maximum key length.
/// @param s1 Left operand.
/// @param s2 Right operand.
/// @return true if s1's key is numerically less than s2's key.
template <typename NumType, td::UINT2 KeyLen>
inline bool operator < (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() < s2.getKey());
}

/// @brief Greater-than operator for StrKeyBase.
/// @tparam NumType Underlying integer type.
/// @tparam KeyLen  Maximum key length.
/// @param s1 Left operand.
/// @param s2 Right operand.
/// @return true if s1's key is numerically greater than s2's key.
template <typename NumType, td::UINT2 KeyLen>
inline bool operator > (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() > s2.getKey());
}

/// @brief Equality operator for StrKeyBase.
/// @tparam NumType Underlying integer type.
/// @tparam KeyLen  Maximum key length.
/// @param s1 Left operand.
/// @param s2 Right operand.
/// @return true if both keys are numerically equal.
template <typename NumType, td::UINT2 KeyLen>
inline bool operator == (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() == s2.getKey());
}

using StrKey  = StrKeyBase<td::LUINT8, 9>; ///< 9-character string key backed by a 64-bit integer.
using StrKey4 = StrKeyBase<td::UINT4,  4>; ///< 4-character string key backed by a 32-bit integer.


}; //namespace td


