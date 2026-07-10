// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CircularBuffer.h
    @brief Fixed-capacity circular (ring) buffer with head/tail tracking. */
#pragma once
#include <cnt/Array.h>

namespace cnt
{
	/// @brief Fixed-capacity circular buffer that stores elements in a ring.
	/// @tparam T     Element type stored in the buffer.
	/// @tparam CAPACITY Maximum number of elements the buffer can hold.
	template <class T, size_t CAPACITY = 4>
	class CircularBuffer
	{
		cnt::Array<T, CAPACITY> _buffer; ///< Underlying storage array.
		size_t _head = 0;                ///< Index of the next write position.
		size_t _tail = 0;		         ///< Index of the next read position.
		bool _full = false;              ///< True when the buffer has reached full capacity.

	public:
		/// @brief Default constructor. Initializes an empty circular buffer.
		CircularBuffer() {}

		/// @brief Inserts an item at the head of the buffer.
		/// @param item The item to insert.
		void put(const T& item)
		{
			if (_full)
			{
				assert(false);
				throw std::runtime_error("CircularBuffer is full!");
			}

			_buffer[_head] = item;

			_head = (_head + 1) % CAPACITY;

			_full = (_head == _tail);
		}

		/// @brief Retrieves and removes the oldest item from the buffer.
		/// @return The oldest item in the buffer.
		T get()
		{
			if (isEmpty())
			{
				assert(false);
				throw std::runtime_error("CircularBuffer is empty!");
			}

			//Read data and advance the tail (we now have a free space)
			auto val = _buffer[_tail];
			_full = false;
			_tail = (_tail + 1) % CAPACITY;
			return val;
		}

		/// @brief Resets the buffer to an empty state without deallocating memory.
		void reset()
		{
			_head = _tail;
			_full = false;
		}

		/// @brief Checks whether the buffer contains no elements.
		/// @return True if the buffer is empty, false otherwise.
		bool isEmpty() const
		{
			//if head and tail are equal, we are empty
			return (!_full && (_head == _tail));
		}

		/// @brief Checks whether the buffer contains at least one element.
		/// @return True if the buffer is not empty, false otherwise.
		bool isNotEmpty() const
		{
			//if head and tail are equal, we are empty
			return (!isEmpty());
		}

		/// @brief Checks whether the buffer has reached its maximum capacity.
		/// @return True if the buffer is full, false otherwise.
		bool isFull() const
		{
			//If tail is ahead the head by 1, we are full
			return _full;
		}

		/// @brief Checks whether the buffer has room for at least one more element.
		/// @return True if the buffer is not full, false otherwise.
		bool isNotFull() const
		{
			return !_full;
		}

		/// @brief Returns the maximum number of elements the buffer can hold.
		/// @return The compile-time capacity of the buffer.
		constexpr size_t capacity() const
		{
			return CAPACITY;
		}

		/// @brief Returns the current number of elements stored in the buffer.
		/// @return The number of elements currently in the buffer.
		size_t size() const
		{
			if (_full)
				return CAPACITY;

			size_t toRet;
			if (_head >= _tail)
			{
				toRet = _head - _tail;
			}
			else
			{
				toRet = CAPACITY + _head - _tail;
			}
			return toRet;
		}
	};
}
