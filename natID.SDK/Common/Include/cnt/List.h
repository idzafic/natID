// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file List.h
 @brief Singly-linked list with optional external allocator support. */
#pragma once
#include <cassert>
#include <td/Types.h>
#include <td/String.h>
#include <ostream>

namespace mem
{
template <class CNT, td::UINT4 SINGLEBUFFLEN>
class CntMemoryManager;

template <class CNT>
class CntFixedSizeMemoryPool;

template <class TLIST, size_t CHUNK_SIZE>
class ListManager;

template < td::UINT4 CHUNKLEN, size_t PAGELEN, size_t LISTPAGE>
class BufferAllocator;

class Buffer;
}

namespace cnt
{
/// @brief Intrusive singly-linked list container.
/// @tparam T         Element type stored in each node.
/// @tparam EXTERN_ALLOCATOR When true the list does not own its nodes and uses an external allocator.
template <typename T, bool EXTERN_ALLOCATOR = false>
class List
{
    template <class CNT, td::UINT4 SINGLEBUFFLEN>
    friend class mem::CntMemoryManager;

    template <class CNT>
    friend class mem::CntFixedSizeMemoryPool;

    template <class TLIST, size_t CHUNK_SIZE>
    friend class mem::ListManager;

    template < td::UINT4 CHUNKLEN, size_t CHUNK_SIZE, size_t LISTPAGE>
    friend class mem::BufferAllocator;

    friend class mem::Buffer;

//    template <typename T_BV, int BLOCK_SIZE_BV>
//    friend class BlockVector;

public:
    /// @brief Intrusive list node holding one element and a pointer to the next node.
    typedef struct _tEntry
    {
        T data; ///< The stored element value.
        struct _tEntry* next = nullptr; ///< Pointer to the next node, nullptr if last.
    } tEntry;

    typedef T T_DATA; ///< Publicly exposed element type alias.
protected:
    tEntry* _root; ///< Pointer to the first node in the list, nullptr when empty.
public:

    class iterator;

    /// @brief Constant forward iterator over List elements.
    class const_iterator
    {
        //tEntry* pIt;
        typename List<T, EXTERN_ALLOCATOR>::tEntry* pIt; ///< Pointer to the current node.

        friend class iterator;
    public:
        /// @brief Constructs an end (null) const_iterator.
        const_iterator()
            : pIt(nullptr)
        {
        }

        /// @brief Constructs a const_iterator pointing to the given node.
        /// @param pPos Pointer to the initial node.
        const_iterator(tEntry* pPos)
            : pIt(pPos)
        {
        }

        /// @brief Constructs a const_iterator from a mutable iterator.
        /// @param it The mutable iterator to copy position from.
        const_iterator(const iterator& it)
            : pIt(it.pIt)
        {
        }

        /// @brief Assigns another const_iterator to this one.
        /// @param it The source const_iterator.
        void operator = (const const_iterator& it)
        {
            pIt = it.pIt;
        }

        /// @brief Checks equality with another const_iterator.
        /// @param it The other const_iterator.
        /// @return True if both point to the same node.
        bool operator == (const const_iterator& it) const
        {
            return (pIt == it.pIt);
        }

        /// @brief Checks inequality with another const_iterator.
        /// @param it The other const_iterator.
        /// @return True if they point to different nodes.
        bool operator != (const const_iterator& it) const
        {
            return (pIt != it.pIt);
        }

        /// @brief Advances the iterator to the next node.
        void operator ++ ()
        {
            assert(pIt);
            pIt = pIt->next;
        }

        /// @brief Dereferences the iterator to obtain a const element reference.
        /// @return Const reference to the element at the current position.
        const T& operator* () const
        {
            return pIt->data;
        }

        /// @brief Provides pointer-style member access to the current element.
        /// @return Const pointer to the current element.
        const T* operator ->() const
        {
            return &(operator*());
        }

