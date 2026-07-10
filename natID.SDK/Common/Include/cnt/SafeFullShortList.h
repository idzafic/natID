// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SafeFullShortList.h
 @brief Short dynamic list that auto-grows on Add, using 16-bit position and size counters. */
#pragma once
#include "td/Types.h"

namespace cnt
{
	/// @brief Dynamic array with 16-bit size counters that expands automatically when elements are added.
	/// @tparam T Element type stored in the list.
	template <typename T>
	class SafeFullShortList
	{
	protected:
		td::WORD _pos;  ///< Current number of elements stored.
		td::WORD _size; ///< Total allocated capacity.
		T* _data;       ///< Pointer to the allocated element buffer.
	public:
		typedef T* iterator;              ///< Mutable iterator type.
		typedef const T* const_iterator;  ///< Const iterator type.

		/// @brief Constructs an empty SafeFullShortList with no allocated memory.
		SafeFullShortList ()
			: _pos (0)
			, _size (0)
			, _data(0)
		{};

		/// @brief Constructs a SafeFullShortList with pre-allocated capacity.
		/// @param size Initial capacity to reserve.
		SafeFullShortList (td::WORD size)
			: _pos (0)
			, _size (0)
			, _data(0)
		{
			reserve(size);
		}

		/// @brief Destroys the list and frees allocated memory.
		~SafeFullShortList ()
		{
			if (_data)
				delete [] _data;;
		}

		/// @brief Copy-constructs a SafeFullShortList from another instance.
		/// @param lst The source list to copy.
		SafeFullShortList (const SafeFullShortList& lst)
			: _pos (0)
			, _size (0)
			, _data(0)
		{
			lst.copyData(*this);
		}

		/// @brief Copies all elements from this list into another list.
		/// @param data Destination list that will receive the copied elements.
		void copyData(SafeFullShortList& data)
		{
			data.reserve(_size);
			for (int i=0; i< _pos; i++)
			{
				data.Add(_data[i]);
			}
		}

		/// @brief Reallocates the buffer to hold exactly size elements and resets the element count.
		/// @param size New capacity.
		void reserve(td::WORD size)
		{
			if (_size != size)
			{
				if (_data)
					delete [] _data;
				_data = new T[ size ];
				_size = size;
			}
			_pos = 0;
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

		/// @brief Returns a const iterator one past the last stored element.
		/// @return Const pointer to one past the last element.
		const_iterator end() const
		{
			return _data + _pos;
		}

		/// @brief Returns a const iterator one past the last stored element.
		/// @return Const pointer to one past the last element.
		const_iterator cend() const
		{
			return _data + _pos;
		}


		//const_iterator cend() const
		//{
		//	return _data + _pos;
		//}

		/// @brief Returns a pointer to one past the end of the allocated capacity.
		/// @return Pointer to the allocated end boundary.
		iterator endHolder() const
		{
			return _data + _size;
		}

		/// @brief Returns a pointer to the element at the given position.
		/// @param pos Zero-based index of the element.
		/// @return Pointer to the element at pos.
		T* getRef(td::WORD pos) const
		{
			assert (_pos >= pos);
			assert (pos >= 0);

			return &_data[pos];
		}

		/// @brief Appends val to the list, growing the buffer by one if it is full.
		/// @param val The value to append.
		void Add(const T& val)
		{
			if (_pos >= _size)
			{
				if (_size == 0)
				{
					_size = 1;
					_pos = 0;
					_data = new T[_size];
				}
				else
				{
					T* tmp = new T[_size + 1];
					for (td::WORD i=0; i < _size; i++)
						tmp[i] = _data[i];
					if (_data)
						delete [] _data;
					_data = tmp;
					++_size;
				}
			}
			_data[_pos++] = val;
		}

		/// @brief Appends val to the list, growing the buffer if full, and returns a pointer to the new element.
		/// @param val The value to append.
		/// @return Pointer to the newly appended element.
		T* push_back(const T& val)
		{
			if (_pos >= _size)
			{
				if (_size == 0)
				{
					_size = 1;
					_pos = 0;
					_data = new T[_size];
				}
				else
				{
					T* tmp = new T[_size + 1];
					for (td::WORD i=0; i < _size; i++)
						tmp[i] = _data[i];
					if (_data)
						delete [] _data;
					_data = tmp;
					++_size;
				}
			}
			_data[_pos++] = val;
			return _data + _pos - 1;
		}

		/// @brief Resets the element count to zero without freeing the buffer.
		void resetPosition()
		{
			_pos = 0;
		}

		/// @brief Returns the number of elements currently stored.
		/// @return Current element count.
		td::WORD size() const
		{
			return _pos;
		}

		/// @brief Returns the total allocated capacity.
		/// @return Maximum number of elements the buffer can hold.
		td::WORD maxSize() const
		{
			return _size;
		}

	};
}
