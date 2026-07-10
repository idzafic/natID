// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Vector.h
 @brief Dynamic array with inline size/capacity header, pluggable allocator policy, and a pool-backed memory manager. */
#pragma once
#include <cassert>
#include <initializer_list>
#include <cstring> // for memset
#include <utility> // for std::move
//#include <functional> // for std::function

namespace cnt
{

// Forward declaration of the memory manager class
/// @brief Forward declaration of the memory manager used by ExternalAllocator.
/// @tparam T          Element type managed by the pool.
/// @tparam BLOCK_SIZE Size in bytes of each memory block in the pool.
template <typename T, td::UINT4 BLOCK_SIZE>
class MemoryManager;

// Null allocator policy - default behavior (internal allocation)
/// @brief Allocator policy tag indicating that Vector should use standard heap allocation.
struct NullAllocator {};

// External allocator policy that uses MemoryManager
/// @brief Allocator policy that delegates memory operations to a MemoryManager instance.
/// @tparam T          Element type.
/// @tparam BLOCK_SIZE Block size used by the associated MemoryManager.
template <typename T, td::UINT4 BLOCK_SIZE>
class ExternalAllocator {
private:
    MemoryManager<T, BLOCK_SIZE>* _memoryManager; ///< Pointer to the memory manager that owns the pool.

public:
    /// @brief Constructs an ExternalAllocator wrapping the given memory manager.
    /// @param manager Pointer to the MemoryManager; must not be null.
    explicit ExternalAllocator(MemoryManager<T, BLOCK_SIZE>* manager)
    : _memoryManager(manager) {
        assert(_memoryManager != nullptr && "Memory manager must not be null");
    }

    /// @brief Copy-constructs from another ExternalAllocator sharing the same manager.
    /// @param other Source allocator.
    ExternalAllocator(const ExternalAllocator& other)
    : _memoryManager(other._memoryManager) {}

    /// @brief Copy-assigns from another ExternalAllocator.
    /// @param other Source allocator.
    /// @return Reference to this object.
    ExternalAllocator& operator=(const ExternalAllocator& other) {
        _memoryManager = other._memoryManager;
        return *this;
    }

    /// @brief Allocates a block that can hold capacity elements plus a header.
    /// @param capacity Number of elements the block should accommodate.
    /// @return Pointer to the allocated raw byte buffer.
    unsigned char* allocate(td::UINT4 capacity) const {
        assert(_memoryManager != nullptr && "Memory manager must be initialized before use");
        return _memoryManager->allocateBlock(capacity);
    }

    /// @brief Returns a previously allocated block to the memory manager.
    /// @param data Pointer to the block to release.
    void deallocate(unsigned char* data) const {
        assert(_memoryManager != nullptr && "Memory manager must be initialized before use");
        _memoryManager->releaseBlock(data);
    }

    // Gets the memory manager
    /// @brief Returns the associated MemoryManager.
    /// @return Pointer to the MemoryManager.
    MemoryManager<T, BLOCK_SIZE>* getMemoryManager() const {
        return _memoryManager;
    }
};

/// @brief Dynamic array whose size and capacity are stored inline before the element data.
/// @tparam T           Element type.
/// @tparam INCREMENT   Growth step; 0 means double the capacity.
/// @tparam AllocPolicy Allocation policy type (NullAllocator or ExternalAllocator).
template <typename T, size_t INCREMENT = 0, typename AllocPolicy = NullAllocator>
class Vector
{
protected:
    unsigned char* _data = nullptr; ///< Pointer to the raw buffer: [UINT4 size][UINT4 capacity][T...].

    // Helper methods to access header information
    /// @brief Returns a reference to the size field stored at the start of the buffer.
    /// @return Mutable reference to the element count.
    inline td::UINT4& size_ref() const
    {
        assert(_data != nullptr);
        return *reinterpret_cast<td::UINT4*>(_data);
    }

    /// @brief Returns a reference to the capacity field stored after the size in the buffer.
    /// @return Mutable reference to the capacity.
    inline td::UINT4& capacity_ref() const
    {
        assert(_data != nullptr);
        return *reinterpret_cast<td::UINT4*>(_data + sizeof(td::UINT4));
    }

