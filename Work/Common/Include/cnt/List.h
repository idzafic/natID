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
    typedef struct _tEntry
    {
        T data;
        struct _tEntry* next = nullptr;
    } tEntry;

    typedef T T_DATA;
protected:
    tEntry* _root;
public:

    class iterator;

    class const_iterator
    {
        //tEntry* pIt;
        typename List<T, EXTERN_ALLOCATOR>::tEntry* pIt;

        friend class iterator;
    public:
        const_iterator()
            : pIt(nullptr)
        {
        }

        const_iterator(tEntry* pPos)
            : pIt(pPos)
        {
        }

        const_iterator(const iterator& it)
            : pIt(it.pIt)
        {
        }

        void operator = (const const_iterator& it)
        {
            pIt = it.pIt;
        }

        bool operator == (const const_iterator& it) const
        {
            return (pIt == it.pIt);
        }

        bool operator != (const const_iterator& it) const
        {
            return (pIt != it.pIt);
        }

        void operator ++ ()
        {
            assert(pIt);
            pIt = pIt->next;
        }

        const T& operator* () const
        {
            return pIt->data;
        }

        const T* operator ->() const
        {
            return &(operator*());
        }

        const tEntry* _getPtr() const
        {
            return pIt;
        }

        tEntry* _getNonConstPtr() const
        {
            return pIt;
        }

        bool end() const
        {
            if (pIt)
                return false;
            return true;
        }
    };

    class iterator
    {
    protected:
        tEntry* pIt;
        friend class const_iterator;
    public:
        iterator()
            : pIt(nullptr)
        {
        }

        iterator(tEntry* pEntry)
            : pIt(pEntry)
        {
        }

        void operator ++ ()
        {
            //int g = 1;
            assert(pIt);
            pIt = pIt->next;
        }

        tEntry* getEntry()
        {
            return pIt;
        }
        
        tEntry* getNextEntry()
        {
            if (pIt)
                return pIt->next;
            return nullptr;
        }
        
        T& operator* () const
        {
            return pIt->data;
        }

        T* operator ->() const
        {
            return &(operator*());
        }

        void operator = (const iterator& it)
        {
            pIt = it.pIt;
        }

        bool operator == (const iterator& it) const
        {
            return (pIt == it.pIt);
        }

        bool operator != (const iterator& it) const
        {
            return (pIt != it.pIt);
        }

        bool operator == (const const_iterator& it) const
        {
            return (pIt == it.pIt);
        }

        bool operator != (const const_iterator& it) const
        {
            return (pIt != it.pIt);
        }

        bool end() const
        {
            if (pIt)
                return false;
            return true;
        }
        
        bool isEnd() const
        {
            if (pIt)
                return false;
            return true;
        }
    };
protected:
    
    tEntry* getLast()
    {
        if (!_root)
            return nullptr;
        tEntry* pLast = _root;
        while (pLast->next)
            pLast = pLast->next;
        return pLast;
    }
    
    void push_back(tEntry* pNew)
    {
        pNew->next = nullptr;
        tEntry* pLast = getLast();
        if (!pLast)
            _root = pNew;
        else
            pLast->next = pNew;
    }
    
    void put_front(tEntry* pNew)
    {
        if (_root)
            pNew->next = _root;
        else
            pNew->next = nullptr;
        
        _root = pNew;
    }
    
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
    
    void remove_front()
    {
        assert(_root);
        _root = _root->next;
    }

  public:
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

    List()
        : _root(nullptr)
    {
    }
    
    List(const List<T, EXTERN_ALLOCATOR>& lst)
        : _root(nullptr)
    {
        operator = (lst);
    }

    ~List()
    {
        clean();
    }

    tEntry* getFirstEntry()
    {
        return _root;
    }
    
    const tEntry* getFirstEntry() const
    {
        return _root;
    }
    
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

    inline iterator push_back(const T& val)
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        pNew->next = nullptr;
        push_back(pNew);
        return iterator(pNew);
    }


    inline iterator put_front(const T& val)
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        put_front(pNew);
        return iterator(_root);
    }
    
    inline iterator put_front_ext(const T& val)
    {
        static_assert(EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->data = val;
        put_front(pNew);
        return iterator(_root);
    }

    inline void pop_front()
    {
        static_assert(!EXTERN_ALLOCATOR);
        assert(_root);

        tEntry* pNext = _root->next;
        delete _root;
        _root = pNext;
    }

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

    inline iterator push_back()
    {
        static_assert(!EXTERN_ALLOCATOR);
        tEntry* pNew = new tEntry;
        pNew->next = nullptr;
        push_back(pNew);
        return iterator(pNew);
    }

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

    bool isEmpty() const
    {
        return (_root == nullptr);
    }
    
    bool isNotEmpty() const
    {
        return (_root != nullptr);
    }
    
    bool hasEntries() const
    {
        return (_root != nullptr);
    }

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

    iterator begin()
    {
        return iterator(_root);
    }

    const_iterator begin() const
    {
        return const_iterator(_root);
    }

    const_iterator cbegin() const
    {
        return const_iterator(_root);
    }

    iterator end()
    {
        return iterator();
    }

    const_iterator cend() const
    {
        return const_iterator();
    }

    const_iterator end() const
    {
        return const_iterator();
    }

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
