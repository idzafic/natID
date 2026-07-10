// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <array>

namespace cnt
{

template<typename T, size_t N>
class FixedVector
{
    std::array<T, N> _data;
    size_t _size = 0;

public:
    void clear()
    {
        _size = 0;
    }

    size_t size() const
    {
        return _size;
    }

    constexpr size_t capacity() const
    {
        return N;
    }

    bool empty() const
    {
        return _size == 0;
    }

    T& push_back(const T& value)
    {
        assert(_size < N);
        _data[_size] = value;
        return _data[_size++];
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        assert(_size < N);

        T& item = _data[_size++];
        item = T(std::forward<Args>(args)...);

        return item;
    }

    T& back()
    {
        return _data[_size - 1];
    }

    T* data()
    {
        return _data.data();
    }

    const T* data() const
    {
        return _data.data();
    }

    auto begin()
    {
        return _data.begin();
    }

    auto end()
    {
        return _data.begin() + _size;
    }

    auto begin() const
    {
        return _data.begin();
    }

    auto end() const
    {
        return _data.begin() + _size;
    }

    T& operator[](size_t idx)
    {
        return _data[idx];
    }

    const T& operator[](size_t idx) const
    {
        return _data[idx];
    }
};

} //namespace cnt
