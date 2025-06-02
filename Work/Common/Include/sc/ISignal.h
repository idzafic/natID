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
#include <td/String.h>

namespace sc
{

class ISignal
{
public:
    enum class Type : unsigned char {LowLimit=0, HighLimit, Rounding, None};

    virtual void handle(Type type) = 0;
};

inline ISignal::Type getSignalType(const td::StringExt* pStr)
{
    if (pStr->compareConstStr("TooLow"))
        return ISignal::Type::LowLimit;
    
    if (pStr->compareConstStr("TooHigh"))
        return ISignal::Type::HighLimit;
    
    if (pStr->compareConstStr("Rounding"))
        return ISignal::Type::Rounding;
    
    return ISignal::Type::None;
}

} //namspace sc

