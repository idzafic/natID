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
template <class TLIST, size_t CHUNK_SIZE>
class ListManager;

template <class CNT, td::UINT4 SINGLEBUFFLEN>
class CntMemoryManager;

template <class CNT>
class CntFixedSizeMemoryPool;
}

namespace cnt
{
	template <typename T, bool EXTERN_ALLOCATOR = false>
	class ListSL
	{
        
        template <class TLIST, size_t CHUNK_SIZE>
        friend class mem::ListManager;
        
		template <class CNT, td::UINT4 SINGLEBUFFLEN>
		friend class mem::CntMemoryManager;
        
        template <class CNT>
        friend class mem::CntFixedSizeMemoryPool;

		template <typename T_BV, int BLOCK_SIZE_BV>
		friend class BlockVector;

	public:
		typedef struct _tEntry
		{
			T data;
			struct _tEntry* next;
		} tEntry;

		typedef T T_DATA;
	protected:
		tEntry* _root = nullptr;
		tEntry* _last = nullptr;
	public:

		class iterator;

		class const_iterator
		{
			//tEntry* pIt;
			typename ListSL<T, EXTERN_ALLOCATOR>::tEntry* pIt;

			friend class iterator;
		public:
			const_iterator()
				: pIt(0)
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
				: pIt(0)
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
		void push_back(tEntry* pNew)
		{
			//assert(!EXTERN_ALLOCATOR);
			pNew->next = nullptr;
			if (_last)
			{
				_last->next = pNew;
				_last = pNew;
			}
			else
			{
				_root = _last = pNew;
			}			
		}
        
        void put_front(tEntry* pNew)
        {
            if (_root)
                pNew->next = _root;
            else
                pNew->next = nullptr;
            
            _root = pNew;
            if (!_last)
                _last = _root;
        }
        
        void remove_front()
        {
            assert(_root);
            _root = _root->next;
            if (!_root)
                _last = nullptr;
        }
        
        void insert_ext(iterator& it, tEntry* pNew)
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
            
            while (pPrev && (pPrev->next != pEntry) )
            {
                pPrev = pPrev->next;
            }
            
            if (!pPrev)
            {
                push_back(pNew);
                return;
            }
            
            auto tmp = pPrev->next;
            pPrev->next = pNew;
            pNew->next = tmp;
        }
				
		//void addRef(const td::Ref<T>& ref)
		//{
		//	td::Ref<T>& ref2 = const_cast<td::Ref<T>&>(ref);
		//	ref2.addReference();			
		//}

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

		ListSL()
		{
		}

		void operator = (const ListSL& lst)
		{
			assert(!EXTERN_ALLOCATOR);
			clean();
			const_iterator it(lst.begin());
			const_iterator itEnd(lst.end());
			while (it != itEnd)
			{
				push_back(*it);
				++it;
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
			operator<<(std::basic_ostream<CharT, Traits>& os, const ListSL& rhs)
		{
			rhs.saveToBinaryStream(os);
			return os;
		}
		
		template <class SER>
		bool loadFromBinaryStream(SER& ar)
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

		ListSL(const ListSL<T, EXTERN_ALLOCATOR>& lst)
			: _root(0)
			, _last(0)
		{
			operator = (lst);
		}

		~ListSL()
		{
			clean();
		}

		T& last()
		{
			assert(_last);
			return _last->data;
		}

		T& first()
		{
			assert(_root);
			return _root->data;
		}
        
        T* firstPtr()
        {
            if(_root)
                return &_root->data;
            return nullptr;
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
			assert(!EXTERN_ALLOCATOR);			
			tEntry* pNew = new tEntry;
			pNew->data = val;
			pNew->next = nullptr;
			if (_last)
			{							
				_last->next = pNew;
				_last = pNew;
			}
			else
			{				
				_root = _last = pNew;
			}
			return iterator(_last);
		}

		template <typename T_CHAR, td::StringEncoding Encoding>
		inline iterator push_front(const td::StringBase<T_CHAR, Encoding, 4, false>& val)
		{
			assert(!EXTERN_ALLOCATOR);
			td::StringBase<T_CHAR, Encoding, 4, false>& val2 = const_cast<td::StringBase<T_CHAR, Encoding, 4, false>&>(val);
			val2.addRef();
			tEntry* pNew = new tEntry;
			pNew->data = val2;

			if (_root)
			{
				pNew->next = _root;
				_root = pNew;
			}
			else
			{
				pNew->next = nullptr;
				_root = _last = pNew;
			}
			return iterator(_root);
		}


		iterator push_front(const T& val)
		{
			assert(!EXTERN_ALLOCATOR);
			tEntry* pNew = new tEntry;
			pNew->data = val;
			
			if (_root)
			{							
				pNew->next = _root;
				_root = pNew;
			}
			else
			{				
				pNew->next = nullptr;
				_root = _last = pNew;
			}
			return iterator(_root);
		}

		void pop_front()
		{
			assert(!EXTERN_ALLOCATOR);
			assert(_root);

			tEntry* pNext = _root->next;
			delete _root;
			_root = pNext;
		}

		T& push()
		{
			assert(!EXTERN_ALLOCATOR);
			tEntry* pNew = new tEntry;
			pNew->next = nullptr;
			if (_last)
			{				
				_last->next = pNew;
				_last = pNew;
			}
			else
			{			
				_root = _last = pNew;
			}
			return _last->data;
		}

		iterator push_back()
		{
			assert(!EXTERN_ALLOCATOR);
			tEntry* pNew = new tEntry;
			pNew->next = nullptr;
			if (_last)
			{				
				_last->next = pNew;
				_last = pNew;
			}
			else
			{				
				_root = _last = pNew;
			}
			//return _last->data;
			return iterator(_last);
		}

		void insert(const const_iterator& it, const T& val)
		{
			assert(!EXTERN_ALLOCATOR);
			if (it.end())
			{
				push_back(val);
				return;

			}
			
			tEntry* pNew = new tEntry;
            
            tEntry* pEntry = const_cast<tEntry*>(it._getPtr());
			if (pEntry->next == nullptr)
				_last = pNew;
			pNew->next = pEntry->next;
			pEntry->next = pNew;
			pNew->data = val;
		}

		bool isEmpty() const
		{
			return (_root == nullptr);
		}


		void clean()
		{
			if (!EXTERN_ALLOCATOR)
			{
				while (_root)
				{
					tEntry* pEntry = _root->next;
					delete _root;
					_root = pEntry;
				}
				_last = nullptr;
			}
            //else
            {
                _root = nullptr;
                _last = nullptr;
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

		iterator lastIter()
		{
			return iterator(_last);
		}

		const_iterator lastIter() const
		{
			return const_iterator(_last);
		}

		size_t size() const
		{
			const tEntry* pEntry = _root;
			if (!pEntry)
				return 0;

			size_t i = 1;
			while (pEntry != _last)
			{
				++i;
				pEntry = pEntry->next;
			}
			return i;
		}
        
        bool hasSize(size_t requestedSize) const
        {
            const tEntry* pEntry = _root;
            if (!pEntry)
            {
                if (requestedSize == 0)
                    return true;
                return false;
            }
            size_t i = 1;
            while (pEntry != _last)
            {
                if ( i> requestedSize)
                    return false;
                ++i;
                pEntry = pEntry->next;
            }
            return (i == requestedSize);
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
                        if (_last == pEntry)
                            _last = _root;
                    }
                    else
                    {
                        if (pEntry->next == nullptr)
                        {
                            _last = pPrev;
                            pPrev->next = nullptr;
                        }
                        else
                            pPrev->next = pEntry->next;
                    }
                    delete pEntry;
                    return true;
                }
                pPrev = pEntry;
                pEntry = pEntry->next;
            }
            return false;
        }
	};


