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
#include <cstring>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace cnt
{

/**
 * @brief Vector with inline storage and heap fallback.
 *
 * Stores the first InlineCapacity elements directly inside
 * the object. Heap allocation occurs only when the size
 * exceeds InlineCapacity.
 *
 * Similar to LLVM SmallVector.
 *
 * @tparam T Element type.
 * @tparam InlineCapacity Inline storage capacity.
 */
template<typename T, size_t InlineCapacity>
class SmallVector
{
private:

    using Storage =
        std::aligned_storage_t<sizeof(T), alignof(T)>;

    Storage _inlineStorage[InlineCapacity];

    T* _data;
    size_t _size;
    size_t _capacity;

    T* inlineData()
    {
        return reinterpret_cast<T*>(_inlineStorage);
    }

    const T* inlineData() const
    {
        return reinterpret_cast<const T*>(_inlineStorage);
    }

    bool usingInlineStorage() const
    {
        return _data == inlineData();
    }

    void destroyElements()
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (size_t i = 0; i < _size; ++i)
                _data[i].~T();
        }
    }

    void grow(size_t newCapacity)
    {
        T* newData =
            static_cast<T*>(operator new(sizeof(T) * newCapacity));

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memcpy(
                newData,
                _data,
                _size * sizeof(T));
        }
        else
        {
            for (size_t i = 0; i < _size; ++i)
            {
                new (newData + i)
                    T(std::move(_data[i]));
            }

            destroyElements();
        }

        if (!usingInlineStorage())
            operator delete(_data);

        _data = newData;
        _capacity = newCapacity;
    }

    void moveFrom(SmallVector&& rhs)
    {
        if (!rhs.usingInlineStorage())
        {
            _data = rhs._data;
            _size = rhs._size;
            _capacity = rhs._capacity;

            rhs._data = rhs.inlineData();
            rhs._size = 0;
            rhs._capacity = InlineCapacity;
        }
        else
        {
            reserve(rhs._size);

            if constexpr (std::is_trivially_copyable_v<T>)
            {
                std::memcpy(
                    _data,
                    rhs._data,
                    rhs._size * sizeof(T));

                _size = rhs._size;
                rhs._size = 0;
            }
            else
            {
                for (size_t i = 0; i < rhs._size; ++i)
                    emplace_back(std::move(rhs._data[i]));

                rhs.clear();
            }
        }
    }

public:

    using value_type = T;
    using size_type = size_t;
    using iterator = T*;
    using const_iterator = const T*;

    /**
     * @brief Constructs an empty vector.
     */
    SmallVector()
        : _data(inlineData())
        , _size(0)
        , _capacity(InlineCapacity)
    {
    }

    /**
     * @brief Copy constructor.
     */
    SmallVector(const SmallVector& rhs)
        : SmallVector()
    {
        reserve(rhs._size);

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memcpy(_data, rhs._data, rhs._size * sizeof(T));
            _size = rhs._size;
        }
        else
        {
            for (size_t i = 0; i < rhs._size; ++i)
                emplace_back(rhs._data[i]);
        }
    }

    /**
     * @brief Move constructor.
     */
    SmallVector(SmallVector&& rhs) noexcept
        : SmallVector()
    {
        moveFrom(std::move(rhs));
    }

    /**
     * @brief Destructor.
     */
    ~SmallVector()
    {
        destroyElements();

        if (!usingInlineStorage())
            operator delete(_data);
    }

    /**
     * @brief Copy assignment.
     */
    SmallVector& operator=(const SmallVector& rhs)
    {
        if (this == &rhs)
            return *this;

        clear();
        reserve(rhs._size);

        if constexpr (std::is_trivially_copyable_v<T>)
        {
            std::memcpy(_data, rhs._data, rhs._size * sizeof(T));
            _size = rhs._size;
        }
        else
        {
            for (size_t i = 0; i < rhs._size; ++i)
                emplace_back(rhs._data[i]);
        }

        return *this;
    }

    /**
     * @brief Move assignment.
     */
    SmallVector& operator=(SmallVector&& rhs) noexcept
    {
        if (this == &rhs)
            return *this;

        clear();

        if (!usingInlineStorage())
            operator delete(_data);

        _data = inlineData();
        _capacity = InlineCapacity;
        _size = 0;

        moveFrom(std::move(rhs));

        return *this;
    }

    /**
     * @brief Removes all elements.
     */
    void clear()
    {
        destroyElements();
        _size = 0;
    }

    /**
     * @brief Reserve capacity.
     */
    void reserve(size_t newCapacity)
    {
        if (newCapacity <= _capacity)
            return;

        grow(newCapacity);
    }

    /**
     * @brief Resize vector.
     */
    void resize(size_t newSize)
    {
        if (newSize < _size)
        {
            while (_size > newSize)
                pop_back();
        }
        else
        {
            reserve(newSize);

            while (_size < newSize)
                emplace_back();
        }
    }

    /**
     * @brief Add element.
     */
    void push_back(const T& value)
    {
        emplace_back(value);
    }

    /**
     * @brief Add moved element.
     */
    void push_back(T&& value)
    {
        emplace_back(std::move(value));
    }

    /**
     * @brief Construct element in-place.
     */
    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        if (_size == _capacity)
            grow(_capacity ? _capacity * 2 : 1);

        T* element = _data + _size;

        new (element) T(std::forward<Args>(args)...);

        ++_size;

        return *element;
    }

    /**
     * @brief Remove last element.
     */
    void pop_back()
    {
        assert(_size > 0);

        --_size;

        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            _data[_size].~T();
        }
    }

    size_t size() const
    {
        return _size;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    bool empty() const
    {
        return _size == 0;
    }

    T& front()
    {
        return _data[0];
    }

    const T& front() const
    {
        return _data[0];
    }

    T& back()
    {
        return _data[_size - 1];
    }

    const T& back() const
    {
        return _data[_size - 1];
    }

    T* data()
    {
        return _data;
    }

    const T* data() const
    {
        return _data;
    }

    T& operator[](size_t index)
    {
        return _data[index];
    }

    const T& operator[](size_t index) const
    {
        return _data[index];
    }

    iterator begin()
    {
        return _data;
    }

    iterator end()
    {
        return _data + _size;
    }

    const_iterator begin() const
    {
        return _data;
    }

    const_iterator end() const
    {
        return _data + _size;
    }

    const_iterator cbegin() const
    {
        return begin();
    }

    const_iterator cend() const
    {
        return end();
    }

    void shrink_to_fit()
    {
    }
};

} // namespace cnt