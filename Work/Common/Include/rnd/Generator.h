// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cstdlib>
#include <time.h>

namespace rnd
{

void init()
{
    srand((unsigned int)time(NULL));
}

inline int getNumber(int from, int to)
{
	double rnd01 = (double) rand() / RAND_MAX;
	int toRet =  (int) ((to - from) * rnd01 + from + 0.49999999);	
	return toRet;
}

inline double getNumber(double from, double to)
{
	double rnd01 = (double) rand() / RAND_MAX;
	return (to - from) * rnd01 + from;
}
}
