// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BlockVector.h
 *  @brief A dynamic vector that allocates elements in fixed-size memory blocks (chunks). */
#pragma once
#include <cassert>
#include <ostream>
#include <td/Types.h>
#include <cnt/List.h>

namespace cnt
{
	/// @brief A vector container that stores elements in contiguous fixed-size blocks to reduce reallocation cost.
	/// @tparam T Element type.
	/// @tparam BLOCK_SIZE Number of elements per allocated block.
	template <typename T, int BLOCK_SIZE>
	class BlockVector
	{
		using LIST = cnt::List<T*> ;
		using LIST_ITER = LIST::iterator;
		using CONST_LIST_ITER = LIST::const_iterator;
    public:
        using DataChunk = T*; ///< Pointer to a contiguous block of BLOCK_SIZE elements.
	protected:
        LIST _blocks;                          ///< Linked list of allocated blocks.
        LIST_ITER _lastUsedBlockIterator;       ///< Iterator pointing to the most recently accessed block.
		td::UINT4 _size;                        ///< Number of elements currently stored.
        td::UINT4 _capacity;                   ///< Total capacity across all allocated blocks.
        td::UINT4 _lastUsedBlockPos;            ///< Index of the most recently accessed block.

		/// @brief Returns a pointer to the block at the given block index.
		/// @param blockPos Zero-based index of the desired block.
		/// @return Pointer to the array of elements in that block.
		inline DataChunk getBlock(td::UINT4 blockPos)
		{
            DataChunk pBlock = nullptr;
			if (_lastUsedBlockPos == blockPos && !_lastUsedBlockIterator.end())
			{
				pBlock = *_lastUsedBlockIterator;
			}
			else if (_lastUsedBlockPos < blockPos)
			{
				do
				{
					++_lastUsedBlockPos;
					++_lastUsedBlockIterator;
				} while (_lastUsedBlockPos != blockPos);

				pBlock = *_lastUsedBlockIterator;
			}
			else
			{
				_lastUsedBlockIterator  = _blocks[blockPos];
				pBlock = *_lastUsedBlockIterator;
				_lastUsedBlockPos = blockPos;
			}

			return pBlock;
		}

		/// @brief Const overload of getBlock.
		/// @param blockPos Zero-based index of the desired block.
		/// @return Const pointer to the array of elements in that block.
        const inline DataChunk getBlock(td::UINT4 blockPos) const
        {
            BlockVector<T, BLOCK_SIZE>* pNonConst = const_cast<BlockVector<T, BLOCK_SIZE> * > (this);
            return pNonConst->getBlock(blockPos);
        }

	public:
		/// @brief Default constructor; creates an empty BlockVector.
		BlockVector()
			: _size(0)
			, _capacity(0)
			, _lastUsedBlockPos(0)
		{};

		/// @brief Constructor that pre-allocates enough blocks for the given capacity.
		/// @param capacity Initial element capacity to reserve.
		BlockVector(size_t capacity)
			: _size(0)
			, _capacity(0)
			, _lastUsedBlockPos(0)
		{
			reserve(capacity);
		}

		/// @brief Destructor; frees all allocated blocks.
		~BlockVector ()
        {
            for (auto& block : _blocks)
            {
                delete [] block;
            }
            _blocks.clean();
		}

		class const_iterator;

		/// @brief Mutable iterator for BlockVector.
		class iterator
		{
			friend class BlockVector;
			friend class const_iterator;
			//tEntry* pIt;
            LIST_ITER _pIter;       ///< Iterator into the list of blocks.
			td::UINT4 _pos;         ///< Current global element position.
            td::UINT4 _posInBlock;  ///< Position within the current block.
            td::UINT4 _size;        ///< Total number of elements (used for bounds checking).

		public:
			/// @brief Default constructor.
			iterator()
				:  _pos(0)
				, _posInBlock(0)
				, _size(0)
			{
			}

			/// @brief Copy constructor.
			/// @param it Iterator to copy from.
			iterator(const iterator& it)
				: _pos(it._pos)
				, _posInBlock(0)
				, _size(it._size)
				, _pIter(it._pIter)
			{
			}

