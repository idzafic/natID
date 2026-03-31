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

template <typename T>
class Stats
{
protected:
    td::String _name;
    T _m2 = 0;
    T _avg = 0;
    T _min = math::maxVal<T>();
    T _max = math::minVal<T>();
    size_t _n = 0;
    
public:
    Stats()
    {}
    
    template <typename TNAME>
    void setName(const TNAME& str)
    {
        _name = str;
    }
    
    const td::String& getName() const
    {
        return _name;
    }
    
    void insert(T x)
    {
        if (_min > x)
            _min = x;
        if (_max < x)
            _max = x;
        _n += 1;
        T newAvg = _avg + (x-_avg)/_n;
        _m2 += (x - _avg) * (x - newAvg);
        _avg = newAvg;
    }
    
    T var() const
    {
        if (_n == 0)
            return 0;
        return _m2 / _n;
    }
    
    T dev() const
    {
        if (_n == 0)
            return 0;
        if (_n == 1)
            return std::sqrt(_m2);
        
        return std::sqrt(_m2 / (_n-1));
    }
    
    T avg() const
    {
        return _avg;
    }
    
    T min() const
    {
        return _min;
    }
    
    T max() const
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
        _min = math::maxVal<T>();
        _max = math::minVal<T>();
        _n = 0;
    }
    
    template <class TSTREAM>
    void show(TSTREAM& oStr) const
    {
        oStr << "Stats";
        if (_name.length() > 0)
            oStr << " for: " << _name;
        oStr << td::endl;
        oStr << "Avg: " << _avg << td::endl;
        oStr << "Min: " << _min << td::endl;
        oStr << "Max: " << _max << td::endl;
        oStr << "Var: " << var() << td::endl;
        oStr << "Dev: " << dev() << td::endl;
        oStr << "Smp: " << _n << td::endl;
    }
};

} //namespace math

