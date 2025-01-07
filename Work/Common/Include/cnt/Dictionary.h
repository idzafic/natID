// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <algorithm>
#include <td/Types.h>
#include <cassert>

namespace cnt
{
	template <typename TKey, typename TVal>
	class Dictionary
	{
	public:
        typedef struct _TE
        {
            TKey first;
            TVal second;
        } TEntry;
        
		typedef TEntry* iterator;

		typedef const TEntry* const_iterator;

	protected:
		typedef struct _tStackEntry
		{
			TEntry** ref;
			int from;
			int to;
		} tStackEntry;
	protected:	
		TEntry* _entries = nullptr;
        td::UINT4 _size = 0;
        td::UINT4 _noEntries = 0;
        td::BYTE _bulk = 0;
        td::BYTE _modified = 0;
        td::BYTE _growStrategy = 0; //0-duplicate (default), 1-grow by one,
                                    //2...254 -otherwise grow by _growStrategy
                                    //255 - grow by 64*1024 entries,
	protected:
        
        inline void sort()
        {
            struct
            {
                bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
            }  mySorter;
            
            std::sort(begin(), end(), mySorter);
        }
        
        const TEntry* findEntry(const TKey& key, td::UINT4 fromPos, td::UINT4 toPos) const
        {
            if (fromPos > toPos)
                return nullptr;
            
            td::UINT4 mid = (fromPos + toPos) / 2;
            if (key == _entries[mid].first)
                return _entries + mid;

            if (key < _entries[mid].first)
            {
                if (mid <= fromPos)
                    return nullptr;
//                if (mid == 0)
//                    return nullptr;
                
                return findEntry(key,fromPos, mid-1);
            }
            
            if (mid >= toPos)
                return nullptr;
            return findEntry(key, mid+1, toPos);
        }
        
	public:

        Dictionary(td::UINT4 size)
        : _entries(nullptr)
        , _size(size)
        , _noEntries(0)
        {
            if (size > 0)
                reserve(size);
        };

        Dictionary()
        : _entries(nullptr)
        , _size(0)
        , _noEntries(0)
        {
        }
        
        ~Dictionary(void)
        {
            if (_entries)
                free(_entries);
        }
        
        void clean()
        {
            if (_entries)
            {
                free(_entries);
                _entries = nullptr;
            }
            _size = 0;
            _noEntries = 0;
            _bulk = 0;
            _modified = 0;
            _growStrategy = 0;
        }

        void reserve(td::UINT4 size)
        {
            if (size == _size)
                return;
            
            if (_entries)
            {
                if (_size < size)
                {
                    free(_entries);
                    _entries = (TEntry*)malloc(sizeof(TEntry)*size);
                    _size = size;
                    return;
                }
                
                _entries = (TEntry*)realloc(_entries, sizeof(TEntry) * size);
                _size = size;
            }

            _entries = (TEntry*)malloc(sizeof(TEntry)*size);
            _size = size;
        }

        iterator begin()
        {
            return (iterator) _entries;
        }
        
        iterator begin() const
        {
            return (iterator) _entries;
        }
        
        iterator end()
        {
            return (iterator) (_entries + _noEntries);
        }

        //iterator end()
        //{
        //    return (iterator) (_entries + _noEntries);
        //}

        const_iterator cbegin() const
        {
            return (iterator) _entries;
        }

        const_iterator end() const
        {
            return (iterator) (_entries + _noEntries);
        }
        
        const_iterator cend() const
        {
            return (iterator) (_entries + _noEntries);
        }

        void insert(const TKey& newKey, const TVal& val)
        {
//            assert(_noEntries < _size);
            if (_noEntries >= _size)
            {
                if (_growStrategy == 0)
                    reserve(_size*2);
                else if (_growStrategy == 1)
                    reserve(_size+1);
                else if (_growStrategy == 255)
                    reserve(_size + 64*1024);
                else
                    reserve(_size + _growStrategy);
            }
            
            _entries[_noEntries].first = newKey;
            _entries[_noEntries].second = val;
            ++_noEntries;
            if (_bulk == 0)
            {
                sort();
                _modified = 0;
            }
            else
                _modified = 1;
            
        }
        
        void startBulkUpdate()
        {
            _bulk = 1;
        }
        
        void endBulkUpdate()
        {
            if (_bulk == 0)
                return;
            _bulk = 0;
            if (_modified != 0)
            {
                sort();
                _modified = 0;
            }
        }

        const_iterator find(const TKey& key) const
        {
            if (_noEntries == 0)
                return end();
            
            //check against first
            TKey currKey = _entries[0].first;
            if (currKey > key)
                return end();
            
            //check against first
            currKey = _entries[_noEntries-1].first;
            if (currKey < key)
                return end();
            
            const TEntry* pEntry = findEntry(key, 0, _noEntries-1);
            
            if (pEntry)
                return const_iterator(pEntry);
            
            return end();
        };
        
        inline bool find(const TKey& key, TVal& val) const
        {
            if (_noEntries == 0)
                return false;
            
            //check against first
            const TKey& currKey = _entries[0].first;
            if (currKey > key)
                return false;
            
            //check against first
            const TKey& currKey2 = _entries[_noEntries-1].first;
            if (currKey2 < key)
                return false;

            const TEntry* pEntry = findEntry(key, 0, _noEntries-1);
            
            if (pEntry)
            {
                val = pEntry->second;
                return true;
            }
            
            return false;
        };
        
        td::UINT4 capacity() const
        {
            return _size;
        }
        
        td::UINT4 size() const
        {
            return _size;
        }
        
        td::UINT4 count() const
        {
            return _noEntries;
        }
        
        bool empty() const
        {
            return (_noEntries == 0);
        }
        
        void setGrowStrategy(td::BYTE growStrategy)
        {
            _growStrategy = growStrategy;
        }
	};
}
