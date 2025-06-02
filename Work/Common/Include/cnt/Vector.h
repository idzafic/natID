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
#include <initializer_list>
#include <cstring> // for memset
#include <utility> // for std::move
//#include <functional> // for std::function

namespace cnt
{

// Forward declaration of the memory manager class
template <typename T, td::UINT4 BLOCK_SIZE>
class MemoryManager;

// Null allocator policy - default behavior (internal allocation)
struct NullAllocator {};

// External allocator policy that uses MemoryManager
template <typename T, td::UINT4 BLOCK_SIZE>
class ExternalAllocator {
private:
    MemoryManager<T, BLOCK_SIZE>* _memoryManager;

public:
    explicit ExternalAllocator(MemoryManager<T, BLOCK_SIZE>* manager)
    : _memoryManager(manager) {
        assert(_memoryManager != nullptr && "Memory manager must not be null");
    }

    ExternalAllocator(const ExternalAllocator& other)
    : _memoryManager(other.memoryManager) {}
    
    ExternalAllocator& operator=(const ExternalAllocator& other) {
        _memoryManager = other._memoryManager;
        return *this;
    }
    
    unsigned char* allocate(td::UINT4 capacity) const {
        assert(_memoryManager != nullptr && "Memory manager must be initialized before use");
        return _memoryManager->allocateBlock(capacity);
    }
    
    void deallocate(unsigned char* data) const {
        assert(_memoryManager != nullptr && "Memory manager must be initialized before use");
        _memoryManager->releaseBlock(data);
    }
    
    // Gets the memory manager
    MemoryManager<T, BLOCK_SIZE>* getMemoryManager() const {
        return _memoryManager;
    }
};

template <typename T, size_t INCREMENT = 0, typename AllocPolicy = NullAllocator>
class Vector
{
protected:
    unsigned char* _data = nullptr;

    // Helper methods to access header information
    inline td::UINT4& size_ref() const
    {
        assert(_data != nullptr);
        return *reinterpret_cast<td::UINT4*>(_data);
    }
    
    inline td::UINT4& capacity_ref() const
    {
        assert(_data != nullptr);
        return *reinterpret_cast<td::UINT4*>(_data + sizeof(td::UINT4));
    }
    
    // Get pointer to the actual array of elements
    inline T* elements() const
    {
        if (_data == nullptr)
            return nullptr;
        return reinterpret_cast<T*>(_data + 2 * sizeof(td::UINT4));
    }

    // Helper to allocate memory with proper structure
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
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T T_DATA;
    typedef T* pointer;
    
    // Default constructor
    Vector() : _data(nullptr) {}
    
    // Copy constructor
    Vector(const Vector<T, INCREMENT, AllocPolicy>& v) : _data(nullptr)
    {
        operator=(v);
    }
    
    // Move constructor
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
    Vector(size_t n) : _data(nullptr)
    {
        if (n > 0)
            reserve(n);
    }
    
    // Constructor with initializer list
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
    ~Vector()
    {
        release();
    }
    
    // Returns current size
    size_t size() const
    {
        if (_data == nullptr)
            return 0;
        return size_ref();
    }
    
    // Returns current capacity
    size_t capacity() const
    {
        if (_data == nullptr)
            return 0;
        return capacity_ref();
    }
    
    // Iterator methods
    iterator begin()
    {
        return elements();
    }
    
    const_iterator begin() const
    {
        return elements();
    }
    
    const_iterator cbegin() const
    {
        return elements();
    }
    
    iterator end()
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }
    
    const_iterator end() const
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }
    
    const_iterator cend() const
    {
        if (_data == nullptr)
            return nullptr;
        return elements() + size_ref();
    }
    
    // Index access operators
    T& operator[](size_t pos)
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }
    
    const T& operator[](size_t pos) const
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }
    
    T& at(size_t pos)
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }
    
    const T& at(size_t pos) const
    {
        assert(_data != nullptr);
        assert(pos < size_ref());
        return elements()[pos];
    }
    
    T& last()
    {
        assert(_data != nullptr);
        assert(size_ref() > 0);
        return elements()[size_ref() - 1];
    }
    
    const T& last() const
    {
        assert(_data != nullptr);
        assert(size_ref() > 0);
        return elements()[size_ref() - 1];
    }
    
    // Reserve capacity
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
            T* oldElements = reinterpret_cast<T*>(oldData + 2 * sizeof(size_t));
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
    bool isEmpty() const
    {
        return (_data == nullptr) || (size_ref() == 0);
    }
    
    // Check if full
    bool isFull() const
    {
        if (_data == nullptr)
            return false;
        return size_ref() == capacity_ref();
    }
    
    // Reset to empty
    void reset()
    {
        if (_data != nullptr) {
            size_ref() = 0;
        }
    }
    
    // Mark as populated with n elements
    void markAsPopulated(size_t nElems)
    {
        assert(_data != nullptr);
        assert(nElems <= capacity_ref());
        size_ref() = nElems;
    }
    
    // Release all memory
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
    bool remove(const T& val)
    {
        ssize_t pos = find(val);
        if (pos >= 0)
            return removePos((size_t)pos);
        return false;
    }
    
    // Remove last element
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
            if (INCREMENT > 0) {
                newCapacity = capacity_ref() + INCREMENT;
            }
            else {
                newCapacity = capacity_ref() * 2;
            }
            
            // Create new block
            td::UINT4 oldSize = size_ref();
            unsigned char* oldData = _data;
            
            allocate(newCapacity);
            
            // Copy elements
            T* oldElements = reinterpret_cast<T*>(oldData + 2 * sizeof(size_t));
            for (size_t i = 0; i < oldSize; ++i) {
                new (elements() + i) T(std::move(oldElements[i]));
                oldElements[i].~T();
            }
            size_ref() = oldSize;
            
            // Clean up old memory
            if constexpr (std::is_same_v<AllocPolicy, NullAllocator>) {
                delete[] oldData;
            }
            else {
                getAllocator().deallocate(oldData);
            }
        }
        
        // Add the new element
        new (elements() + size_ref()) T(val);
        size_ref()++;
    }
    
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
    void zeroAll()
    {
        if (_data == nullptr)
            return;
            
        T* elems = elements();
        for (size_t i = 0; i < capacity_ref(); ++i)
            elems[i] = T();
    }
    
    // Add multiple elements from an array
    void push_back(const T* pVals, size_t nVals)
    {
        for (size_t i = 0; i < nVals; ++i)
            push_back(pVals[i]);
    }
    
    // Add all elements from another vector
    template <size_t INCREMENT2, typename AllocPolicy2>
    void push_back(const Vector<T, INCREMENT2, AllocPolicy2>& v)
    {
        for (const auto& e : v)
            push_back(e);
    }
    
    // Apply function to each element
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
template <typename T, td::UINT4 BLOCK_SIZE>
class MemoryManager
{
private:
    struct MemoryBlock {
        unsigned char* data;
        td::UINT4 capacity;
        td::UINT4 used;
//        bool inUse;
        
        MemoryBlock(unsigned char* d, td::UINT4 c)
            : data(d), capacity(c), used(0) {}
    };
    
    Vector<MemoryBlock> _blocks;
    
public:
    MemoryManager(td::UINT4 nBlocks = 1024)
    {
        _blocks.reserve(nBlocks);
    }
    
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
template <typename T, size_t INCREMENT = 0, td::UINT4 BLOCK_SIZE = 64*1024>
using ExternalVector = Vector<T, INCREMENT, ExternalAllocator<T, BLOCK_SIZE>>;

} // namespace cnt
