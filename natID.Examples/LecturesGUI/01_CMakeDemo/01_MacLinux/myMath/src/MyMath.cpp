#include "../../common/include/MyMath.h"

MyMath::MyMath(int initialVal)
: _currVal(initialVal)
{
    
}

double MyMath::add(double toAdd)
{
    _currVal += toAdd;
    return _currVal;
}

double MyMath::mult(double toMult)
{
    _currVal *= toMult;
    return _currVal;
}
