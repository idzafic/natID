// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include "Limits.h"

namespace math
{

class Stats
{
protected:
    td::String _name;
    double _m2 = 0;
    double _avg = 0;
    double _min = math::maxVal<double>();
    double _max = math::minVal<double>();
    size_t _n = 0;
    
public:
    Stats()
    {}
    
    template <typename T>
    void setName(const T& str)
    {
        _name = str;
    }
    
    const td::String& getName() const
    {
        return _name;
    }
    
    void insert(double x)
    {
        if (_min > x)
            _min = x;
        if (_max < x)
            _max = x;
        _n += 1;
        double newAvg = _avg + (x-_avg)/_n;
        _m2 += (x - _avg) * (x - newAvg);
        _avg = newAvg;
    }
    
    double var() const
    {
        if (_n == 0)
            return 0;
        return _m2 / _n;
    }
    
    double dev() const
    {
        if (_n == 0)
            return 0;
        if (_n == 1)
            return std::sqrt(_m2);
        
        return std::sqrt(_m2 / (_n-1));
    }
    
    double avg() const
    {
        return _avg;
    }
    
    double min() const
    {
        return _min;
    }
    
    double max() const
    {
        return _max;
    }
    
    size_t getNoOfEntries() const
    {
        return _n;
    }
    
    void reset()
    {
        _m2 = 0;
        _avg = 0;
        _min = math::maxVal<double>();
        _max = math::minVal<double>();
        _n = 0;
    }
};

} //namespace math

