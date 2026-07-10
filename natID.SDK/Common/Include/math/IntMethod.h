// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IntMethod.h
 * @brief Enumeration and utilities for numerical integration methods.
 */
#pragma once
#include <td/Types.h>

namespace math
{
/// @brief Enumeration of supported numerical integration methods.
enum class IntMethod : unsigned char
{
    Euler=0,       ///< Explicit Euler (first-order, non-iterative).
    Trapezoidal,   ///< Implicit trapezoidal (second-order, iterative).
    RK4,           ///< Classical Runge-Kutta 4 (two iterative sub-stages).
    RK6,           ///< Runge-Kutta 6 (three iterative sub-stages).
    Gauss2s4,      ///< Gauss-Legendre 2-stage, order 4 (implicit, iterative).
    Gauss3s6,      ///< Gauss-Legendre 3-stage, order 6 (implicit, iterative).
    Radau1A2s3     ///< Radau IA 2-stage, order 3 (implicit, iterative).
};

/// @brief Returns the number of iterative stages required by the given integration method.
/// @param method The integration method to query.
/// @return Number of iterative stages; 0 for methods that do not use iterative stages.
inline td::UINT2 getNoOfIterativeStages(math::IntMethod method)
{
    switch(method)
    {
        case math::IntMethod::RK4:
            return 2;
        case math::IntMethod::RK6:
            return 3;
        default:
            return 0;
    }
    return 0;
}

}
