// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <type_traits>
#include <td/Types.h>
#include <vector>
#include <list>

namespace cnt
{
    template <class T, bool EXTERN_ALLOCATOR>
    class SafeFullVector;

    template <class T, size_t SIZE>
    class Array;


    template<class CNT>
    struct RingBase;

    template<class T, size_t FIX_SIZE>
    struct RingBase<T[FIX_SIZE]>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using cnt_iterator = pointer;

    protected:
        T _container[FIX_SIZE];
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase()
            : _container(), _first(&_container[0]), _last(&_container[FIX_SIZE - 1]), _capacity(FIX_SIZE)
        {}
    };

    template<class T>
    struct RingBase<T[]>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using cnt_iterator = pointer;

    protected:
        pointer _container;
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase(size_type capacity)
            : _container(new T[capacity]), _first(_container), _last(_container + capacity - 1), _capacity(capacity)
        {}
        RingBase() = default;

        ~RingBase()
        {
            clean();
        }
        
    public:
        void resize(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");

            if (_container)
                delete[] _container;

            _container = new T[size];
            _capacity = size;
            _first = _container;
            _last = _container + size - 1;
        }
        
        void clean()
        {
            if(_container)
            {
                delete[] _container;
                _container = nullptr;
            }
        }
    };

    template<class T, size_t FIX_SIZE>
    struct RingBase<cnt::Array<T, FIX_SIZE>>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using cnt_iterator = pointer;

    protected:
        cnt::Array<T, FIX_SIZE> _container;
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase()
            : _container(), _first(_container.begin()), _last(_container.end()-1), _capacity(FIX_SIZE)
        {}
    };

    template<class T, bool EXTERN_ALLOCATOR>
    struct RingBase<cnt::SafeFullVector<T, EXTERN_ALLOCATOR>>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using cnt_iterator = typename cnt::SafeFullVector<T, EXTERN_ALLOCATOR>::iterator;

    protected:
        cnt::SafeFullVector<T, EXTERN_ALLOCATOR> _container;
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(_container.end() - 1), _capacity(capacity)
        {}
        RingBase() = default;
    protected:
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");

            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = _container.end() - 1;
        }
    };

    template <class T, class Alloc>
    struct RingBase<std::vector<T, Alloc>>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = typename std::vector<T, Alloc>::pointer;
        using const_pointer = typename  std::vector<T, Alloc>::const_pointer;
        using reference = typename std::vector<T, Alloc>::reference;
        using const_reference = typename std::vector<T, Alloc>::const_reference;
        using cnt_iterator = typename std::vector<T, Alloc>::iterator;

    protected:
        std::vector<T, Alloc> _container;
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(_container.end() - 1), _capacity(capacity)
        {}
        RingBase() = default;

    protected:
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");
            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = _container.end() - 1;
        }
    };

    template <class T, class Alloc>
    struct RingBase<std::list<T, Alloc>>
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = typename std::list<T, Alloc>::pointer;
        using const_pointer = typename  std::list<T, Alloc>::const_pointer;
        using reference = typename std::list<T, Alloc>::reference;
        using const_reference = typename std::list<T, Alloc>::const_reference;
        using cnt_iterator = typename std::list<T, Alloc>::iterator;

    protected:
        std::list<T, Alloc> _container;
        cnt_iterator _first; //first element in container
        cnt_iterator _last; //last element in container
        size_type _capacity; //number of elements in a ring
    protected:
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(std::prev(_container.end())), _capacity(capacity)
        {}
        RingBase() = default;

    protected:
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");
            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = std::prev(_container.end());
        }
    };

    template< class CNT, bool PUSH_ON_FULL = true, bool LIFO = false>
    class Ring : public RingBase<CNT>
    {
        using value_type = typename RingBase<CNT>::value_type;
        using size_type = typename RingBase<CNT>::size_type;
        using pointer = typename RingBase<CNT>::pointer;
        using const_pointer = typename RingBase<CNT>::const_pointer;
        using reference = typename RingBase<CNT>::reference;
        using const_reference = typename RingBase<CNT>::const_reference;
        using cnt_iterator = typename RingBase<CNT>::cnt_iterator;
    public:
        template<bool DIRECTION, bool CONSTNESS>
        class iterator_
        {
            friend class Ring;
        public:
            using iterator_type = std::random_access_iterator_tag;
            using difference_type = ssize_t;
            using pointer = std::conditional_t<CONSTNESS, Ring::const_pointer, Ring::pointer>;
            using reference = std::conditional_t<CONSTNESS, Ring::const_reference, Ring::reference>;

        private:
            const Ring* _ring;
            size_type _pos;
            cnt_iterator _ptr;


        private:
            iterator_(const Ring* ring, size_type pos = 0)
                : _ring(ring), _pos(pos), _ptr(ring->_current)
            {
                if constexpr (!DIRECTION)
                {
                    if (pos > 0)
                        return;
                    else if (_ring->_size == _ring->_capacity)
                    {
                        if (_ptr == _ring->_last)
                            _ptr = _ring->_first;
                        else
                            ++_ptr;
                    }
                    else
                    {
                        difference_type offset = _ring->_head - _ring->_size + 1;
                        if (offset >= 0)
                            _ptr = std::next(_ring->_first, offset);
                        else
                            _ptr = std::next(_ring->_first, offset + _ring->_capacity);
                     
                    }
                }
            }

        public:
            iterator_(const iterator_& other)
                : _ring(other._ring), _pos(other._pos), _ptr(other._ptr)
            {}

        public:
            inline reference operator*() const
            {
                return *_ptr;
            }
            inline pointer operator->() const
            {
                return _ptr;
            }            

        private:
            void decrease()
            {
                if (_ptr == _ring->_last)
                    _ptr = _ring->_first;
                else
                    ++_ptr;
            }
            void increase()
            {
                if (_ptr == _ring->_first)
                    _ptr = _ring->_last;
                else
                    --_ptr;
            }
        public:
            inline iterator_& operator--()
            {
                if (_pos == 0)
                    return *this;

                if constexpr (DIRECTION)
                {
                    decrease();
                }
                else
                {
                    increase();
                }
                --_pos;
                return *this;
            }
            inline iterator_ operator--(int)
            {
                iterator_ ret = *this;
                ++(*this);
                return ret;
            }
            inline iterator_& operator++()
            {
                if (_pos == _ring->_size)
                    return *this;

                if constexpr(DIRECTION)
                {
                    increase();
                }
                else
                {
                    decrease();
                }

                ++_pos;
                return *this;

            }
            inline iterator_ operator++(int)
            {
                iterator_ ret = *this;
                --(*this);
                return ret;
            }
            inline iterator_& operator+=(size_type diff)
            {
                while (diff-- > 0)
                    ++(*this);

                return *this;
            }
            inline iterator_& operator-=(size_type diff)
            {
                while (diff-- > 0)
                    --(*this);

                return *this;
            }
            inline iterator_ operator+(size_type diff)
            {
                iterator_ ret = *this;
                while (diff-- > 0)
                    ++ret;

                return ret;
            }
            inline iterator_ operator-(size_type diff)
            {
                iterator_ ret = *this;
                while (diff-- > 0)
                    --ret;

                return ret;
            }

            friend difference_type operator-(const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos - rhs._pos;
            }
            friend bool operator< (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos < rhs._pos;
            }
            friend bool operator> (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos > rhs._pos;
            }

            friend bool operator== (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._ring == rhs._ring && lhs._pos == rhs._pos;
            }
            friend bool operator!= (const iterator_& lhs, const iterator_& rhs)
            {
                return !(lhs == rhs);
            }
        };

     public:
         using iterator = iterator_<true, false>;
         using const_iterator = iterator_<true, true>;
         using reverse_iterator = iterator_<false, false>;
         using const_reverse_iterator = iterator_<false, true>;
         using difference_type = typename iterator::difference_type;

    private:

        size_type _size; //current size, must be at most _capacity
        size_type _head; //valid position is between 0 and _capacity
        cnt_iterator _current; //element _head points to

        using RingBase<CNT>::_first;
        using RingBase<CNT>::_last;
        using RingBase<CNT>::_capacity;
    public:

        Ring()
            : RingBase<CNT>()
            , _size(0)
            , _head(0)
            , _current(_first)
        {}

        Ring(size_type capacity)
            : RingBase<CNT>(capacity)
            , _size(0)
            , _head(0)
            , _current(_first)
        {}
    public:

        template<class... Args>
        inline std::conditional_t<PUSH_ON_FULL, reference, bool> emplace(Args&&... args)
        {
            assert(_capacity > 0 && "Ring cannot be 0-sized");
            if (_size < _capacity) //ring not full yet
            {
                if (_size == 0)
                {
                    ++_size;
                    (*_current) = value_type(std::forward<Args>(args)...);
                }
                else
                {
                    ++_size;
                    ++_head;
                    ++_current;
                    if (_head >= _capacity)
                    {
                        _head = 0;
                        _current = _first;
                    }

                    (*_current) = value_type(std::forward<Args>(args)...);
                }

                if constexpr (PUSH_ON_FULL)
                    return (*_current);
                else
                    return true;
            }
            else
            {
                if constexpr (!PUSH_ON_FULL)
                    return false;
                else
                {
                    ++_head;
                    ++_current;
                    if (_head >= _capacity)
                    {
                        _head = 0;
                        _current = _first;
                    }

                    (*_current) = value_type(std::forward<Args>(args)...);
                    return (*_current);
                }

            }
        }

        inline std::conditional_t<PUSH_ON_FULL, reference, bool> push(value_type&& value)
        {
            return emplace(std::move(value));
        }

        inline std::conditional_t<PUSH_ON_FULL, void, bool> push(const value_type& value)
        {
            if constexpr (PUSH_ON_FULL)
                emplace(value);
            else
                return emplace(value);
        }

        inline void pop()
        {
            assert(_size != 0 && "Pop called on empty ring!");

            --_size;

            if constexpr (LIFO)
            {
                if (_current == _first)
                {
                    _current = _last;
                    _head = _capacity - 1;
                }
                else
                {
                    --_head;
                    --_current;
                }
            }
        }


        inline void reset()
        {
            _size = 0;
            _head = 0;
            _current = _first;
        }
        
        
        
        inline void resize(size_type size)
        {
            RingBase<CNT>::reset(size);
            reset();
        }

        inline const_reference front() const
        {
            return (*_current);
        }

        inline const_reference back() const
        {
            if (_size == _capacity)
            {
                if (_current == _last)
                    return (*_first);
                else
                    return *std::next(_current);
            }
            else
            {
                difference_type offset = _head - _size + 1;
                if (offset >= 0)
                    return *std::next(_first, offset);
                else
                    return *std::next(_first, offset + _capacity);
            }
        }
        inline const_reference at(size_type position) const
        {
            assert(position < _size && "Invalid position");
            return *(begin() + position);
        }
        inline reference at(size_type position) 
        {
            assert(position < _size && "Invalid position");
            return *(begin() + position);
        }
        inline const_reference operator[](size_type position) const
        {
            return at(position);
        }
        inline reference operator[](size_type position)
        {
            return at(position);
        }
        inline auto size() const
        {
            return _size;
        } 
        inline bool empty() const
        {
            return _size == 0;
        }
        inline bool full() const
        {
            if constexpr (PUSH_ON_FULL)
                return false;

            return _size == _capacity;
        }
        inline iterator begin()
        {
            return iterator(this);
        }
        inline iterator end()
        {
            return iterator(this, _size);
        }
        inline const_iterator begin() const
        {
            return const_iterator(this);
        }
        inline const_iterator end() const
        {
            return const_iterator(this, _size);
        }
        inline const_iterator cbegin() const
        {
            return const_iterator(this);
        }
        inline const_iterator cend() const
        {
            return const_iterator(this, _size);
        }

        inline reverse_iterator rbegin()
        {
            return reverse_iterator(this);
        }
        inline reverse_iterator rend()
        {
            return reverse_iterator(this, _size);
        }

        inline const_reverse_iterator crbegin() const
        {
            return reverse_iterator(this);
        }
        inline const_reverse_iterator crend() const
        {
            return reverse_iterator(this, _size);
        }
    };

    template<class T, size_t SIZE>
    using FixedQueue = Ring<cnt::Array<T, SIZE>, false, false>;
    template<class T>
    using FullQueue = Ring<cnt::SafeFullVector<T>, false, false>;
}
