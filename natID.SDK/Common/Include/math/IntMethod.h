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

namespace math
{
	enum class IntMethod : unsigned char {Euler=0, Trapezoidal, RK4, RK6, Gauss2s4, Gauss3s6, Radau1A2s3};

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
