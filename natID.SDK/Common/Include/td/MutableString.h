// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MutableString.h
    @brief Mutable, heap-owning string class derived from StringBase with append and line-reading utilities. */
#pragma once
#include "String.h"

namespace td
{

/// @brief A mutable string that owns its buffer and supports in-place append, reserve, and line-extraction operations.
/// @tparam T_CHAR     Character element type (e.g. UTF8, UTF16, UTF32).
/// @tparam Encoding   String encoding tag.
/// @tparam SPACE_FOR_SIZE Inline space used by the base class to store the length.
template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>
class MutableStringBase : protected StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>
{
protected:
    using BASE = StringBase<T_CHAR, Encoding, SPACE_FOR_SIZE, false>; ///< Alias for the base string type.
    using THISOBJ = MutableStringBase<T_CHAR, Encoding, SPACE_FOR_SIZE>; ///< Alias for this type.

    size_t _capacity = 0; ///< Allocated buffer capacity in character units, including the trailing null.
public:
    /// @brief Default constructor. Creates an empty string with no allocated buffer.
    MutableStringBase()
    : BASE()
    {
    }

    // Move constructor
    /// @brief Move constructor. Takes ownership of the buffer from str.
    /// @param str The string to move from. Its buffer pointer is set to null after the move.
    inline MutableStringBase(MutableStringBase&& str) noexcept
    {
        BASE::buffer = str.buffer;
        str.buffer = nullptr;
    }

    // Move assignment operator
    /// @brief Move assignment operator. Transfers buffer ownership from str to this object.
    /// @param str The string to move from.
    /// @return Reference to this object.
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

    /// @brief Destructor. Frees the owned buffer.
    ~MutableStringBase()
    {
        if (BASE::buffer)
        {
            delete[] BASE::buffer;
            BASE::buffer = nullptr;
        }
    }

    /// @brief Returns a non-const reference to the underlying base string.
    /// @return Mutable reference to the base StringBase.
    BASE& getString()
    {
        return *this;
    }

    /// @brief Returns a const reference to the underlying base string.
    /// @return Const reference to the base StringBase.
    const BASE& getString() const
    {
        return *this;
    }

    /// @brief Creates and returns an independent (cloned) immutable copy of this string.
    /// @return A new StringBase that owns a copy of the current content.
    [[nodiscard]] BASE getImmutableString() const
    {
        return BASE::clone();
    }

    //includes trailing zero
    /// @brief Returns the current buffer capacity, including the trailing null terminator.
    /// @return Capacity in character units.
    size_t getCapacity() const
    {
        return _capacity;
    }

    /// @brief Returns the current buffer capacity (alias for getCapacity).
    /// @return Capacity in character units.
    size_t capacity() const
    {
        return _capacity;
    }

    /// @brief Returns a pointer to the null-terminated character data.
    /// @return Pointer to the internal buffer, or nullptr if empty.
    const T_CHAR* c_str() const
    {
        return BASE::c_str();
    }

    /// @brief Returns the logical length of the string in Unicode code points.
    /// @return Number of code points (not bytes).
    size_t length() const
    {
        return BASE::length();
    }

    /// @brief Returns the length of the string in code units (e.g. bytes for UTF-8).
    /// @return Number of code units.
    size_t unitLength() const
    {
        return BASE::unitLength();
    }

    /// @brief Ensures the buffer can hold at least the given number of code units (plus a null terminator), then resets the string to empty.
    /// @param capacity Desired capacity in code units, including space for the trailing null.
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

    /// @brief Appends a formatted string (printf-style) to the current content.
    /// @param format printf-style format string.
    /// @param ...    Variadic arguments matching the format.
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


    /// @brief Resets the string length to zero and null-terminates the buffer (does not free memory).
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

    /// @brief Resets the string to empty without releasing the buffer (alias for resetLen).
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

    /// @brief Appends raw character data of the given length to the current content.
    /// @param pStr       Pointer to the characters to append.
    /// @param inputUnitLen Number of code units to append; 0 means compute strlen.
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

    /// @brief Appends a single character to the current content.
    /// @param ch The character to append.
    void append(T_CHAR ch)
    {
        append(&ch, 1);
    }

    /// @brief Appends the content of a td::String.
    /// @param str The string to append.
    void append(const td::String& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }

    /// @brief Appends the content of a td::StringExt.
    /// @param str The string to append.
    void append(const td::StringExt& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }

    /// @brief Appends a null-terminated C-string.
    /// @param pStr The C-string to append.
    void operator += (const char* pStr)
    {
        auto strLen = strlen(pStr);
        append(pStr, strLen);
    }

    /// @brief Appends a td::String.
    /// @param str The string to append.
    void operator += (const td::String& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }

    /// @brief Appends a td::StringExt.
    /// @param str The string to append.
    void operator += (const td::StringExt& str)
    {
        auto strLen = str.length();
        append(str.c_str(), strLen);
    }

    //fromPosition is input/output argument
    /// @brief Reads one line from fromString starting at fromPosition and stores it in this object.
    /// @param fromString   The source string to read from.
    /// @param fromPosition On input: the offset to start reading from. On output: the offset after the consumed line (including the newline).
    /// @param addNewLine   If true, a newline character is appended to the extracted line.
    /// @return True if a line was extracted, false if fromPosition was already at or past the end.
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

using MutableString = MutableStringBase<UTF8, td::EncodingUTF8, 4>;       ///< UTF-8 mutable string alias.
using MutableStringUTF16 = MutableStringBase<UTF16, td::EncodingUTF16, 4>; ///< UTF-16 mutable string alias.
using MutableStringUTF32 = MutableStringBase<UTF32, td::EncodingUTF32, 4>; ///< UTF-32 mutable string alias.

} //namespace td
