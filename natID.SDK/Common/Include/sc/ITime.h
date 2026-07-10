// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ITime.h
 *  @brief Interface for configuring the time-stepping method used by dynamic solvers.
 */
#pragma once

#include <td/Types.h>
#include <td/String.h>
#include "ISolutionBuffer.h"
#include "ISignal.h"
#include <complex>
#include <math/IntMethod.h>

namespace sc
{

/// @brief Abstract interface for setting integration time parameters on a dynamic solver.
class ITime
{
public:
    /// @brief Select the numerical integration method (e.g. Euler, Runge-Kutta).
    /// @param m The integration method to apply.
    virtual void setTimeMethod(math::IntMethod m) = 0;
    /// @brief Set the start time and step size for the integrator.
    /// @param t0 Start time.
    /// @param dT Integration step size.
    virtual void setTimes(double t0, double dT) = 0;
};

} //namspace sc

