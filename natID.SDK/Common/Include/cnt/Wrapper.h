// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Wrapper.h
 @brief Non-owning view over a raw array or cnt::Array, providing iterator and index access. */
#pragma once
#include <cassert>
#include "Array.h"

namespace cnt
{

/// @brief Non-owning wrapper that provides array-like access to an externally-managed buffer.
/// @tparam T Element type pointed to by the wrapped buffer.
template <typename T>
class Wrapper
{
protected:
    T* _pData; ///< Pointer to the first element of the wrapped buffer.
    size_t _n; ///< Number of elements in the wrapped buffer.
public:
    /// @brief Constructs a Wrapper from a raw pointer and element count.
    /// @param pData Pointer to the first element.
    /// @param n     Number of elements accessible through pData.
    Wrapper(T* pData, size_t n)
    : _pData(pData)
    , _n(n)
    {
    }

    /// @brief Constructs a Wrapper from a cnt::Array.
    /// @tparam SIZE Compile-time size of the source array.
    /// @param  arr  The source array to wrap.
    template <size_t SIZE>
    Wrapper(cnt::Array<T, SIZE>& arr)
    : _pData(arr.begin())
    , _n(SIZE)
    {
    }

    /// @brief Provides mutable indexed access to elements.
    /// @param i Zero-based index; must be less than size().
    /// @return Reference to the element at index i.
    T& operator [] (size_t i)
    {
        assert(_n > i);
        return _pData[i];
    }

    /// @brief Provides const indexed access to elements.
    /// @param i Zero-based index; must be less than size().
    /// @return Const reference to the element at index i.
    const T& operator [] (size_t i) const
    {
        assert(_n > i);
        return _pData[i];
    }

    /// @brief Returns the number of elements accessible through this Wrapper.
    /// @return Element count.
    size_t size() const
    {
        return _n;
    }

    /// @brief Returns a pointer to the first element.
    /// @return Pointer to the first element.
    T* begin()
    {
        return _pData;
    }

    /// @brief Returns a const pointer to the first element.
    /// @return Const pointer to the first element.
    const T* begin() const
    {
        return _pData;
    }

    /// @brief Returns a const pointer one past the last element.
    /// @return Const pointer to one past the last element.
    const T* end() const
    {
        return _pData + _n;
    }

    /// @brief Returns a const pointer one past the last element.
    /// @return Const pointer to one past the last element.
    const T* cend() const
    {
        return _pData + _n;
    }
};

} //namespaced cnt
