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
#include "ISolutionBuffer.h"
#include "ISignal.h"
#include <complex>
#include <math/IntMethod.h>

namespace sc
{

class ITime
{
public:
    virtual void setTimeMethod(math::IntMethod m) = 0;
    virtual void setTimes(double t0, double dT) = 0;
};

} //namspace sc

