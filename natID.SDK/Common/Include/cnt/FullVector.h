// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file FullVector.h
    @brief Reference-counted fixed-size vector and a growable list derived from it. */
#pragma once
#include <cassert>
#include <ostream>
#include <cstring> // for memset, memcpy
#include <mem/Utils.h>

namespace mem
{
	/// @brief Forward declaration of the memory manager that can befriend FullVector.
	template <class CNT, td::UINT4 SINGLEBUFFLEN>
	class CntMemoryManager;
}

namespace cnt
{
    /// @brief Fixed-size vector with optional reference counting and external allocator support.
    /// @tparam T                Element type stored in the vector.
    /// @tparam EXTERN_ALLOCATOR If true, the vector wraps externally managed memory and uses reference counting.
    template <typename T, bool EXTERN_ALLOCATOR = false>
    class FullVector
    {
		template <class CNT, td::UINT4 SINGLEBUFFLEN>
		friend class mem::CntMemoryManager;

    protected:
        T* _data; ///< Pointer to the start of the internal memory block (contains header + element data).

        // Helper methods for reference counting
        /// @brief Returns a reference to the size field stored at the start of _data.
        /// @return Reference to the 32-bit size value.
        inline td::UINT4& getSize() const
        {
            assert(_data);
            return *reinterpret_cast<td::UINT4*>(_data);
        }

        /// @brief Returns a reference to the reference-count field stored after the size in _data.
        /// @return Reference to the 32-bit reference count.
        inline td::UINT4& getRefCount() const
        {
            assert(_data);
            return *(reinterpret_cast<td::UINT4*>(_data) + 1);
        }

        /// @brief Returns a pointer to the first element in the data block, skipping the header.
        /// @return Pointer to the first element, or nullptr if _data is null.
        inline T* getDataStart() const
        {
            return _data ? reinterpret_cast<T*>(reinterpret_cast<char*>(_data) + 2 * sizeof(size_t)) : nullptr;
        }

        /// @brief Increments the reference count when _data is non-null.
        inline void incrementRef() const
        {
            if (_data)
                getRefCount()++;
        }

        /// @brief Decrements the reference count and frees memory when it reaches zero.
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

        /// @brief Allocates a new memory block for the given number of elements and initialises them.
        /// @param size Number of elements to allocate.
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
        typedef T* iterator;          ///< Mutable iterator type.
        typedef const T* const_iterator; ///< Immutable iterator type.
        typedef T T_DATA;             ///< Alias for the element type.
        typedef T tEntry;             ///< Alias for the element type (legacy name).

        // Default constructor
        /// @brief Default constructor. Creates an empty vector with no allocated memory.
        FullVector()
            : _data(nullptr)
        {}

        // Size constructor
        /// @brief Constructs a vector and reserves storage for the given number of elements.
        /// @param size Number of elements to allocate.
        FullVector(size_t size)
            : _data(nullptr)
        {
            static_assert(!EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, true>");
            assert(!EXTERN_ALLOCATOR);
            if (!EXTERN_ALLOCATOR)
                reserve(size);
        }

        // External data constructor
        /// @brief Constructs a vector wrapping externally managed memory (EXTERN_ALLOCATOR must be true).
        /// @param inputData          Pointer to the external data buffer.
        /// @param lengthOfInputData  Number of elements pointed to by inputData.
        FullVector(T* inputData, size_t lengthOfInputData)
            : _data(inputData)
        {
            static_assert(EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, false>! This is only for extern alloc...");
            assert(EXTERN_ALLOCATOR);
        }

        // Copy constructor
        /// @brief Copy constructor. For internal allocator: deep-copies data. For external: shares the pointer.
        /// @param vect The vector to copy from.
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
        /// @brief Move constructor. Transfers ownership of the memory block.
        /// @param vect The vector to move from; left empty afterwards.
        FullVector(FullVector&& vect)
            : _data(vect._data)
        {
            vect._data = nullptr;
        }

        // Destructor
        /// @brief Destructor. Decrements the reference count and frees memory if it reaches zero.
        ~FullVector()
        {
            decrementRef();
        }

        // Reserve memory
        /// @brief Reallocates storage to hold exactly size elements, discarding existing content.
        /// @param size The desired number of elements.
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
        /// @brief Grows the vector to newSize, preserving existing elements.
        /// @param newSize New minimum number of elements; must be greater than the current size.
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
        /// @brief Binds the vector to an externally managed data pointer (EXTERN_ALLOCATOR must be true).
        /// @param inputData         New external data pointer.
        /// @param lengthOfInputData Number of elements at inputData.
        void init(T* inputData, size_t lengthOfInputData)
        {
            static_assert(EXTERN_ALLOCATOR, "Cannot call this method on FullVector<T, false>");
            assert(EXTERN_ALLOCATOR);

            decrementRef();
            _data = inputData;
        }

        // Clean up the vector
        /// @brief Releases the memory block and sets the vector to an empty state.
        inline void clean()
        {
            decrementRef();
            _data = nullptr;
        }

        // Get access to elements
        /// @brief Returns a const reference to the element at the given index.
        /// @param pos Zero-based index of the element.
        /// @return Const reference to the element.
        inline const T& operator[](size_t pos) const
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }

        /// @brief Returns a mutable reference to the element at the given index.
        /// @param pos Zero-based index of the element.
        /// @return Mutable reference to the element.
        inline T& operator[](size_t pos)
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }

        /// @brief Returns a const reference to the element at pos (function-call syntax).
        /// @param pos Zero-based index.
        /// @return Const reference to the element.
        inline const T& operator()(size_t pos) const
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }

        /// @brief Returns a mutable reference to the element at pos (function-call syntax).
        /// @param pos Zero-based index.
        /// @return Mutable reference to the element.
        inline T& operator()(size_t pos)
        {
            assert(_data && pos >= 0 && pos < size());
            return getDataStart()[pos];
        }

        // Assignment operator
        /// @brief Copy-assignment operator. Reallocates if sizes differ; shares pointer for external allocator.
        /// @param vect The source vector.
        /// @return Reference to this vector.
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
        /// @brief Sets all bytes of the element storage to zero using memset.
        inline void zeros()
        {
            if (_data)
            {
                T* dataStart = getDataStart();
                memset(dataStart, 0, size() * sizeof(T));
            }
        }

        // Take data from another vector
        /// @brief Copies all elements from vect into this vector (sizes must match).
        /// @param vect Source vector; must have the same size.
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
        /// @brief Copies up to nElements elements from a raw array into this vector.
        /// @param pData     Pointer to the source array.
        /// @param nElements Number of elements to copy; clamped to the vector's size.
        inline void takeDataFrom(const T* pData, size_t nElements)
        {
            assert(_data && nElements <= size());

            size_t count = (nElements > size()) ? size() : nElements;
            T* destData = getDataStart();

            for (size_t i = 0; i < count; ++i)
                destData[i] = pData[i];
        }

        // Set all elements to the same value
        /// @brief Assigns val to every element in the vector.
        /// @param val The value to assign.
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
        /// @brief Returns a mutable iterator to the first element.
        /// @return Pointer to the first element, or nullptr if empty.
        inline iterator begin()
        {
            return _data ? getDataStart() : nullptr;
        }

        /// @brief Returns a const iterator to the first element.
        /// @return Const pointer to the first element, or nullptr if empty.
        inline const_iterator begin() const
        {
            return _data ? getDataStart() : nullptr;
        }

        /// @brief Returns a mutable iterator one past the last element.
        /// @return Pointer past the last element, or nullptr if empty.
        inline iterator end()
        {
            return _data ? getDataStart() + size() : nullptr;
        }

        /// @brief Returns a const iterator one past the last element.
        /// @return Const pointer past the last element, or nullptr if empty.
        inline const_iterator end() const
        {
            return _data ? getDataStart() + size() : nullptr;
        }

        /// @brief Returns a const iterator to the first element.
        /// @return Const pointer to the first element, or nullptr if empty.
        inline const_iterator cbegin() const
        {
            return _data ? getDataStart() : nullptr;
        }

        /// @brief Returns a const iterator one past the last element.
        /// @return Const pointer past the last element, or nullptr if empty.
        inline const_iterator cend() const
        {
            return _data ? getDataStart() + size() : nullptr;
        }

        /// @brief Returns a pointer to the last element.
        /// @return Pointer to the last element, or nullptr if empty.
        inline iterator last() const
        {
            return _data ? getDataStart() + size() - 1 : nullptr;
        }

