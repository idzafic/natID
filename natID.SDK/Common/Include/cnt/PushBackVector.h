// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PushBackVector.h
 @brief Dynamic array that grows on push_back and retains maximum allocated capacity across resets. */
#pragma once
#include <cassert>
#include <td/Types.h>
#include <td/Concepts.h>

//#include <ostream>

//inkrementira se a i ima reset method tako da najveca velicina buffera ostaje rezervisana
//(clean samo pomjera top pointer na pocetak)

namespace cnt
{
//keeps max size
/// @brief Append-only dynamic array that preserves its allocated buffer on reset.
/// @tparam T         Element type.
/// @tparam INCREMENT Growth step size when expanding; 0 means double the capacity.
template <typename T, size_t INCREMENT = 0>
class PushBackVector
{
protected:
    T* _data;     ///< Pointer to the allocated element buffer.
    T* _top;      ///< Pointer to one past the last inserted element.
    size_t _capacity; ///< Total number of elements the buffer can hold.

    /// @brief Returns a const pointer to one past the end of the allocated buffer.
    /// @return Const pointer to the end of capacity.
    inline const T* getEndPos() const
    {
        return _data + _capacity;
    }

    /// @brief Returns a pointer to one past the end of the allocated buffer.
    /// @return Pointer to the end of capacity.
    inline T* getEndPos()
    {
        return _data + _capacity;
    }

public:
    typedef T* iterator;             ///< Mutable iterator type.
    typedef const T*  const_iterator; ///< Const iterator type.
    typedef T T_DATA;                ///< Element type alias.
    typedef T* pointer;              ///< Pointer type alias.

    /// @brief Constructs an empty PushBackVector with no allocated memory.
    PushBackVector()
    : _data(nullptr)
    , _top(nullptr)
    , _capacity(0)
    {
    }

    /// @brief Copy assignment from a PushBackVector with a different increment is deleted.
    template <size_t INCREMENT2>
    PushBackVector<T, INCREMENT>& operator = (const PushBackVector<T, INCREMENT2>& v) = delete;

    /// @brief Copy constructor is deleted; use move semantics instead.
    PushBackVector(const PushBackVector<T, INCREMENT>& v) = delete;

    /// @brief Constructs a PushBackVector with pre-allocated capacity for n elements.
    /// @param n Number of elements to pre-allocate.
    PushBackVector(size_t n)
    : _data(nullptr)
    , _top(nullptr)
    , _capacity(0)
    {
        reserve(n);
    }

    /// @brief Constructs a PushBackVector from an initializer list.
    /// @param elems Initializer list of elements to append.
    PushBackVector(const std::initializer_list<T>& elems)
    : _data(nullptr)
    , _top(nullptr)
    , _capacity(0)
    {
        auto n = elems.size();

        if (n > 0)
        {
            reserve(n);
            for (const auto& e : elems)
            {
                push_back(e);
            }
        }
    }

    //move constructor
    /// @brief Move-constructs a PushBackVector from another instance with a potentially different increment.
    /// @tparam INCREMENT2 Growth increment of the source vector.
    /// @param  vect        Source vector to move from.
    template <size_t INCREMENT2>
    PushBackVector(PushBackVector<T, INCREMENT2>&& vect)
    {
        _data = vect._data;
        _top = vect._top;
        _capacity = vect._capacity;
        vect._data = nullptr;
        vect._top = nullptr;
        vect._capacity = 0;
    };

    /// @brief Move-assigns from another PushBackVector.
    /// @param vect Source vector to move from.
    /// @return Reference to this object.
    // Move assignment operator
    PushBackVector& operator=(PushBackVector&& vect) noexcept
    {
        if (this != &vect)
        {
            release();

            _data = vect._data;
            vect._data = nullptr;

            _top = vect._top;
            vect._top = nullptr;

            _capacity = vect._capacity;
            vect._capacity = 0;
        }
        return *this;
    }

    /// @brief Destroys the vector and frees all allocated memory.
    ~PushBackVector()
    {
        if (_data)
            delete [] _data;
    }

    /// @brief Returns an iterator to the first element.
    /// @return Pointer to the first element.
    iterator begin()
    {
        return _data;
    }

    /// @brief Returns a const iterator to the first element.
    /// @return Const pointer to the first element.
    const_iterator begin() const
    {
        return _data;
    }

    /// @brief Returns a const iterator to the first element.
    /// @return Const pointer to the first element.
    const_iterator cbegin() const
    {
        return _data;
    }

    /// @brief Returns an iterator one past the last inserted element.
    /// @return Pointer to one past the last element.
    iterator end()
    {
        return _top;
    }

    /// @brief Returns a const iterator one past the last inserted element.
    /// @return Const pointer to one past the last element.
    const_iterator end() const
    {
        return _top;
    }

