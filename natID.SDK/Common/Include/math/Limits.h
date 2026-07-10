// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Limits.h
 * @brief Template helpers for querying numeric type limits.
 */
#pragma once
#include <td/Types.h>
#include <limits>
#include <compiler/MinMaxFix.h>

namespace math
{

/// @brief Returns the maximum finite value representable by type T.
/// @return Maximum value of type T.
template <typename T>
inline constexpr T maxVal()
{
    return std::numeric_limits<T>::max();
}

/// @brief Returns the lowest (most negative) finite value representable by type T.
/// @return Lowest value of type T.
template <typename T>
inline constexpr T minVal()
{
    return std::numeric_limits<T>::lowest();
}

/// @brief Returns the smallest positive value representable by type T (machine epsilon for floats).
/// @return Minimum positive value of type T.
template <typename T>
inline constexpr T minEps()
{
    return std::numeric_limits<T>::min();
}

};
