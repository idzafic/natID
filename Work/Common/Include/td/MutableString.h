// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "String.h"

namespace td
{

template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>
class MutableStringBase : protected StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>
{
protected:
    using BASE = StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>;
    using THISOBJ = MutableStringBase<T_CHAR, Encoding, SPACE_FOR_SIZE>;
    
    size_t _capacity = 0; //includes trailing zero
public:
    MutableStringBase()
    : BASE()
    {
    }
    
    // Move constructor
    inline MutableStringBase(MutableStringBase&& str) noexcept
    {
        BASE::buffer = str.buffer;
        str.buffer = nullptr;
    }
    
    // Move assignment operator
    inline MutableStringBase& operator=(MutableStringBase&& str) noexcept
    {
        if (this != &str)
        {
            if (BASE::buffer)
                delete[] BASE::buffer;
            
            BASE::buffer = str.buffer;
            str.buffer = nullptr;
        }
        return *this;
    }
    
    ~MutableStringBase()
    {
        if (BASE::buffer)
        {
            delete[] BASE::buffer;
            BASE::buffer = nullptr;
        }
    }
    
    BASE& getString()
    {
        return *this;
    }
    
    const BASE& getString() const
    {
        return *this;
    }
    
    [[nodiscard]] BASE getImmutableString() const
    {
        return BASE::clone();
    }
    
    //includes trailing zero
    size_t getCapacity() const
    {
        return _capacity;
    }
    
    size_t capacity() const
    {
        return _capacity;
    }
    
    const T_CHAR* c_str() const
    {
        return BASE::c_str();
    }
    
    size_t length() const
    {
        return BASE::length();
    }
    
    size_t unitLength() const
    {
        return BASE::unitLength();
    }
    
    void reserve(size_t capacity)
    {
        if (_capacity < capacity)
        {
            _capacity = capacity;
            BASE::reserve(capacity);
            BASE::setRefsToMaximum();
        }
        
        BASE::resetSize(0);
        T_CHAR* pBeg = BASE::getBegin();
        if (pBeg)
            pBeg[0] = 0;
    }
    
    inline void appendFormat(const char* format, ...)
    {
        va_list argptr;
        {
            va_start(argptr, format);
            char buffer[1024];
            int nLen = vsnprintf(buffer, 1024, format, argptr);
            va_end(argptr);
            
            if (nLen > 0)
            {
                append(buffer, nLen);
                return;
            }
        }

        {
            int nLen = 0;
#ifdef MU_WINDOWS
            va_start(argptr, format);
            nLen = _vscprintf(format, argptr);
            va_end(argptr);
#else
            va_start(argptr, format);
            nLen = vsnprintf(nullptr, 0, format, argptr);
            va_end(argptr);
#endif

            if ((nLen > 0) && (nLen <= 0x0000FFFF) ) //maximum string length for format is 64 kB
            {
                char* tmp = new char[nLen + 1];
                va_start(argptr, format);
                nLen = SNPRINTF(tmp, nLen, _TRUNCATE, format, argptr);
                va_end(argptr);
                if (nLen > 0)
                    append(tmp, nLen);
                delete [] tmp;
            }
            else if (nLen > 0)
            {
                assert(false && "String is too long for format!");
            }
        }
    }
    
    
    void resetLen()
    {
        BASE::resetSize(0);
        if (_capacity > 0)
        {
            T_CHAR* pBeg = BASE::getBegin();
            if (pBeg)
                pBeg[0] = 0;
        }
    }
    
    void reset()
    {
        BASE::resetSize(0);
        if (_capacity > 0)
        {
            T_CHAR* pBeg = BASE::getBegin();
            if (pBeg)
                pBeg[0] = 0;
        }
    }
    
    void append(const T_CHAR* pStr, size_t inputUnitLen = 0)
    {
        if (inputUnitLen == 0)
        {
            inputUnitLen = td::strLen(pStr);
            if (inputUnitLen == 0)
                return;
        }
        
        size_t currLen = unitLength();
        if (currLen + inputUnitLen >= _capacity)
        {
            BASE::appendString(pStr, (int)inputUnitLen);
            _capacity = BASE::length()+1;
            BASE::setRefsToMaximum();
        }
        else
        {
            size_t newLen = currLen + inputUnitLen;
            char* pBeg = (char*) BASE::getBegin();
            assert(pBeg);
            memcpy(pBeg+currLen, pStr, inputUnitLen*sizeof(T_CHAR));
            pBeg[newLen] = 0;
            BASE::resetSize(newLen);
        }
    }
    
    void append(T_CHAR ch)
    {
        append(&ch, 1);
    }
    
    void append(const td::String& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }
    
    void append(const td::StringExt& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }
    
    void operator += (const char* pStr)
    {
        auto strLen = strlen(pStr);
        append(pStr, strLen);
    }
    
    void operator += (const td::String& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }
    
    void operator += (const td::StringExt& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }
    
    //fromPosition is input/output argument
    inline bool getLine(const BASE& fromString, td::UINT4& fromPosition, bool addNewLine = false)
    {
        td::UINT4 strLen = fromString.unitLength();
        if (fromPosition >= strLen)
            return false;
        
        strLen -= fromPosition;
        int pos = fromString.findNextNewLinePosition(fromPosition);
        if (pos == (int) fromPosition)
        {
            reset();
            if (addNewLine)
            {
                reserve(1);
                append("\n", 1);
            }
                
            fromPosition = pos + 1;
            return true;
        }
        
        if (pos > 0)
            strLen = pos - fromPosition;
        
        auto pStr = fromString.c_str();
        pStr += fromPosition;
        auto pLast = pStr + strLen - 1;
        
        if (pos > 0)
            fromPosition = pos + 1;
        else
            fromPosition += strLen;
        
        if (*pLast == (T_CHAR) '\r') //check if it's windows CR/LF
            --strLen;
        
        if (strLen == 0)
        {
            reset();
            if (addNewLine)
            {
                reserve(1);
                append("\n", 1);
            }
            return true;
        }
        
        if (addNewLine)
            reserve(strLen + 1);
        else
            reserve(strLen);
        
        append(pStr, strLen);
        if (addNewLine)
            append("\n", 1);
        return true;
    }
};

using MutableString = MutableStringBase<UTF8, td::EncodingUTF8, 4>;
using MutableStringUTF16 = MutableStringBase<UTF16, td::EncodingUTF16, 4>;
using MutableStringUTF32 = MutableStringBase<UTF32, td::EncodingUTF32, 4>;

} //namespace td