    /// @brief Returns a const iterator one past the last inserted element.
    /// @return Const pointer to one past the last element.
    const_iterator cend() const
    {
        return _top;
    }


    /// @brief Resets the size to zero without freeing the allocated buffer.
    void reset()
    {
        _top = _data;
    }

    /// @brief Directly sets the logical size of the vector.
    /// @param nElems Number of elements to mark as populated; must not exceed capacity.
    void markAsPopulated(size_t nElems)
    {
        //use with care
        assert(nElems <= _capacity);
        _top = _data + nElems;

    }
    //const_iterator cend() const
    //{
    //	return _top;
    //}

    /// @brief Returns the number of elements currently stored.
    /// @return Number of elements.
    size_t size() const
    {
        return (size_t) (_top - _data);
    }

    /// @brief Returns the total number of elements the buffer can hold without reallocation.
    /// @return Allocated capacity.
    size_t capacity() const
    {
        return _capacity;
    }

    /// @brief Provides const indexed access to elements.
    /// @param pos Zero-based index.
    /// @return Const reference to the element at pos.
    const T& operator [] (size_t pos) const
    {
        auto sz = size();
        assert(pos < sz);
        return _data[pos];
    }

    /// @brief Provides mutable indexed access to elements.
    /// @param pos Zero-based index.
    /// @return Reference to the element at pos.
    T& operator [] (size_t pos)
    {
        auto sz = size();
        assert(pos < sz);
        return _data[pos];
    }

    /// @brief Provides const bounds-checked access to elements.
    /// @param pos Zero-based index.
    /// @return Const reference to the element at pos.
    const T& at(size_t pos) const
    {
        assert(pos < size());
        return _data[pos];
    }

    /// @brief Provides mutable bounds-checked access to elements.
    /// @param pos Zero-based index.
    /// @return Reference to the element at pos.
    T& at(size_t pos)
    {
        assert(pos < size());
        return _data[pos];
    }

    /// @brief Returns a const reference to the last inserted element.
    /// @return Const reference to the last element.
    const T& last() const
    {
        assert(size() > 0);
        assert(_capacity > 0);
        assert(_top);
        return *(_top - 1);
    }

    /// @brief Returns a reference to the last inserted element.
    /// @return Reference to the last element.
    T& last()
    {
        assert(size() > 0);
        assert(_capacity > 0);
        assert(_top);
        return *(_top - 1);
    }

    /// @brief Returns a const reference to the last inserted element (alias for last()).
    /// @return Const reference to the last element.
    const T& back() const
    {
        assert(size() > 0);
        assert(_capacity > 0);
        assert(_top);
        return *(_top - 1);
    }

    /// @brief Returns a reference to the last inserted element (alias for last()).
    /// @return Reference to the last element.
    T& back()
    {
        assert(size() > 0);
        assert(_capacity > 0);
        assert(_top);
        return *(_top - 1);
    }

    //T& operator () (size_t pos) const
    //{
    //	assert(pos < _size);
    //	return _data[pos];
    //}

    /// @brief Reallocates the buffer to hold exactly n elements and resets the size to zero.
    /// @param n New capacity.
    void reserve(size_t n)
    {
        if (_capacity != n)
        {
            if (_data)
                delete [] _data;
            if (n > 0)
                _data = new T[n];
            else
                _data = nullptr;

            _capacity = n;
        }
        _top = _data;
        //_endPos = _data + _size;
    }

    /// @brief Ensures the buffer holds at least n elements, reallocating only if needed; resets size to zero.
    /// @param n Minimum required capacity.
    void reserveMin(size_t n)
    {
        assert(n>0);
        //_container.reserve(n);
        if (_capacity < n)
        {
            if (_data)
                delete [] _data;
            _data = new T[n];
            _capacity = n;
        }
        _top = _data;
        //_endPos = _data + _size;
    }


    /// @brief Checks whether the vector contains no elements.
    /// @return True if empty.
    bool isEmpty() const
    {
        return _top == _data;
    }

    /// @brief Checks whether the vector has reached its current capacity.
    /// @return True if size equals capacity.
    bool isFull() const
    {
        auto sz = size();
        return (sz == _capacity);
    }

    /// @brief Resets the size to zero without freeing memory (alias for reset()).
    void clean()
    {
        //just move pointer to begining
        _top = _data;
    }

    /// @brief Frees all allocated memory and resets the vector to an empty state.
    void release()
    {
        if (_data)
        {
            delete [] _data;
            _data = nullptr;
            _top = nullptr;
            _capacity = 0;
        }
    }

    /// @brief Searches for the first occurrence of val and returns its index.
    /// @param val The value to find.
    /// @return Zero-based index of the first match, or -1 if not found.
    ssize_t find(const T& val) const
    {
        ssize_t nElems = (ssize_t)size();
        for (ssize_t i = 0; i < nElems; ++i)
        {
            if (_data[i] == val)
            {
                return i;
            }
        }
        return -1;
    }

