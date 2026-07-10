// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Generator.h
 *  @brief Simple random number generation utilities using the C standard library rand() function.
 */
#pragma once
#include <cstdlib>
#include <time.h>

namespace rnd
{

/// @brief Seeds the C standard library random number generator with the current time.
void init()
{
    srand((unsigned int)time(NULL));
}

/// @brief Returns a random integer in the closed interval [from, to].
/// @param from Lower bound of the desired range (inclusive).
/// @param to   Upper bound of the desired range (inclusive).
/// @return A random integer value between from and to.
inline int getNumber(int from, int to)
{
	double rnd01 = (double) rand() / RAND_MAX;
	int toRet =  (int) ((to - from) * rnd01 + from + 0.49999999);
	return toRet;
}

/// @brief Returns a random double in the interval [from, to].
/// @param from Lower bound of the desired range.
/// @param to   Upper bound of the desired range.
/// @return A random double value between from and to.
inline double getNumber(double from, double to)
{
	double rnd01 = (double) rand() / RAND_MAX;
	return (to - from) * rnd01 + from;
}
}
