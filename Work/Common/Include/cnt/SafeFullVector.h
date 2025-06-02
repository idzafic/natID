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
#include <ostream>
#include <td/Types.h>
#include <mem/Utils.h>

namespace mem
{
	template <class CNT, td::UINT4 SINGLEBUFFLEN>
	class CntMemoryManager;
}

namespace cnt
{
	template <typename T, bool EXTERN_ALLOCATOR = false>
	class SafeFullVector
	{
		template <class CNT, td::UINT4 SINGLEBUFFLEN>
		friend class mem::CntMemoryManager;
        
	protected:
		T* _data;
		size_t _size;	
				
		//inline void fastTakeDataFrom(const SafeFullVector&  vect)
		//{
		//	td::fastMemCopy(begin(), vect.begin(), vect.size());			
		//}

	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		typedef T T_DATA;
		typedef T tEntry;

		SafeFullVector ()
			: _data(nullptr)
			, _size(0)
		{}

		SafeFullVector (size_t size)
			: _data(nullptr)
			, _size(0)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, true>");
			assert(!EXTERN_ALLOCATOR);
			if (!EXTERN_ALLOCATOR)
				reserve(size);
		}

		SafeFullVector(T* inputData, size_t lengthOfInputData)
			: _data(inputData)
			, _size(lengthOfInputData)
		{			
			static_assert(EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, false>! This is only for extern alloc...");
			assert(EXTERN_ALLOCATOR);			
		}

		SafeFullVector (const SafeFullVector& vect)
			: _data(nullptr)
			, _size(0)
		{
			
			if (!EXTERN_ALLOCATOR)
			{
				size_t remSize = vect.size();

				if (_size != remSize)
					reserve(remSize);

				//memcpy(_data, vect.getData(), (_size) * sizeof(T));

				for (size_t i = 0; i < _size; i++)
				{
					_data[i] = vect(i);
				}
			}
			else
			{
				_data = vect._data;
				_size = vect._size;
			}
		}
        
        //move constructor
        SafeFullVector(SafeFullVector&& vect)
        {
            _data = vect._data;
            _size = vect._size;
            vect._data = nullptr;
            vect._size = 0;
        }


		~SafeFullVector ()
		{
			if (!EXTERN_ALLOCATOR && _data)
				delete [] _data;
		}
        
        void transferTo(SafeFullVector& vect)
        {
            vect._data = _data;
            vect._size = _size;
            _data = nullptr;
            _size = 0;
        }

		inline void resize(size_t newSize)
		{			
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, true>");
			assert(!EXTERN_ALLOCATOR);
			if (newSize <= _size)
			{
				return;
			}
			
			T* pNew = new T[newSize];

			if (_data)
			{
				//delete [] _data;
				for (size_t i=0; i<_size; ++i)
					pNew[i] = std::move(_data[i]);
				delete [] _data;
			}
			_data = pNew;
			
			_size = newSize;
		}

		void init(T* inputData, size_t lengthOfInputData)
		{
			static_assert(EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, true>");
			assert(EXTERN_ALLOCATOR);
			_data = inputData;
			_size = lengthOfInputData;
		}

		inline void clean()
		{
			if (!EXTERN_ALLOCATOR && _data)
			{			
				delete[] _data;								
			}
			
			_size = 0;
			_data = nullptr;
		}

		template <class TSER>
		void serialize(TSER& s) const
		{
			s << "Size:" << _size << std::endl;
			for (int i=0; i<_size; i++)
				s << _data[i] << " ";
			s << std::endl;
		}

		inline const T& operator [] (size_t pos) const
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		inline T& operator [] (size_t pos)
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		inline const T& operator () (size_t pos) const
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		inline T& operator () (size_t pos)
		{
			assert(pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		inline void zeros()
		{
			if (_size > 0)
                memset(_data, 0, sizeof(T)*_size);
                //mem::fastZero(_data, _size);;
		}
		
		inline void takeDataFrom(const SafeFullVector& vect)
		{			
			int size = vect.size();
			assert(size == _size);
			//reserve(size);
			for (int i=0; i< size; i++)
			{
				_data[i] = vect[i];
			}
		}
        
        inline void takeDataFrom(const T* pData, size_t nElements)
        {
            assert(nElements <= _size);
            size_t nToTake = nElements;
            if (nElements > _size)
                nElements = _size;
            //reserve(size);
            for (int i=0; i< nElements; i++)
            {
                _data[i] = pData[i];
            }
        }

		//void serialize(void* archive, int type = 0);

	public:
	
		inline SafeFullVector& operator = (const SafeFullVector& vect)
		{
			if (!EXTERN_ALLOCATOR)
			{

				//assert(0);
				size_t size = vect.size();
				//assert(size == _size);

				if (_size != size)
					reserve(size);

				//td::fastCopyMem(_data, vect._data, size);
				for (size_t i = 0; i < size; i++)
				{
					_data[i] = vect[i];
				}
			}
			else
			{
				_data = vect._data;
				_size = vect._size;
			}
			return *this;
		}

		inline void setAll(const T& val)
		{
			for (size_t i = 0; i<_size; ++i)
				_data[i] = val;
		}

		inline iterator begin()
		{
			return _data;
		}

		inline const_iterator begin() const
		{
			return _data;
		}

		inline iterator end() 
		{
			return _data + _size;
		}

		inline const_iterator end() const
		{
			return _data + _size;
		}
		
		inline const_iterator cbegin() const
		{
			return _data;
		}

		inline const_iterator cend() const
		{
			return _data + _size;
		}

		inline iterator last() const
		{
			return _data + _size -1;
		}

		inline iterator getRef(size_t pos) const
		{
			return _data + pos;
		}

		inline void RemoveAll( [[maybe_unused]] bool deletePtrs = true)
		{
			//if (deletePtrs)
			//{
			//	for (int i=0; i<_size; i++)
			//	{
			//		if (_data[i])
			//		{
			//			assert(_data[i] != 0);
			//			T pVal = _data[i];
			//			delete pVal;
			//			_data[i] = 0;
			//		}
			//	}
			//}


			//clear();
		}

		inline void reserve(size_t size)
		{
			if (!EXTERN_ALLOCATOR)
			{
				if (_size == size)
					return;

				if (_data)
					delete[] _data;
                
                if (size > 0)
                    _data = new T[size];
                else
                    _data = nullptr;

				_size = size;
			}			
		}

		inline size_t size() const
		{
			return _size;
		}

		inline size_t GetSize() const
		{
			return _size;
		}

		inline size_t GetCount() const
		{
			return _size;
		}

		inline void SetSize(size_t size)
		{
			reserve(size);
		}

#ifdef _SAFE_VECTOR_SERIALIZE
		template <class TArchive>
		void Serialize(TArchive& ar)
		{
			if (ar.IsLoading())
			{
				int size;
				ar >> size;
				reserve(size);
				for (int i=0; i<size; i++)
					ar >> _data[i];
			}
			else
			{
				ar << _size;
				for (size_t i = 0; i<_size; i++)
					ar << _data[i];
			}
		}
#endif

		T* getData()
		{
			return _data;
		}

		const T* getData() const
		{
			return _data;
		}

		inline bool isEmpty() const
		{
			return _size == 0;
		}

		bool find(const T& valToFind) const
		{
			for (size_t i = 0; i < _size; ++i)
			{
				if (_data[i] == valToFind)
					return true;
			}				
			return false;
		}

		bool find(const T& valToFind, size_t& pos) const
		{
			for (size_t i = 0; i < _size; ++i)
			{
				if (_data[i] == valToFind)
				{
					pos = i;
					return true;
				}
					
			}
			return false;
		}

		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const SafeFullVector& rhs)
		{
			for (size_t i = 0; i < rhs._size; ++i)
				os << rhs._data[i] << " ";
			//rhs.show(os);
			return os;
		}

		//template<class CharT, class Traits>
		//friend std::basic_istream<CharT, Traits>&
		//	operator>>(std::basic_istream<CharT, Traits>& is, SafeFullVector& rhs)
		//{
		//	assert(!EXTERN_ALLOCATOR);
		//	std::string str;
		//	os >> str;
		//	rhs = str;
		//	return os;
		//}
	};



	template <typename T>
	class SafeFullList : public SafeFullVector<T>
	{
	protected:
		void populateData(T* data) const
		{
			for (size_t i=0; i< SafeFullVector<T>::_size; i++)
			{
				data[i] = SafeFullVector<T>::_data[i];
			}
		}

	public:
		SafeFullList() : SafeFullVector<T>() {};

		SafeFullList& operator = (const SafeFullList& l2)
		{
			int size = l2.size();
			//this is test

			if (size > 0)
			{
				SafeFullVector<T>::reserve(size);
				l2.populateData(SafeFullVector<T>::_data);
			}
			else
			{
				SafeFullVector<T>::_data = 0;
			}
			return *this;
		}

		bool IsEmpty() const
		{
			return ((SafeFullVector<T>::_size <= 0) ? true: false);
		}

		void Add(const T& val)
		{
			if (SafeFullVector<T>::_size > 0)
			{
				//SafeFullVector<T> tmp(*this);
				//reserve(_size+1);
				T* tmp = new T[SafeFullVector<T>::_size + 1];

				for (size_t i = 0; i<SafeFullVector<T>::_size; i++)
				{
					tmp[i] = SafeFullVector<T>::_data[i];
				}

				delete [] SafeFullVector<T>::_data;

				SafeFullVector<T>::_data = tmp;

				//memcpy(_data, tmp.getData(), (_size - 1) * sizeof(T));
				SafeFullVector<T>::_data[SafeFullVector<T>::_size++] = val;
				//_size++;
			}
			else
			{
				SafeFullVector<T>::reserve(1);
				SafeFullVector<T>::_data[0] = val;
			}
		};

		void AddTail(const T& val)
		{
			Add(val);
		}

		void RemoveAll( [[maybe_unused]] bool deletePtrs = true)
		{
			SafeFullVector<T>::clean();
		}

		void push_back(const T& val)
		{
			Add(val);
		}

		void RemoveAt(int pos)
		{
			assert(SafeFullVector<T>::_size >= 1);

			if (SafeFullVector<T>::_size > 1)
			{
				SafeFullVector<T> tmp(*this);
				reserve(SafeFullVector<T>::_size-1);
				for (int i=0; i< pos; i++)
					SafeFullVector<T>::_data[i]= tmp[i];

				for (size_t i = pos; i< SafeFullVector<T>::_size; i++)
					SafeFullVector<T>::_data[i]= tmp[i+1];
			}
			else
			{
				SafeFullVector<T>::clean();
			}
		};
	};	
}