    // Get pointer to the actual array of elements
    /// @brief Returns a pointer to the first element in the buffer.
    /// @return Pointer to the element array, or nullptr if the buffer is null.
    inline T* elements() const
    {
        if (_data == nullptr)
            return nullptr;
        return reinterpret_cast<T*>(_data + 2 * sizeof(td::UINT4));
    }

    // Helper to allocate memory with proper structure
    /// @brief Allocates a raw buffer for capacity elements and initialises the header.
    /// @param capacity Number of elements the new buffer should hold.
    void allocate(td::UINT4 capacity)
    {
        assert(capacity > 0);

        if constexpr (std::is_same_v<AllocPolicy, NullAllocator>) {
            // Standard internal allocation
            // Calculate total memory needed: header (8 bytes) + elements
            size_t totalBytes = 2 * sizeof(td::UINT4) + capacity * sizeof(T);
            _data = new unsigned char[totalBytes];

            // Initialize header
            size_ref() = 0;
            capacity_ref() = td::UINT4(capacity);

            // Zero out the elements area
            memset(elements(), 0, capacity * sizeof(T));
        }
        else
        {
            // Use external memory manager through the allocator instance
            _data = getAllocator().allocate(capacity);
            assert(_data != nullptr);

            // Memory from manager already has capacity set, just initialize size
            size_ref() = 0;
        }
    }

    // Helper to get allocator instance (for non-NullAllocator case)
    template <typename A = AllocPolicy>
    typename std::enable_if<!std::is_same_v<A, NullAllocator>, const A&>::type
    getAllocator() const {
        static_assert(sizeof(AllocPolicy) > 0, "AllocPolicy must be complete");
        return static_cast<const AllocPolicy&>(*this);
    }

public:
    typedef T* iterator;              ///< Mutable iterator type.
    typedef const T* const_iterator;  ///< Const iterator type.
    typedef T T_DATA;                 ///< Element type alias.
    typedef T* pointer;               ///< Pointer type alias.

    // Default constructor
    /// @brief Constructs an empty Vector with no allocated buffer.
    Vector() : _data(nullptr) {}

    // Copy constructor
    /// @brief Copy-constructs a Vector from another instance.
    /// @param v Source Vector to copy.
    Vector(const Vector<T, INCREMENT, AllocPolicy>& v) : _data(nullptr)
    {
        operator=(v);
    }

    // Move constructor
    /// @brief Move-constructs a Vector from another instance, potentially with a different policy.
    /// @tparam INCREMENT2   Increment of the source vector.
    /// @tparam AllocPolicy2 Allocator policy of the source vector.
    /// @param  v            Source Vector to move from.
    template <size_t INCREMENT2, typename AllocPolicy2>
    Vector(Vector<T, INCREMENT2, AllocPolicy2>&& v)
    {
        if (v._data == nullptr)
        {
            _data = nullptr;
            return;
        }

        if constexpr (std::is_same_v<AllocPolicy, AllocPolicy2>) {
            // Same allocator policy, can just move the pointer
            _data = v._data;
            v._data = nullptr;
        }
        else {
            // Different allocator policies, need to copy data
            size_t nSize = v.size();
            if (nSize > 0) {
                reserve(nSize);
                for (size_t i = 0; i < nSize; ++i)
                    push_back(std::move(v[i]));
            }
            v.release();
        }
    }

    // Constructor with size
    /// @brief Constructs a Vector with pre-allocated capacity for n elements.
    /// @param n Number of elements to pre-allocate.
    Vector(size_t n) : _data(nullptr)
    {
        if (n > 0)
            reserve(n);
    }

    // Constructor with initializer list
    /// @brief Constructs a Vector from an initializer list.
    /// @param elems Elements to append.
    Vector(const std::initializer_list<T>& elems) : _data(nullptr)
    {
        auto n = elems.size();
        if (n > 0) {
            reserve(n);
            for (const auto& e : elems) {
                push_back(e);
            }
        }
    }