        /// @brief Returns a const pointer to the underlying entry node.
        /// @return Const pointer to the current tEntry node.
        const tEntry* _getPtr() const
        {
            return pIt;
        }

        /// @brief Returns a non-const pointer to the underlying entry node.
        /// @return Pointer to the current tEntry node.
        tEntry* _getNonConstPtr() const
        {
            return pIt;
        }

        /// @brief Checks whether the iterator has reached the end of the list.
        /// @return True if the iterator is past the last element.
        bool end() const
        {
            if (pIt)
                return false;
            return true;
        }
    };

    /// @brief Mutable forward iterator over List elements.
    class iterator
    {
    protected:
        tEntry* pIt; ///< Pointer to the current node.
        friend class const_iterator;
    public:
        /// @brief Constructs an end (null) iterator.
        iterator()
            : pIt(nullptr)
        {
        }

        /// @brief Constructs an iterator pointing to the given node.
        /// @param pEntry Pointer to the initial node.
        iterator(tEntry* pEntry)
            : pIt(pEntry)
        {
        }

        /// @brief Advances the iterator to the next node.
        void operator ++ ()
        {
            //int g = 1;
            assert(pIt);
            pIt = pIt->next;
        }

        /// @brief Returns the underlying tEntry node pointer.
        /// @return Pointer to the current tEntry node.
        tEntry* getEntry()
        {
            return pIt;
        }

        /// @brief Returns the tEntry node following the current one.
        /// @return Pointer to the next tEntry node, or nullptr if there is none.
        tEntry* getNextEntry()
        {
            if (pIt)
                return pIt->next;
            return nullptr;
        }

        /// @brief Dereferences the iterator to obtain a mutable element reference.
        /// @return Reference to the element at the current position.
        T& operator* () const
        {
            return pIt->data;
        }

        /// @brief Provides pointer-style member access to the current element.
        /// @return Pointer to the current element.
        T* operator ->() const
        {
            return &(operator*());
        }

        /// @brief Assigns another iterator to this one.
        /// @param it The source iterator.
        void operator = (const iterator& it)
        {
            pIt = it.pIt;
        }

        /// @brief Checks equality with another iterator.
        /// @param it The other iterator.
        /// @return True if both point to the same node.
        bool operator == (const iterator& it) const
        {
            return (pIt == it.pIt);
        }

        /// @brief Checks inequality with another iterator.
        /// @param it The other iterator.
        /// @return True if they point to different nodes.
        bool operator != (const iterator& it) const
        {
            return (pIt != it.pIt);
        }

        /// @brief Checks equality with a const_iterator.
        /// @param it The const_iterator to compare against.
        /// @return True if both point to the same node.
        bool operator == (const const_iterator& it) const
        {
            return (pIt == it.pIt);
        }

        /// @brief Checks inequality with a const_iterator.
        /// @param it The const_iterator to compare against.
        /// @return True if they point to different nodes.
        bool operator != (const const_iterator& it) const
        {
            return (pIt != it.pIt);
        }

        /// @brief Checks whether the iterator has reached the end of the list.
        /// @return True if the iterator is past the last element.
        bool end() const
        {
            if (pIt)
                return false;
            return true;
        }

        /// @brief Checks whether the iterator has reached the end of the list.
        /// @return True if the iterator is past the last element.
        bool isEnd() const
        {
            if (pIt)
                return false;
            return true;
        }
    };
protected:

    /// @brief Returns a pointer to the last node in the list.
    /// @return Pointer to the last tEntry node, or nullptr if the list is empty.
    tEntry* getLast()
    {
        if (!_root)
            return nullptr;
        tEntry* pLast = _root;
        while (pLast->next)
            pLast = pLast->next;
        return pLast;
    }

    /// @brief Appends an already-allocated node to the end of the list.
    /// @param pNew Pointer to the node to append.
    void push_back(tEntry* pNew)
    {
        pNew->next = nullptr;
        tEntry* pLast = getLast();
        if (!pLast)
            _root = pNew;
        else
            pLast->next = pNew;
    }