			/// @brief Assignment from another mutable iterator.
			/// @param it Source iterator.
			void operator = (const iterator& it)
			{
				_pos = it._pos;
				_posInBlock = it._posInBlock;
				_size = it._size;
				_pIter = it._pIter;
			}

			/// @brief Assignment from a const iterator.
			/// @param it Source const iterator.
			void operator = (const const_iterator& it)
			{
				_pos = it._pos;
				_posInBlock = it._posInBlock;
				_size = it._size;
				_pIter = it._pIter;
			}

			/// @brief Equality comparison with another mutable iterator.
			/// @param it Iterator to compare with.
			/// @return true if both iterators point to the same position.
			bool operator == (const iterator& it) const
			{
				return (_pos == it._pos);
			}

			/// @brief Equality comparison with a const iterator.
			/// @param it Const iterator to compare with.
			/// @return true if both iterators point to the same position.
			bool operator == (const const_iterator& it) const
			{
				return (_pos == it._pos);
			}

			/// @brief Inequality comparison with another mutable iterator.
			/// @param it Iterator to compare with.
			/// @return true if the iterators point to different positions.
			bool operator != (const iterator& it) const
			{
				return (_pos != it._pos);
			}

			/// @brief Inequality comparison with a const iterator.
			/// @param it Const iterator to compare with.
			/// @return true if the iterators point to different positions.
			bool operator != (const const_iterator& it) const
			{
				return (_pos != it._pos);
			}

			/// @brief Pre-increment operator; advances to the next element.
			void operator ++ ()
			{
				assert(_pos < _size);
				++_pos;
				++_posInBlock;
				if (_posInBlock == BLOCK_SIZE)
				{
					_posInBlock = 0;
					++_pIter;
				}
			}

			/// @brief Const dereference operator.
			/// @return Const reference to the current element.
			const T& operator* () const
			{
				const DataChunk pData = *_pIter;
				return pData[ _posInBlock];
			}

			/// @brief Const member access operator.
			/// @return Const pointer to the current element.
			const T* operator ->() const
			{
				return &(operator*());
			}

			/// @brief Mutable dereference operator.
			/// @return Reference to the current element.
			T& operator* ()
			{
				const DataChunk pData = *_pIter;
				return pData[_posInBlock];
			}

			/// @brief Mutable member access operator.
			/// @return Pointer to the current element.
			T* operator ->()
			{
				return &(operator*());
			}
		};

		/// @brief Const iterator for BlockVector.
		class const_iterator
		{
			friend class BlockVector;
			friend class iterator;
			//tEntry* pIt;
			td::UINT4 _pos;         ///< Current global element position.
            td::UINT4 _posInBlock;  ///< Position within the current block.
            td::UINT4 _size;        ///< Total number of elements.
			CONST_LIST_ITER _pIter; ///< Const iterator into the list of blocks.
		public:
			/// @brief Default constructor.
			const_iterator()
				: _pos(0)
				, _posInBlock(0)
				, _size(0)
			{
			}

			/// @brief Construct from a mutable iterator.
			/// @param it Mutable iterator to convert.
			const_iterator(const iterator& it)
				: _pos(it._pos)
				, _posInBlock(0)
				, _size(it._size)
				, _pIter(it._pIter)
			{
			}

			/// @brief Copy constructor.
			/// @param it Const iterator to copy from.
			const_iterator(const const_iterator& it)
				: _pos(it._pos)
				, _posInBlock(0)
				, _size(it._size)
				, _pIter(it._pIter)
			{
			}

			/// @brief Assignment from a mutable iterator.
			/// @param it Source mutable iterator.
			void operator = (const iterator& it)
			{
				_pos = it._pos;
				_posInBlock = it._posInBlock;
				_size = it._size;
				_pIter = it._pIter;
			}

			/// @brief Assignment from another const iterator.
			/// @param it Source const iterator.
			void operator = (const const_iterator& it)
			{
				_pos = it._pos;
				_posInBlock = it._posInBlock;
				_size = it._size;
				_pIter = it._pIter;
			}

			/// @brief Equality comparison with a mutable iterator.
			/// @param it Mutable iterator to compare with.
			/// @return true if both point to the same position.
			bool operator == (const iterator& it) const
			{
				return (_pos == it._pos);
			}

