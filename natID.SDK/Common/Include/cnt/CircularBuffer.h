// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cnt/Array.h>

namespace cnt
{
	template <class T, size_t CAPACITY = 4>
	class CircularBuffer 
	{
		cnt::Array<T, CAPACITY> _buffer;
		size_t _head = 0;
		size_t _tail = 0;		
		bool _full = false;

	public:
		CircularBuffer() {}
		
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

		void reset()
		{
			_head = _tail;
			_full = false;
		}

		bool isEmpty() const
		{
			//if head and tail are equal, we are empty
			return (!_full && (_head == _tail));
		}
		bool isNotEmpty() const
		{
			//if head and tail are equal, we are empty
			return (!isEmpty());
		}

		bool isFull() const
		{
			//If tail is ahead the head by 1, we are full
			return _full;
		}

		bool isNotFull() const
		{
			return !_full;
		}

		constexpr size_t capacity() const
		{
			return CAPACITY;
		}

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
