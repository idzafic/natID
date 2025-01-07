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
#include <td/Types.h>
//#include <ostream>

//inkrementira se a i ima reset method tako da najveca velicina buffera ostaje rezervisana 
//(clean samo pomjera top pointer na pocetak)

namespace cnt
{
//keeps max size
template <typename T, size_t INCREMENT = 0>
class PushBackVector
{
protected:
    T* _data;
    T* _top;
    size_t _capacity;
    //T* _endPos;
    inline const T* getEndPos() const
    {
        return _data + _capacity;
    }
    
    inline T* getEndPos()
    {
        return _data + _capacity;
    }
    
public:
    typedef T* iterator;
    typedef const T*  const_iterator;
    typedef T T_DATA;
    typedef T* pointer;
    PushBackVector()
        : _top(nullptr)
        , _data(nullptr)
        , _capacity(0)
        //, _endPos(0)
    {
    }
    template <size_t INCREMENT2>
    PushBackVector<T, INCREMENT>& operator = (const PushBackVector<T, INCREMENT2>& v)
    {
        if (_data)
        {
            delete [] _data;
            _data = nullptr;
            _top = nullptr;
            _capacity = 0;
            //_endPos = nullptr;
        }
        size_t nSize = v.size();
        if (nSize <= 0)
            return *this;
        reserve(nSize);
        for (size_t i = 0; i<nSize; ++i)
        {
            push_back(v[i]);
        }
        return *this;
    }

    PushBackVector(const PushBackVector<T, INCREMENT>& v)
        : _top(nullptr)
        , _data(nullptr)
        , _capacity(0)
        //, _endPos(nullptr)
    {
        operator = (v);
    }

    PushBackVector(size_t n)
        : _top(nullptr)
        , _data(nullptr)
        , _capacity(0)
        //, _endPos(nullptr)
    {
        reserve(n);
    }
    
    PushBackVector(const std::initializer_list<T>& elems)
        : _top(nullptr)
        , _data(nullptr)
        , _capacity(0)
        //, _endPos(nullptr)
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

    ~PushBackVector()
    {
        if (_data)
            delete [] _data;
    }

    iterator begin()
    {
        return _data;
    }

    const_iterator begin() const
    {
        return _data;
    }

    const_iterator cbegin() const
    {
        return _data;
    }

    iterator end()
    {
        return _top;
    }
    
    const_iterator end() const
    {
        return _top;
    }
    
    const_iterator cend() const
    {
        return _top;
    }


    void reset()
    {
        _top = _data;
    }
    
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

    size_t size() const
    {
        return (size_t) (_top - _data);
    }

    size_t capacity() const
    {
        return _capacity;
    }

    const T& operator [] (size_t pos) const
    {
        auto sz = size();
        assert(pos < sz);
        return _data[pos];
    }

    T& operator [] (size_t pos)
    {
        assert(pos < size());
        return _data[pos];
    }
    
    const T& at(size_t pos) const
    {
        assert(pos < size());
        return _data[pos];
    }

    T& at(size_t pos)
    {
        assert(pos < size());
        return _data[pos];
    }
    
    const T& last() const
    {
        assert(size() > 0);
        assert(_capacity > 0);
        assert(_top);
        return *(_top - 1);
    }

    T& last()
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

    void reserve(size_t n)
    {
        assert(n>0);
        //_container.reserve(n);
        if (_capacity != n)
        {
            if (_data)
                delete [] _data;
            _data = new T[n];
            _capacity = n;
        }
        _top = _data;
        //_endPos = _data + _size;
    }
    
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
    
    
    bool isEmpty() const
    {
        return _top == _data;
    }

    bool isFull() const
    {
        auto sz = size();
        return (sz == _capacity);
    }
    
    void clean()
    {
        //just move pointer to begining
        _top = _data;
    }
    
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
            *pPrev = *pPos;
            pPrev = pPos;
            ++pPos;
        }
        -- _top;
        if (_top < _data)
            _top = _data;
        return true;
    }
    
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

    T& push_back()
    {
        const T* endPos = getEndPos();
        if (_top >= endPos)
        {
            T val;
            push_back(val);
        }
        else
        {
            ++_top;
        }
        return *(_top - 1);
    }
    
    T* push_back2()
    {
        const T* endPos = getEndPos();
        if (_top >= endPos)
        {
            T val;
            push_back(val);
        }
        else
        {
            ++_top;
        }
        return _top - 1;
    }

    void push_back(const T& val)
    {
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
                    pNewData[i] = _data[i];
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
                    pNewData[i] = _data[i];
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

    void zeroAll()
    {
        for (size_t i=0; i<_capacity; ++i)
            _data[i] = 0;
    }
    
    void push_back(const T* pVals, size_t nVals)
    {
        for (size_t i=0; i<nVals; ++i)
        {
            push_back(pVals[i]);
        }
    }
    template <size_t INCREMENT2>
    void push_back(const PushBackVector<T, INCREMENT2>& v)
    {
        for (const auto& e : v)
        {
            push_back(e);
        }
    }
            
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
