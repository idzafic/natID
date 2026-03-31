#include "../../common/include/MyMath.h"
#include "NeVidimSe.h"

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

NeVidimSe::NeVidimSe()
{
    
}
double NeVidimSe::getSomething()
{
    return 9.81;
}
