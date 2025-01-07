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

namespace cnt
{
	template <typename T>
	class HashBucket
	{
	public:
		typedef struct _tEntry
		{
			T data;
			struct _tEntry* next;
		} tEntry;
		typedef T T_DATA;
	protected:
		tEntry* _root;
	public:

		class iterator;

		class const_iterator
		{
			typename HashBucket<T>::tEntry* pIt;
			friend class iterator;
		public:
			inline const_iterator()
				: pIt(nullptr)
			{
			}

			inline const_iterator(tEntry* pPos)
				: pIt(pPos)
			{
			}

			inline const_iterator(const iterator& it)
				: pIt(it.pIt)
			{
			}

			inline void operator = (const const_iterator& it)
			{
				pIt = it.pIt;
			}

			inline bool operator == (const const_iterator& it) const
			{
				return (pIt == it.pIt);
			}

			inline bool operator != (const const_iterator& it) const
			{
				return (pIt != it.pIt);
			}

			inline void operator ++ ()
			{
				assert(pIt);
				pIt = pIt->next;
			}

			inline const T& operator* () const
			{
				return pIt->data;
			}

			inline const T* operator ->() const
			{
				return &(operator*());
			}

			inline bool end() const
			{
				if (pIt)
					return false;
				return true;
			}
		};

		class iterator
		{
		protected:
			//tEntry* pIt;
			typename HashBucket<T>::tEntry* pIt;
			friend class const_iterator;
		public:
			inline iterator()
				: pIt(nullptr)
			{
			}

			inline iterator(tEntry* pEntry)
				: pIt(pEntry)
			{
			}

			inline void operator ++ ()
			{
				assert(pIt);
				pIt = pIt->next;
			}

			inline T& operator* () const
			{
				return pIt->data;
			}

			inline T* operator ->() const
			{
				return &(operator*());
			}

			inline void operator = (const iterator& it)
			{
				pIt = it.pIt;
			}

			inline bool operator == (const iterator& it) const
			{
				return (pIt == it.pIt);
			}

			inline bool operator != (const iterator& it) const
			{
				return (pIt != it.pIt);
			}

			inline bool operator == (const const_iterator& it) const
			{
				return (pIt == it.pIt);
			}

			inline bool operator != (const const_iterator& it) const
			{
				return (pIt != it.pIt);
			}

			inline bool end() const
			{
				if (pIt)
					return false;
				return true;
			}
		};
	public:
		inline HashBucket()
			: _root(nullptr)
		{
		}

		inline ~HashBucket()
		{
			//memory manager is used
		}

		inline T& first()
		{
			assert(_root);
			return _root->data;
		}		

		inline bool isEmpty() const
		{
			if (_root == 0)
				return true;
			return false;
		}

		template<typename TALLOC>
		inline T& push(const T& val, TALLOC& alloc)
		{
			tEntry* pNew = alloc.alloc();
			pNew->data = val;
			pNew->next = _root;
			_root = pNew;
			return pNew->data;
		}
		template<typename TALLOC>
		inline T& push(TALLOC& alloc)
		{
			tEntry* pNew = alloc.alloc();
			pNew->next = _root;
			_root = pNew;
			return pNew->data;
		}

		inline void pop()
		{
			assert(_root);
			tEntry* pEntry = _root->next;
			delete _root;
			_root = pEntry;
		}

		inline const T* exists(const T& val) const
		{
			const tEntry* pEntry = _root;
			while (pEntry)
			{
				if (pEntry->data == val)
				{
					return &pEntry->data;
				}
				pEntry = pEntry->next;
			}
			return 0;
		}
		
		inline T* exists(unsigned int hashNo, int nLen) const
		{
			tEntry* pEntry = _root;
			while (pEntry)
			{
				if ( (pEntry->data.hashNo == hashNo) && (pEntry->data.pName->length() == nLen) )
				{
					return &pEntry->data;
				}
				pEntry = pEntry->next;
			}
			return 0;
		}

		inline T* exists(unsigned int hashNo) const
		{
			tEntry* pEntry = _root;
			while (pEntry)
			{
				if ( pEntry->data.hashNo == hashNo)
				{
					return &pEntry->data;
				}
				pEntry = pEntry->next;
			}
			return nullptr;
		}

		inline T* exists(unsigned int hashNo, const char* str) const
		{
			tEntry* pEntry = _root;
			while (pEntry)
			{
				if ( (pEntry->data.hashNo == hashNo) && (strcmp(pEntry->data.pName->c_str(), str) == 0))
				{
					return &pEntry->data;
				}
				pEntry = pEntry->next;
			}
			return nullptr;
		}

		inline T* exists(unsigned int hashNo, int len, const char* str) const
		{
			tEntry* pEntry = _root;
			while (pEntry)
			{
				if (((unsigned int) pEntry->data.hashNo == hashNo))
				{
					if (pEntry->data.pName)
					{
						if (pEntry->data.pName->isEqual(str, len))
							return &pEntry->data;
					}
					else
						return &pEntry->data;
				}
				pEntry = pEntry->next;
			}
			return nullptr;
		}

		inline T& top() const
		{
			return _root->data;
		}

		inline void clean()
		{
			//while (_root)
			//{
			//	tEntry* pEntry = _root->next;
			//	delete _root;
			_root = nullptr;
			//}
		}

		inline iterator begin()
		{
			return iterator(_root);
		}

		inline const_iterator begin() const
		{
			return const_iterator(_root);
		}

		inline const_iterator end() const
		{
			return const_iterator();
		}
	};
};