    // Assignment operator
    /// @brief Copy-assigns from a Vector with a potentially different increment/policy.
    /// @tparam INCREMENT2   Increment of the source vector.
    /// @tparam AllocPolicy2 Allocator policy of the source vector.
    /// @param  v            Source Vector.
    /// @return Reference to this object.
    template <size_t INCREMENT2, typename AllocPolicy2>
    Vector<T, INCREMENT, AllocPolicy>& operator=(const Vector<T, INCREMENT2, AllocPolicy2>& v)
    {
        if (_data) {
            // Clean up existing data
            release();
        }

        size_t nSize = v.size();
        if (nSize <= 0)
            return *this;

        reserve(nSize);
        for (size_t i = 0; i < nSize; ++i) {
            push_back(v[i]);
        }
        return *this;
    }

    // Destructor
    /// @brief Destroys the Vector, calling element destructors and freeing memory.
    ~Vector()
    {
        release();
    }

    // Returns current size
    /// @brief Returns the number of elements currently stored.
    /// @return Element count.
    size_t size() const
    {
        if (_data == nullptr)
            return 0;
        return size_ref();
    }

    // Returns current capacity
    /// @brief Returns the maximum number of elements the current buffer can hold.
    /// @return Allocated capacity.
    size_t capacity() const
    {
        if (_data == nullptr)
            return 0;
        return capacity_ref();
    }

    // Iterator methods
    /// @brief Returns an iterator to the first element.
    /// @return Pointer to the first element.
    iterator begin()
    {
        return elements();
    }

    /// @brief Returns a const iterator to the first element.
    /// @return Const pointer to the first element.
    const_iterator begin() const
    {
        return elements();
    }

    /// @brief Returns a const iterator to the first element.
    /// @return Const pointer to the first element.
    const_iterator cbegin() const
    {
        return elements();
    }

    /// @brief Returns an iterator one past the last element.
    /// @return Pointer to one past the last element, or nullptr if empty.
    iterator end()
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }

    /// @brief Returns a const iterator one past the last element.
    /// @return Const pointer to one past the last element, or nullptr if empty.
    const_iterator end() const
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }

    /// @brief Returns a const iterator one past the last element.
    /// @return Const pointer to one past the last element, or nullptr if empty.
    const_iterator cend() const
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }

    // Index access operators
    /// @brief Provides mutable indexed access to elements.
    /// @param pos Zero-based index.
    /// @return Reference to the element at pos.
    T& operator[](size_t pos)
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }

    /// @brief Provides const indexed access to elements.
    /// @param pos Zero-based index.
    /// @return Const reference to the element at pos.
    const T& operator[](size_t pos) const
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }

    /// @brief Provides mutable bounds-checked access to elements.
    /// @param pos Zero-based index.
    /// @return Reference to the element at pos.
    T& at(size_t pos)
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }

    /// @brief Provides const bounds-checked access to elements.
    /// @param pos Zero-based index.
    /// @return Const reference to the element at pos.
    const T& at(size_t pos) const
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }

    /// @brief Returns a reference to the last element.
    /// @return Reference to the last element.
    T& last()
    {
        assert(_data != nullptr);
        assert(size_ref() > 0);
        return elements()[size_ref() - 1];
    }

    /// @brief Returns a const reference to the last element.
    /// @return Const reference to the last element.
    const T& last() const
    {
        assert(_data != nullptr);
        assert(size_ref() > 0);
        return elements()[size_ref() - 1];
    }

    // Reserve capacity
    /// @brief Reallocates the buffer to hold exactly n elements, preserving existing elements.
    /// @param n New capacity; must be greater than zero.
    void reserve(size_t n)
    {
        assert(n > 0);

        if (_data == nullptr) {
            allocate(td::UINT4(n));
            return;
        }

        // If capacity is already n, do nothing
        if (capacity_ref() == n)
            return;

        // Create new memory block
        td::UINT4 oldSize = size_ref();
        unsigned char* oldData = _data;

        allocate(td::UINT4(n));

        // Copy elements
        if (oldSize > 0)
        {
            T* oldElements = reinterpret_cast<T*>(oldData + 2 * sizeof(td::UINT4));
            for (size_t i = 0; i < oldSize; ++i)
            {
                new (elements() + i) T(std::move(oldElements[i]));
                oldElements[i].~T();
            }
            size_ref() = oldSize;
        }

        // Clean up old memory
        if constexpr (std::is_same_v<AllocPolicy, NullAllocator>) {
            delete[] oldData;
        }
        else {
            getAllocator().deallocate(oldData);
        }
    }

    // Reserve minimum capacity
    /// @brief Ensures the buffer holds at least n elements; no-op if capacity is already sufficient.
    /// @param n Minimum required capacity.
    void reserveMin(size_t n)
    {
        assert(n > 0);

        if (_data == nullptr)
        {
            allocate(n);
            return;
        }

        // If capacity is already >= n, do nothing
        if (capacity_ref() >= n)
            return;

        reserve(n);
    }

    // Check if empty
    /// @brief Checks whether the Vector contains no elements.
    /// @return True if the Vector is empty.
    bool isEmpty() const
    {
        return (_data == nullptr) || (size_ref() == 0);
    }

    // Check if full
    /// @brief Checks whether the Vector has reached its current capacity.
    /// @return True if size equals capacity.
    bool isFull() const
    {
        if (_data == nullptr)
            return false;
        return size_ref() == capacity_ref();
    }

    // Reset to empty
    /// @brief Resets the element count to zero without freeing the buffer.
    void reset()
    {
        if (_data != nullptr) {
            size_ref() = 0;
        }
    }

    // Mark as populated with n elements
    /// @brief Directly sets the logical size of the Vector.
    /// @param nElems Number of elements to mark as populated; must not exceed capacity.
    void markAsPopulated(size_t nElems)
    {
        assert(_data != nullptr);
        assert(nElems <= capacity_ref());
        size_ref() = nElems;
    }

    // Release all memory
    /// @brief Destroys all elements and frees the buffer.
    void release()
    {
        if (_data) {
            // Destruct all elements
            size_t currentSize = size_ref();
            T* elems = elements();
            for (size_t i = 0; i < currentSize; ++i)
                elems[i].~T();

            if constexpr (std::is_same_v<AllocPolicy, NullAllocator>)
                delete[] _data;
            else
                getAllocator().deallocate(_data);
            _data = nullptr;
        }
    }

    // Find element
    /// @brief Searches for val and returns its zero-based index.
    /// @param val The value to find.
    /// @return Zero-based index of the first match, or -1 if not found.
    ssize_t find(const T& val) const
    {
        if (_data == nullptr)
            return -1;

        size_t currentSize = size_ref();
        T* elems = elements();

        for (ssize_t i = 0; i < (ssize_t)currentSize; ++i)
        {
            if (elems[i] == val)
                return i;
        }
        return -1;
    }

    // Remove element at position
    /// @brief Removes the element at the given zero-based index, shifting subsequent elements left.
    /// @param iPos Zero-based index of the element to remove.
    /// @return True on success, false if iPos is out of range.
    bool removePos(size_t iPos)
    {
        if (_data == nullptr)
            return false;

        td::UINT4 currentSize = size_ref();
        if (currentSize == 0)
            return false;

        if (iPos >= currentSize)
            return false;

        T* elems = elements();

        // Move elements to fill the gap
        for (size_t i = iPos; i < currentSize - 1; ++i)
            elems[i] = std::move(elems[i + 1]);

        // Destroy the last element
        elems[currentSize - 1].~T();

        // Update size
        size_ref() = currentSize - 1;
        return true;
    }

    // Remove element by value
    /// @brief Removes the first element equal to val.
    /// @param val The value to remove.
    /// @return True if an element was removed, false if val was not found.
    bool remove(const T& val)
    {
        ssize_t pos = find(val);
        if (pos >= 0)
            return removePos((size_t)pos);
        return false;
    }

    // Remove last element
    /// @brief Removes the last element.
    /// @return True on success, false if the Vector is empty.
    bool remove_back()
    {
        if (_data == nullptr)
            return false;

        size_t currentSize = size_ref();
        if (currentSize == 0)
            return false;

        // Destroy the last element
        elements()[currentSize - 1].~T();

        // Update size
        size_ref() = currentSize - 1;
        return true;
    }

    // Add new element (default constructed)
    /// @brief Appends a default-constructed element, growing the buffer if needed.
    /// @return Reference to the newly appended element.
    T& push_back()
    {
        if (_data == nullptr || size_ref() >= capacity_ref())
        {
            T val;
            push_back(val);
        }
        else
        {
            // Place new element
            new (elements() + size_ref()) T();
            size_ref()++;
        }
        return elements()[size_ref() - 1];
    }

    // Add new element (default constructed) and return pointer
    /// @brief Appends a default-constructed element and returns a pointer to it.
    /// @return Pointer to the newly appended element.
    T* push_back2()
    {
        if (_data == nullptr || size_ref() >= capacity_ref())
        {
            T val;
            push_back(val);
        }
        else
        {
            // Place new element
            new (elements() + size_ref()) T();
            size_ref()++;
        }
        return elements() + size_ref() - 1;
    }

    // Add new element
    /// @brief Appends a copy of val at the back, growing the buffer if needed.
    /// @param val The value to append.
    void push_back(const T& val)
    {
        if (_data == nullptr)
        {
            // First allocation
            td::UINT4 initialCapacity = 1;
            allocate(initialCapacity);
        }
        else if (size_ref() >= capacity_ref())
        {
            // Need to grow
            td::UINT4 newCapacity;
            if (INCREMENT > 0)
                newCapacity = capacity_ref() + INCREMENT;
            else
                newCapacity = capacity_ref() * 2;

            // Create new block
            td::UINT4 oldSize = size_ref();
            unsigned char* oldData = _data;

            allocate(newCapacity);

            // Copy elements
            T* oldElements = reinterpret_cast<T*>(oldData + 2 * sizeof(td::UINT4));
            for (size_t i = 0; i < oldSize; ++i) {
                new (elements() + i) T(std::move(oldElements[i]));
                oldElements[i].~T();
            }
            size_ref() = oldSize;

            // Clean up old memory
            if constexpr (std::is_same_v<AllocPolicy, NullAllocator>) {
                delete[] oldData;
            }
            else
            {
                getAllocator().deallocate(oldData);
            }
        }

        // Add the new element
        new (elements() + size_ref()) T(val);
        size_ref()++;
    }

    /// @brief Copy-assigns from another Vector of the same type.
    /// @param v Source Vector.
    /// @return Const reference to this object.
    const Vector& operator = (const Vector& v)
    {
        auto nElems = v.size();
        if (nElems > 0)
        {
            reserve(nElems);
            for (const auto& e : v)
                push_back(e);
        }
        else
            release();
        return *this;
    }

    // Insert element at position
    /// @brief Inserts val at the given position, shifting subsequent elements right.
    /// @param pos Zero-based insertion index; if out of range, appends.
    /// @param val The value to insert.
    void insert(size_t pos, const T& val)
    {
        if (_data == nullptr || pos >= size_ref())
        {
            push_back(val);
            return;
        }

        td::UINT4 currentSize = size_ref();

        // Check if we need to resize
        if (currentSize >= capacity_ref()) {
            // Similar to push_back, we need to grow
            td::UINT4 newCapacity;
            if (INCREMENT > 0)
                newCapacity = capacity_ref() + INCREMENT;
            else
                newCapacity = capacity_ref() * 2;

            // Create new block
            unsigned char* oldData = _data;
            T* oldElements = reinterpret_cast<T*>(oldData + 2 * sizeof(td::UINT4));

            allocate(newCapacity);

            // Copy elements with gap at pos
            for (size_t i = 0; i < pos; ++i) {
                new (elements() + i) T(std::move(oldElements[i]));
                oldElements[i].~T();
            }

            // Insert new element
            new (elements() + pos) T(val);

            // Copy remaining elements
            for (size_t i = pos; i < currentSize; ++i) {
                new (elements() + i + 1) T(std::move(oldElements[i]));
                oldElements[i].~T();
            }

            size_ref() = currentSize + 1;

            // Clean up old memory
            if constexpr (std::is_same_v<AllocPolicy, NullAllocator>) {
                delete[] oldData;
            }
            else {
                getAllocator().deallocate(oldData);
            }
        }
        else {
            // We have enough space, just make room
            T* elems = elements();

            // Move elements to make space
            for (size_t i = currentSize; i > pos; --i) {
                new (elems + i) T(std::move(elems[i - 1]));
                elems[i - 1].~T();
            }

            // Insert the new element
            new (elems + pos) T(val);

            size_ref() = currentSize + 1;
        }
    }

    // Zero all elements
    /// @brief Default-constructs (zeroes) all allocated elements up to capacity.
    void zeroAll()
    {
        if (_data == nullptr)
            return;

        T* elems = elements();
        for (size_t i = 0; i < capacity_ref(); ++i)
            elems[i] = T();
    }

    // Add multiple elements from an array
    /// @brief Appends nVals elements from the raw array pVals.
    /// @param pVals Pointer to source array.
    /// @param nVals Number of elements to append.
    void push_back(const T* pVals, size_t nVals)
    {
        for (size_t i = 0; i < nVals; ++i)
            push_back(pVals[i]);
    }

    // Add all elements from another vector
    /// @brief Appends all elements from another Vector (potentially different template parameters).
    /// @tparam INCREMENT2   Increment of the source vector.
    /// @tparam AllocPolicy2 Allocator policy of the source vector.
    /// @param  v            Source Vector.
    template <size_t INCREMENT2, typename AllocPolicy2>
    void push_back(const Vector<T, INCREMENT2, AllocPolicy2>& v)
    {
        for (const auto& e : v)
            push_back(e);
    }

    // Apply function to each element
    /// @brief Applies a callable to every element in order.
    /// @tparam MyFunction Callable type accepting a const T reference.
    /// @param  f          The callable to invoke on each element.
    template <typename MyFunction>
    void for_each(MyFunction& f) const
    {
        auto first = cbegin();
        auto last = cend();

        while (first != last)
        {
            f(*first);
            ++first;
        }
    }

    // Call each element as a function
    /// @brief Calls operator() on every element in order.
    void forEach() const
    {
        auto it = cbegin();
        auto itEnd = cend();
        while (it != itEnd)
        {
            (*it)();
            ++it;
        }
    }

    // Move element from one position to another
    /// @brief Moves an element from fromPos to toPos, shifting elements between them.
    /// @param fromPos Source zero-based index.
    /// @param toPos   Destination zero-based index.
    /// @return True if the move was performed, false on invalid input.
    bool move(size_t fromPos, size_t toPos)
    {
        if (_data == nullptr)
            return false;

        if (fromPos == toPos)
            return false;

        size_t currentSize = size_ref();
        if (fromPos >= currentSize || toPos >= currentSize)
            return false;

        T* elems = elements();
        T val = std::move(elems[fromPos]);

        if (fromPos > toPos)
        {
            // Move toward front
            for (size_t i = fromPos; i > toPos; --i)
                elems[i] = std::move(elems[i - 1]);
        }
        else {
            // Move toward back
            for (size_t i = fromPos; i < toPos; ++i)
                elems[i] = std::move(elems[i + 1]);
        }

        elems[toPos] = std::move(val);
        return true;
    }

    // Output to stream
    /// @brief Writes the element count and all elements to a stream.
    /// @tparam TSTREAM Stream type supporting operator<<.
    /// @param  o       Output stream.
    /// @param  pSep    Separator string placed before each element.
    template <class TSTREAM>
    void toStream(TSTREAM& o, const char* pSep = " ")
    {
        auto first = begin();
        auto last = end();
        size_t elSize = size();
        o << elSize;
        while (first != last) {
            o << *pSep << *first;
            ++first;
        }
        o << std::endl;
    }
};

