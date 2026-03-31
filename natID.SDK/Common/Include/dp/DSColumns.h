// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/Column.h>
#include <dp/DSColumn.h>
#include <cnt/SafeFullVector.h>

namespace dp
{

class DSColumns
{
protected:
    cnt::SafeFullVector<dp::DSColumn>& _dsColumns;
    int _iCol;
public:
    dp::DSColumn& operator[] (size_t i)
    {
        return _dsColumns[i];
    }

    DSColumns(cnt::SafeFullVector<dp::DSColumn>& cols)
    : _dsColumns(cols)
    , _iCol(0)
    {
    }

    DSColumns& operator << (DSColumn::Type type)
    {
        if (type == DSColumn::Type::FromDP)
            _dsColumns[_iCol].fromDP = 1;
        else
        {
            _dsColumns[_iCol].fromDP = 0;
            if (type == DSColumn::Type::CalcOnLoad)
                _dsColumns[_iCol].toStore = 1;
            else
                _dsColumns[_iCol].toStore = 0;
        }
        return *this;
    }

    template <size_t size>
    DSColumns& operator << (const char(&strColumnName)[size])
    {
        _dsColumns[_iCol].name = strColumnName;
        return *this;
    }

    DSColumns& operator << (const char* strColumnName)
    {
        td::String& name = _dsColumns[_iCol].name;
        name = strColumnName;
        return *this;
    }

    DSColumns& operator << (td::Variant& var)
    {
        dp::Column col;
        col.init(var);
        _dsColumns[_iCol].pExtData = col.pData;
        _dsColumns[_iCol++].type = col.cppType;
        return *this;
    }
    
    DSColumns& operator << (td::DataType dataType)
    {
        _dsColumns[_iCol].pExtData = nullptr;
        _dsColumns[_iCol++].type = dataType;
        return *this;
    }

    template <typename T>
    DSColumns& operator << (T& val)
    {
        dp::Column col;
        col.init(val);
        _dsColumns[_iCol].pExtData = col.pData;
        _dsColumns[_iCol++].type = col.cppType;
        return *this;
    }
};

} //namespace dp
