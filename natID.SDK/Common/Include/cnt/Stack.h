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
#include <td/Types.h>
#include <mem/Utils.h>

namespace cnt
{
	template <typename T, int INCREMENT = 16>
	class Stack
	{
	protected:		
		T* _data;
		td::INT2 _size;
		td::INT2 _top;
		inline void reserve(int size)
		{
			_data= new T[INCREMENT];
			_size = size;
		}

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
		typedef T* iterator;
		typedef const T* const_iterator;

		inline Stack()
			: _data(nullptr)
			, _size(0)
			, _top(-1)
		{
			//int g = 1;
		};		

		~Stack()
		{
			if (_data)
				delete [] _data;
		}

		inline void reset()
		{
			_top = -1;
		}

		inline T& first()
		{
			assert(_data);
			assert(_top >= 0);
			return _data[0];
		}

		inline bool isEmpty() const
		{
			if (_top < 0)
				return true;
			return false;
		}
        
        inline bool hasEntry() const
        {
            if (_top >= 0)
                return true;
            return false;
        }
        
        td::INT2 size() const
        {
            return _top + 1;
        }

		inline T& push()
		{
			++_top;
			if (_top == _size)
			{
				resize();
			}
			return _data[_top];
		}

		inline T& push(const T& val)
		{
			T& inStack = push();
			inStack = val;
			return inStack;
		}		

		inline void pop()
		{
            assert(_top >= 0);
            assert(_data != nullptr);

			--_top;
		}

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

		inline T& top() const
		{
			assert(_top >= 0);
			return _data[_top];
		}

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

		inline iterator begin()
		{
			return _data;
		}

		inline const_iterator begin() const
		{
			return _data;
		}
		
		inline const_iterator cbegin() const
		{
			return _data;
		}

		inline const_iterator end() const
		{
			return _data + _top + 1;
		}
		
		inline const_iterator cend() const
		{
			return _data + _top + 1;
		}
	};
}