    /// @brief Removes the element at the given index, shifting subsequent elements left.
    /// @param iPos Zero-based index of the element to remove.
    /// @return True on success, false if iPos is out of range.
    bool remove(size_t iPos)
    {
        assert( (iPos >= 0) && (iPos < _capacity) );

        if (_top == nullptr)
            return false;

        if (_top == _data)
            return false;
        T* pPos = _data + iPos;
        const T* endPos = _top;

        if (pPos >= endPos)
            return false;
        //move data
        T* pPrev = pPos;
        ++pPos;
        while (pPos < endPos)
        {
            *pPrev = std::move(*pPos);
            pPrev = pPos;
            ++pPos;
        }
        -- _top;
        if (_top < _data)
            _top = _data;
        return true;
    }

    /// @brief Removes the first element equal to val.
    /// @param val The value to remove.
    /// @return True if an element was removed, false if val was not found.
    bool remove(const T& val)
    {
        size_t nElems = size();
        for (size_t i  = 0; i < nElems; ++i)
        {
            if (_data[i] == val)
            {
                remove(i);
                return true;
            }
        }
        return false;
    }

    /// @brief Removes the last element.
    /// @return True on success, false if the vector is empty.
    bool remove_back()
    {
        if (!_data)
            return false;
        auto nElems = size();
        if (nElems == 0)
            return false;
        --_top;
        if (_top < _data)
            _top = _data;
        return true;
    }

    /// @brief Appends a default-constructed element at the back, growing the buffer if needed.
    /// @return Reference to the newly appended element.
    inline T& push_back() requires td::conc::DefaultConstructor<T>
    {
        const T* endPos = getEndPos();
        if (_top >= endPos)
        {
            // Calculate new capacity
            size_t newSize;
            if constexpr (INCREMENT > 0)
                newSize = _capacity + INCREMENT;
            else
                newSize = (_capacity > 0) ? _capacity * 2 : 1;

            T* pNewData = new T[newSize]; // default-constructed

            assert(pNewData != nullptr);

            // Move existing elements into new array
            if (_capacity > 0)
            {
                for (size_t i = 0; i < _capacity; ++i)
                    pNewData[i] = std::move(_data[i]); // Move construct

                if (_data)
                    delete[] _data;

                _data = pNewData;
                _top = _data + _capacity;
            }
            else
            {
                _data = pNewData;
                _top = _data;
            }

            _capacity = newSize;
        }
		T& toRet = *_top;
        ++_top;
        return toRet;
    }


    /// @brief Copies all elements from another vector, releasing current contents first.
    /// @param v Source vector to copy from.
    void copyFrom(const PushBackVector<T>& v)
    {
        release();
        auto nElems = v.size();
        reserve(nElems);
        for (const auto& e: v)
            push_back(e);
    }

//    inline T& push_back() requires (!td::conc::DefaultConstructor<T>)
//    {
//        const T* endPos = getEndPos();
//        if (_top >= endPos)
//        {
//            // Calculate new capacity
//            size_t newSize;
//            if constexpr (INCREMENT > 0)
//                newSize = _capacity + INCREMENT;
//            else
//                newSize = (_capacity > 0) ? _capacity * 2 : 1;
//
//            size_t nBytes = newSize*sizeof(T);
//            T* pNewData = (T*) new td::BYTE[nBytes]; // default-constructed
//            assert(pNewData != nullptr);
////            memset(pNewData, 0, nBytes);
//
//            // Move existing elements into new array
//            if (_capacity > 0)
//            {
//                for (size_t i = 0; i < _capacity; ++i)
//                    pNewData[i] = std::move(_data[i]); // Move construct
//                //set the rest to zero
//                memset((void*)(&pNewData[_capacity]), 0, (newSize -_capacity) * sizeof(T));
//
//                delete[] _data;
//                _data = pNewData;
//                _top = _data + _capacity;
//            }
//            else
//            {
//                memset(pNewData, 0, nBytes);
//                _data = pNewData;
//                _top = _data;
//            }
//
//            _capacity = newSize;
//        }
//        return *(_top++);
//    }

    /// @brief Appends a default-constructed element and returns a pointer to it.
    /// @return Pointer to the newly appended element.
    inline T* push_back2()
    {
        return &push_back();
    }

