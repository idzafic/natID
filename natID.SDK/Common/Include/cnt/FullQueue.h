// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FullQueue.h
    @brief Ring buffer container with multiple storage back-end specialisations and FIFO/LIFO support. */
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

    /// @brief Primary template declaration for Ring storage-policy base classes.
    /// @tparam CNT Container type used as the storage back-end (intentionally undefined; use a partial specialisation).
    template<class CNT>
    struct RingBase;

    /// @brief Base storage policy for Ring using a compile-time fixed-size C array.
    /// @tparam T        Element type.
    /// @tparam FIX_SIZE Number of elements in the fixed array.
    template<class T, size_t FIX_SIZE>
    struct RingBase<T[FIX_SIZE]>
    {
    public:
        using value_type = T;         ///< Type of elements stored.
        using size_type = size_t;     ///< Type used for sizes and indices.
        using pointer = value_type*;  ///< Pointer to element.
        using const_pointer = const value_type*; ///< Const pointer to element.
        using reference = value_type&;            ///< Reference to element.
        using const_reference = const value_type&; ///< Const reference to element.
        using cnt_iterator = pointer; ///< Iterator type for this storage back-end.

    protected:
        T _container[FIX_SIZE]; ///< Fixed-size storage array.
        cnt_iterator _first;    ///< Pointer to the first element of the ring.
        cnt_iterator _last;     ///< Pointer to the last element of the ring.
        size_type _capacity;    ///< Maximum number of elements.
    protected:
        /// @brief Default constructor. Sets up first/last pointers for the fixed array.
        RingBase()
            : _container(), _first(&_container[0]), _last(&_container[FIX_SIZE - 1]), _capacity(FIX_SIZE)
        {}
    };

    /// @brief Base storage policy for Ring using a heap-allocated dynamic array.
    /// @tparam T Element type.
    template<class T>
    struct RingBase<T[]>
    {
    public:
        using value_type = T;           ///< Type of elements stored.
        using size_type = size_t;       ///< Type used for sizes and indices.
        using pointer = value_type*;    ///< Pointer to element.
        using const_pointer = const value_type*; ///< Const pointer to element.
        using reference = value_type&;            ///< Reference to element.
        using const_reference = const value_type&; ///< Const reference to element.
        using cnt_iterator = pointer;   ///< Iterator type for this storage back-end.

    protected:
        pointer _container;          ///< Heap-allocated storage array.
        cnt_iterator _first;         ///< Pointer to the first element of the ring.
        cnt_iterator _last;          ///< Pointer to the last element of the ring.
        size_type _capacity;         ///< Maximum number of elements.
    protected:
        /// @brief Constructs storage with the given capacity, allocating on the heap.
        /// @param capacity Number of elements to allocate.
        RingBase(size_type capacity)
            : _container(new T[capacity]), _first(_container), _last(_container + capacity - 1), _capacity(capacity)
        {}
        /// @brief Default constructor for deferred initialisation.
        RingBase() = default;

        /// @brief Destructor. Frees the heap-allocated container.
        ~RingBase()
        {
            clean();
        }

    public:
        /// @brief Resizes the underlying storage to the given number of elements.
        /// @param size New capacity; must be greater than 0.
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

        /// @brief Frees the heap-allocated storage and nulls the pointer.
        void clean()
        {
            if(_container)
            {
                delete[] _container;
                _container = nullptr;
            }
        }
    };

    /// @brief Base storage policy for Ring backed by a cnt::Array.
    /// @tparam T        Element type.
    /// @tparam FIX_SIZE Compile-time size of the cnt::Array.
    template<class T, size_t FIX_SIZE>
    struct RingBase<cnt::Array<T, FIX_SIZE>>
    {
    public:
        using value_type = T;          ///< Type of elements stored.
        using size_type = size_t;      ///< Type used for sizes and indices.
        using pointer = value_type*;   ///< Pointer to element.
        using const_pointer = const value_type*; ///< Const pointer to element.
        using reference = value_type&;            ///< Reference to element.
        using const_reference = const value_type&; ///< Const reference to element.
        using cnt_iterator = pointer;  ///< Iterator type for this storage back-end.

    protected:
        cnt::Array<T, FIX_SIZE> _container; ///< Fixed-size cnt::Array storage.
        cnt_iterator _first;                 ///< Pointer to the first element of the ring.
        cnt_iterator _last;                  ///< Pointer to the last element of the ring.
        size_type _capacity;                 ///< Maximum number of elements.
    protected:
        /// @brief Default constructor. Sets up first/last pointers for the cnt::Array.
        RingBase()
            : _container(), _first(_container.begin()), _last(_container.end()-1), _capacity(FIX_SIZE)
        {}
    };

    /// @brief Base storage policy for Ring backed by a cnt::SafeFullVector.
    /// @tparam T                Element type.
    /// @tparam EXTERN_ALLOCATOR If true, the SafeFullVector uses an external allocator.
    template<class T, bool EXTERN_ALLOCATOR>
    struct RingBase<cnt::SafeFullVector<T, EXTERN_ALLOCATOR>>
    {
    public:
        using value_type = T;          ///< Type of elements stored.
        using size_type = size_t;      ///< Type used for sizes and indices.
        using pointer = value_type*;   ///< Pointer to element.
        using const_pointer = const value_type*; ///< Const pointer to element.
        using reference = value_type&;            ///< Reference to element.
        using const_reference = const value_type&; ///< Const reference to element.
        using cnt_iterator = typename cnt::SafeFullVector<T, EXTERN_ALLOCATOR>::iterator; ///< Iterator type.

    protected:
        cnt::SafeFullVector<T, EXTERN_ALLOCATOR> _container; ///< SafeFullVector storage.
        cnt_iterator _first;   ///< Iterator to the first element of the ring.
        cnt_iterator _last;    ///< Iterator to the last element of the ring.
        size_type _capacity;   ///< Maximum number of elements.
    protected:
        /// @brief Constructs storage with the given capacity.
        /// @param capacity Number of elements to reserve.
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(_container.end() - 1), _capacity(capacity)
        {}
        /// @brief Default constructor for deferred initialisation.
        RingBase() = default;
    protected:
        /// @brief Resets and resizes the underlying SafeFullVector.
        /// @param size New capacity; must be greater than 0.
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");

            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = _container.end() - 1;
        }
    };

    /// @brief Base storage policy for Ring backed by a std::vector.
    /// @tparam T     Element type.
    /// @tparam Alloc Allocator type for the std::vector.
    template <class T, class Alloc>
    struct RingBase<std::vector<T, Alloc>>
    {
    public:
        using value_type = T;          ///< Type of elements stored.
        using size_type = size_t;      ///< Type used for sizes and indices.
        using pointer = typename std::vector<T, Alloc>::pointer;              ///< Pointer to element.
        using const_pointer = typename  std::vector<T, Alloc>::const_pointer; ///< Const pointer to element.
        using reference = typename std::vector<T, Alloc>::reference;           ///< Reference to element.
        using const_reference = typename std::vector<T, Alloc>::const_reference; ///< Const reference to element.
        using cnt_iterator = typename std::vector<T, Alloc>::iterator; ///< Iterator type.

    protected:
        std::vector<T, Alloc> _container; ///< std::vector storage back-end.
        cnt_iterator _first;               ///< Iterator to the first element of the ring.
        cnt_iterator _last;                ///< Iterator to the last element of the ring.
        size_type _capacity;               ///< Maximum number of elements.
    protected:
        /// @brief Constructs storage with the given capacity.
        /// @param capacity Number of elements to reserve.
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(_container.end() - 1), _capacity(capacity)
        {}
        /// @brief Default constructor for deferred initialisation.
        RingBase() = default;

    protected:
        /// @brief Resets and resizes the underlying std::vector.
        /// @param size New capacity; must be greater than 0.
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");
            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = _container.end() - 1;
        }
    };

    /// @brief Base storage policy for Ring backed by a std::list.
    /// @tparam T     Element type.
    /// @tparam Alloc Allocator type for the std::list.
    template <class T, class Alloc>
    struct RingBase<std::list<T, Alloc>>
    {
    public:
        using value_type = T;          ///< Type of elements stored.
        using size_type = size_t;      ///< Type used for sizes and indices.
        using pointer = typename std::list<T, Alloc>::pointer;              ///< Pointer to element.
        using const_pointer = typename  std::list<T, Alloc>::const_pointer; ///< Const pointer to element.
        using reference = typename std::list<T, Alloc>::reference;           ///< Reference to element.
        using const_reference = typename std::list<T, Alloc>::const_reference; ///< Const reference to element.
        using cnt_iterator = typename std::list<T, Alloc>::iterator; ///< Iterator type.

    protected:
        std::list<T, Alloc> _container; ///< std::list storage back-end.
        cnt_iterator _first;             ///< Iterator to the first element of the ring.
        cnt_iterator _last;              ///< Iterator to the last element of the ring.
        size_type _capacity;             ///< Maximum number of elements.
    protected:
        /// @brief Constructs storage with the given capacity.
        /// @param capacity Number of elements to reserve.
        RingBase(size_type capacity)
            : _container(capacity), _first(_container.begin()), _last(std::prev(_container.end())), _capacity(capacity)
        {}
        /// @brief Default constructor for deferred initialisation.
        RingBase() = default;

    protected:
        /// @brief Resets and resizes the underlying std::list.
        /// @param size New capacity; must be greater than 0.
        void reset(size_type size)
        {
            assert(size > 0 && "Size must be greater than 0");
            _container.reserve(size);
            _capacity = size;
            _first = _container.begin();
            _last = std::prev(_container.end());
        }
    };

    /// @brief Generic ring buffer with configurable storage, push-on-full behaviour, and LIFO support.
    /// @tparam CNT          Container type used as the storage back-end.
    /// @tparam PUSH_ON_FULL If true, pushing when full overwrites the oldest element; if false, push returns false.
    /// @tparam LIFO         If true, pop removes from the most recently pushed end (stack semantics).
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
        /// @brief Bidirectional iterator for Ring supporting forward and reverse traversal.
        /// @tparam DIRECTION True for forward iteration, false for reverse.
        /// @tparam CONSTNESS True for const access, false for mutable access.
        template<bool DIRECTION, bool CONSTNESS>
        class iterator_
        {
            friend class Ring;
        public:
            using iterator_type = std::random_access_iterator_tag; ///< Iterator category.
            using difference_type = ssize_t;                        ///< Signed distance type.
            using pointer = std::conditional_t<CONSTNESS, Ring::const_pointer, Ring::pointer>;     ///< Pointer type.
            using reference = std::conditional_t<CONSTNESS, Ring::const_reference, Ring::reference>; ///< Reference type.

        private:
            const Ring* _ring; ///< Pointer to the owning Ring.
            size_type _pos;    ///< Logical position within the ring.
            cnt_iterator _ptr; ///< Physical iterator into the container.


        private:
            /// @brief Constructs an iterator at the given logical position.
            /// @param ring Owning ring instance.
            /// @param pos  Logical position (0 = most-recent for DIRECTION=true).
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
            /// @brief Copy constructor.
            /// @param other The iterator to copy from.
            iterator_(const iterator_& other)
                : _ring(other._ring), _pos(other._pos), _ptr(other._ptr)
            {}

        public:
            /// @brief Dereferences the iterator to access the current element.
            /// @return Reference to the current element.
            inline reference operator*() const
            {
                return *_ptr;
            }
            /// @brief Arrow operator for member access on the current element.
            /// @return Pointer to the current element.
            inline pointer operator->() const
            {
                return _ptr;
            }

        private:
            /// @brief Moves _ptr one step toward decreasing logical position (wraps around).
            void decrease()
            {
                if (_ptr == _ring->_last)
                    _ptr = _ring->_first;
                else
                    ++_ptr;
            }
            /// @brief Moves _ptr one step toward increasing logical position (wraps around).
            void increase()
            {
                if (_ptr == _ring->_first)
                    _ptr = _ring->_last;
                else
                    --_ptr;
            }
        public:
            /// @brief Pre-decrement operator.
            /// @return Reference to this iterator after decrement.
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
            /// @brief Post-decrement operator.
            /// @return Copy of this iterator before decrement.
            inline iterator_ operator--(int)
            {
                iterator_ ret = *this;
                ++(*this);
                return ret;
            }
            /// @brief Pre-increment operator.
            /// @return Reference to this iterator after increment.
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
            /// @brief Post-increment operator.
            /// @return Copy of this iterator before increment.
            inline iterator_ operator++(int)
            {
                iterator_ ret = *this;
                --(*this);
                return ret;
            }
            /// @brief Advances the iterator by diff steps.
            /// @param diff Number of steps to advance.
            /// @return Reference to this iterator.
            inline iterator_& operator+=(size_type diff)
            {
                while (diff-- > 0)
                    ++(*this);

                return *this;
            }
            /// @brief Retreats the iterator by diff steps.
            /// @param diff Number of steps to retreat.
            /// @return Reference to this iterator.
            inline iterator_& operator-=(size_type diff)
            {
                while (diff-- > 0)
                    --(*this);

                return *this;
            }
            /// @brief Returns a new iterator advanced by diff steps.
            /// @param diff Number of steps to advance.
            /// @return New iterator pointing diff positions ahead.
            inline iterator_ operator+(size_type diff)
            {
                iterator_ ret = *this;
                while (diff-- > 0)
                    ++ret;

                return ret;
            }
            /// @brief Returns a new iterator retreated by diff steps.
            /// @param diff Number of steps to retreat.
            /// @return New iterator pointing diff positions behind.
            inline iterator_ operator-(size_type diff)
            {
                iterator_ ret = *this;
                while (diff-- > 0)
                    --ret;

                return ret;
            }

            /// @brief Computes the signed distance between two iterators.
            /// @param lhs Left-hand iterator.
            /// @param rhs Right-hand iterator.
            /// @return Signed difference of logical positions.
            friend difference_type operator-(const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos - rhs._pos;
            }
            /// @brief Less-than comparison of two iterators.
            /// @param lhs Left-hand iterator.
            /// @param rhs Right-hand iterator.
            /// @return True if lhs is before rhs.
            friend bool operator< (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos < rhs._pos;
            }
            /// @brief Greater-than comparison of two iterators.
            /// @param lhs Left-hand iterator.
            /// @param rhs Right-hand iterator.
            /// @return True if lhs is after rhs.
            friend bool operator> (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._pos > rhs._pos;
            }

            /// @brief Equality comparison of two iterators.
            /// @param lhs Left-hand iterator.
            /// @param rhs Right-hand iterator.
            /// @return True if both iterators refer to the same position in the same ring.
            friend bool operator== (const iterator_& lhs, const iterator_& rhs)
            {
                return lhs._ring == rhs._ring && lhs._pos == rhs._pos;
            }
            /// @brief Inequality comparison of two iterators.
            /// @param lhs Left-hand iterator.
            /// @param rhs Right-hand iterator.
            /// @return True if the iterators refer to different positions.
            friend bool operator!= (const iterator_& lhs, const iterator_& rhs)
            {
                return !(lhs == rhs);
            }
        };

     public:
         using iterator = iterator_<true, false>;                   ///< Mutable forward iterator.
         using const_iterator = iterator_<true, true>;              ///< Immutable forward iterator.
         using reverse_iterator = iterator_<false, false>;          ///< Mutable reverse iterator.
         using const_reverse_iterator = iterator_<false, true>;     ///< Immutable reverse iterator.
         using difference_type = typename iterator::difference_type; ///< Signed distance type.

    private:

        size_type _size;         ///< Current number of elements in the ring.
        size_type _head;         ///< Logical index of the most recently pushed element.
        cnt_iterator _current;   ///< Physical iterator to the element at _head.

        using RingBase<CNT>::_first;
        using RingBase<CNT>::_last;
        using RingBase<CNT>::_capacity;
    public:

        /// @brief Constructs an empty Ring using the default storage constructor.
        Ring()
            : RingBase<CNT>()
            , _size(0)
            , _head(0)
            , _current(_first)
        {}

        /// @brief Constructs an empty Ring with the given storage capacity.
        /// @param capacity Number of elements the ring can hold.
        Ring(size_type capacity)
            : RingBase<CNT>(capacity)
            , _size(0)
            , _head(0)
            , _current(_first)
        {}
    public:

        /// @brief Constructs an element in place from forwarded arguments and adds it to the ring.
        /// @tparam Args Constructor argument types for value_type.
        /// @param args  Arguments forwarded to the value_type constructor.
        /// @return If PUSH_ON_FULL: reference to the inserted element. Otherwise: bool indicating success.
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

        /// @brief Pushes an rvalue element into the ring.
        /// @param value The value to push (moved into the ring).
        /// @return If PUSH_ON_FULL: reference to the inserted element. Otherwise: bool success flag.
        inline std::conditional_t<PUSH_ON_FULL, reference, bool> push(value_type&& value)
        {
            return emplace(std::move(value));
        }

        /// @brief Pushes a copy of the given value into the ring.
        /// @param value The value to push.
        /// @return If !PUSH_ON_FULL: bool indicating whether the push succeeded.
        inline std::conditional_t<PUSH_ON_FULL, void, bool> push(const value_type& value)
        {
            if constexpr (PUSH_ON_FULL)
                emplace(value);
            else
                return emplace(value);
        }

        /// @brief Removes the most recently pushed element (LIFO) or adjusts size (FIFO).
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


        /// @brief Resets the ring to an empty state without deallocating storage.
        inline void reset()
        {
            _size = 0;
            _head = 0;
            _current = _first;
        }


        /// @brief Resizes the underlying storage and resets the ring.
        /// @param size New capacity.
        inline void resize(size_type size)
        {
            RingBase<CNT>::reset(size);
            reset();
        }

        /// @brief Returns a const reference to the most recently pushed element.
        /// @return Const reference to the front (newest) element.
        inline const_reference front() const
        {
            return (*_current);
        }

        /// @brief Returns a const reference to the oldest element in the ring.
        /// @return Const reference to the back (oldest) element.
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
        /// @brief Returns a const reference to the element at the given logical position.
        /// @param position Zero-based index into the ring (0 = oldest).
        /// @return Const reference to the element at position.
        inline const_reference at(size_type position) const
        {
            assert(position < _size && "Invalid position");
            return *(begin() + position);
        }
        /// @brief Returns a mutable reference to the element at the given logical position.
        /// @param position Zero-based index into the ring (0 = oldest).
        /// @return Reference to the element at position.
        inline reference at(size_type position)
        {
            assert(position < _size && "Invalid position");
            return *(begin() + position);
        }
        /// @brief Returns a const reference to the element at the given index.
        /// @param position Zero-based index.
        /// @return Const reference to the element.
        inline const_reference operator[](size_type position) const
        {
            return at(position);
        }
        /// @brief Returns a mutable reference to the element at the given index.
        /// @param position Zero-based index.
        /// @return Reference to the element.
        inline reference operator[](size_type position)
        {
            return at(position);
        }
        /// @brief Returns the current number of elements in the ring.
        /// @return Current element count.
        inline auto size() const
        {
            return _size;
        }
        /// @brief Checks whether the ring contains no elements.
        /// @return True if empty, false otherwise.
        inline bool empty() const
        {
            return _size == 0;
        }
        /// @brief Checks whether the ring has reached capacity (only meaningful when PUSH_ON_FULL is false).
        /// @return True if the ring is full, false otherwise.
        inline bool full() const
        {
            if constexpr (PUSH_ON_FULL)
                return false;

            return _size == _capacity;
        }
        /// @brief Returns a mutable forward iterator to the oldest element.
        /// @return Forward iterator to the oldest element.
        inline iterator begin()
        {
            return iterator(this);
        }
        /// @brief Returns a mutable forward iterator past the newest element.
        /// @return Forward iterator one past the end.
        inline iterator end()
        {
            return iterator(this, _size);
        }
        /// @brief Returns a const forward iterator to the oldest element.
        /// @return Const forward iterator to the oldest element.
        inline const_iterator begin() const
        {
            return const_iterator(this);
        }
        /// @brief Returns a const forward iterator past the newest element.
        /// @return Const forward iterator one past the end.
        inline const_iterator end() const
        {
            return const_iterator(this, _size);
        }
        /// @brief Returns a const forward iterator to the oldest element.
        /// @return Const forward iterator to the oldest element.
        inline const_iterator cbegin() const
        {
            return const_iterator(this);
        }
        /// @brief Returns a const forward iterator past the newest element.
        /// @return Const forward iterator one past the end.
        inline const_iterator cend() const
        {
            return const_iterator(this, _size);
        }

        /// @brief Returns a mutable reverse iterator to the newest element.
        /// @return Reverse iterator to the newest element.
        inline reverse_iterator rbegin()
        {
            return reverse_iterator(this);
        }
        /// @brief Returns a mutable reverse iterator past the oldest element.
        /// @return Reverse iterator one past the oldest element.
        inline reverse_iterator rend()
        {
            return reverse_iterator(this, _size);
        }

        /// @brief Returns a const reverse iterator to the newest element.
        /// @return Const reverse iterator to the newest element.
        inline const_reverse_iterator crbegin() const
        {
            return reverse_iterator(this);
        }
        /// @brief Returns a const reverse iterator past the oldest element.
        /// @return Const reverse iterator one past the oldest element.
        inline const_reverse_iterator crend() const
        {
            return reverse_iterator(this, _size);
        }
    };

    /// @brief Alias for a fixed-size FIFO queue backed by a cnt::Array.
    /// @tparam T    Element type.
    /// @tparam SIZE Compile-time capacity.
    template<class T, size_t SIZE>
    using FixedQueue = Ring<cnt::Array<T, SIZE>, false, false>;

    /// @brief Alias for a dynamically sized FIFO queue backed by cnt::SafeFullVector.
    /// @tparam T Element type.
    template<class T>
    using FullQueue = Ring<cnt::SafeFullVector<T>, false, false>;
}
