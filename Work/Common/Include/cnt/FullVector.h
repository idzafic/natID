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
#include <cstring> // for memset, memcpy
#include <mem/Utils.h>

namespace mem
{
	template <class CNT, td::UINT4 SINGLEBUFFLEN>
	class CntMemoryManager;
}

namespace cnt
{
    template <typename T, bool EXTERN_ALLOCATOR = false>
    class FullVector
    {
		template <class CNT, td::UINT4 SINGLEBUFFLEN>
		friend class mem::CntMemoryManager;

    protected:
        T* _data;
        
        // Helper methods for reference counting
        inline td::UINT4& getSize() const
        {
            assert(_data);
            return *reinterpret_cast<td::UINT4*>(_data);
        }
        
        inline td::UINT4& getRefCount() const
        {
            assert(_data);
            return *(reinterpret_cast<td::UINT4*>(_data) + 1);
        }
        
        inline T* getDataStart() const
        {
            return _data ? reinterpret_cast<T*>(reinterpret_cast<char*>(_data) + 2 * sizeof(size_t)) : nullptr;
        }
        
        inline void incrementRef() const
        {
            if (_data)
                getRefCount()++;
        }
        
        inline void decrementRef()
        {
			if constexpr(!EXTERN_ALLOCATOR)
			{
				if (_data && --getRefCount() == 0)
            	{
                	delete[] reinterpret_cast<char*>(_data);
                	_data = nullptr;
            	}
			}
        }
        
        inline void allocate(size_t size)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, true>");
            assert(!EXTERN_ALLOCATOR);
            
            if (size == 0)
            {
                _data = nullptr;
                return;
            }
            
            // Allocate memory for header (size + refCount) and data
            char* buffer = new char[2 * sizeof(size_t) + size * sizeof(T)];
            _data = reinterpret_cast<T*>(buffer);
            
            // Initialize size and refCount
            getSize() = size;
            getRefCount() = 1;
            
            // Initialize elements with default constructor
            T* dataStart = getDataStart();
            for (size_t i = 0; i < size; ++i)
                new (&dataStart[i]) T();
        }

    public:
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef T T_DATA;
        typedef T tEntry;

        // Default constructor
        FullVector()
            : _data(nullptr)
        {}

