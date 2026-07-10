// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Limits.h
    @brief Utility function templates for querying the minimum, maximum, and smallest-positive values of arithmetic types. */
#pragma once
#include <td/Types.h>
#include <compiler/MinMaxFix.h>
#include <limits>


namespace td
{

/// @brief Returns the lowest (most negative) representable value of type T.
/// @tparam T An arithmetic type.
/// @param unused An instance of T used only for type deduction.
/// @return The lowest representable value.
template <typename T>
inline T getMinValue(T)
{
    return std::numeric_limits<T>::lowest();
}

/// @brief Returns the smallest positive value of type T (for floating-point types this is the minimum normal positive value).
/// @tparam T An arithmetic type.
/// @param unused An instance of T used only for type deduction.
/// @return The smallest positive representable value.
template <typename T>
inline T getDeltaValue(T)
{
    return std::numeric_limits<T>::min();
}

/// @brief Returns the largest representable value of type T.
/// @tparam T An arithmetic type.
/// @param unused An instance of T used only for type deduction.
/// @return The maximum representable value.
template <typename T>
inline T getMaxValue(T)
{
    return std::numeric_limits<T>::max();
}

/// @brief Returns the lowest (most negative) representable value of type T (no-argument form).
/// @tparam T An arithmetic type.
/// @return The lowest representable value.
template <typename T>
inline T getMinValue()
{
    return std::numeric_limits<T>::lowest();
}

/// @brief Returns the smallest positive value of type T (no-argument form).
/// @tparam T An arithmetic type.
/// @return The smallest positive representable value.
template <typename T>
inline T getDeltaValue()
{
    return std::numeric_limits<T>::min();
}

/// @brief Returns the largest representable value of type T (no-argument form).
/// @tparam T An arithmetic type.
/// @return The maximum representable value.
template <typename T>
inline T getMaxValue()
{
    return std::numeric_limits<T>::max();
}

} //namespace td