    /// @brief Inserts an already-allocated node at the front of the list.
    /// @param pNew Pointer to the node to prepend.
    void put_front(tEntry* pNew)
    {
        if (_root)
            pNew->next = _root;
        else
            pNew->next = nullptr;

        _root = pNew;
    }

    /// @brief Inserts an already-allocated node before the element referenced by the given iterator.
    /// @param it   Iterator pointing to the position before which to insert.
    /// @param pNew The node to insert.
    void insert(iterator it, tEntry pNew)
    {
        auto pEntry = _root;
        if (!pEntry)
        {
            put_front(pNew);
            return;
        }
        pEntry = it.getEntry();
        if (!pEntry)
        {
            put_front(pNew);
            return;
        }

        auto pPrev = _root;
        if (pEntry == pPrev)
        {
            put_front(pNew);
            return;
        }

        while (pPrev->next != pEntry)
        {
            pPrev = pPrev->next;
        }

        auto tmp = pPrev->next;
        pPrev->next = pNew;
        pNew->next = tmp;
    }

    /// @brief Removes and discards the first node of the list.
    void remove_front()
    {
        assert(_root);
        _root = _root->next;
    }

  public:
    /// @brief Returns an iterator to the element at the given position.
    /// @param pos Zero-based index of the desired element.
    /// @return Iterator pointing to the element at pos.
    iterator operator [] (size_t pos)
    {
        iterator it(begin());
        const_iterator itEnd(end());
        size_t i = 0;
        while (it != itEnd)
        {
            if (i == pos)
                return it;
            ++it;
            ++i;
        }
        //ne smije nikad doci ovdje
        assert(false);
        return it;
    }

    /// @brief Returns a const_iterator to the element at the given position.
    /// @param pos Zero-based index of the desired element.
    /// @return Const iterator pointing to the element at pos.
    const_iterator operator [] (size_t pos) const
    {
        const_iterator it(begin());
        const_iterator itEnd(end());
        size_t i = 0;
        while (it != itEnd)
        {
            if (i == pos)
                return it;
            ++it;
            ++i;
        }
        //ne smije nikad doci ovdje
        assert(false);
        return it;
    }

public:

    /// @brief Constructs an empty list.
    List()
        : _root(nullptr)
    {
    }

    /// @brief Copy-constructs a list from another list.
    /// @param lst The source list to copy from.
    List(const List<T, EXTERN_ALLOCATOR>& lst)
        : _root(nullptr)
    {
        operator = (lst);
    }

    /// @brief Destroys the list and frees all owned nodes.
    ~List()
    {
        clean();
    }

    /// @brief Returns a pointer to the first entry node.
    /// @return Pointer to the root tEntry node, or nullptr if empty.
    tEntry* getFirstEntry()
    {
        return _root;
    }

    /// @brief Returns a const pointer to the first entry node.
    /// @return Const pointer to the root tEntry node, or nullptr if empty.
    const tEntry* getFirstEntry() const
    {
        return _root;
    }

    /// @brief Copy-assigns from another list.
    /// @param lst The source list to assign from.
    inline void operator = (const List& lst)
    {
        if constexpr(!EXTERN_ALLOCATOR)
        {
            clean();
            const_iterator it(lst.begin());
            const_iterator itEnd(lst.end());
            while (it != itEnd)
            {
                push_back(*it);
                ++it;
            }
        }
        else
        {
            _root = lst._root;
        }

    }

    /// @brief Serialises the list to a binary stream.
    /// @tparam SER  Stream type supporting operator<<.
    /// @param  ar   Output stream to write to.
    template <class SER>
    void saveToBinaryStream(SER& ar) const
    {
        td::UINT4 nCount = (td::UINT4) size();
        ar << nCount;
        const_iterator it(begin());
        const_iterator itEnd(end());
        while (it != itEnd)
        {
            ar << *it;
            ++it;
        }
    }

