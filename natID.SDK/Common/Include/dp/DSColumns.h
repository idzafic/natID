// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DSColumns.h
 * @brief Provides a stream-like builder interface for populating a vector of DSColumn descriptors.
 */
#pragma once
#include <dp/Column.h>
#include <dp/DSColumn.h>
#include <cnt/SafeFullVector.h>

namespace dp
{

/// @brief Stream-like helper for configuring a vector of DSColumn objects sequentially.
class DSColumns
{
protected:
    cnt::SafeFullVector<dp::DSColumn>& _dsColumns; ///< Reference to the underlying DSColumn storage.
    int _iCol;                                      ///< Index of the column currently being configured.
public:
    /// @brief Accesses the DSColumn at the given index.
    /// @param i Zero-based column index.
    /// @return Reference to the requested DSColumn.
    dp::DSColumn& operator[] (size_t i)
    {
        return _dsColumns[i];
    }

    /// @brief Constructs a DSColumns wrapper over the given DSColumn storage vector.
    /// @param cols Reference to the vector of DSColumn objects to wrap.
    DSColumns(cnt::SafeFullVector<dp::DSColumn>& cols)
    : _dsColumns(cols)
    , _iCol(0)
    {
    }

    /// @brief Sets the origin type of the current column and advances the cursor if not FromDP.
    /// @param type The DSColumn::Type value indicating how the column value is produced.
    /// @return Reference to this DSColumns object for chaining.
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

    /// @brief Sets the name of the current column from a string literal.
    /// @tparam size Length of the literal.
    /// @param strColumnName The column name.
    /// @return Reference to this DSColumns object for chaining.
    template <size_t size>
    DSColumns& operator << (const char(&strColumnName)[size])
    {
        _dsColumns[_iCol].name = strColumnName;
        return *this;
    }

    /// @brief Sets the name of the current column from a null-terminated C string.
    /// @param strColumnName Null-terminated column name.
    /// @return Reference to this DSColumns object for chaining.
    DSColumns& operator << (const char* strColumnName)
    {
        td::String& name = _dsColumns[_iCol].name;
        name = strColumnName;
        return *this;
    }

    /// @brief Binds the current column to a Variant value and advances the cursor.
    /// @param var The Variant variable to bind.
    /// @return Reference to this DSColumns object for chaining.
    DSColumns& operator << (td::Variant& var)
    {
        dp::Column col;
        col.init(var);
        _dsColumns[_iCol].pExtData = col.pData;
        _dsColumns[_iCol++].type = col.cppType;
        return *this;
    }

    /// @brief Sets the data type of the current column without binding an external pointer, and advances the cursor.
    /// @param dataType The td::DataType to assign to the current column.
    /// @return Reference to this DSColumns object for chaining.
    DSColumns& operator << (td::DataType dataType)
    {
        _dsColumns[_iCol].pExtData = nullptr;
        _dsColumns[_iCol++].type = dataType;
        return *this;
    }

    /// @brief Binds the current column to an arbitrary typed variable and advances the cursor.
    /// @tparam T Type of the variable.
    /// @param val The variable to bind.
    /// @return Reference to this DSColumns object for chaining.
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