			/// @brief Equality comparison with another const iterator.
			/// @param it Const iterator to compare with.
			/// @return true if both point to the same position.
			bool operator == (const const_iterator& it) const
			{
				return (_pos == it._pos);
			}

			/// @brief Inequality comparison with a mutable iterator.
			/// @param it Mutable iterator to compare with.
			/// @return true if the iterators point to different positions.
			bool operator != (const iterator& it) const
			{
				return (_pos != it._pos);
			}

			/// @brief Inequality comparison with another const iterator.
			/// @param it Const iterator to compare with.
			/// @return true if the iterators point to different positions.
			bool operator != (const const_iterator& it) const
			{
				return (_pos != it._pos);
			}

			/// @brief Pre-increment operator; advances to the next element.
			void operator ++ ()
			{
				assert(_pos < _size);
				++_pos;
				++_posInBlock;
				if (_posInBlock == BLOCK_SIZE)
				{
					_posInBlock = 0;
					++_pIter;
				}
			}

			/// @brief Dereference operator.
			/// @return Const reference to the current element.
			const T& operator* () const
			{
				T* pData = *_pIter;
				return pData[_posInBlock];
			}

			/// @brief Member access operator.
			/// @return Const pointer to the current element.
			const T* operator ->() const
			{
				return &(operator*());
			}
		};


		/// @brief Returns a mutable iterator to the first element.
		/// @return Iterator positioned at element 0.
		iterator begin()
		{
			iterator it;
			it._pos = 0;
			it._size = _size;
			it._pIter = _blocks.begin();
			return it;
		}

		/// @brief Returns a mutable iterator positioned at a given element.
		/// @param pos Zero-based index of the desired element.
		/// @return Iterator positioned at element pos.
        iterator getPosition(td::UINT4 pos)
        {
            assert(pos < _size);
            iterator it;
            it._pos = pos;
            it._size = _size;
            it._pIter = _blocks.begin();
            return it;
        }

		/// @brief Returns a const iterator to the first element.
		/// @return Const iterator positioned at element 0.
		const_iterator begin() const
		{
			const_iterator it;
			it._pos = 0;
			it._size = _size;
			it._pIter = _blocks.begin();
			return it;
		}

		/// @brief Returns a const iterator to the first element (alternative).
		/// @return Const iterator positioned at element 0.
        const_iterator cbegin() const
        {
            const_iterator it;
            it._pos = 0;
            it._size = _size;
            it._pIter = _blocks.begin();
            return it;
        }

		/// @brief Returns a const iterator past the last element.
		/// @return Const end iterator.
		const_iterator end() const
		{
			const_iterator it;
			it._pos = _size;
			it._size = _size;
			LIST_ITER lIter;
			it._pIter = lIter;
			return it;
		}

		/// @brief Clears all elements and releases all allocated blocks.
		inline void clean()
		{
			_size = 0;
			_capacity = 0;
			_lastUsedBlockPos = 0;
			LIST_ITER itDummy;
			_lastUsedBlockIterator = itDummy;
			_blocks.clean();
		}

		/// @brief Resets the element count to zero without freeing blocks.
		inline void reset()
		{
			_size = 0;
			_lastUsedBlockPos = 0;
			_lastUsedBlockIterator = _blocks.begin();
		}

		/// @brief Appends a default-constructed element to the vector.
		inline void push_back()
		{
			assert(BLOCK_SIZE > 0);
			if (BLOCK_SIZE <= 0)
				return;

			assert(_size <= _capacity);
			if (_size >= _capacity)
			{
				T* block = new T[BLOCK_SIZE];
				_lastUsedBlockIterator = _blocks.push_back(block);
				_lastUsedBlockPos = _size / BLOCK_SIZE;
				_capacity += BLOCK_SIZE;
			}
			++_size;
		}

		/// @brief Appends a copy of the given value to the vector.
		/// @param val Value to append.
		inline void push_back(const T& val)
		{
			assert(BLOCK_SIZE > 0);

			if constexpr (BLOCK_SIZE <= 0)
				return;
			else
			{
				assert(_size <= _capacity);
				if (_size >= _capacity)
				{
					T* block = new T[BLOCK_SIZE];
					_lastUsedBlockIterator = _blocks.push_back(block);
					_lastUsedBlockPos = _size / BLOCK_SIZE;
					_capacity += BLOCK_SIZE;
				}
				td::UINT4 blockPos = _size / BLOCK_SIZE;
                td::UINT4 posInBlock = _size % BLOCK_SIZE;

				T* pBlock = getBlock(blockPos);
				assert(pBlock);
				pBlock[posInBlock] = val;
				++_size;
			}

		}

