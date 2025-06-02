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
#include <limits>
#include <compiler/MinMaxFix.h>

namespace math
{

template <typename T>
inline constexpr T maxVal()
{
    return std::numeric_limits<T>::max();
}

template <typename T>
inline constexpr T minVal()
{
    return std::numeric_limits<T>::lowest();
}

template <typename T>
inline constexpr T minEps()
{
    return std::numeric_limits<T>::min();
}

};