        /// @brief Returns a pointer to the element at the given position.
        /// @param pos Zero-based index.
        /// @return Pointer to the element at pos, or nullptr if empty.
        inline iterator getRef(size_t pos) const
        {
            return _data ? getDataStart() + pos : nullptr;
        }

        /// @brief No-op provided for interface compatibility; does not remove elements.
        /// @param deletePtrs Unused parameter.
        inline void RemoveAll([[maybe_unused]] bool deletePtrs = true)
        {
            // No implementation, matching the original
        }

        // Size functions
        /// @brief Returns the number of elements in the vector.
        /// @return Element count, or 0 if no storage is allocated.
        inline size_t size() const
        {
            return _data ? getSize() : 0;
        }

        /// @brief Returns the number of elements (alias for size()).
        /// @return Element count.
        inline size_t GetSize() const
        {
            return size();
        }

        /// @brief Returns the number of elements (alias for size()).
        /// @return Element count.
        inline size_t GetCount() const
        {
            return size();
        }

        /// @brief Reserves storage for the given number of elements (alias for reserve()).
        /// @param size Desired capacity.
        inline void SetSize(size_t size)
        {
            reserve(size);
        }

        // Serialization
        /// @brief Serializes the vector's size and elements to the given stream.
        /// @tparam TSER Stream type supporting operator<<.
        /// @param s     The output stream.
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
        /// @brief Returns a raw pointer to the element storage.
        /// @return Mutable pointer to the first element, or nullptr if empty.
        T* getData()
        {
            return _data ? getDataStart() : nullptr;
        }

        /// @brief Returns a const raw pointer to the element storage.
        /// @return Const pointer to the first element, or nullptr if empty.
        const T* getData() const
        {
            return _data ? getDataStart() : nullptr;
        }

        // Check if vector is empty
        /// @brief Checks whether the vector contains no elements.
        /// @return True if the vector is empty or unallocated, false otherwise.
        inline bool isEmpty() const
        {
            return _data == nullptr || size() == 0;
        }

        // Find an element
        /// @brief Searches for the first occurrence of valToFind.
        /// @param valToFind The value to search for.
        /// @return True if the value is found, false otherwise.
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

        /// @brief Searches for the first occurrence of valToFind and records its index.
        /// @param valToFind The value to search for.
        /// @param pos       Output parameter set to the index of the found element.
        /// @return True if found, false otherwise.
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
        /// @brief Writes all elements separated by spaces to an output stream.
        /// @tparam CharT  Character type of the stream.
        /// @tparam Traits Character traits of the stream.
        /// @param os  The output stream.
        /// @param rhs The vector to output.
        /// @return Reference to os.
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
    /// @brief Growable list built on top of FullVector supporting append and remove-at operations.
    /// @tparam T Element type.
    template <typename T>
    class FullList : public FullVector<T>
    {
    protected:
        /// @brief Copies all elements from this list into the provided raw buffer.
        /// @param data Destination buffer; must have capacity >= size().
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
        /// @brief Default constructor. Creates an empty list.
        FullList() : FullVector<T>() {}

        /// @brief Copy-assignment operator. Copies all elements from l2 into this list.
        /// @param l2 The source list.
        /// @return Reference to this list.
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

        /// @brief Checks whether the list contains no elements.
        /// @return True if empty, false otherwise.
        bool isEmpty() const
        {
            return FullVector<T>::isEmpty();
        }

        /// @brief Appends a copy of val to the end of the list.
        /// @param val The value to add.
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

        /// @brief Alias for add(); appends val to the tail of the list.
        /// @param val The value to add.
        void addTail(const T& val)
        {
            add(val);
        }

        /// @brief Clears all elements from the list.
        /// @param deletePtrs Unused parameter kept for interface compatibility.
        void removeAll([[maybe_unused]] bool deletePtrs = true)
        {
            FullVector<T>::clean();
        }

        /// @brief Appends val to the list (STL-compatible name; delegates to Add()).
        /// @param val The value to push back.
        void push_back(const T& val)
        {
            Add(val);
        }

        /// @brief Removes the element at the given position, shifting subsequent elements down.
        /// @param pos Zero-based index of the element to remove.
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