		/// @brief Inserts a value at the specified position, shifting subsequent elements.
		/// @param pos Zero-based index at which to insert.
		/// @param val Value to insert.
		inline void insert(size_t pos, const T& val)
		{
			assert(BLOCK_SIZE > 0);
			if constexpr(BLOCK_SIZE <= 0)
				return;
			else
			{
				if (pos >= _size || _size == 0)
					return push_back(val);

				if (_size >= _capacity)
				{
					T* block = new T[BLOCK_SIZE];
					_lastUsedBlockIterator = _blocks.push_back(block);
					_lastUsedBlockPos = _size / BLOCK_SIZE;
					_capacity += BLOCK_SIZE;
				}

				int firstBlock = (int)(pos / BLOCK_SIZE);
				int nBlocks = (int)(_size / BLOCK_SIZE);
				if (_size % BLOCK_SIZE)
					++nBlocks;

				int lastBlock = nBlocks - 1;

				for (int i = lastBlock; i >= firstBlock; --i) //i<=lastBlock zato sto --i pravi veliki pozitivan broj kada je
				{
					T* pCB = getBlock(i);
					bool copyToNext = true;
					size_t endPos = BLOCK_SIZE - 1;
					if (i == lastBlock)
					{
						if (_size % BLOCK_SIZE)
						{
							copyToNext = false;
							endPos = _size % BLOCK_SIZE;
						}
					}

					if (copyToNext)
					{
						T* pNB = getBlock(i + 1);
						pNB[0] = pCB[BLOCK_SIZE - 1];
						//int g = 5;
					}

					size_t startPos = 0;

					if (i == firstBlock)
						startPos = pos % BLOCK_SIZE;;

					for (size_t k = endPos; k > startPos; --k)
						pCB[k] = pCB[k - 1];

					if (i == firstBlock)
						pCB[startPos] = val;

				}

				++_size;
			}

		}

		/// @brief Clears the vector (alias for clean()).
		void clear()
		{
			clean();
		}

		/// @brief Removes the element at the specified position.
		/// @param pos Zero-based index of the element to remove.
		void erase(size_t pos)
		{
			assert(BLOCK_SIZE > 0);
            if constexpr(BLOCK_SIZE > 0)
            {
                //static_assert(BLOCK_SIZE > 0, "BLOCK_SIZE is zero!! ");
                if (pos >= _size)
                    return;

                assert(_size);

                _lastUsedBlockPos = 0;
                _lastUsedBlockIterator = _blocks.begin();

                if (pos == _size)
                {
                    //erase last element
                    if (_size > 0)
                        --_size;
                    return;
                }
                size_t blockPos = pos / BLOCK_SIZE;
                size_t nBlocks = _size / BLOCK_SIZE;
                if (_size % BLOCK_SIZE)
                    ++nBlocks;

                pos %= BLOCK_SIZE;
                while (blockPos != nBlocks)
                {
                    if (blockPos == nBlocks - 1)
                    {
                        //element is in last block
                        size_t elemsInCurrentBlock = _size % BLOCK_SIZE;
                        if (elemsInCurrentBlock == 0)
                            elemsInCurrentBlock = BLOCK_SIZE;
                        if (elemsInCurrentBlock == 1)
                            break;
                        size_t nToCopy = elemsInCurrentBlock - pos - 1;
                        if (nToCopy > 0)
                        {
                            T* pBlock = getBlock(blockPos);
                            for (size_t i = pos, iPos = 1; iPos <= nToCopy; ++i, ++iPos)
                                pBlock[i] = pBlock[i + 1];
                        }
                    }
                    else
                    {
                        //deleting element is in mid-block
                        T* pBlock = getBlock(blockPos);
                        for (size_t i = pos; i < BLOCK_SIZE-1; ++i)
                            pBlock[i] = pBlock[i + 1];
                        T* pNextBlock = getBlock(blockPos+1);
                        pBlock[BLOCK_SIZE - 1] = pNextBlock[0];
                        pos = 0;
                    }
                    ++blockPos;
                }
                --_size;
            }
		}

