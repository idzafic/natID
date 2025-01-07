// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "cnt/Array.h"
#include "mem/CntMemoryManager.h"

namespace cnt
{
	template <class TSTR>
	class HashEntry
	{
	public:
		unsigned int nEntries;
		unsigned int hashNo;
		TSTR* pName;

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

		HashEntry()
			: nEntries(1)
			, hashNo(0)
			, pName(0){}

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

		template<class OSTREAM>
		void show(OSTREAM& o) const
		{
			o << hashNo << " (" << nEntries << ") " << pName->c_str() << '\n';
		}
	};

	///SIZE must be power of 2
	template <class TBUCKETCNT, class TSTR, unsigned int SIZE>
	class HashList
	{
		typedef typename TBUCKETCNT::const_iterator ITERHASHENTRY;
	protected:	
		 
		Array<TBUCKETCNT, SIZE + 1> entries;
		mem::CntMemoryManager<TBUCKETCNT, 128> _hashBucketMemMgr;
	public:		
		inline HashList()
		{
		}

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

		inline const HashEntry<TSTR>* find(unsigned int hashKey, const TSTR* pStr) const
		{
			HashEntry<TSTR> e(hashKey, pStr);
			int pos = hashKey & SIZE;
			return entries[pos].exists(e);			
		}

		inline const HashEntry<TSTR>* find(unsigned int hashKey, const char* pStr) const
		{
			int pos = hashKey & SIZE;			
			return entries[pos].exists(hashKey, pStr);
		}

		inline const HashEntry<TSTR>* find(unsigned int hashKey) const
		{
			int pos = hashKey & SIZE;			
			return entries[pos].exists(hashKey);
		}

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
