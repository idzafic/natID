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
#include <cstring>
#include <cassert>

namespace td
{

template <typename NumType, td::UINT2 KeyLen>
class StrKeyBase
{
private:
    union
    {
        NumType _key;
        char _str[sizeof(NumType)];
    };
    
public:
    static constexpr td::UINT2 Length = KeyLen;
    
    StrKeyBase()
    : _key(0)
    {
    }
    
    StrKeyBase(const char* pStr, int strLen = -1)
    {
        set(pStr, strLen);
    }
    
    template <class TSTR>
    StrKeyBase(const TSTR& str)
    {
        set(str.c_str(), int(str.length()));
    }
    
    constexpr td::UINT2 length()
    {
        return KeyLen;
    }
    
    inline NumType getKey() const
    {
        return _key;
    }
    
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

    template <class TSTR>
    inline void set(const TSTR& str)
    {
        set(str.c_str(), str.length());
    }

    void operator = (const StrKeyBase<NumType, KeyLen>& key)
    {
        _key = key._key;
    }
    
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
    
    template <class TSTR>
    void toString(TSTR& str) const
    {
        char tmp[KeyLen+1];
        get(tmp);
        str = &tmp[0];
    }
    
};

template <typename NumType, td::UINT2 KeyLen>
inline bool operator < (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() < s2.getKey());
}

template <typename NumType, td::UINT2 KeyLen>
inline bool operator > (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() > s2.getKey());
}

template <typename NumType, td::UINT2 KeyLen>
inline bool operator == (const StrKeyBase<NumType, KeyLen>& s1, const StrKeyBase<NumType, KeyLen>& s2)
{
    return (s1.getKey() == s2.getKey());
}

using StrKey = StrKeyBase<td::LUINT8, 9>;
using StrKey4 = StrKeyBase<td::UINT4, 4>;


}; //namespace td

	
