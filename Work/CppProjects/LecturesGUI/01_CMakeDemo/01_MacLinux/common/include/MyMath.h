#pragma once

extern "C"
{
int saberiDvaBroja(int a, int b);
}

class MyMath
{
protected:
    double _currVal = 0;
public:
    MyMath(int initialVal);
    double add(double toAdd);
    double mult(double toMult);
};