// Stream output operator for Vector
/// @brief Writes a Vector in bracket notation to a stream.
/// @tparam T          Element type.
/// @tparam INCREMENT  Growth increment.
/// @tparam AllocPolicy Allocator policy.
/// @tparam STREAM     Output stream type.
/// @param  os         Output stream.
/// @param  v          Vector to write.
/// @return Reference to os.
template <typename T, size_t INCREMENT, typename AllocPolicy, class STREAM>
STREAM& operator<<(STREAM& os, const Vector<T, INCREMENT, AllocPolicy>& v)
{
    os << "[";

    const size_t size = v.size();
    if (size > 0)
    {
        os << v[0];
        for (size_t i = 1; i < size; ++i)
            os << ", " << v[i];
    }

    os << "]";
    return os;
}

// Memory Manager class
/// @brief Pool-based memory manager that pre-allocates large blocks and sub-allocates from them.
/// @tparam T          Element type managed.
/// @tparam BLOCK_SIZE Default size in bytes for each pool block.
template <typename T, td::UINT4 BLOCK_SIZE>
class MemoryManager
{
private:
    /// @brief Descriptor for a single pool block.
    struct MemoryBlock {
        unsigned char* data; ///< Pointer to the raw block memory.
        td::UINT4 capacity;  ///< Total capacity of the block in bytes.
        td::UINT4 used;      ///< Number of bytes consumed from this block.
//        bool inUse;

