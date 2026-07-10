// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DSColumn.h
 * @brief Defines the DSColumn structure describing a single column in a data-set binding.
 */
#pragma once
#include <td/String.h>
#include <dp/Column.h>

namespace dp
{

/// @brief Describes a single column in a data set, including its name, type, origin, and storage intent.
class DSColumn
{
public:
    /// @brief Indicates how the column value is produced.
    enum class Type {FromDP,       ///< Value is fetched directly from the data provider.
                     CalcOnLoad,   ///< Value is calculated when the data set is loaded and stored.
                     CalcOnConsume ///< Value is calculated on demand and not stored.
                    };

    td::String name;              ///< Name of the column.
    void* pExtData = nullptr;     ///< Pointer to the bound external data variable.
    td::DataType type = td::DataType::TD_NONE; ///< C++ data type of this column.
    char fromDP = 1;              ///< Non-zero if the value comes from the data provider.
    char toStore = 1;             ///< Non-zero if the value should be stored in the data set.

    /// @brief Assigns column metadata from another DSColumn (does not copy the name allocation).
    /// @param b The source DSColumn to copy from.
    void operator = (const DSColumn& b)
    {
        pExtData = b.pExtData;
        name = b.name;
        type = b.type;
        fromDP = b.fromDP;
        toStore = b.toStore;
    }

    /// @brief Returns whether this column's value should be stored in the data set.
    /// @return True if the column value is meant to be stored.
    bool shouldStore() const
    {
        return toStore != 0;
    }

};

} // namespace dp
