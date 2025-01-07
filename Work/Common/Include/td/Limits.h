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
#include <compiler/MinMaxFix.h>
#include <limits>


namespace td
{

template <typename T>
inline T getMinValue(T)
{
    return std::numeric_limits<T>::lowest();
}

template <typename T>
inline T getDeltaValue(T)
{
    return std::numeric_limits<T>::min();
}

template <typename T>
inline T getMaxValue(T)
{
    return std::numeric_limits<T>::max();
}

template <typename T>
inline T getMinValue()
{
    return std::numeric_limits<T>::lowest();
}

template <typename T>
inline T getDeltaValue()
{
    return std::numeric_limits<T>::min();
}

template <typename T>
inline T getMaxValue()
{
    return std::numeric_limits<T>::max();
}

} //namespace td

	
