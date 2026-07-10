// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FastStack.h
    @brief Lightweight stack with raw pointer arithmetic and no bounds checking. */
#pragma once
namespace cnt
{
	/// @brief A fast, minimalist stack backed by a heap-allocated array.
	/// @tparam TStackVal The type of elements stored in the stack.
	template <class TStackVal>
	class FastStack
	{
		TStackVal* _entries; ///< Pointer to the base of the allocated storage array.
		TStackVal* _top;     ///< Pointer to the current top element.
	public:

		/// @brief Constructs a FastStack with the given element capacity.
		/// @param size Maximum number of elements the stack can hold.
		FastStack(int size)
		{
			_entries = new TStackVal[size+1];
			_top = _entries;
		};

		/// @brief Advances the top pointer without writing a value.
		void push()
		{
			_top++;
		};

		/// @brief Pushes a copy of the given value onto the top of the stack.
		/// @param val The value to push.
		void push(TStackVal& val)
		{
			_top++;
			*_top = val;
		};

		/// @brief Removes the top element by moving the top pointer down.
		void pop()
		{
			_top--;
		}

		/// @brief Checks whether the stack contains no elements.
		/// @return True if the stack is empty, false otherwise.
		bool isEmpty()
		{
			return (_top <= _entries) ? true : false;
		};

		/// @brief Returns a pointer to the current top element.
		/// @return Pointer to the top element.
		TStackVal* top()
		{
			return _top;
		}
	public:

		/// @brief Destructor. Releases the underlying storage array.
		~FastStack(void)
		{
			delete [] _entries;
		};
	};
}