    /// @brief Serialises the list to a text stream.
    /// @tparam SER  Stream type supporting operator<<.
    /// @param  ar   Output stream to write to.
    template <class SER>
    void saveToTextStream(SER& ar) const
    {
        td::UINT4 nCount = (td::UINT4) size();
        ar << nCount;
        if (nCount == 0)
            return;
        ar << ' ';
        const_iterator it(begin());
        const_iterator itEnd(end());
        while (it != itEnd)
        {
            ar << *it << ' ';
            ++it;
        }
    }

    /// @brief Writes a human-readable representation to the given stream.
    /// @tparam OS   Output stream type.
    /// @param  ar   Output stream to write to.
    template <class OS>
    void show(OS& ar) const
    {
        size_t nCount = size();
        ar << nCount << " ";
        const_iterator it = begin();
        const_iterator itEnd = end();
        while (it != itEnd)
        {
            ar << *it << " ";
            ++it;
        }
    }

    //output to ostream
    template<class CharT, class Traits>
    friend std::basic_ostream<CharT, Traits>&
        operator << (std::basic_ostream<CharT, Traits>& os, const List& rhs)
    {
        auto flags = os.flags();
        if (flags & std::ios_base::binary)
            rhs.saveToBinaryStream(os);
        else
            rhs.saveToTextStream(os);
        return os;
    }

    /// @brief Deserialises the list from a stream, replacing any existing content.
    /// @tparam SER  Stream type supporting operator>>.
    /// @param  ar   Input stream to read from.
    /// @return True on success.
    template <class SER>
    bool loadFromStream(SER& ar)
    {
        clean();
        td::UINT4 nCount;
        ar >> nCount;

        for (td::UINT4 i = 0; i < nCount; ++i)
        {
            T& val = push();
            ar >> val;
        }
        return true;
    }

    template<class CharT, class Traits>
    friend std::basic_istream<CharT, Traits>&
        operator >> (std::basic_istream<CharT, Traits>& is, List& rhs)
    {
        rhs.loadFromStream(is);
        return is;
    }

    //template <typename T_CHAR, td::StringEncoding Encoding>
    //inline iterator push_back(const td::StringBase<T_CHAR, Encoding, 4, false>& val)
    //{
    //	assert(!EXTERN_ALLOCATOR);
    //	td::StringBase<T_CHAR, Encoding, 4, false>& val2 = const_cast<td::StringBase<T_CHAR, Encoding, 4, false>&>(val);
    //	val2.addRef();
    //	tEntry* pNew = new tEntry;
    //	pNew->data = val2;
    //	pNew->next = nullptr;
    //	if (_last)
    //	{
    //		_last->next = pNew;
    //		_last = pNew;
    //	}
    //	else
    //	{
    //		_root = _last = pNew;
    //	}
    //	return iterator(_last);
    //}

    /// @brief Copies a value and appends it as a new node at the back of the list.
    /// @param val The value to append.
    /// @return Iterator pointing to the newly appended node.
    inline iterator push_back(const T& val)
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        pNew->next = nullptr;
        push_back(pNew);
        return iterator(pNew);
    }


