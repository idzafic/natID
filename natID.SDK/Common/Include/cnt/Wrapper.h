// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cassert>
#include "Array.h"

namespace cnt
{

template <typename T>
class Wrapper
{
protected:
    T* _pData;
    size_t _n;
public:
    Wrapper(T* pData, size_t n)
    : _pData(pData)
    , _n(n)
    {
    }
    
    template <size_t SIZE>
    Wrapper(cnt::Array<T, SIZE>& arr)
    : _pData(arr.begin())
    , _n(SIZE)
    {
    }
    
    T& operator [] (size_t i)
    {
        assert(_n > i);
        return _pData[i];
    }
    
    const T& operator [] (size_t i) const
    {
        assert(_n > i);
        return _pData[i];
    }
    
    size_t size() const
    {
        return _n;
    }
    
    T* begin()
    {
        return _pData;
    }
    
    const T* begin() const
    {
        return _pData;
    }
    
    const T* end() const
    {
        return _pData + _n;
    }
    
    const T* cend() const
    {
        return _pData + _n;
    }
};

} //namespaced cnt
