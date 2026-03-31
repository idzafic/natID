// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "td/Types.h"

namespace cnt
{
	template <typename T>
	class SafeFullShortList
	{
	protected:
		td::WORD _pos;
		td::WORD _size;
		T* _data;
	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		SafeFullShortList ()
			: _pos (0)
			, _size (0)
			, _data(0)
		{};

		SafeFullShortList (td::WORD size)
			: _pos (0)
			, _size (0)
			, _data(0)
		{
			reserve(size);
		}

		~SafeFullShortList ()
		{
			if (_data)
				delete [] _data;;
		}

		SafeFullShortList (const SafeFullShortList& lst)
			: _pos (0)
			, _size (0)
			, _data(0)
		{
			lst.copyData(*this);
		}

		void copyData(SafeFullShortList& data)
		{
			data.reserve(_size);
			for (int i=0; i< _pos; i++)
			{
				data.Add(_data[i]);
			}
		}

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

		iterator begin()
		{
			return _data;
		}
		
		const_iterator begin() const
		{
			return _data;
		}
		
		const_iterator cbegin() const
		{
			return _data;
		}

		const_iterator end() const
		{
			return _data + _pos;
		}
		
		const_iterator cend() const
		{
			return _data + _pos;
		}
		

		//const_iterator cend() const
		//{
		//	return _data + _pos;
		//}

		iterator endHolder() const
		{
			return _data + _size;
		}

		T* getRef(td::WORD pos) const
		{
			assert (_pos >= pos);
			assert (pos >= 0);

			return &_data[pos];
		}

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

		void resetPosition()
		{
			_pos = 0;
		}

		td::WORD size() const
		{
			return _pos;
		}

		td::WORD maxSize() const
		{
			return _size;
		}

	};
}