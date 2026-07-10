// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HashList.h
    @brief Hash table mapping string keys to HashEntry records using open bucket chaining. */
#pragma once
#include "cnt/Array.h"
#include "mem/CntMemoryManager.h"

namespace cnt
{
	/// @brief A single entry in the hash table associating a hash code with a string object.
	/// @tparam TSTR String type stored in the entry (must provide c_str() and equality comparison).
	template <class TSTR>
	class HashEntry
	{
	public:
		unsigned int nEntries; ///< Number of times this entry has been referenced.
		unsigned int hashNo;   ///< Hash code of the stored string.
		TSTR* pName;           ///< Pointer to the string associated with this hash entry.

		/// @brief Equality operator comparing two entries by their hash code.
		/// @param entry The entry to compare against.
		/// @return True if both entries share the same hash code.
		bool operator == (const HashEntry& entry)
		{
			if (hashNo == entry.hashNo)
			{
#ifdef _DEBUG
				assert(*pName == *entry.pName);
#endif
				return true;
			}
			return false;
			//return ( (hashNo == entry.hashNo) && (*pName == *entry.pName) );
		}

		/// @brief Default constructor. Initialises an empty entry with zero hash and null name.
		HashEntry()
			: nEntries(1)
			, hashNo(0)
			, pName(0){}

		/// @brief Constructs an entry with the given hash code and string pointer.
		/// @param hashKey Hash code of the string.
		/// @param pStr    Pointer to the string object.
		HashEntry (unsigned int hashKey, TSTR* pStr)
			: nEntries(1)
			, hashNo(hashKey)
			, pName(pStr)
		{
		}

		//~HashEntry ()
		//{
		//	if (pName)
		//		delete pName;
		//}

		/// @brief Writes a human-readable representation of this entry to the given stream.
		/// @tparam OSTREAM Output stream type.
		/// @param o The output stream.
		template<class OSTREAM>
		void show(OSTREAM& o) const
		{
			o << hashNo << " (" << nEntries << ") " << pName->c_str() << '\n';
		}
	};

	///SIZE must be power of 2
	/// @brief Fixed-size hash table that stores HashEntry objects in bucket chains.
	/// @tparam TBUCKETCNT Container type used for each hash bucket (e.g., HashBucket<HashEntry<TSTR>>).
	/// @tparam TSTR       String type stored in hash entries.
	/// @tparam SIZE       Number of hash buckets minus one; must be a power of 2.
	template <class TBUCKETCNT, class TSTR, unsigned int SIZE>
	class HashList
	{
		typedef typename TBUCKETCNT::const_iterator ITERHASHENTRY; ///< Const iterator type for bucket traversal.
	protected:

		Array<TBUCKETCNT, SIZE + 1> entries;                          ///< Array of hash buckets; size is SIZE+1.
		mem::CntMemoryManager<TBUCKETCNT, 128> _hashBucketMemMgr;     ///< Memory manager for bucket node allocation.
	public:
		/// @brief Default constructor. Creates an empty hash list.
		inline HashList()
		{
		}

        /// @brief Resets all buckets and the memory manager to an empty state.
        void reset()
        {
            for (auto& bucket : entries)
            {
                bucket.clean();
            }
            _hashBucketMemMgr.clean();
        }
		//inline ~HashList()
		//{
			//for (int i=0; i<SIZE+1; ++i)
			//{
			//	ITERHASHENTRY it = entries[i].begin();
			//	ITERHASHENTRY itEnd = entries[i].end();
			//	while (it != itEnd)
			//	{
			//		if (it->pName)
			//			delete it->pName;
			//		++it;
			//	}
			//}
			//_hashBucketMemMgr.clean();
		//}

