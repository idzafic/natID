// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SafeFullStack.h
 @brief Dynamically-growing stack container backed by a contiguous array. */
#pragma once
#include <td/Types.h>

namespace cnt
{
	/// @brief Stack (LIFO) container that doubles its capacity when full.
	/// @tparam T Element type stored in the stack.
	template <class T>
	class SafeFullStack
	{
	protected:
		T* _container;         ///< Pointer to the allocated element buffer.
		T* _top;               ///< Pointer to one past the last pushed element.
		td::UINT4 _capacity;   ///< Total allocated element capacity.
		td::UINT4 _size;       ///< Number of elements currently on the stack.

	public:
		typedef T* iterator;             ///< Mutable iterator type.
		typedef const T* const_iterator; ///< Const iterator type.

		/// @brief Constructs an empty SafeFullStack with no allocated memory.
		SafeFullStack()
			: _container(nullptr)
			, _top(nullptr)
			, _capacity(0)
			, _size(0)
		{
		}

		/// @brief Constructs a SafeFullStack with the given initial capacity.
		/// @param initialSize Number of elements to pre-allocate.
		SafeFullStack(int initialSize)
			: _container(new T[initialSize])
			, _top(_container)
			, _capacity(initialSize)
			, _size(0)
		{
		}

		/// @brief Destroys the stack and frees all allocated memory.
		~SafeFullStack()
		{
			if (_container)
				delete [] _container;
		}

		/// @brief Returns a reference to the element at the top of the stack without removing it.
		/// @return Reference to the top element.
		T& top() const
		{
			assert(_capacity > 0);
			return *(_top - 1);
		};

        /// @brief Pushes a default-constructed element onto the stack and returns a pointer to it.
        /// @return Pointer to the newly pushed element.
        T* getCurrent()
        {
            push();
            T* toRet = _top - 1;
            return toRet;
        }

		/// @brief Reallocates the buffer to hold exactly n elements and resets the stack to empty.
		/// @param n New capacity; must be greater than zero.
		void reserve(size_t n)
		{
			assert(n>0);
			//_container.reserve(n);
			if (_capacity != n)
			{
				if (_container)
				{
					delete [] _container;
				}
				_container = new T[n];
				_capacity = td::UINT4(n);
			}
			_top = _container;
		}

		/// @brief Returns an iterator to the bottom element of the stack.
		/// @return Pointer to the first (bottom) element.
		iterator begin() const
		{
			return _container;
		}

		/// @brief Returns a const iterator to the bottom element of the stack.
		/// @return Const pointer to the first (bottom) element.
		const_iterator cbegin() const
		{
			return _container;
		}

		/// @brief Returns a const iterator one past the top element.
		/// @return Const pointer to one past the top element.
		const_iterator end() const
		{
			return _container + _size;
		}

		/// @brief Returns a const iterator one past the top element.
		/// @return Const pointer to one past the top element.
		const_iterator cend() const
		{
			return _container + _size;
		}

		/// @brief Pushes a copy of val onto the top of the stack, doubling capacity if needed.
		/// @param val The value to push.
		void push (const T& val)
		{
			assert(_capacity > 0);

			if (_top == _container + _capacity)
			{
				//resize container
                size_t oldSize = _capacity;

                size_t newSize = oldSize * 2;

				T* tmp = new T[newSize];

				//_container.reserve(newSize);
				//memcpy(_container.begin(), tmp.getData(), (oldSize) * sizeof(T));
				for (size_t i=0; i<oldSize; i++)
				{
					tmp[i] = _container[i];
				}
				if (_container)
					delete [] _container;

				_container = tmp;

				_top = &_container[oldSize];

				_capacity = td::UINT4(newSize);
			}
			assert(_top);

			*_top = val;
			++_size;
			++_top;

		};

        /// @brief Pushes a default-constructed element onto the top of the stack, doubling capacity if needed.
        void push()
        {
            assert(_capacity > 0);


			if (_top == _container + _capacity)
            {
                //resize container
                td::UINT4 oldSize = _capacity;
                td::UINT4 newSize = oldSize * 2;

                T* tmp = new T[newSize];

                //_container.reserve(newSize);
                //memcpy(_container.begin(), tmp.getData(), (oldSize) * sizeof(T));
                for (size_t i=0; i<oldSize; i++)
                {
                    tmp[i] = _container[i];
                }

                if (_container)
                    delete [] _container;

                _container = tmp;

                _top = &_container[oldSize];

				_capacity = newSize;
            }
			assert(_top);
			++_size;
            ++_top;
        }

		/// @brief Removes the top element from the stack.
		void pop()
		{
			assert(_capacity > 0);
			assert(_size > 0);
			assert(_top != _container);
			_top--;
			--_size;
		};

		/// @brief Checks whether the stack is empty.
		/// @return True if no elements are on the stack.
		bool empty() const
		{
			return (_top == _container);
		};

		/// @brief Checks whether the stack has at least one element.
		/// @return True if the stack is not empty.
		bool notEmpty() const
		{
			return (_top != _container);
		};

		/// @brief Removes all elements from the stack without freeing the buffer.
		void clean()
		{
			_top = _container;
		}

        /// @brief Returns the total allocated capacity.
        /// @return Number of elements the buffer can hold.
        size_t capacity() const
		{
			return _capacity;
		}

		/// @brief Returns the number of elements currently on the stack.
		/// @return Element count.
		size_t size() const
		{
			return _size;
		}

		/// @brief Resets the stack to empty without freeing the buffer (alias for clean()).
		void reset()
		{
			_top = _container;
		}

		/// @brief Checks whether pData points to the top element.
		/// @param pData Pointer to compare against the top.
		/// @return True if pData points to the current top element.
		bool isLast(const T* pData) const
		{
			if (_capacity == 0)
				return false;

			const T* last = (_top - 1);
			return (last == pData);
		}

		/// @brief Checks whether the stack has reached its current capacity.
		/// @return True if size equals capacity.
		bool isFull() const
		{
			return (_capacity == _size);
		}

		/// @brief Returns the zero-based position of pData within the stack, or -1 if not found.
		/// @param pData Pointer to the element whose position is requested.
		/// @return Zero-based index, or -1 if out of range.
		int getPosition(const T* pData) const
		{
			if (_capacity == 0)
				return -1;

			if ((pData >= _container) && (pData < _container + _size))
				return int(pData - _container);

			return -1;
		}

		/// @brief Returns a pointer to the element at the given zero-based position.
		/// @param position Zero-based index; must be less than size.
		/// @return Pointer to the element.
		T* getEntry(td::UINT4 position)
		{
			assert(position < _size);
			return _container + position;
		}
	};

}