    /// @brief Copies a value and inserts it as a new node at the front of the list.
    /// @param val The value to prepend.
    /// @return Iterator pointing to the newly inserted node.
    inline iterator put_front(const T& val)
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        put_front(pNew);
        return iterator(_root);
    }

    /// @brief Copies a value and inserts it at the front using an external allocator.
    /// @param val The value to prepend.
    /// @return Iterator pointing to the newly inserted node.
    inline iterator put_front_ext(const T& val)
    {
        static_assert(EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        put_front(pNew);
        return iterator(_root);
    }

    /// @brief Removes and destroys the first node of the list.
    inline void pop_front()
    {
        static_assert(!EXTERN_ALLOCATOR);
        assert(_root);

        tEntry* pNext = _root->next;
        delete _root;
        _root = pNext;
    }

    /// @brief Appends a default-constructed element at the back and returns a reference to it.
    /// @return Reference to the newly appended element.
    inline T& push()
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->next = nullptr;
        tEntry* pLast = getLast();

        if (pLast)
            pLast->next = pNew;
        else
            _root = pNew;
        return pNew->data;
    }

    /// @brief Appends a default-constructed node at the back of the list.
    /// @return Iterator pointing to the newly appended node.
    inline iterator push_back()
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->next = nullptr;
        push_back(pNew);
        return iterator(pNew);
    }

    /// @brief Inserts a value immediately after the node pointed to by the given iterator.
    /// @param it  Iterator pointing to the node after which to insert.
    /// @param val The value to insert.
    inline void insertAfter(const const_iterator& it, const T& val)
    {
        static_assert(!EXTERN_ALLOCATOR);
        if (it.end())
        {
            push_back(val);
            return;

        }
        tEntry* pEntry = const_cast<tEntry*>(it._getPtr());
        tEntry* pNew = new tEntry;
        pNew->next = pEntry->next;
        pEntry->next = pNew;
        pNew->data = val;
    }

    /// @brief Checks whether the list contains no elements.
    /// @return True if the list is empty.
    bool isEmpty() const
    {
        return (_root == nullptr);
    }

    /// @brief Checks whether the list contains at least one element.
    /// @return True if the list is not empty.
    bool isNotEmpty() const
    {
        return (_root != nullptr);
    }

    /// @brief Checks whether the list contains at least one element.
    /// @return True if the list has entries.
    bool hasEntries() const
    {
        return (_root != nullptr);
    }

    /// @brief Removes all elements from the list, freeing owned nodes.
    void clean()
    {
        if constexpr(!EXTERN_ALLOCATOR)
        {
            while (_root)
            {
                tEntry* pEntry = _root->next;
                delete _root;
                _root = pEntry;
            }
        }
    }

    /// @brief Returns an iterator to the first element.
    /// @return Iterator pointing to the first node.
    iterator begin()
    {
        return iterator(_root);
    }

    /// @brief Returns a const_iterator to the first element.
    /// @return Const iterator pointing to the first node.
    const_iterator begin() const
    {
        return const_iterator(_root);
    }

    /// @brief Returns a const_iterator to the first element.
    /// @return Const iterator pointing to the first node.
    const_iterator cbegin() const
    {
        return const_iterator(_root);
    }

    /// @brief Returns an iterator representing the past-the-end position.
    /// @return End iterator.
    iterator end()
    {
        return iterator();
    }

    /// @brief Returns a const past-the-end iterator.
    /// @return Const end iterator.
    const_iterator cend() const
    {
        return const_iterator();
    }

    /// @brief Returns a const past-the-end iterator.
    /// @return Const end iterator.
    const_iterator end() const
    {
        return const_iterator();
    }

    /// @brief Returns the number of elements in the list.
    /// @return Number of nodes traversed.
    size_t size() const
    {
        const tEntry* pEntry = _root;
        if (!pEntry)
            return 0;

        size_t i = 1;
        while (pEntry->next)
        {
            ++i;
            pEntry = pEntry->next;
        }
        return i;
    }

    /// @brief Finds and removes the first node whose data compares equal to the given value.
    /// @param data The value to search for and remove.
    /// @return True if a matching node was found and removed, false otherwise.
    bool remove(const T& data)
    {
        tEntry* pEntry = _root;
        if (!pEntry)
            return false;

        tEntry* pPrev = nullptr;

        while (pEntry != nullptr)
        {
            if (data == pEntry->data)
            {
                //remove this one
                if (pPrev == nullptr)
                {
                    _root = pEntry->next;
                }
                else
                {
                    if (pEntry->next == nullptr)
                        pPrev->next = nullptr;
                    else
                        pPrev->next = pEntry->next;
                }
                if (!EXTERN_ALLOCATOR)
                    delete pEntry;
                return true;
            }
            pPrev = pEntry;
            pEntry = pEntry->next;
        }
        return false;
    }
};


} //namespace cnt