		/// @brief Adds a string to the hash list, creating a new entry or incrementing an existing one.
		/// @tparam TCHAR Character type of the input string.
		/// @param hashKey Hash code computed from pStr.
		/// @param pStr    Null-terminated string to add.
		/// @return Pointer to the resulting HashEntry (never null).
		template <typename TCHAR>
		inline const HashEntry<TSTR>* add(unsigned int hashKey, const TCHAR* pStr)
		{
			int pos = hashKey & SIZE;
			HashEntry<TSTR>* toRet = entries[pos].exists(hashKey, pStr);
			if (toRet == 0)
			{
				//std::cout << "HashList! ENTERING element at position " << pos << std::endl;
				TSTR* pNewStr = new TSTR(pStr);
				HashEntry<TSTR> e(hashKey, pNewStr);
				return &(entries[pos].push(e, _hashBucketMemMgr));
			}
			toRet->nEntries++;
			//std::cout << "HashList! Element FOUND at position " << pos << std::endl;
			return toRet;
		}

		/// @brief Adds a length-delimited string using an external allocation holder.
		/// @tparam TCHAR       Character type of the input string.
		/// @tparam TALLOCHOLDER Type that provides allocStringObject().
		/// @param hashKey     Hash code computed from pStr.
		/// @param strLen      Length of the string in characters.
		/// @param pStr        Pointer to the string characters.
		/// @param allocHolder Object used to allocate the TSTR instance.
		/// @return Pointer to the resulting HashEntry (never null).
		template <typename TCHAR, class TALLOCHOLDER>
		inline const HashEntry<TSTR>* add(unsigned int hashKey, int strLen, const TCHAR* pStr, TALLOCHOLDER* allocHolder)
		{
			int pos = hashKey & SIZE;
			HashEntry<TSTR>* toRet = entries[pos].exists(hashKey, strLen, pStr);
			if (toRet == nullptr)
			{
				//std::cout << "HashList! ENTERING element at position " << pos << std::endl;
				TSTR* pNewStr = allocHolder->allocStringObject(strLen, pStr);//new TSTR(pStr);
                if (pNewStr)
                {
                    HashEntry<TSTR> e(hashKey, pNewStr);
                    return &(entries[pos].push(e, _hashBucketMemMgr));
                }
			}
            assert(toRet);
			toRet->nEntries++;
			//std::cout << "HashList! Element FOUND at position " << pos << std::endl;
			return toRet;
		}

		/// @brief Looks up a hash entry by hash code and TSTR pointer.
		/// @param hashKey Hash code to search for.
		/// @param pStr    Pointer to the TSTR object to compare.
		/// @return Const pointer to the matching entry, or nullptr if not found.
		inline const HashEntry<TSTR>* find(unsigned int hashKey, const TSTR* pStr) const
		{
			HashEntry<TSTR> e(hashKey, pStr);
			int pos = hashKey & SIZE;
			return entries[pos].exists(e);
		}

		/// @brief Looks up a hash entry by hash code and C-string.
		/// @param hashKey Hash code to search for.
		/// @param pStr    Null-terminated C-string to compare.
		/// @return Const pointer to the matching entry, or nullptr if not found.
		inline const HashEntry<TSTR>* find(unsigned int hashKey, const char* pStr) const
		{
			int pos = hashKey & SIZE;
			return entries[pos].exists(hashKey, pStr);
		}

		/// @brief Looks up a hash entry by hash code alone.
		/// @param hashKey Hash code to search for.
		/// @return Const pointer to the matching entry, or nullptr if not found.
		inline const HashEntry<TSTR>* find(unsigned int hashKey) const
		{
			int pos = hashKey & SIZE;
			return entries[pos].exists(hashKey);
		}

		/// @brief Writes all non-empty buckets and their entries to the given stream.
		/// @tparam OSTREAM Output stream type.
		/// @param o The output stream.
		template <class OSTREAM>
		inline void show(OSTREAM& o) const
		{
			for (unsigned int i=0; i< SIZE+1; ++i)
			{
				if (!entries[i].isEmpty())
				{
					o << i << "\n";
                                        typename TBUCKETCNT::const_iterator it = entries[i].begin();
                                        typename TBUCKETCNT::const_iterator itEnd = entries[i].end();
					while (it != itEnd)
					{
						o << '\t';
						it->show(o);
						++it;
					}
				}
			}
		}
	};
};