    /// @brief Appends a copy of val at the back, growing the buffer if needed.
    /// @param val The value to append.
    void push_back(const T& val) requires td::conc::CopyAssign<T>
    {
        const T* endPos = getEndPos();
        if (_top >= endPos)
        {
            //detect new size
            size_t newSize;
            if constexpr (INCREMENT > 0)
                newSize = _capacity + INCREMENT;
            else
                newSize = (_capacity > 0) ? _capacity * 2 : 1; //duplicate

            T* pNewData = new T[newSize];

            assert(pNewData != nullptr);

            if (_capacity > 0)
            {
                //copy old data
                for (size_t i = 0; i<_capacity; i++)
                {
                    pNewData[i] = std::move(_data[i]);
                    //memset((void*)(_data + i), 0, sizeof(T));
                }
                size_t delta = size();
                delete [] _data;
                _data = pNewData;
                _top = _data + delta;
            }
            else
            {
                _data = pNewData;
                _top = _data;
            }
            _capacity = newSize;
            //_endPos = _data + _size;
        }
        *_top = val;
        ++_top ;
    }

    /// @brief Inserts val at the given position, shifting subsequent elements right.
    /// @param pos Zero-based index at which to insert; if >= size, appends.
    /// @param val The value to insert.
    void insert(size_t pos, const T& val)
    {

        if (pos >= size())
        {
            push_back(val);
            return;
        }

        const T* endPos = getEndPos();
        if (_top >= endPos)
        {
            //detect new size
            size_t newSize;
            if (INCREMENT > 0)
            {
                newSize = _capacity + INCREMENT;
            }
            else
            {
                //duplicate
                if (_capacity > 0)
                    newSize = _capacity * 2;
                else
                    newSize = 1;  //worst case
            }

            T* pNewData = new T[newSize];

            assert(pNewData != nullptr);

            if (_capacity > 0)
            {
                //copy old data
                for (size_t i = 0; i<_capacity; i++)
                {
                    pNewData[i] = std::move(_data[i]);
                }
                size_t delta = size();
                delete [] _data;
                _data = pNewData;
                _top = _data + delta;
            }
            else
            {
                _data = pNewData;
                _top = _data;
            }
            _capacity = newSize;
            //_endPos = _data + _size;
        }
        ++_top ;
        auto nElems = size();
        for (size_t i = nElems-1; i>pos; --i)
        {
            _data[i] = _data[i-1];
        }
        _data[pos] = val;
    }

    /// @brief Sets all allocated elements (up to capacity) to zero.
    void zeroAll()
    {
        for (size_t i=0; i<_capacity; ++i)
            _data[i] = 0;
    }

    /// @brief Appends nVals elements from a raw array.
    /// @param pVals Pointer to the source array.
    /// @param nVals Number of elements to append.
    void push_back(const T* pVals, size_t nVals)
    {
        for (size_t i=0; i<nVals; ++i)
        {
            push_back(pVals[i]);
        }
    }

    /// @brief Appends all elements from another PushBackVector (potentially different increment).
    /// @tparam INCREMENT2 Increment of the source vector.
    /// @param  v          Source vector.
    template <size_t INCREMENT2>
    void push_back(const PushBackVector<T, INCREMENT2>& v)
    {
        for (const auto& e : v)
        {
            push_back(e);
        }
    }

    /// @brief Applies a callable to every element in order.
    /// @tparam MyFunction Callable type accepting a const T reference.
    /// @param  f          The callable to invoke on each element.
    template <typename MyFunction>
    void for_each(MyFunction& f) const
    {
                    auto first = cbegin();
        auto last = cend();

        while (first != last)
        {
            f(*first);
            ++first;
        }
    }

    /// @brief Calls operator() on every element in order.
    void forEach() const
    {
        auto it = cbegin();
        auto itEnd = cend();
        while (it != itEnd)
        {
            (*it)();
            ++it;
        }
    }

    /// @brief Moves an element from fromPos to toPos, shifting elements between them.
    /// @param fromPos Source zero-based index.
    /// @param toPos   Destination zero-based index.
    /// @return True if the move was performed, false if positions are equal.
    bool move(size_t fromPos, size_t toPos)
    {
        if (fromPos == toPos)
            return false;

        T val = _data[fromPos];
        if (fromPos > toPos)
        {
            for (size_t i=fromPos; i>toPos; --i)
            {
                _data[i] = _data[i-1];
            }
            _data[toPos] = val;
        }
        else
        {
            for (size_t i=fromPos; i<toPos; ++i)
            {
                _data[i] = _data[i+1];
            }
            _data[toPos] = val;
        }
        return true;
    }

    /// @brief Writes the element count and all elements to a stream.
    /// @tparam TSTREAM Stream type supporting operator<<.
    /// @param  o       Output stream.
    /// @param  pSep    Separator string placed before each element.
    template <class TSTREAM>
    void toStream(TSTREAM& o, const char* pSep = " ")
    {
        auto first = begin();
        auto last = cend();
        size_t elSize = size();
        o << elSize;
        while (first != last)
        {
            o << *pSep << *first;

            ++first;
        }
        o << std::endl;
    }
};

} //namespace cnt
