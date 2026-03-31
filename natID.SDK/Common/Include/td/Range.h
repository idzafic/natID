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

namespace td
{

template <typename T>
class Range
{
public:
    T location;
    T length;
    Range()
    : location(0)
    , length(0)
    {}
    
    Range(const Range<T>& range)
    : location(range.location)
    , length(range.length)
    {}

    Range(T locIn, T lengthIn)
    : location(locIn)
    , length(lengthIn)
    {}
    
    void operator = (const Range& range)
    {
        location = range.location;
        length = range.length;
    };
    
    void c_str(char* buff, int buffLen) const
    {
        if (SNPRINTF(buff, buffLen, _TRUNCATE, "location=\"%g\" length=\"%g\"", location, length) < 0)
            buff[buffLen - 1] = 0;
    }
    
    template <class TOSTREAM>
    void show(TOSTREAM& o) const
    {
        o << "(" << location << "," << length << ")";
    }
    
    template<class Archive>
    bool loadFromBinaryStream(Archive& ar)
    {
        ar >> location >> length;
        return true;
    }
    
    template<class Archive>
    void saveToBinaryStream(Archive& ar) const
    {
        ar << location << length;
    }
    
    void operator -= (T delta)
    {
        location -= delta;
    }
    
    void operator += (T delta)
    {
        location += delta;
    }
    
    void translate(T delta)
    {
        location += delta;
    }
    
    void scale(T delta)
    {
        location *= delta;
    }
};

} //namespace td
