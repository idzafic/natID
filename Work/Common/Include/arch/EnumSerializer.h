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

namespace arch
{
    
template <typename T>
class EnumLoader
{
protected:
    T& _val;
    T _maxVal;
    T _defaultVal;
    td::BYTE _exception = 0;
    
public:
    EnumLoader(T& val, T maxVal, T defaultVal, bool bRaiseEception = false)
    : _val(val)
    , _maxVal(maxVal)
    , _defaultVal(defaultVal)
    {
        if (bRaiseEception)
            _exception = 1;
    }
    
    template<class TArch>
    bool loadFromBinaryStream(TArch& ar)
    {
        td::BYTE byteVal = 0; //(td::BYTE) _val;
        ar >> byteVal;
        
        if (byteVal > (td::BYTE) _maxVal)
        {
            _val = _defaultVal;
            if (_exception)
                return false;
            return true;
        }
        _val = (T) byteVal;
        return true;
    }
};


template <typename T>
class EnumSaver
{
protected:
    T _val;
    
public:
    EnumSaver(T val)
    : _val(val)
    {
    }
        
    template<class TArch>
    void saveToBinaryStream(TArch& ar) const
    {
        td::BYTE byteVal = (td::BYTE) _val;
        ar << byteVal;
    }
};

}
