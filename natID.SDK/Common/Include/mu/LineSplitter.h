// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LineSplitter.h
    @brief Provides a template class for splitting character lines by separator characters. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/MutableString.h>
#include <fstream>
#include <mu/muLib.h>

namespace mu
{

/// @brief Template base class that splits a null-terminated character line into parts using configurable separators.
/// @tparam T_CHAR Character type used for the line and separator strings.
template <typename T_CHAR>
class LineSplitterBase
{
private:
    const T_CHAR* _separators = nullptr; ///< Null-terminated list of separator characters.
    const T_CHAR* _line = nullptr; ///< Pointer to the current line being split.
    td::MutableString _mStr; ///< Internal buffer for the last extracted part.
    td::UINT4 _lineLen = 0; ///< Length of the current line in characters.
    int _startPosition = 0;    //pos ///< Current scan position within the line.

    /// @brief Checks whether the given character is one of the configured separators.
    /// @param ch Character to test.
    /// @return true if ch is a separator, false otherwise.
    bool isSeparator(T_CHAR ch) const
    {
        const char* ptr = _separators;
        while (ptr && (*ptr != 0) )
        {
            if (ch == *ptr)
                return true;
            ++ptr;
        }
        return false;
    }

    /// @brief Finds the index of the next separator at or after _startPosition.
    /// @return Index of the separator, or -1 if no separator was found.
    inline int findNextSeparator() const
    {
        for (td::UINT4 i = _startPosition; i < _lineLen; ++i)
        {
            T_CHAR myCh = _line[i];
            if (isSeparator(myCh))
                return i;
        }
        return -1;
    }

    /// @brief Counts the total number of separator characters in the current line.
    /// @return The count of separator occurrences.
    inline td::UINT4 getSepCounts() const
    {
        td::UINT4 toRet = 0;
        for (td::UINT4 i = 0; i < _lineLen; ++i)
        {
            if (isSeparator(_line[i]))
                ++toRet;
        }
        return toRet;
    }

    /// @brief Returns the position of the first non-whitespace character starting at _startPosition.
    /// @return Index of the first non-whitespace character, or -1 if none found.
    int getFirstNonWhiteSpacePosition() const
    {
        if (_startPosition >= _lineLen)
            return -1;

        for (td::UINT4 pos = _startPosition; pos<_lineLen; ++pos)
        {
            auto ch = _line[pos];
            if (!td::isWhiteSpace(ch))
                return pos;
        }
        return -1;
    }

    LineSplitterBase() = delete;
public:

    /// @brief Constructs a LineSplitterBase with the given separator characters.
    /// @param separators Null-terminated string containing all separator characters.
    /// @param buffLen Initial reservation size for the internal output buffer.
    LineSplitterBase(const char* separators, size_t buffLen = 128)
    : _separators(separators)
    {
        _mStr.reserve(buffLen);
    }

    /// @brief Sets the line to be split and resets the scan position.
    /// @param line Pointer to the null-terminated line to split.
    /// @return true if the line is non-empty and ready to split, false otherwise.
    bool setLine(const T_CHAR* line)
    {
        _line = line;

        if (!_line)
            _lineLen = 0;
        else
            _lineLen = td::UINT4(std::strlen(_line));

        _startPosition = 0;    //pos
        _mStr.reset();
        if (_lineLen == 0)
            return false;
        return true;
    }

    /// @brief Advances the scan position to the first non-whitespace character.
    /// @return Pointer to the first non-whitespace character, or nullptr if none.
    const T_CHAR* findFirstNonWhiteSpace()
    {
        _startPosition = getFirstNonWhiteSpacePosition();
        if (_startPosition < 0)
        {
            _startPosition = _lineLen;
            return nullptr;
        }
        return _line + _startPosition;
    }

    /// @brief Extracts the next whitespace-trimmed part delimited by a separator character.
    /// @return Pointer to the extracted part as a C string, or nullptr when the line is exhausted.
    const char* getNextPart()
    {
        _mStr.reset();

        if (_startPosition >= _lineLen)
            return nullptr;

        _startPosition = getFirstNonWhiteSpacePosition();
        if (_startPosition < 0)
        {
            return 0;
        }

        int iPos2 = findNextSeparator();

        if (iPos2 > _startPosition)
        {
            _mStr.append(_line+_startPosition, iPos2 - _startPosition);
            _startPosition = iPos2 + 1;
            return _mStr.c_str(); //str = subStr(iPos, iPos2 - 1);
        }
        else if (iPos2 < 0)
        {
            _mStr.append(_line+_startPosition, _lineLen - _startPosition);
            _startPosition = _lineLen+1;
            return _mStr.c_str(); //str = subStr(iPos, -1);
        }

        assert(false);
        return nullptr;
    }

    /// @brief Returns the character length of the most recently extracted part.
    /// @return Length of the last split token.
    auto getSplitLen() const
    {
        return _mStr.length();
    }

};

/// @brief Convenience alias for LineSplitterBase using plain char.
using LineSplitter = LineSplitterBase<char>;
} //namespace mu
