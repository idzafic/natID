// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CntMemoryManager.h
    @brief Dynamic chunked memory manager that allocates container node entries from fixed-size buffer chunks. */
#pragma once
#include <cassert>
#include <cnt/ListSL.h>
#include <cnt/List.h>
#include <cnt/SafeFullVector.h>


namespace mem
{
	//template <typename T_CHAR, StringEncoding Encoding, int SPACE_FOR_SIZE>
	/// @brief A growable memory manager that allocates container node entries from a linked list of fixed-size buffer chunks,
	///        avoiding per-element heap allocation.
	/// @tparam CNT Container type (e.g. cnt::List or cnt::ListSL) whose entry and data types are used.
	/// @tparam SINGLEBUFFLEN Number of entries per buffer chunk (default: 4096).
	template <class CNT, td::UINT4 SINGLEBUFFLEN = 1024 * 4>
	class CntMemoryManager : public cnt::ListSL< typename CNT::tEntry[SINGLEBUFFLEN], false >
	{
	protected:
		typedef typename CNT::T_DATA T_DATA;                                      ///< Data type stored within each container node.
		//typedef typename T_UNIT[SINGLEBUFFLEN] CHUNK;
		typedef cnt::ListSL<typename CNT::tEntry[SINGLEBUFFLEN], false> BASE;     ///< Base singly-linked list of buffer chunks.
		typedef typename CNT::iterator CNT_ITER;                                  ///< Iterator type of the managed container.
		typedef typename BASE::const_iterator CONST_BASE_ITER;                   ///< Const iterator over the chunk list.
		typedef typename CNT::tEntry T_UNIT;                                      ///< Type of a single container node entry.

		T_UNIT* _firstAvailableUnit = nullptr; ///< Pointer to the first free slot in the current chunk.
		T_UNIT* _lasttAvailableUnit = nullptr; ///< Pointer one past the last slot in the current chunk.
		//BASE_ITER _currentChunk;
	protected:

		/// @brief Allocates a new buffer chunk and updates the available-slot pointers.
		inline void allocateBuffer()
		{
			BASE::push();
			_firstAvailableUnit = (T_UNIT*)(BASE::_last);
			_lasttAvailableUnit = _firstAvailableUnit + SINGLEBUFFLEN;
		}

	public:

        /// @brief Resets the manager to an empty state, releasing all allocated chunks.
        void clean()
        {
            _firstAvailableUnit = nullptr;
            _lasttAvailableUnit = nullptr;
            BASE::clean();
        }
		/// @brief Default constructor; creates an empty manager with no pre-allocated chunks.
		inline CntMemoryManager()
			: _firstAvailableUnit(nullptr)
			, _lasttAvailableUnit(nullptr)
		{
			//BASE::push();
			//_currentChunk = BASE::begin();
			//_firstAvailableUnit = (T_UNIT*)(BASE::_root);
			//_lasttAvailableUnit = _firstAvailableUnit + SINGLEBUFFLEN;
		}

		/*inline CNT_ITER push_back(cnt::ListSL<T_DATA, true>& lst)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);
			++_firstAvailableUnit;
			CNT_ITER it(lst.lastIter());
			return it;
		}*/

		/// @brief Allocates and returns a pointer to the next free T_UNIT slot, growing the pool if needed.
		/// @return Pointer to the newly allocated T_UNIT.
		inline T_UNIT* alloc()
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			//lst.push_back(_firstAvailableUnit);
			T_UNIT* toRet = _firstAvailableUnit;
			++_firstAvailableUnit;
			return toRet;
		}

