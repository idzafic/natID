// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CntFixedSizeMemoryPool.h
    @brief Fixed-capacity memory pool that allocates container entries from a pre-reserved vector. */
#pragma once
#include <cassert>
#include <cnt/List.h>
#include <cnt/ListSL.h>
#include <cnt/SafeFullVector.h>


namespace mem
{
	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>
	/// @brief A fixed-size memory pool backed by a SafeFullVector that hands out pre-allocated container entry nodes.
	/// @tparam CNT Container type (e.g. cnt::List or cnt::ListSL) whose entry and data types are used.
	template <class CNT>
	class CntFixedSizeMemoryPool
	{
	protected:
        typedef typename CNT::tEntry T_UNIT;  ///< Type of a single container node entry.
		typedef typename CNT::T_DATA T_DATA;  ///< Type of the data held within each node.
		typedef cnt::SafeFullVector<T_UNIT> TCNTDATA; ///< Type of the backing storage vector.
        //cnt::SafeFullVector<T_UNIT> _data;
		TCNTDATA _data; ///< Pre-allocated vector of container node entries.

        size_t _pos = 0; ///< Index of the next free slot in _data.
	public:

		/// @brief Default constructor; creates an empty pool.
		inline CntFixedSizeMemoryPool()
		{
		}

		/// @brief Reserves storage for nElems entries in the pool.
		/// @param nElems Number of entries to pre-allocate.
        void reserve(size_t nElems)
        {
            _data.reserve(nElems);
        }

		/// @brief Allocates the next entry from the pool and appends it to a singly-linked list.
		/// @param lst Singly-linked list to which the new entry is appended.
		/// @return Pointer to the data member of the newly allocated entry.
		inline T_DATA* push_back(cnt::ListSL<T_DATA, true>& lst)
		{
            T_UNIT& entry = _data[_pos++];
			lst.push_back(&entry);
			T_DATA* toRet = &(entry.data);
			return toRet;
		}

        /// @brief Allocates the next entry from the pool and appends it to a doubly-linked list.
        /// @param lst Doubly-linked list to which the new entry is appended.
        /// @return Pointer to the data member of the newly allocated entry.
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst)
        {
            T_UNIT& entry = _data[_pos++];
            lst.push_back(&entry);
            T_DATA* toRet = &(entry.data);
            return toRet;
        }
	};
}