	//double linked list
	template <typename T>
	class ListDL
	{
	public:
		typedef struct _tEntry
		{
			T data;
			struct _tEntry* next;
			struct _tEntry* prev;
		} tEntry;

	protected:
		tEntry* _root;
		tEntry* _last;
	public:

		class iterator;

		class const_iterator
		{
			tEntry* pIt;
			friend class iterator;
		public:
			const_iterator()
				: pIt(0)
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

			void operator -- ()
			{
				assert(pIt);
				pIt = pIt->prev;
			}

			const T& operator* () const
			{
				return pIt->data;
			}

			const T* operator ->() const
			{
				return &(operator*());
			}
		};

		class iterator
		{
		protected:
			tEntry* pIt;
			friend class const_iterator;
		public:
			iterator()
				: pIt(0)
			{
			}

			iterator(tEntry* pEntry)
				: pIt(pEntry)
			{
			}

			void operator ++ ()
			{
				assert(pIt);
				pIt = pIt->next;
			}

			void operator -- ()
			{
				assert(pIt);
				pIt = pIt->prev;
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
		};
	public:

		ListDL()
			: _root(0)
			, _last(0)
		{
		}

		~ListDL()
		{
			while (_root)
			{
				tEntry* pEntry = _root->next;
				delete _root;
				_root = pEntry;
			}
		}

		void push_back(const T& val)
		{
			if (_last)
			{
				tEntry* pNew = new tEntry;
				pNew->data = val;
				pNew->next = 0;
				pNew->prev = _last;
				_last->next = pNew;
				_last = pNew;
			}
			else
			{
				tEntry* pNew = new tEntry;
				pNew->data = val;
				pNew->next = 0;
				pNew->prev = 0;
				_root = _last = pNew;
			}
		}

		T& last()
		{
			assert(_last);
			return _last->data;
		}

		T& first()
		{
			assert(_root);
			return _root->data;

		}

		iterator begin()
		{
			return iterator(_root);
		}

		const_iterator begin() const
		{
			return const_iterator(_root);
		}

		iterator rbegin()
		{
			return iterator(_last);
		}

		const_iterator rbegin() const
		{
			return const_iterator(_last);
		}

		const_iterator end() const
		{
			return const_iterator();
		}

		const_iterator rend() const
		{
			return const_iterator();
		}

		int size() const
		{
			const tEntry* pEntry = _root;
			if (!pEntry)
				return 0;

			int i = 1;
			while (pEntry != _last)
			{
				++i;
				pEntry = pEntry->next;
			}
			return i;
		}	
	};
};
