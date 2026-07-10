// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SafeFullVector.h
 @brief Fixed-capacity or externally-managed vector and an array-like list built on top of it. */
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
	/// @brief Contiguous-array container with optional external memory ownership.
	/// @tparam T                Element type.
	/// @tparam EXTERN_ALLOCATOR When true the container does not allocate or free memory itself.
	template <typename T, bool EXTERN_ALLOCATOR = false>
	class SafeFullVector
	{
		template <class CNT, td::UINT4 SINGLEBUFFLEN>
		friend class mem::CntMemoryManager;

	protected:
		T* _data;      ///< Pointer to the element buffer.
		size_t _size;  ///< Number of elements the buffer holds.

		//inline void fastTakeDataFrom(const SafeFullVector&  vect)
		//{
		//	td::fastMemCopy(begin(), vect.begin(), vect.size());
		//}

	public:
		typedef T* iterator;              ///< Mutable iterator type.
		typedef const T* const_iterator;  ///< Const iterator type.
		typedef T T_DATA;                 ///< Element type alias.
		typedef T tEntry;                 ///< Entry type alias.

		/// @brief Constructs an empty SafeFullVector with no allocated memory.
		SafeFullVector ()
			: _data(nullptr)
			, _size(0)
		{}

		/// @brief Constructs a SafeFullVector and allocates memory for size elements.
		/// @param size Number of elements to allocate.
		SafeFullVector (size_t size)
			: _data(nullptr)
			, _size(0)
		{
			static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, true>");
			assert(!EXTERN_ALLOCATOR);
			if (!EXTERN_ALLOCATOR)
				reserve(size);
		}

		/// @brief Constructs a SafeFullVector wrapping an externally allocated buffer.
		/// @param inputData           Pointer to the externally managed buffer.
		/// @param lengthOfInputData   Number of elements in the external buffer.
		SafeFullVector(T* inputData, size_t lengthOfInputData)
			: _data(inputData)
			, _size(lengthOfInputData)
		{
			static_assert(EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, false>! This is only for extern alloc...");
			assert(EXTERN_ALLOCATOR);
		}

		//SafeFullVector (const SafeFullVector& vect)
		//	: _data(nullptr)
		//	, _size(0)
		//{
		//
		//	if constexpr (!EXTERN_ALLOCATOR)
		//	{
		//		size_t remSize = vect.size();

		//		if (_size != remSize)
		//			reserve(remSize);

		//		for (size_t i = 0; i < _size; i++)
		//		{
		//			_data[i] = std::move(vect(i));
		//		}
		//	}
		//	else
		//	{
		//		_data = vect._data;
		//		_size = vect._size;
		//	}
		//}

        //move constructor
        /// @brief Move-constructs a SafeFullVector from another instance, taking ownership of its buffer.
        /// @param vect Source vector to move from.
        SafeFullVector(SafeFullVector&& vect)
        {
            _data = vect._data;
            _size = vect._size;
            vect._data = nullptr;
            vect._size = 0;
        }


		/// @brief Destroys the vector and frees the buffer if it owns it.
		~SafeFullVector ()
		{
			if (!EXTERN_ALLOCATOR && _data)
				delete [] _data;
		}

        /// @brief Transfers ownership of this vector's buffer to another SafeFullVector.
        /// @param vect Destination vector that will take ownership.
        void transferTo(SafeFullVector& vect)
        {
            vect._data = _data;
            vect._size = _size;
            _data = nullptr;
            _size = 0;
        }

		/// @brief Grows the buffer to newSize elements, preserving existing data.
		/// @param newSize New minimum size; no-op if newSize <= current size.
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

		/// @brief Initialises an externally-allocated vector by setting its data pointer and size.
		/// @param inputData          Pointer to the external buffer.
		/// @param lengthOfInputData  Number of elements in the buffer.
		void init(T* inputData, size_t lengthOfInputData)
		{
			static_assert(EXTERN_ALLOCATOR, "Cannot call this method on SafeFullVector<T, true>");
			assert(EXTERN_ALLOCATOR);
			_data = inputData;
			_size = lengthOfInputData;
		}

		/// @brief Frees the owned buffer and resets size to zero.
		inline void clean()
		{
			if (!EXTERN_ALLOCATOR && _data)
			{
				delete[] _data;
			}

			_size = 0;
			_data = nullptr;
		}

		/// @brief Writes a textual representation of the vector to the given stream.
		/// @tparam TSER Stream type supporting operator<<.
		/// @param  s    Output stream.
		template <class TSER>
		void serialize(TSER& s) const
		{
			s << "Size:" << _size << std::endl;
			for (int i=0; i<_size; i++)
				s << _data[i] << " ";
			s << std::endl;
		}

		/// @brief Provides const indexed access to elements.
		/// @param pos Zero-based index.
		/// @return Const reference to the element at pos.
		inline const T& operator [] (size_t pos) const
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		/// @brief Provides mutable indexed access to elements.
		/// @param pos Zero-based index.
		/// @return Reference to the element at pos.
		inline T& operator [] (size_t pos)
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		/// @brief Provides const call-style indexed access to elements.
		/// @param pos Zero-based index.
		/// @return Const reference to the element at pos.
		inline const T& operator () (size_t pos) const
		{
			assert (pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		/// @brief Provides mutable call-style indexed access to elements.
		/// @param pos Zero-based index.
		/// @return Reference to the element at pos.
		inline T& operator () (size_t pos)
		{
			assert(pos >= 0 && pos < _size && _data);
			return *(_data + pos);
		}

		/// @brief Sets all elements to zero using memset.
		inline void zeros()
		{
			if (_size > 0)
                memset(_data, 0, sizeof(T)*_size);
                //mem::fastZero(_data, _size);;
		}

		/// @brief Copies all elements from another SafeFullVector of the same size.
		/// @param vect Source vector; must have the same size.
		inline void takeDataFrom(const SafeFullVector& vect)
		{
			auto size = vect.size();
			assert(size == _size);
			//reserve(size);
			for (decltype(size) i=0; i < size; i++)
			{
				_data[i] = vect[i];
			}
		}

        /// @brief Copies nElements elements from a raw array into this vector.
        /// @param pData     Pointer to the source data.
        /// @param nElements Number of elements to copy; clamped to the current size.
        inline void takeDataFrom(const T* pData, size_t nElements)
        {
            assert(nElements <= _size);
            size_t nToTake = nElements;
            if (nElements > _size)
                nElements = _size;
            //reserve(size);
            for (size_t i=0; i< nElements; i++)
            {
                _data[i] = pData[i];
            }
        }

		//void serialize(void* archive, int type = 0);

	public:

		/// @brief Copy-assigns from another SafeFullVector, resizing this vector if needed.
		/// @param vect Source vector.
		/// @return Reference to this object.
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

		/// @brief Sets every element in the vector to val.
		/// @param val The value to assign to all elements.
		inline void setAll(const T& val)
		{
			for (size_t i = 0; i<_size; ++i)
				_data[i] = val;
		}

		/// @brief Returns an iterator to the first element.
		/// @return Pointer to the first element.
		inline iterator begin()
		{
			return _data;
		}

		/// @brief Returns a const iterator to the first element.
		/// @return Const pointer to the first element.
		inline const_iterator begin() const
		{
			return _data;
		}

		/// @brief Returns an iterator one past the last element.
		/// @return Pointer to one past the last element.
		inline iterator end()
		{
			return _data + _size;
		}

		/// @brief Returns a const iterator one past the last element.
		/// @return Const pointer to one past the last element.
		inline const_iterator end() const
		{
			return _data + _size;
		}

		/// @brief Returns a const iterator to the first element.
		/// @return Const pointer to the first element.
		inline const_iterator cbegin() const
		{
			return _data;
		}

		/// @brief Returns a const iterator one past the last element.
		/// @return Const pointer to one past the last element.
		inline const_iterator cend() const
		{
			return _data + _size;
		}

		/// @brief Returns an iterator pointing to the last element.
		/// @return Pointer to the last element.
		inline iterator last() const
		{
			return _data + _size -1;
		}

		/// @brief Returns a pointer to the element at the given position.
		/// @param pos Zero-based index.
		/// @return Pointer to the element at pos.
		inline iterator getRef(size_t pos) const
		{
			return _data + pos;
		}

		/// @brief Stub for clearing all elements; currently a no-op.
		/// @param deletePtrs Unused parameter retained for API compatibility.
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

		/// @brief Reallocates the buffer to hold exactly size elements.
		/// @param size New capacity; existing contents are discarded.
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

		/// @brief Returns the number of elements in the vector.
		/// @return Element count.
		inline size_t size() const
		{
			return _size;
		}

		/// @brief Returns the number of elements in the vector (legacy alias).
		/// @return Element count.
		inline size_t GetSize() const
		{
			return _size;
		}

		/// @brief Returns the number of elements in the vector (legacy alias).
		/// @return Element count.
		inline size_t GetCount() const
		{
			return _size;
		}

		/// @brief Resizes the buffer via reserve (legacy alias).
		/// @param size New capacity.
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

		/// @brief Returns a raw pointer to the underlying element buffer.
		/// @return Pointer to the buffer.
		T* getData()
		{
			return _data;
		}

		/// @brief Returns a const raw pointer to the underlying element buffer.
		/// @return Const pointer to the buffer.
		const T* getData() const
		{
			return _data;
		}

		/// @brief Checks whether the vector contains no elements.
		/// @return True if size is zero.
		inline bool isEmpty() const
		{
			return _size == 0;
		}

		/// @brief Searches for valToFind and returns true if found.
		/// @param valToFind The value to search for.
		/// @return True if the value is present in the vector.
		bool find(const T& valToFind) const
		{
			for (size_t i = 0; i < _size; ++i)
			{
				if (_data[i] == valToFind)
					return true;
			}
			return false;
		}

		/// @brief Searches for valToFind and returns its position via an output parameter.
		/// @param valToFind The value to search for.
		/// @param pos       Receives the zero-based index of the first match.
		/// @return True if the value was found; pos is set only on success.
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



	/// @brief Dynamic-growth list built on SafeFullVector that supports append and remove-at operations.
	/// @tparam T Element type.
	template <typename T>
	class SafeFullList : public SafeFullVector<T>
	{
	protected:
		/// @brief Copies all elements into the provided raw array.
		/// @param data Destination array large enough to hold all elements.
		void populateData(T* data) const
		{
			for (size_t i=0; i< SafeFullVector<T>::_size; i++)
			{
				data[i] = SafeFullVector<T>::_data[i];
			}
		}

	public:
		/// @brief Constructs an empty SafeFullList.
		SafeFullList() : SafeFullVector<T>() {};

		/// @brief Copy-assigns from another SafeFullList.
		/// @param l2 Source list.
		/// @return Reference to this object.
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

		/// @brief Checks whether the list is empty.
		/// @return True if the list contains no elements.
		bool IsEmpty() const
		{
			return ((SafeFullVector<T>::_size <= 0) ? true: false);
		}

		/// @brief Appends val to the list, growing the buffer by one element.
		/// @param val The value to append.
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

		/// @brief Appends val to the tail of the list (alias for Add).
		/// @param val The value to append.
		void AddTail(const T& val)
		{
			Add(val);
		}

		/// @brief Removes all elements from the list.
		/// @param deletePtrs Unused; retained for API compatibility.
		void RemoveAll( [[maybe_unused]] bool deletePtrs = true)
		{
			SafeFullVector<T>::clean();
		}

		/// @brief Appends val to the list (alias for Add).
		/// @param val The value to append.
		void push_back(const T& val)
		{
			Add(val);
		}

		/// @brief Removes the element at the given zero-based index.
		/// @param pos Zero-based index of the element to remove.
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
