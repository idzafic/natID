#pragma once
#include "MyMathLib.h"

MYMATH_SHLIB_API int saberiDvaBroja(int a, int b);

class MYMATH_SHLIB_API MyMath
{
protected:
    double _currVal = 0;
public:
    MyMath(int initialVal);
    double add(double toAdd);
    double mult(double toMult);
};


