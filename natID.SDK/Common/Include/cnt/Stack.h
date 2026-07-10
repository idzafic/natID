// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Stack.h
 @brief Dynamically-growing stack container with configurable growth increment. */
#pragma once
#include <cassert>
#include <td/Types.h>
#include <mem/Utils.h>

namespace cnt
{
	/// @brief LIFO stack backed by a dynamically-allocated array that grows in fixed increments.
	/// @tparam T         Element type.
	/// @tparam INCREMENT Number of elements added when the stack needs to grow.
	template <typename T, int INCREMENT = 16>
	class Stack
	{
	protected:
		T* _data;        ///< Pointer to the allocated element buffer.
		td::INT2 _size;  ///< Total allocated capacity.
		td::INT2 _top;   ///< Index of the top element; -1 when empty.

		/// @brief Allocates a new buffer of the given size without copying existing data.
		/// @param size Number of elements to allocate.
		inline void reserve(int size)
		{
			_data= new T[INCREMENT];
			_size = size;
		}

		/// @brief Grows the buffer by INCREMENT elements, preserving existing data.
		inline void resize()
		{
			td::INT2 newSize = _size + INCREMENT;
			T* newData = new T[newSize];
			if (_size == 0)
			{
				_size = newSize;
				_data = newData;
				return;
			}
			if (_top >= 0)
				mem::fastCopy(newData, _data, _top + 1);
			delete[] _data;
			_data = newData;
			_size = newSize;
		}
	protected:
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const Stack& rhs)
		{
			os << rhs._top + 1;
			for (int i = rhs._top; i >= 0; --i)
				os << " " << rhs._data[i];
			return os;
		}
	public:
		typedef T* iterator;             ///< Mutable iterator type.
		typedef const T* const_iterator; ///< Const iterator type.

		/// @brief Constructs an empty Stack with no allocated memory.
		inline Stack()
			: _data(nullptr)
			, _size(0)
			, _top(-1)
		{
			//int g = 1;
		};

		/// @brief Destroys the stack and frees all allocated memory.
		~Stack()
		{
			if (_data)
				delete [] _data;
		}

		/// @brief Resets the stack to empty without freeing the buffer.
		inline void reset()
		{
			_top = -1;
		}

		/// @brief Returns a reference to the element at the bottom of the stack.
		/// @return Reference to the first (bottom) element.
		inline T& first()
		{
			assert(_data);
			assert(_top >= 0);
			return _data[0];
		}

		/// @brief Checks whether the stack is empty.
		/// @return True if no elements are on the stack.
		inline bool isEmpty() const
		{
			if (_top < 0)
				return true;
			return false;
		}

        /// @brief Checks whether the stack has at least one element.
        /// @return True if the stack contains at least one element.
        inline bool hasEntry() const
        {
            if (_top >= 0)
                return true;
            return false;
        }

        /// @brief Returns the number of elements currently on the stack.
        /// @return Element count.
        td::INT2 size() const
        {
            return _top + 1;
        }

		/// @brief Pushes a default-constructed element onto the stack, growing the buffer if needed.
		/// @return Reference to the newly pushed element.
		inline T& push()
		{
			++_top;
			if (_top == _size)
			{
				resize();
			}
			return _data[_top];
		}

		/// @brief Pushes a copy of val onto the stack, growing the buffer if needed.
		/// @param val The value to push.
		/// @return Reference to the newly pushed element.
		inline T& push(const T& val)
		{
			T& inStack = push();
			inStack = val;
			return inStack;
		}

		/// @brief Removes the top element from the stack.
		inline void pop()
		{
            assert(_top >= 0);
            assert(_data != nullptr);

			--_top;
		}

		/// @brief Searches for val in the stack and returns a pointer to the first match.
		/// @param val The value to search for.
		/// @return Const pointer to the matching element, or nullptr if not found.
		inline const T* exists(const T& val) const
		{
			for (td::INT2 i = 0; i <= _top; ++i)
			{
				if (_data[i] == val)
					return _data + i;
			}
			return nullptr;
		}

		//inline T* exists(unsigned int hashNo, int nLen) const
		//{
		//	for (int i = 0; i <= _top; ++i)
		//	{
		//		if ((_data[i].hashNo == hashNo) && (_data[i].pName->length() == nLen))
		//			return _data + i;
		//	}
		//	return nullptr;
		//}

		//inline T* exists(unsigned int hashNo) const
		//{
		//	for (int i = 0; i <= _top; ++i)
		//	{
		//		if (_data[i].hashNo == hashNo)
		//			return _data + i;
		//	}
		//	return nullptr;
		//}

		//inline T* exists(unsigned int hashNo, const char* str) const
		//{
		//	for (int i = 0; i <= _top; ++i)
		//	{
		//		if ((_data[i].hashNo == hashNo) && (strcmp(_data[i].pName->c_str(), str) == 0))
		//			return _data + i;
		//	}
		//	return nullptr;
		//}

		/// @brief Returns a reference to the top element without removing it.
		/// @return Reference to the top element.
		inline T& top() const
		{
			assert(_top >= 0);
			return _data[_top];
		}

		/// @brief Frees the buffer and resets the stack to an unallocated state.
		inline void clean()
		{
			if (_data)
			{
				delete [] _data;
				_data = nullptr;
				_top = 0;
				_size = 0;
			}
		}

		/// @brief Returns an iterator to the bottom element.
		/// @return Pointer to the first element.
		inline iterator begin()
		{
			return _data;
		}

		/// @brief Returns a const iterator to the bottom element.
		/// @return Const pointer to the first element.
		inline const_iterator begin() const
		{
			return _data;
		}

		/// @brief Returns a const iterator to the bottom element.
		/// @return Const pointer to the first element.
		inline const_iterator cbegin() const
		{
			return _data;
		}

		/// @brief Returns a const iterator one past the top element.
		/// @return Const pointer to one past the top element.
		inline const_iterator end() const
		{
			return _data + _top + 1;
		}

		/// @brief Returns a const iterator one past the top element.
		/// @return Const pointer to one past the top element.
		inline const_iterator cend() const
		{
			return _data + _top + 1;
		}
	};
}
