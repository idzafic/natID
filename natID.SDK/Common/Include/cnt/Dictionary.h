// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Dictionary.h
    @brief Sorted key-value dictionary backed by a flat array with binary search lookup. */
#pragma once
#include <algorithm>
#include <td/Types.h>
#include <cassert>

namespace cnt
{
	/// @brief Sorted associative container mapping keys to values using a flat array.
	/// @tparam TKey The key type; must support operator< and operator==.
	/// @tparam TVal The mapped value type.
	template <typename TKey, typename TVal>
	class Dictionary
	{
	public:
        /// @brief A single key-value pair stored in the dictionary.
        using TEntry =  struct _TE
        {
            TKey first;  ///< The key of the entry.
            TVal second; ///< The value associated with the key.
        };

		using iterator = TEntry*; ///< Mutable iterator type.

		using const_iterator = const TEntry*; ///< Immutable iterator type.

	protected:
		/// @brief Internal helper struct used during iterative sorting.
		using tStackEntry = struct _tSE
		{
			TEntry** ref; ///< Pointer to entry pointer.
			int from;     ///< Start index of the range.
			int to;       ///< End index of the range.
        };
	protected:
		TEntry* _entries = nullptr;     ///< Pointer to the flat array of entries.
        td::UINT4 _capacity = 0;        ///< Allocated capacity of the entries array.
        td::UINT4 _noEntries = 0;       ///< Number of entries currently stored.
        td::BYTE _bulk = 0;             ///< Non-zero when bulk update mode is active.
        td::BYTE _modified = 0;         ///< Non-zero if entries were added during bulk update.
        td::BYTE _growStrategy = 0;     ///< Growth strategy: 0=double, 1=by one, 2-254=by value, 255=by 64K.
	protected:

        /// @brief Sorts entries by key using std::sort.
        inline void sort()
        {
            struct
            {
                bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
            }  mySorter;

            std::sort(begin(), end(), mySorter);
        }

        /// @brief Recursively searches for a key using binary search.
        /// @param key     The key to search for.
        /// @param fromPos Lower bound of the search range (inclusive).
        /// @param toPos   Upper bound of the search range (inclusive).
        /// @return Pointer to the matching entry, or nullptr if not found.
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

                return findEntry(key,fromPos, mid-1);
            }