		//inline const T& operator [] (size_t pos) const
		//{
		//	assert (pos >= 0 && pos < _size);
		//	size_t blockPos = pos / BLOCK_SIZE;
		//	size_t posInBlock = pos % BLOCK_SIZE;
		//	const T* pBlock = *(getBlock(blockPos));
		//	assert(pBlock);
		//	return pBlock[posInBlock];
		//}

		/// @brief Mutable element access by index.
		/// @param pos Zero-based index of the element.
		/// @return Reference to the element at pos.
		inline T& operator [] (size_t pos)
		{
			assert(BLOCK_SIZE > 0);
			//static_assert(BLOCK_SIZE > 0, "BLOCK_SIZE is zero!! Ovdje je problem!!");
            if constexpr(BLOCK_SIZE > 0)
            {
                td::UINT4 blockPos = td::UINT4(pos / BLOCK_SIZE);
                td::UINT4 posInBlock = pos % BLOCK_SIZE;
                assert(pos < _size);
                T* pBlock = getBlock(blockPos);
                assert(pBlock);
                return pBlock[posInBlock];
            }
			else
			{
				assert(false);
                td::UINT4 blockPos = 0;
                td::UINT4 posInBlock = 0;
				assert(pos < _size);
				T* pBlock = getBlock(blockPos);
				assert(pBlock);
				return pBlock[posInBlock];
			}

		}

		/// @brief Const element access by index.
		/// @param pos Zero-based index of the element.
		/// @return Const reference to the element at pos.
        const inline T& operator [] (size_t pos) const
        {
            assert(BLOCK_SIZE > 0);
            //static_assert(BLOCK_SIZE > 0, "BLOCK_SIZE is zero!! Ovdje je problem!!");
            if constexpr(BLOCK_SIZE > 0)
            {
                td::UINT4 blockPos = td::UINT4(pos / BLOCK_SIZE);
                td::UINT4 posInBlock = pos % BLOCK_SIZE;
                assert(pos < _size);
                const T* pBlock = getBlock(blockPos);
                assert(pBlock);
                return pBlock[posInBlock];
            }
            else
            {
                assert(false);
                td::UINT4 blockPos = 0;
                td::UINT4 posInBlock = 0;
                assert(pos < _size);
                T* pBlock = getBlock(blockPos);
                assert(pBlock);
                return pBlock[posInBlock];
            }

        }

		//inline const T& operator () (size_t pos) const
		//{
		//	assert(pos >= 0 && pos < _size);
		//	size_t blockPos = pos / BLOCK_SIZE;
		//	size_t posInBlock = pos % BLOCK_SIZE;
		//	const T* pBlock = *(getBlock(blockPos));
		//	assert(pBlock);
		//	return pBlock[posInBlock];
		//}

		/// @brief Mutable element access using function-call syntax.
		/// @param pos Zero-based index of the element.
		/// @return Reference to the element at pos.
		inline T& operator () (size_t pos)
		{
			static_assert(BLOCK_SIZE > 0, "BLOCK_SIZE is zero!! ");
			assert(pos >= 0 && pos < _size);
			td::UINT4 blockPos = td::UINT4(pos / BLOCK_SIZE);
			td::UINT4 posInBlock = pos % BLOCK_SIZE;
			T* pBlock = getBlock(blockPos);
			assert(pBlock);
			return pBlock[posInBlock];
		}

		/// @brief Zeroes all elements in every allocated block using fastZero.
		inline void zeros()
		{
			auto it(_blocks.begin());
			auto itEnd(_blocks.end());
			while (it != itEnd)
			{
				T* pData = *it;
                mem::fastZero(pData, BLOCK_SIZE);
				++it;
			}
		}


		/// @brief Copy-assignment operator; replaces this vector's contents with those of another.
		/// @param vect Source BlockVector to copy from.
		/// @return Reference to this vector.
		inline BlockVector& operator = (const BlockVector&  vect)
		{
			size_t sizeIn = vect.size();

			if (sizeIn > _capacity)
			{
				clean();
				reserve(sizeIn);
			}
			else
			{
				_size = 0;
			}

			auto it(vect.begin());
			auto itEnd(vect.end());
			while (it != itEnd)
			{
				push_back(*it);
				++it;
			}
			return *this;
		}


