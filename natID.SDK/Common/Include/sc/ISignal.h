// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISignal.h
 *  @brief Interface for handling solver signal events such as variable limit violations.
 */
#pragma once

#include <td/Types.h>
#include <td/String.h>

namespace sc
{

/// @brief Abstract interface for receiving solver signal notifications.
class ISignal
{
public:
    /// @brief Classification of a solver signal event.
    enum class Type : unsigned char {LowLimit=0, LowMedium, Default, HighMedium, HighLimit, Rounding, None};

    /// @brief Handle a signal of the given type.
    /// @param type The signal type that occurred.
    virtual void handle(Type type) = 0;
};

/// @brief Convert a string representation of a signal type to its enum value.
/// @param pStr Pointer to a StringExt containing the signal name (e.g. "TooLow", "TooHigh").
/// @return Corresponding ISignal::Type value, or ISignal::Type::None if unrecognised.
inline ISignal::Type getSignalType(const td::StringExt* pStr)
{
    if (pStr->compareConstStr("TooLow"))
        return ISignal::Type::LowLimit;

    if (pStr->compareConstStr("TooHigh"))
        return ISignal::Type::HighLimit;

    if (pStr->compareConstStr("Rounding"))
        return ISignal::Type::Rounding;

    if (pStr->compareConstStr("MedLow"))
        return ISignal::Type::LowMedium;

    if (pStr->compareConstStr("MedHigh"))
        return ISignal::Type::HighMedium;

    if (pStr->compareConstStr("Default"))
        return ISignal::Type::Default;

    return ISignal::Type::None;
}

} //namspace sc

