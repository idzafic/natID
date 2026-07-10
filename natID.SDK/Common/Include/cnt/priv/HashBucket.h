// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HashBucket.h
 @brief Singly-linked hash bucket used internally by hash-map implementations. */
#pragma once
#include <cassert>

namespace cnt
{
	/// @brief Intrusive singly-linked list representing one bucket of a hash table.
	/// @tparam T Element type stored in each node.
	template <typename T>
	class HashBucket
	{
	public:
		/// @brief Intrusive list node holding one element and a pointer to the next node in the bucket.
		typedef struct _tEntry
		{
			T data;              ///< The stored element value.
			struct _tEntry* next; ///< Pointer to the next node in the bucket chain.
		} tEntry;
		typedef T T_DATA; ///< Publicly exposed element type alias.
	protected:
		tEntry* _root; ///< Pointer to the first node in the bucket, nullptr when empty.
	public:

		class iterator;

		/// @brief Constant forward iterator over HashBucket elements.
		class const_iterator
		{
			typename HashBucket<T>::tEntry* pIt; ///< Pointer to the current node.
			friend class iterator;
		public:
			/// @brief Constructs an end (null) const_iterator.
			inline const_iterator()
				: pIt(nullptr)
			{
			}

			/// @brief Constructs a const_iterator pointing to the given node.
			/// @param pPos Pointer to the initial node.
			inline const_iterator(tEntry* pPos)
				: pIt(pPos)
			{
			}

			/// @brief Constructs a const_iterator from a mutable iterator.
			/// @param it The mutable iterator to copy position from.
			inline const_iterator(const iterator& it)
				: pIt(it.pIt)
			{
			}

			/// @brief Assigns another const_iterator to this one.
			/// @param it The source const_iterator.
			inline void operator = (const const_iterator& it)
			{
				pIt = it.pIt;
			}

			/// @brief Checks equality with another const_iterator.
			/// @param it The other const_iterator.
			/// @return True if both point to the same node.
			inline bool operator == (const const_iterator& it) const
			{
				return (pIt == it.pIt);
			}

			/// @brief Checks inequality with another const_iterator.
			/// @param it The other const_iterator.
			/// @return True if they point to different nodes.
			inline bool operator != (const const_iterator& it) const
			{
				return (pIt != it.pIt);
			}

			/// @brief Advances the iterator to the next node.
			inline void operator ++ ()
			{
				assert(pIt);
				pIt = pIt->next;
			}

			/// @brief Dereferences the iterator to obtain a const element reference.
			/// @return Const reference to the element at the current position.
			inline const T& operator* () const
			{
				return pIt->data;
			}

			/// @brief Provides pointer-style member access to the current element.
			/// @return Const pointer to the current element.
			inline const T* operator ->() const
			{
				return &(operator*());
			}

			/// @brief Checks whether the iterator has reached the end of the bucket.
			/// @return True if the iterator is past the last element.
			inline bool end() const
			{
				if (pIt)
					return false;
				return true;
			}
		};

		/// @brief Mutable forward iterator over HashBucket elements.
		class iterator
		{
		protected:
			//tEntry* pIt;
			typename HashBucket<T>::tEntry* pIt; ///< Pointer to the current node.
			friend class const_iterator;
		public:
			/// @brief Constructs an end (null) iterator.
			inline iterator()
				: pIt(nullptr)
			{
			}

			/// @brief Constructs an iterator pointing to the given node.
			/// @param pEntry Pointer to the initial node.
			inline iterator(tEntry* pEntry)
				: pIt(pEntry)
			{
			}

			/// @brief Advances the iterator to the next node.
			inline void operator ++ ()
			{
				assert(pIt);
				pIt = pIt->next;
			}

			/// @brief Dereferences the iterator to obtain a mutable element reference.
			/// @return Reference to the element at the current position.
			inline T& operator* () const
			{
				return pIt->data;
			}

			/// @brief Provides pointer-style member access to the current element.
			/// @return Pointer to the current element.
			inline T* operator ->() const
			{
				return &(operator*());
			}

			/// @brief Assigns another iterator to this one.
			/// @param it The source iterator.
			inline void operator = (const iterator& it)
			{
				pIt = it.pIt;
			}

			/// @brief Checks equality with another iterator.
			/// @param it The other iterator.
			/// @return True if both point to the same node.
			inline bool operator == (const iterator& it) const
			{
				return (pIt == it.pIt);
			}

			/// @brief Checks inequality with another iterator.
			/// @param it The other iterator.
			/// @return True if they point to different nodes.
			inline bool operator != (const iterator& it) const
			{
				return (pIt != it.pIt);
			}

