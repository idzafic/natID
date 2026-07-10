// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file EnumSerializer.h
    @brief Provides helper classes for loading and saving enumeration values in binary archives. */
#pragma once
#include <td/Types.h>

namespace arch
{

/// @brief Helper class for safely loading an enumeration value from a binary archive.
/// @tparam T The enumeration type to deserialize.
template <typename T>
class EnumLoader
{
protected:
    T& _val;            ///< Reference to the enum variable that will receive the loaded value.
    T _maxVal;          ///< Maximum valid enum value; values above this use the default.
    T _defaultVal;      ///< Default value assigned when the loaded byte exceeds the maximum.
    td::BYTE _exception = 0; ///< If non-zero, out-of-range values cause loadFromBinaryStream to return false.

public:
    /// @brief Constructs an EnumLoader with range checking parameters.
    /// @param val Reference to the enum variable to populate.
    /// @param maxVal The largest valid value for the enumeration.
    /// @param defaultVal Value to assign when the stored byte is out of range.
    /// @param bRaiseEception If true, out-of-range values cause the load to fail; otherwise the default is silently used.
    EnumLoader(T& val, T maxVal, T defaultVal, bool bRaiseEception = false)
    : _val(val)
    , _maxVal(maxVal)
    , _defaultVal(defaultVal)
    {
        if (bRaiseEception)
            _exception = 1;
    }

    /// @brief Reads one byte from the archive and converts it to the enum value.
    /// @tparam TArch Type of the input archive.
    /// @param ar The binary input archive to read from.
    /// @return True if the loaded value is within range; false if out-of-range and exceptions are enabled.
    template<class TArch>
    bool loadFromBinaryStream(TArch& ar)
    {
        td::BYTE byteVal = 0; //(td::BYTE) _val;
        ar >> byteVal;

        if (byteVal > (td::BYTE) _maxVal)
        {
            _val = _defaultVal;
            if (_exception)
                return false;
            return true;
        }
        _val = (T) byteVal;
        return true;
    }
};


/// @brief Helper class for saving an enumeration value as a single byte to a binary archive.
/// @tparam T The enumeration type to serialize.
template <typename T>
class EnumSaver
{
protected:
    T _val; ///< The enum value to be serialized.

public:
    /// @brief Constructs an EnumSaver with the enum value to write.
    /// @param val The enumeration value to serialize.
    EnumSaver(T val)
    : _val(val)
    {
    }

    /// @brief Writes the enum value as a single byte to the archive.
    /// @tparam TArch Type of the output archive.
    /// @param ar The binary output archive to write to.
    template<class TArch>
    void saveToBinaryStream(TArch& ar) const
    {
        td::BYTE byteVal = (td::BYTE) _val;
        ar << byteVal;
    }
};

}