        // Size constructor
        FullVector(size_t size)
            : _data(nullptr)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, true>");
            assert(!EXTERN_ALLOCATOR);
            if (!EXTERN_ALLOCATOR)
                reserve(size);
        }

        // External data constructor
        FullVector(T* inputData, size_t lengthOfInputData)
            : _data(inputData)
        {
            static_assert(EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, false>! This is only for extern alloc...");
            assert(EXTERN_ALLOCATOR);
        }

        // Copy constructor
        FullVector(const FullVector& vect)
            : _data(nullptr)
        {
            if (!EXTERN_ALLOCATOR)
            {
                if (vect._data)
                {
                    size_t vecSize = vect.size();
                    reserve(vecSize);
                    
                    // Copy data from vect
                    T* srcData = vect.getDataStart();
                    T* destData = getDataStart();
                    for (size_t i = 0; i < vecSize; ++i)
                        destData[i] = srcData[i];
                }
            }
            else
            {
                _data = vect._data;
                if (_data)
                    incrementRef();
            }
        }
        
        // Move constructor
        FullVector(FullVector&& vect)
            : _data(vect._data)
        {
            vect._data = nullptr;
        }

        // Destructor
        ~FullVector()
        {
            decrementRef();
        }

        // Reserve memory
        inline void reserve(size_t size)
        {
            if (!EXTERN_ALLOCATOR)
            {
                if (_data && getSize() == size)
                    return;
                
                decrementRef();
                allocate(size);
            }
        }
        
        // Resize the vector
        inline void resize(size_t newSize)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, true>");
            assert(!EXTERN_ALLOCATOR);
            
            if (_data && newSize <= size())
                return;
            
            FullVector temp(*this);
            size_t oldSize = size();
            
            // Allocate new buffer
            reserve(newSize);
            
            // Copy old data
            if (temp._data)
            {
                T* srcData = temp.getDataStart();
                T* destData = getDataStart();
                for (size_t i = 0; i < oldSize; ++i)
                    destData[i] = std::move(srcData[i]);
            }
        }

        // Initialize with external data
        void init(T* inputData, size_t lengthOfInputData)
        {
            static_assert(EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, false>");
            assert(EXTERN_ALLOCATOR);
            
            decrementRef();
            _data = inputData;
        }
        
        // Clean up the vector
        inline void clean()
        {
            decrementRef();
            _data = nullptr;
        }
        
        // Get access to elements
        inline const T& operator[](size_t pos) const
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }
        
        inline T& operator[](size_t pos)
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }
        
        inline const T& operator()(size_t pos) const
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }
        
        inline T& operator()(size_t pos)
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }
        
        // Assignment operator
        inline FullVector& operator=(const FullVector& vect)
        {
            if (this != &vect)
            {
                if (!EXTERN_ALLOCATOR)
                {
                    size_t vecSize = vect.size();
                    if (_data && getSize() != vecSize)
                    {
                        decrementRef();
                        allocate(vecSize);
                    }
                    else if (!_data && vecSize > 0)
                    {
                        allocate(vecSize);
                    }
                    
                    if (vecSize > 0)
                    {
                        // Copy data
                        T* srcData = vect.getDataStart();
                        T* destData = getDataStart();
                        for (size_t i = 0; i < vecSize; ++i)
                            destData[i] = srcData[i];
                    }
                }
                else
                {
                    decrementRef();
                    _data = vect._data;
                    incrementRef();
                }
            }
            return *this;
        }
        
        // Fill vector with zeros
        inline void zeros()
        {
            if (_data)
            {
                T* dataStart = getDataStart();
                memset(dataStart, 0, size() * sizeof(T));
            }
        }
        
        // Take data from another vector
        inline void takeDataFrom(const FullVector& vect)
        {
            assert(_data && vect._data && size() == vect.size());
            
            size_t vecSize = vect.size();
            T* srcData = vect.getDataStart();
            T* destData = getDataStart();
            
            for (size_t i = 0; i < vecSize; ++i)
                destData[i] = srcData[i];
        }
        
        // Take data from an array
        inline void takeDataFrom(const T* pData, size_t nElements)
        {
            assert(_data && nElements <= size());
            
            size_t count = (nElements > size()) ? size() : nElements;
            T* destData = getDataStart();
            
            for (size_t i = 0; i < count; ++i)
                destData[i] = pData[i];
        }
        
        // Set all elements to the same value
        inline void setAll(const T& val)
        {
            if (_data)
            {
                T* dataStart = getDataStart();
                for (size_t i = 0; i < size(); ++i)
                    dataStart[i] = val;
            }
        }
        
        // Iterator functions
        inline iterator begin()
        {
            return _data ? getDataStart() : nullptr;
        }
        
        inline const_iterator begin() const
        {
            return _data ? getDataStart() : nullptr;
        }
        
        inline iterator end()
        {
            return _data ? getDataStart() + size() : nullptr;
        }
        
        inline const_iterator end() const
        {
            return _data ? getDataStart() + size() : nullptr;
        }
        
        inline const_iterator cbegin() const
        {
            return _data ? getDataStart() : nullptr;
        }
        
        inline const_iterator cend() const
        {
            return _data ? getDataStart() + size() : nullptr;
        }
        
        inline iterator last() const
        {
            return _data ? getDataStart() + size() - 1 : nullptr;
        }
        
        inline iterator getRef(size_t pos) const
        {
            return _data ? getDataStart() + pos : nullptr;
        }
        
        inline void RemoveAll([[maybe_unused]] bool deletePtrs = true)
        {
            // No implementation, matching the original
        }
        
        // Size functions
        inline size_t size() const
        {
            return _data ? getSize() : 0;
        }
        
        inline size_t GetSize() const
        {
            return size();
        }
        
        inline size_t GetCount() const
        {
            return size();
        }
        
        inline void SetSize(size_t size)
        {
            reserve(size);
        }
        
        // Serialization
        template <class TSER>
        void serialize(TSER& s) const
        {
            s << "Size:" << size() << std::endl;
            if (_data)
            {
                T* dataStart = getDataStart();
                for (size_t i = 0; i < size(); i++)
                    s << dataStart[i] << " ";
            }
            s << std::endl;
        }
        
        // Data access
        T* getData()
        {
            return _data ? getDataStart() : nullptr;
        }
        
        const T* getData() const
        {
            return _data ? getDataStart() : nullptr;
        }
        
        // Check if vector is empty
        inline bool isEmpty() const
        {
            return _data == nullptr || size() == 0;
        }
        
        // Find an element
        bool find(const T& valToFind) const
        {
            if (_data)
            {
                T* dataStart = getDataStart();
                for (size_t i = 0; i < size(); ++i)
                {
                    if (dataStart[i] == valToFind)
                        return true;
                }
            }
            return false;
        }
        
        bool find(const T& valToFind, size_t& pos) const
        {
            if (_data)
            {
                T* dataStart = getDataStart();
                for (size_t i = 0; i < size(); ++i)
                {
                    if (dataStart[i] == valToFind)
                    {
                        pos = i;
                        return true;
                    }
                }
            }
            return false;
        }
        
        // Stream output
        template<class CharT, class Traits>
        friend std::basic_ostream<CharT, Traits>&
            operator<<(std::basic_ostream<CharT, Traits>& os, const FullVector& rhs)
        {
            if (rhs._data)
            {
                T* dataStart = rhs.getDataStart();
                for (size_t i = 0; i < rhs.size(); ++i)
                    os << dataStart[i] << " ";
            }
            return os;
        }
    };
    
    // Derived SafeFullList implementation that uses FullVector
    template <typename T>
    class FullList : public FullVector<T>
    {
    protected:
        void populateData(T* data) const
        {
            if (FullVector<T>::_data)
            {
                T* srcData = FullVector<T>::getDataStart();
                for (size_t i = 0; i < FullVector<T>::size(); i++)
                {
                    data[i] = srcData[i];
                }
            }
        }

    public:
        FullList() : FullVector<T>() {}

        FullList& operator = (const FullList& l2)
        {
            size_t size = l2.size();

            if (size > 0)
            {
                FullVector<T>::reserve(size);
                l2.populateData(FullVector<T>::getDataStart());
            }
            else
            {
                FullVector<T>::clean();
            }
            return *this;
        }

        bool isEmpty() const
        {
            return FullVector<T>::isEmpty();
        }

        void add(const T& val)
        {
            size_t currentSize = FullVector<T>::size();
            if (currentSize > 0)
            {
                FullList tmp(*this);
                FullVector<T>::reserve(currentSize + 1);
                
                tmp.populateData(FullVector<T>::getDataStart());
                FullVector<T>::getDataStart()[currentSize] = val;
            }
            else
            {
                FullVector<T>::reserve(1);
                FullVector<T>::getDataStart()[0] = val;
            }
        }

        void addTail(const T& val)
        {
            add(val);
        }

        void removeAll([[maybe_unused]] bool deletePtrs = true)
        {
            FullVector<T>::clean();
        }

        void push_back(const T& val)
        {
            Add(val);
        }

        void removePos(int pos)
        {
            size_t currentSize = FullVector<T>::size();
            assert(currentSize >= 1);

            if (currentSize > 1)
            {
                FullList tmp(*this);
                FullVector<T>::reserve(currentSize - 1);
                
                T* srcData = tmp.getDataStart();
                T* destData = FullVector<T>::getDataStart();
                
                for (int i = 0; i < pos; i++)
                    destData[i] = srcData[i];

                for (size_t i = pos; i < currentSize - 1; i++)
                    destData[i] = srcData[i + 1];
            }
            else
            {
                FullVector<T>::clean();
            }
        }
    };
} // namespace cnt