            if (mid >= toPos)
                return nullptr;
            return findEntry(key, mid+1, toPos);
        }

	public:

        /// @brief Constructs a dictionary and pre-allocates storage.
        /// @param capacity Initial number of entries to reserve.
        Dictionary(td::UINT4 capacity)
        : _entries(nullptr)
        , _capacity(0)
        , _noEntries(0)
        {
            if (capacity > 0)
                reserve(capacity);
        };

        /// @brief Default constructor. Creates an empty dictionary with no storage.
        Dictionary()
        : _entries(nullptr)
        , _capacity(0)
        , _noEntries(0)
        {
        }

        /// @brief Destructor. Frees all allocated entry storage.
        ~Dictionary(void)
        {
            if (_entries)
                delete [] _entries;
        }

        /// @brief Removes all entries and resets the dictionary to an empty state.
        void clean()
        {
            if (_entries)
            {
                delete [] _entries;
                _entries = nullptr;
            }
            _capacity = 0;
            _noEntries = 0;
            _bulk = 0;
            _modified = 0;
            _growStrategy = 0;
        }

        /// @brief Reserves storage for the specified number of entries.
        /// @param capacity The desired capacity; existing entries are preserved up to min(old, new).
        void reserve(td::UINT4 capacity)
        {
            if (capacity == 0 || capacity == _capacity)
                return;

            TEntry* newEntries = new TEntry[capacity];

            if (_entries)
            {
                td::UINT4 copySize = _capacity < capacity ? _capacity : capacity;
                for (td::UINT4 i = 0; i < copySize; ++i)
                    newEntries[i] = std::move(_entries[i]);  // Move elements if possible

                delete[] _entries;
            }

            _entries = newEntries;
            _capacity = capacity;
        }

        /// @brief Returns a mutable iterator to the first entry.
        /// @return Iterator pointing to the first entry.
        iterator begin()
        {
            return (iterator) _entries;
        }

        /// @brief Returns a const iterator to the first entry.
        /// @return Const iterator pointing to the first entry.
        iterator begin() const
        {
            return (iterator) _entries;
        }

        /// @brief Returns a mutable iterator past the last entry.
        /// @return Iterator pointing one past the last entry.
        iterator end()
        {
            return (iterator) (_entries + _noEntries);
        }

        //iterator end()
        //{
        //    return (iterator) (_entries + _noEntries);
        //}

        /// @brief Returns a const iterator to the first entry.
        /// @return Const iterator to the first entry.
        const_iterator cbegin() const
        {
            return (iterator) _entries;
        }

        /// @brief Returns a const iterator past the last entry.
        /// @return Const iterator one past the last entry.
        const_iterator end() const
        {
            return (iterator) (_entries + _noEntries);
        }

        /// @brief Returns a const iterator past the last entry.
        /// @return Const iterator one past the last entry.
        const_iterator cend() const
        {
            return (iterator) (_entries + _noEntries);
        }

        /// @brief Inserts a new key-value pair and keeps the dictionary sorted.
        /// @param newKey The key to insert.
        /// @param val    The value associated with the key.
        void insert(const TKey& newKey, const TVal& val)
        {
//            assert(_noEntries < _size);
            if (_noEntries >= _capacity)
            {
                if (_growStrategy == 0)
                    reserve(_capacity*2);
                else if (_growStrategy == 1)
                    reserve(_capacity+1);
                else if (_growStrategy == 255)
                    reserve(_capacity + 64*1024);
                else
                    reserve(_capacity + _growStrategy);
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

        /// @brief Begins a bulk update session; defers sorting until endBulkUpdate() is called.
        void startBulkUpdate()
        {
            _bulk = 1;
        }

        /// @brief Ends a bulk update session and re-sorts the dictionary if entries were added.
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

        /// @brief Searches for a key and returns a const iterator to its entry.
        /// @param key The key to search for.
        /// @return Const iterator to the found entry, or end() if not found.
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
        }

        /// @brief Searches for a key and copies its associated value if found.
        /// @param key The key to search for.
        /// @param val Output parameter filled with the found value.
        /// @return True if the key was found, false otherwise.
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
        }

        /// @brief Searches for a key and returns a pointer to its value.
        /// @param key The key to look up.
        /// @return Const pointer to the found value, or nullptr if the key is absent.
        inline const TVal* findValPtr(const TKey& key) const
        {
            if (_noEntries == 0)
                return nullptr;

            //check against first
            const TKey& currKey = _entries[0].first;
            if (currKey > key)
                return nullptr;

            //check against first
            const TKey& currKey2 = _entries[_noEntries-1].first;
            if (currKey2 < key)
                return nullptr;

            const TEntry* pEntry = findEntry(key, 0, _noEntries-1);

            if (pEntry)
            {
                return &(pEntry->second);
            }

            return nullptr;
        }

        /// @brief Returns the allocated capacity of the dictionary.
        /// @return The number of entries that can be stored without reallocation.
        td::UINT4 capacity() const
        {
            return _capacity;
        }

        /// @brief Returns the number of entries currently stored.
        /// @return The current entry count.
        td::UINT4 size() const
        {
            return _noEntries;
        }

        /// @brief Returns the number of entries currently stored (alias for size()).
        /// @return The current entry count.
        td::UINT4 count() const
        {
            return _noEntries;
        }

        /// @brief Checks whether the dictionary contains no entries.
        /// @return True if the dictionary is empty, false otherwise.
        bool empty() const
        {
            return (_noEntries == 0);
        }

        /// @brief Sets the growth strategy used when the dictionary needs to expand.
        /// @param growStrategy 0=double, 1=grow by one, 2-254=grow by that amount, 255=grow by 64K.
        void setGrowStrategy(td::BYTE growStrategy)
        {
            _growStrategy = growStrategy;
        }
	};
}
