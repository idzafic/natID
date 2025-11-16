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
	template <class T>
	class SafeFullStack
	{
	protected:
		T* _container;
		T* _top;
		td::UINT4 _capacity;
		td::UINT4 _size;

	public:
		typedef T* iterator;
		typedef const T* const_iterator;

		SafeFullStack()
			: _container(nullptr)
			, _top(nullptr)
			, _capacity(0)
			, _size(0)
		{
		}

		SafeFullStack(int initialSize)
			: _container(new T[initialSize])
			, _top(_container)
			, _capacity(initialSize)
			, _size(0)
		{
		}

		~SafeFullStack()
		{
			if (_container)
				delete [] _container;
		}

		T& top() const
		{
			assert(_capacity > 0);
			return *(_top - 1);
		};
        
        T* getCurrent()
        {
            push();
            T* toRet = _top - 1;
            return toRet;
        }

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

		iterator begin() const
		{
			return _container;
		}
		
		const_iterator cbegin() const
		{
			return _container;
		}

		const_iterator end() const
		{
			return _container + _size;
		}
		
		const_iterator cend() const
		{
			return _container + _size;
		}

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
        
        void push()
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

				_capacity = newSize;
            }
			assert(_top);
			++_size;
            ++_top;
        }

		void pop()
		{
			assert(_capacity > 0);
			assert(_size > 0);
			assert(_top != _container);
			_top--;
			--_size;
		};

		bool empty() const
		{
			return (_top == _container);
		};

		bool notEmpty() const
		{
			return (_top != _container);
		};

		void clean()
		{
			_top = _container;
		}

        size_t capacity() const
		{
			return _capacity;
		}

		size_t size() const
		{
			return _size;
		}

		void reset()
		{
			_top = _container;
		}
	};

}