			/// @brief Checks equality with a const_iterator.
			/// @param it The const_iterator to compare against.
			/// @return True if both point to the same node.
			inline bool operator == (const const_iterator& it) const
			{
				return (pIt == it.pIt);
			}

			/// @brief Checks inequality with a const_iterator.
			/// @param it The const_iterator to compare against.
			/// @return True if they point to different nodes.
			inline bool operator != (const const_iterator& it) const
			{
				return (pIt != it.pIt);
			}

			/// @brief Checks whether the iterator has reached the end of the bucket.
			/// @return True if the iterator is past the last element.
			inline bool end() const
			{
				if (pIt)
					return false;
				return true;
			}
		};
	public:
		/// @brief Constructs an empty HashBucket.
		inline HashBucket()
			: _root(nullptr)
		{
		}

		/// @brief Destroys the HashBucket; memory is expected to be managed externally.
		inline ~HashBucket()
		{
			//memory manager is used
		}

		/// @brief Returns a reference to the first element in the bucket.
		/// @return Reference to the first element.
		inline T& first()
		{
			assert(_root);
			return _root->data;
		}

		/// @brief Checks whether the bucket contains no elements.
		/// @return True if the bucket is empty.
		inline bool isEmpty() const
		{
			if (_root == 0)
				return true;
			return false;
		}

		/// @brief Allocates a new node via alloc, copies val into it, and prepends it to the bucket.
		/// @tparam TALLOC  Allocator type providing an alloc() method that returns tEntry*.
		/// @param  val     The value to store.
		/// @param  alloc   The allocator used to obtain the new node.
		/// @return Reference to the newly inserted element.
		template<typename TALLOC>
		inline T& push(const T& val, TALLOC& alloc)
		{
			tEntry* pNew = alloc.alloc();
			pNew->data = val;
			pNew->next = _root;
			_root = pNew;
			return pNew->data;
		}

		/// @brief Allocates a new node via alloc and prepends a default-constructed element to the bucket.
		/// @tparam TALLOC Allocator type providing an alloc() method.
		/// @param  alloc  The allocator used to obtain the new node.
		/// @return Reference to the newly inserted element.
		template<typename TALLOC>
		inline T& push(TALLOC& alloc)
		{
			tEntry* pNew = alloc.alloc();
			pNew->next = _root;
			_root = pNew;
			return pNew->data;
		}

		/// @brief Removes and deletes the first node in the bucket.
		inline void pop()
		{
			assert(_root);
			tEntry* pEntry = _root->next;
			delete _root;
			_root = pEntry;
		}

		/// @brief Searches for the first element equal to val.
		/// @param val The value to search for.
		/// @return Const pointer to the matching element, or nullptr if not found.
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

		/// @brief Searches for the first element matching the given hash number and name length.
		/// @param hashNo Hash value to match.
		/// @param nLen   Required name length to match.
		/// @return Pointer to the matching element, or nullptr if not found.
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

		/// @brief Searches for the first element matching the given hash number.
		/// @param hashNo Hash value to match.
		/// @return Pointer to the matching element, or nullptr if not found.
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

		/// @brief Searches for the first element matching hash number and null-terminated string.
		/// @param hashNo Hash value to match.
		/// @param str    Null-terminated string to compare against the element name.
		/// @return Pointer to the matching element, or nullptr if not found.
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

		/// @brief Searches for the first element matching hash number, string length, and string content.
		/// @param hashNo Hash value to match.
		/// @param len    Expected string length.
		/// @param str    Null-terminated string to compare.
		/// @return Pointer to the matching element, or nullptr if not found.
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

		/// @brief Returns a reference to the first (top) element without removing it.
		/// @return Reference to the first element.
		inline T& top() const
		{
			return _root->data;
		}

		/// @brief Resets the bucket to empty without freeing node memory.
		inline void clean()
		{
			//while (_root)
			//{
			//	tEntry* pEntry = _root->next;
			//	delete _root;
			_root = nullptr;
			//}
		}

		/// @brief Returns an iterator to the first element.
		/// @return Iterator pointing to the first node.
		inline iterator begin()
		{
			return iterator(_root);
		}

		/// @brief Returns a const iterator to the first element.
		/// @return Const iterator pointing to the first node.
		inline const_iterator begin() const
		{
			return const_iterator(_root);
		}

		/// @brief Returns a const past-the-end iterator.
		/// @return Const end iterator.
		inline const_iterator end() const
		{
			return const_iterator();
		}
	};
};