		/// @brief Reserves capacity for at least the given number of elements.
		/// @param capacity Desired minimum element capacity.
		inline void reserve(size_t capacity)
		{
			_size = 0;
			if (capacity == 0)
			{
				clean();
				return;
			}
			size_t nBlocks = capacity / BLOCK_SIZE ;
			if (capacity % BLOCK_SIZE)
				++nBlocks;

			//size_t allocatedBlocks = _blocks.size();
			size_t newCapacity = nBlocks * BLOCK_SIZE;
			if (_capacity == newCapacity)
			{
				_lastUsedBlockPos = 0;
				_lastUsedBlockIterator = _blocks.begin();
				return;
			}

			_blocks.clean();

			for (size_t i = 0; i < nBlocks; ++i)
			{
				T* block = new T[BLOCK_SIZE];
				_blocks.push_back(block);
			}
			_lastUsedBlockPos = 0;
			_lastUsedBlockIterator = _blocks.begin();
			_capacity = td::UINT4(newCapacity);
		}

		/// @brief Returns the number of elements currently stored.
		/// @return Current element count.
		inline size_t size() const
		{
			return _size;
		}

		/// @brief Checks whether the vector contains no elements.
		/// @return true if the vector is empty.
		inline bool isEmpty() const
		{
			if (BLOCK_SIZE <= 0)
				return true;
			return _size == 0;
		}

		/// @brief Stream output operator; writes all elements separated by spaces.
		/// @param os Output stream.
		/// @param rhs BlockVector to output.
		/// @return Reference to the output stream.
		template<class CharT, class Traits>
		friend std::basic_ostream<CharT, Traits>&
			operator<<(std::basic_ostream<CharT, Traits>& os, const BlockVector& rhs)
		{
			size_t fullBlocks = rhs._size / BLOCK_SIZE;
			auto it(rhs._blocks.begin());
			auto itEnd(rhs._blocks.end());
			while (fullBlocks && it != itEnd)
			{
				T* pBlockData = *it;
				for (size_t i = 0; i < BLOCK_SIZE; ++i)
					os << pBlockData[i] << " ";
				++it;
				--fullBlocks;
			}
			size_t remElemsFromLastBlock = rhs._size % BLOCK_SIZE;
			if (remElemsFromLastBlock > 0)
			{
				T* pBlockData = *it;
				for (size_t i = 0; i < remElemsFromLastBlock; ++i)
					os << pBlockData[i] << " ";
			}

			//rhs.show(os);
			return os;
		}

		/// @brief Searches for the first element satisfying the finder predicate.
		/// @tparam TFINDER Callable type; called with a reference to each element.
		/// @param finder Predicate that returns true when the desired element is found.
		/// @return Pointer to the found element, or nullptr if not found.
		template <typename TFINDER>
		T* find(TFINDER& finder)
		{
			static_assert(BLOCK_SIZE > 0, "BLOCK_SIZE is zero!!");

			size_t fullBlocks = _size / BLOCK_SIZE;
			auto it(_blocks.begin());
			auto itEnd(_blocks.end());
			while (fullBlocks && it != itEnd)
			{
				T* pBlockData = *it;
				for (size_t i = 0; i < BLOCK_SIZE; ++i)
				{
					if (finder(pBlockData[i]))
						return pBlockData + i;
				}
				++it;
				--fullBlocks;
			}
			size_t remElemsFromLastBlock = _size % BLOCK_SIZE;
			if (remElemsFromLastBlock > 0)
			{
				T* pBlockData = *it;
				for (size_t i = 0; i < remElemsFromLastBlock; ++i)
				{
					if (finder(pBlockData[i]))
						return pBlockData + i;
				}
			}

			return nullptr;
		}

		//template<class CharT, class Traits>
		//friend std::basic_istream<CharT, Traits>&
		//	operator>>(std::basic_istream<CharT, Traits>& is, SafeFullVector& rhs)
		//{
		//	assert(!EXTERN_ALLOCATOR);
		//	std::string str;
		//	os >> str;
		//	rhs = str;
		//	return os;
		//}
	};
}
