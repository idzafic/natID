// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Array.h
    @brief Fixed-size array container with bounds-checked element access. */
#pragma once
#include <cassert>
#include <mem/Utils.h>


namespace cnt
{
	/// @brief Fixed-size array with compile-time size, stored inline without heap allocation.
	/// @tparam T Element type.
	/// @tparam SIZE Number of elements in the array.
	template <typename T, size_t SIZE>
	class Array
	{
		T _data[SIZE]; ///< Internal storage for array elements.
	public:
		/// @brief Default constructor; elements are default-initialized.
		Array(){}

		/// @brief Constructs the array and optionally zeroes all elements.
		/// @param bClean If true, zeroes all elements using mem::fastZero.
        Array(bool bClean)
        {
            if (bClean)
                clean();
        }

		/// @brief Constructs the array from an initializer list; list size must equal SIZE.
		/// @param elems Initializer list whose size must match SIZE.
        Array(const std::initializer_list<T>& elems)
        {
            size_t nElems = elems.size();
            assert(nElems == SIZE);

            size_t i=0;
            for (const T& elem : elems)
            {
                _data[i++] = elem;
            }
        }

//        Array& operator = (const std::initializer_list<T>& elems)
//        {
//            size_t nElems = elems.size();
//            assert(nElems == SIZE);
//
//            size_t i=0;
//            for (const T& elem : elems)
//            {
//                _data[i++] = elem;
//            }
//            return *this;
//        }

		/// @brief Sets every element in the array to the given value.
		/// @param val Value to assign to all elements.
        void setAll(T val)
        {
            for (size_t i=0; i<SIZE; ++i)
                _data[i] = val;
        }

		/// @brief Zeroes all elements using a fast memory zero operation.
		inline void clean()
		{
			mem::fastZero(_data, SIZE);
		}

		/// @brief Returns the number of elements (compile-time constant SIZE).
		/// @return Number of elements in the array.
		inline constexpr size_t size() const
		{
			return SIZE;
		}

		/// @brief Provides read-only indexed access with bounds assertion.
		/// @param pos Zero-based index into the array.
		/// @return Const reference to the element at position pos.
		inline const T& operator [] (size_t pos) const
		{
			assert ( (pos >=0)  && (pos < SIZE));
			return _data[pos];
		}

		/// @brief Provides read-write indexed access with bounds assertion.
		/// @param pos Zero-based index into the array.
		/// @return Reference to the element at position pos.
		inline T& operator [] (size_t pos)
		{
			assert ( (pos >=0)  && (pos < SIZE));
			return _data[pos];
		}

		/// @brief Returns a const pointer to the first element.
		/// @return Const pointer to the beginning of the array.
		const T* begin() const
		{
			return &(_data[0]);
		}

		/// @brief Returns a pointer to the first element.
		/// @return Pointer to the beginning of the array.
		T* begin()
		{
			return &(_data[0]);
		}

		/// @brief Returns a pointer one past the last element.
		/// @return Pointer to one past the end of the array.
		T* end()
		{
			return &(_data[SIZE]);
		}

		/// @brief Returns a const pointer one past the last element.
		/// @return Const pointer to one past the end of the array.
		const T* end() const
		{
			return &(_data[SIZE]);
		}

		/// @brief Returns a const pointer one past the last element (constant end).
		/// @return Const pointer to one past the end of the array.
        const T* cend() const
        {
            return &(_data[SIZE]);
        }
	};
};