        /// @brief Constructs a MemoryBlock with the given data pointer and capacity.
        /// @param d Pointer to the raw memory.
        /// @param c Total capacity of the block in bytes.
        MemoryBlock(unsigned char* d, td::UINT4 c)
            : data(d), capacity(c), used(0) {}
    };

    Vector<MemoryBlock> _blocks; ///< List of allocated pool blocks.

public:
    /// @brief Constructs a MemoryManager pre-reserving space for nBlocks block descriptors.
    /// @param nBlocks Expected maximum number of blocks.
    MemoryManager(td::UINT4 nBlocks = 1024)
    {
        _blocks.reserve(nBlocks);
    }

    /// @brief Destroys the MemoryManager and frees all pool blocks.
    ~MemoryManager() {
        // Clean up all allocated blocks
        for (auto& block : _blocks)
        {
            if (block.data) {
                delete[] block.data;
            }
        }
    }

    // Allocate a new block or return an existing one of sufficient size
    /// @brief Returns a pointer to a buffer that can hold capacity elements, reusing a pool block if possible.
    /// @param capacity Number of elements the buffer should accommodate.
    /// @return Pointer to the raw byte buffer with inline size/capacity header.
    unsigned char* allocateBlock(td::UINT4 capacity) {
        // Calculate total bytes needed
        td::UINT4 totalBytes = 2 * sizeof(td::UINT4) + capacity * sizeof(T);

        // First, try to find an unused block with sufficient capacity
        for (auto& block : _blocks)
        {
            assert(block.capacity >= block.used);
            td::UINT4 freeSpace = block.capacity - block.used;
            if (freeSpace >= totalBytes)
            {
                // Found a suitable block
//                    block.inUse = true;
                unsigned char* pBegin = block.data + block.used;
                // Initialize header
                *reinterpret_cast<td::UINT4*>(pBegin) = 0; // size = 0
                *reinterpret_cast<td::UINT4*>(pBegin + sizeof(td::UINT4)) = capacity;

                // Zero out elements area
                T* elements = reinterpret_cast<T*>(pBegin + 2 * sizeof(td::UINT4));
                memset(elements, 0, capacity * sizeof(T));

                return block.data;
            }
        }

        td::UINT4 toAlloc = BLOCK_SIZE;
        if (totalBytes > BLOCK_SIZE)
            toAlloc = totalBytes;


        // No suitable existing block, create a new one
        unsigned char* newData = new unsigned char[totalBytes];

        // Initialize header
        *reinterpret_cast<td::UINT4*>(newData) = 0; // size = 0
        *reinterpret_cast<td::UINT4*>(newData + sizeof(td::UINT4)) = capacity;

        // Zero out elements area
        T* elements = reinterpret_cast<T*>(newData + 2 * sizeof(td::UINT4));
        memset(elements, 0, capacity * sizeof(T));

        // Store in our blocks list
        _blocks.push_back(MemoryBlock(newData, capacity));
        _blocks.last().inUse = true;

        return newData;
    }

    // Return a block to the pool
    /// @brief Marks a previously allocated block as available for reuse.
    /// @param data Pointer to the block to release; must have been returned by allocateBlock.
    void releaseBlock(unsigned char* data) {
        if (!data) return;

        // Find the block
        for (auto& block : _blocks) {
            if (block.data == data) {
                block.inUse = false;
                return;
            }
        }

        // Should never reach here if all memory is managed by this manager
        assert(false && "Attempted to release a block not managed by this MemoryManager");
    }
};

// Define a helper alias template for vectores using external allocators
/// @brief Convenience alias for a Vector backed by an ExternalAllocator pool.
/// @tparam T          Element type.
/// @tparam INCREMENT  Growth increment.
/// @tparam BLOCK_SIZE Pool block size.
template <typename T, size_t INCREMENT = 0, td::UINT4 BLOCK_SIZE = 64*1024>
using ExternalVector = Vector<T, INCREMENT, ExternalAllocator<T, BLOCK_SIZE>>;

} // namespace cnt
