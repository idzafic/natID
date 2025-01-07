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
#include <mem/Utils.h>


namespace cnt
{
	template <typename T, size_t SIZE>
	class Array
	{
		T _data[SIZE];
	public:
		Array(){}
        Array(bool bClean)
        {
            if (bClean)
                clean();
        }
        
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

		inline void clean()
		{
			mem::fastZero(_data, SIZE);
		}

		inline constexpr size_t size() const
		{
			return SIZE;
		}

		inline const T& operator [] (size_t pos) const
		{
			assert ( (pos >=0)  && (pos < SIZE));
			return _data[pos];
		}

		inline T& operator [] (size_t pos)
		{
			assert ( (pos >=0)  && (pos < SIZE));
			return _data[pos];
		}

		const T* begin() const
		{
			return &(_data[0]);
		}

		T* begin()
		{
			return &(_data[0]);
		}
		T* end() 
		{
			return &(_data[SIZE]);
		}
		const T* end() const
		{
			return &(_data[SIZE]);
		}
        
        const T* cend() const
        {
            return &(_data[SIZE]);
        }
	};
};
