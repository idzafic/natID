// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

template <typename T_CHAR>
class LineSplitterBase
{
private:
    const T_CHAR* _separators = nullptr;
    const T_CHAR* _line = nullptr;
    td::MutableString _mStr;
    td::UINT4 _lineLen = 0;
    int _startPosition = 0;    //pos

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
    
    LineSplitterBase(const char* separators, size_t buffLen = 128)
    : _separators(separators)
    {
        _mStr.reserve(buffLen);
    }
    
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
    
    auto getSplitLen() const
    {
        return _mStr.length();
    }

};

using LineSplitter = LineSplitterBase<char>;
} //namespace mu