		/// @brief Allocates the next entry and appends it to a singly-linked list; returns a pointer to its data.
		/// @param lst Singly-linked list to append the new entry to.
		/// @return Pointer to the data member of the newly allocated entry.
		inline T_DATA* push_back(cnt::ListSL<T_DATA, true>& lst)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);
			T_DATA* toRet = &_firstAvailableUnit->data;
			++_firstAvailableUnit;
			return toRet;
		}

		/// @brief Allocates the next entry, sets its data to val, and appends it to a singly-linked list.
		/// @param lst Singly-linked list to append the new entry to.
		/// @param val Value to copy into the new entry's data member.
		inline void push_back(cnt::ListSL<T_DATA, true>& lst, const T_DATA& val)
		{
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				allocateBuffer();
			}
			lst.push_back(_firstAvailableUnit);
			_firstAvailableUnit->data = val;
			++_firstAvailableUnit;
		}

		/// @brief Allocates the next entry and appends it to a SafeFullVector; returns a pointer to its data.
		///        Handles the case where the current chunk is exhausted by copying existing vector data to the new chunk.
		/// @param vect SafeFullVector to grow.
		/// @return Pointer to the newly appended data element.
		inline T_DATA* push_back(cnt::SafeFullVector<T_DATA, true>& vect)
		{
			size_t nToCopy = 0;
			if (_firstAvailableUnit == _lasttAvailableUnit)
			{
				nToCopy = vect._size;
				allocateBuffer();
			}

			if (vect._size == 0)
			{
				//first time
				T_DATA* pNew = _firstAvailableUnit;
				vect._size = 1;
				vect._data = pNew;
				++_firstAvailableUnit;
				return pNew;
			}
			else if (nToCopy == 0)
			{
				//executes in most cases
				size_t pos = vect._size;
				vect._size++;
				++_firstAvailableUnit;
				return vect._data + pos;
			}

			//new vector entry cannot be placed on the previous chunk
			//executes only when vector cannot be allocated on prev. chunk

			assert(nToCopy < SINGLEBUFFLEN);

			T_DATA* pNew = _firstAvailableUnit;
			mem::fastCopy(pNew, vect._data, nToCopy);
			vect._size++;
			vect._data = pNew;
			_firstAvailableUnit += vect._size;
			return vect._data + nToCopy;
		}

		/// @brief Allocates the next entry, sets it to val, and appends it to a SafeFullVector.
		/// @param vect SafeFullVector to grow.
		/// @param val Value to store in the new element.
		inline void push_back(cnt::SafeFullVector<T_DATA, true>& vect, const T_DATA& val)
		{
			T_DATA* pVal = push_back(vect);
			*pVal = val;
		}

        /// @brief Allocates the next entry and appends it to a doubly-linked list; returns a pointer to its data.
        /// @param lst Doubly-linked list to append the new entry to.
        /// @return Pointer to the data member of the newly allocated entry.
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst)
        {
            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            lst.push_back(_firstAvailableUnit);
            T_DATA* toRet = &_firstAvailableUnit->data;
            ++_firstAvailableUnit;
            return toRet;
        }

        /// @brief Allocates the next entry, links it after pPrev, and returns a pointer to its data.
        ///        Falls back to regular push_back if pPrev is nullptr.
        /// @param lst Doubly-linked list that owns pPrev.
        /// @param pPrev Entry after which the new node is inserted; pass nullptr to append at the tail.
        /// @return Pointer to the data member of the newly allocated entry.
        inline T_DATA* push_back(cnt::List<T_DATA, true>& lst, T_UNIT* pPrev)
        {
            if (!pPrev)
                return push_back(lst);

            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            pPrev->next = _firstAvailableUnit;
            T_DATA* toRet = &_firstAvailableUnit->data;
            ++_firstAvailableUnit;
            return toRet;
        }

        /// @brief Allocates the next entry, sets its data to val, and appends it to a doubly-linked list.
        /// @param lst Doubly-linked list to append the new entry to.
        /// @param val Value to copy into the new entry's data member.
        inline void push_back(cnt::List<T_DATA, true>& lst, const T_DATA& val)
        {
            if (_firstAvailableUnit == _lasttAvailableUnit)
            {
                allocateBuffer();
            }
            lst.push_back(_firstAvailableUnit);
            _firstAvailableUnit->data = val;
            ++_firstAvailableUnit;
        }
	};
}
